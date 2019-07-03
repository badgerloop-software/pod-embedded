#include <stdio.h>
#include <data.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <state_machine.h>

#define PASS 1
#define FAIL 0

#define WAIT(x) (usleep((x) * 1000000.0))

#define PASS_STR "\033[1;32m[PASS]\033[0m "
#define FAIL_STR "\033[1;31m[FAIL]\033[0m "

#define ASSERT_STATE_IS(x) \
        (!strcmp((getCurrState()->name), (findState(x)->name)) \
        ? PASS : FAIL)

#define RUN_TEST(x) (x() == PASS \
    ? printf(PASS_STR #x "\n") : printf(FAIL_STR #x " @ state %s\n", getCurrState()->name))

pthread_t smThread;

/***********************************
 * Test breakdown:
 * test1 - Battery state of charge is too low to run from pumpdown
 * test2 - Battery voltage is too low
 * test3 - Motor controller is overheating
 * test4 - Missed 5 retro strips in a row
 * test5 - Pressure vessel is depressurizing
 * test6 - Behavior on emergency brake assertion
 * test7 - Loss of connection with either module
 * test8 - ...
 *
 ***/

void genericInit() {
    
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
    b->Soc              = 80;
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
    r->lvVoltage        = 0;
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

/* Battery low */
static int hvBattSOCLowTest() 
    {
    genericInit();    
    WAIT(1.5);
    
    return ASSERT_STATE_IS(PRE_RUN_FAULT_NAME);
    }

/* Voltage Low, pack and cell */
static int hvBattLowVoltTest() 
    {
    genericInit();
    data->bms->packVoltage = 200;
    WAIT(1.5);
    return ASSERT_STATE_IS(RUN_FAULT_NAME);
    }

static int rmsOverheatTest()
    {
    genericInit();
    return ASSERT_STATE_IS(RUN_FAULT_NAME);
    }

/* Missed 5 tape strips in a row */
static int navMissedRetroTest()
    {
    genericInit();
    return ASSERT_STATE_IS(RUN_FAULT_NAME);
    }

void *stateMachineLoop() 
    {
    while(1)
        {
        runStateMachine();
        WAIT(.2);
        }
    return NULL;
    }

int main() {
    initData();
    genericInit();
    buildStateMachine();
    if (pthread_create(&smThread, NULL, stateMachineLoop, NULL) != 0)
        return -1;
    WAIT(1);
    RUN_TEST(hvBattSOCLowTest);
    RUN_TEST(hvBattLowVoltTest);
    RUN_TEST(rmsOverheatTest);
    RUN_TEST(navMissedRetroTest);
}
