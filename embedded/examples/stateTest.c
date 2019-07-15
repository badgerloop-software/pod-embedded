#include <stdio.h>
#include <data.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <state_machine.h>
#include <states.h>

#define PASS 0
#define FAIL 1

#define WAIT(x) (usleep((x) * 1000000.0))

/* So that we don't have any changes made to the statemachine reverted mid-setup */
#define FREEZE_SM (sem_wait(&smSem))
#define UNFREEZE_SM (sem_post(&smSem))

#define PASS_STR "\033[1;32m[PASS]\033[0m "
#define FAIL_STR "\033[1;31m[FAIL]\033[0m "

#define ASSERT_STATE_IS(x) \
        (!strcmp((getCurrState()->name), (findState(x)->name)) \
        ? PASS : FAIL)

#define RUN_TEST(x) (x() == PASS \
    ? printf(PASS_STR #x "\n\n") : printf(FAIL_STR #x " @ state %s\n\n", getCurrState()->name))

#define BANNER(x) (printf("----STARTING %s----\n", (x)))

static pthread_t smThread;
static sem_t smSem;

static void genericInit(char *name);
static void goToState(char *name);
static int checkForChange(char *name);
extern stateMachine_t stateMachine;

/***********************************
 * Below is a brief summary of every test contained in this module. 
 *      They should have a description, start state, expected end state, 
 *      and any other behaviors that should be seen (e.g. emergency brakes actuated)
 *
 * Test breakdown:
 * test1 - Battery state of charge is too low to run from pumpdown
 *      Start: pumpdown      Expected: pre-run fault
 * test2 - Battery voltage is too low
 *      Start: Ready         Expected: pre-run fault
 * test3 - Motor controller is overheating
 *      Start: Propulsion    Expected: run fault
 * test4 - Missed 5 retro strips in a row
 *      Start: Propulsion    
 * test5 - Pressure vessel is depressurizing
 * test6 - Behavior on emergency brake assertion
 * test7 - Loss of connection with either module
 * test8 - Motor is not shutting off
 * test9 - ...
 *
 ***/

/***
 * Programming notes for adding tests: 
 *     - Each test should follow a similar pattern
 *          1. FREEZE_SM      // Prevents the state machine from running during setup, allowing all setup to happen before running
 *          2. genericInit()  // Makes generally nominal (not always true but its almost always close) 
 *          4. UNFREEZE_SM    // Lets the SM run again
 *          5. WAIT()         // Lets the SM run for a little bit to allow changes to propogate
 *          6. <CHECK STATE HASNT CHANGED>  // we dont want any false positives!
 *          7. <test specific setup>    // This is where the real test portion really happens
 *          8, WAIT()          // To see if it changes
 *          6. ASSERT_STATE_IS(<name of expected state>)  // Returns PASS/FAIL based on if the state is where you expect
 */

/* Battery low */
static int hvBattSOCLowTest() 
    {
    FREEZE_SM;
    genericInit("High V SOC Low Test");
    goToState(PUMPDOWN_NAME);
    UNFREEZE_SM;

    WAIT(0.5);    
    
    if (checkForChange(PUMPDOWN_NAME) != PASS) return FAIL;
    
    data->bms->Soc = 50;
    
    WAIT(0.5);
    
    return ASSERT_STATE_IS(NON_RUN_FAULT_NAME);
    }

/* Voltage Low, pack and cell */
static int hvBattLowVoltTest() 
    {
    FREEZE_SM;
    genericInit("High V Low Voltage Test");
    goToState(PROPULSION_NAME);
    UNFREEZE_SM;
    WAIT(0.5);
    
    if (checkForChange(PROPULSION_NAME) != PASS) return FAIL;

    data->bms->packVoltage = 200;

    WAIT(0.5);

    return ASSERT_STATE_IS(RUN_FAULT_NAME);
    }

static int rmsOverheatTest()
    {
    FREEZE_SM;
    genericInit("RMS Overheating Test");
    goToState(PROPULSION_NAME);
    UNFREEZE_SM;
    
    WAIT(.5);
   
    if (checkForChange(PROPULSION_NAME) != PASS) return FAIL;

    data->rms->igbtTemp = 150;
    WAIT(.5);

    return ASSERT_STATE_IS(RUN_FAULT_NAME);
    }

/* Missed 5 tape strips in a row */
static int navMissedRetroTest()
    {

    genericInit("Nav Missed 5 Retro Test");

    return ASSERT_STATE_IS(RUN_FAULT_NAME);
    }

static int crawlTimerTest()
    {
    FREEZE_SM;
    genericInit("Crawl Timer Test");
    goToState(CRAWL_NAME);
    UNFREEZE_SM;

    WAIT(.5);

    if (checkForChange(CRAWL_NAME) != PASS) return FAIL;
    

    printf("check\n");

    uint64_t offset = getuSTimestamp();
    
    for (int i = 0; i < (30 * 5); i++) {
        printf("crawl timer: %llu\r", getuSTimestamp() - data->timers->crawlTimer);
        fflush(stdin);
        usleep(200000);
    }
   
    WAIT(.2);

    return ASSERT_STATE_IS(BRAKING_NAME);

    }


/* PV depressurizing.*/
static int pvDepressurizingTest()
    {
    char *statesToTest[] = 
        {
        PUMPDOWN_NAME,
        PROPULSION_NAME,
        BRAKING_NAME,
        STOPPED_NAME,
        CRAWL_NAME,
        POST_RUN_NAME,
        NULL
        };
    int i = 0;
    char testName[100];

    for (i = 0; statesToTest[i] != NULL; i++) 
        {
        FREEZE_SM;
        sprintf(testName, "PV Losing Pressure in %s", statesToTest[i]);
        genericInit(testName);
        goToState(statesToTest[i]);
        UNFREEZE_SM;
        WAIT(.2);

/*        if (checkForChange(statesToTest[i]) == FAIL) return FAIL;*/
        
        data->pressure->pv = 5;
        
        WAIT(.5);
        
        if (ASSERT_STATE_IS(NON_RUN_FAULT_NAME) != PASS &&
                ASSERT_STATE_IS(RUN_FAULT_NAME) != PASS)
            return FAIL;
        printf(PASS_STR"in %s\n",statesToTest[i]);
        }
    return PASS;
    }

void *stateMachineLoop() 
    {
    while(1)
        {
        sem_wait(&smSem);
        runStateMachine();
        sem_post(&smSem);
        WAIT(.1);
        }
    return NULL;
    }

        
/* Drives the tests */        
int main() {
    initData();
    buildStateMachine();
/*    genericInit();*/
    if (sem_init(&smSem, 0, 1) != 0) {
        return -1;   
    }
    if (pthread_create(&smThread, NULL, stateMachineLoop, NULL) != 0)
        return -1;
    WAIT(.5);
    RUN_TEST(crawlTimerTest);
    RUN_TEST(hvBattSOCLowTest);
    RUN_TEST(hvBattLowVoltTest);
    RUN_TEST(rmsOverheatTest);
    RUN_TEST(navMissedRetroTest);
    RUN_TEST(pvDepressurizingTest);
    sem_destroy(&smSem);
}


/* Helpers for each test */

static void genericInit(char *name) {
    BANNER(name);
    goToState(IDLE_NAME);
    /* Just remember: pmbrft */
    pressure_t *p = data->pressure;
    motion_t   *m = data->motion;
    bms_t      *b = data->bms;
    rms_t      *r = data->rms;
    flags_t    *f = data->flags;
    timers_t   *t = data->timers;
    
    f->readyPump        = 0; //false;
    f->pumpDown         = 0;
    f->readyCommand     = 0;
    f->propulse         = 0;
    f->emergencyBrake   = 0;
    f->shouldStop       = 0;
    f->shutdown         = 0;

    t->startTime = getuSTimestamp();
    t->oldRetro  = 0;
    t->lastRetro = 0;
    
    p->primTank = 1500;
    p->primLine = 200;
    p->primAct  = 1;
    p->secTank  = 1500;
    p->secLine  = 200;
    p->secAct   = 1;
    p->pv       = 14;

    m->pos          = 0;
    m->vel          = 0;
    m->accel        = 0;
    m->retroCount   = 0;

    b->packCurrent      = 0;
    b->packVoltage      = 270;
    b->packDCL          = 0;
    b->packCCL          = 0;
    b->packResistance   = 0;
    b->packHealth       = 100;
    b->packOpenVoltage  = 0;
    b->packCycles       = 0;
    b->packAh           = 0;
    b->inputVoltage     = 0;
    b->Soc              = 95;
    b->relayStatus      = 0;
    b->highTemp         = 25;
    b->lowTemp          = 20;
    b->cellMaxVoltage   = 3200; /* These three are mV */
    b->cellMinVoltage   = 3000;
    b->cellAvgVoltage   = 3100;
    b->maxCells         = 72;
    b->numCells         = 72;

    r->igbtTemp             = 23;
    r->gateDriverBoardTemp  = 22;
    r->controlBoardTemp     = 21;
    r->motorTemp        = 25;
    r->motorSpeed       = 0;
    r->phaseACurrent    = 0;
    r->phaseBCurrent    = 0;
    r->phaseCCurrent    = 0;
    r->dcBusVoltage     = 0;
    r->lvVoltage        = 13;
    r->canCode1         = 0;
    r->canCode2         = 0;
    r->faultCode1       = 0;
    r->faultCode2       = 0;
    r->commandedTorque  = 0;
    r->actualTorque     = 0;
    r->relayState       = 0;
    r->electricalFreq   = 0;
    r->dcBusCurrent     = 0;
    r->outputVoltageLn  = 0;
    r->VSMCode          = 0;
    r->keyMode          = 0;
}

static void goToState(char *name) {
    stateMachine.currState = findState(name);
}

static int checkForChange(char *name) {
    if (ASSERT_STATE_IS(name) != PASS)
        {
        fprintf(stderr, "Failed to stay in state: %s\n", name);
        return FAIL;
        }
    return PASS;
}
