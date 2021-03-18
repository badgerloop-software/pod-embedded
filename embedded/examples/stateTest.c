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
    
    setBmsSoc(50);
    
    WAIT(0.5);
    
    return ASSERT_STATE_IS(NON_RUN_FAULT_NAME);
    }

static int bmsTest() {
    //min voltage, max temp, SOC, Current, packV
    FREEZE_SM;
    genericInit("BMS passes health checks");
    UNFREEZE_SM;
    

    WAIT(0.5);
    return 0;    
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

    setBmsPackVoltage(200);

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

    setRmsIgbtTemp(150);
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
        printf("crawl timer: %llu\r", getuSTimestamp() - getTimersCrawlTimer());
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
        
        setPressurePv(5);
        
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

    setFlagsReadyPump(0);
    setFlagsPumpDown(0);
    setFlagsReadyCommand(0);
    setFlagsPropulse(0);
    setFlagsEmergencyBrake(0);
    setFlagsShouldStop(0);
    setFlagsShutdown(0);
    
    setTimersStartTime(getuSTimestamp());
    setTimersOldRetro(0);
    setTimersLastRetro(0);
    
    setPressurePrimTank(1500);
    setPressurePrimLine(200);
    setPressurePrimAct(1);
    setPressureSecTank(1500);
    setPressureSecLine(200);
    setPressureSecAct(1);
    setPressurePv(14);

    setMotionPos(0);
    setMotionVel(0);
    setMotionAccel(0);
    setMotionRetroCount(0);

    
    setBmsPackCurrent(0);
    setBmsPackVoltage(270);
    setBmsPackDCL(0);
    setBmsPackCCL(0);
    setBmsPackResistance(0);
    setBmsPackHealth(100);
    setBmsPackOpenVoltage(0);
    setBmsPackCycles(0);
    setBmsPackAh(0);
    setBmsInputVoltage(0);
    setBmsSoc(95);
    setBmsRelayStatus(0);
    setBmsHighTemp(25);
    setBmsLowTemp(20);
    setBmsCellMaxVoltage(3200); /* These three are mV */
    setBmsCellMinVoltage(3000);
    setBmsCellAvgVoltage(3100);
    setBmsMaxCells(72);
    setBmsNumCells(72);

    setRmsIgbtTemp(23);
    setRmsGateDriverBoardTemp(22);
    setRmsControlBoardTemp(21);
    setRmsMotorTemp(25);
    setRmsMotorSpeed(0);
    setRmsPhaseACurrent(0);
    setRmsPhaseBCurrent(0);
    setRmsPhaseCCurrent(0);
    setRmsDcBusVoltage(0);
    setRmsLvVoltage(13);
    setRmsCanCode1(0);
    setRmsCanCode2(0);
    setRmsFaultCode1(0);
    setRmsFaultCode2(0);
    setRmsCommandedTorque(0);
    setRmsActualTorque(0);
    setRmsRelayState(0);
    setRmsElectricalFreq(0);
    setRmsDcBusCurrent(0);
    setRmsOutputVoltageLn(0);
    setRmsVSMCode(0);
    setRmsKeyMode(0);
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
