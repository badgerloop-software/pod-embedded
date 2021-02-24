/**
 * Author: Eric
 */

#include <gtest/gtest.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern "C" {
#include <can_devices.h>
#include <data.h>
#include <load_software_parameters.h>
#include <state_machine.h>
#include <states.h>
}

#include "gtest_globals.h"
#include "software_parameters.h"

#define WAIT(x) (usleep((x)*1000000.0))

/* So that we don't have any changes made to the statemachine reverted mid-setup */
#define FREEZE_SM (sem_wait(&smSem))
#define UNFREEZE_SM (sem_post(&smSem))

#define GO_TO_STATE(x) (stateMachine.currState = findState(x))

static pthread_t smThread;
static sem_t smSem;
extern stateMachine_t stateMachine;

class StateTest : public testing::Test {
protected:
    static void SetUpTestSuite()
    {
        // Executes before entire test suite
        initData();
        SetupCANDevices();
        loadParameters(executable_path, (char*)"../Test/run_profiles/Test_profiles/nominal_profile.txt");
        /*    genericInit();*/
        buildStateMachine();
        if (sem_init(&smSem, 0, 1) != 0) {
            FAIL();
        }
        if (pthread_create(&smThread, NULL, &StateTest::stateMachineLoop, NULL) != 0) {
            FAIL();
        }
    }

    void SetUp()
    {
        // Restart state machine timer
        FREEZE_SM;
        stateMachine.start = getuSTimestamp();
        UNFREEZE_SM;
    }

    void TearDown()
    {
        // Revert back to idle
        FREEZE_SM;
        GO_TO_STATE(IDLE_NAME);
        UNFREEZE_SM;
        WAIT(0.5);
    }

    static void TearDownTestSuite()
    {
        // Executes after entire test suite
        // destroyStateMachine();
        pthread_kill(smThread, 0);
        sem_destroy(&smSem);
    }

    static void* stateMachineLoop(void* context)
    {
        while (1) {
            sem_wait(&smSem);
            runStateMachine();
            sem_post(&smSem);
            WAIT(.1);
        }
        return NULL;
    }
};

void assertStateIs(char* test_name)
{
    if (!strcmp(getCurrState()->name, findState(test_name)->name)) {
        return;
    } else {
        fprintf(stderr, "Expected to be in state %s but was in state %s\n", test_name, getCurrState()->name);
        FAIL() << "State Failure";
    }
}

void assertStateIsNot(char* test_name)
{
    if (strcmp(getCurrState()->name, findState(test_name)->name)) {
        return;
    } else {
        fprintf(stderr, "Expected transition out of state %s but was still in state %s\n", test_name, getCurrState()->name);
        FAIL() << "State Transition Failure";
    }
}

void assertStateIsFault() {
       if (strcmp(getCurrState()->name, NON_RUN_FAULT_NAME) || strcmp(getCurrState()->name, RUN_FAULT_NAME)) {
        return;
    } else {
        fprintf(stderr, "Expected to be in a fault state but was in %s\n", getCurrState()->name);
        FAIL() << "Fault Error";
    } 
}

int checkForChange(char* name)
{
    assertStateIsNot(name);
}

static void genericInit(void)
{
    GO_TO_STATE(IDLE_NAME);

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

    setPressurePrimTank(1200);
    setPressurePrimLine(200);
    setPressurePrimAct(1);
    setPressureSecTank(1200);
    setPressureSecLine(100);
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
void debuggingHelper(){
    fprintf(stderr, "[LOG] getRmsCommandedTorque(): %d\n", getRmsCommandedTorque() );
    fprintf(stderr, "[LOG] getFlagsShouldBrake():   %d\n", getFlagsShouldBrake() );
    fprintf(stderr, "[LOG] getFlagsBrakePrimAct():  %d\n", getFlagsBrakePrimAct() );
    fprintf(stderr, "[LOG] getFlagsBrakePrimRetr(): %d\n", getFlagsBrakePrimRetr() );
}
/**
 * Ensure SetUp ran correctly and we are placed in the idle state
 */
TEST_F(StateTest, StateMachineInit)
{
    assertStateIs(IDLE_NAME);
}

/**
 * Battery state of charge is too low to run from pumpdown
 * Start state: Pumpdown        Expected end state: Non-run fault
 */
TEST_F(StateTest, HV_Battery_SOC_Test)
{
    // Set state machine to the pumpdown state
    FREEZE_SM;
    genericInit();
    fprintf(stderr, "[LOG] GOING INTO PUMPDOWN\n");
    GO_TO_STATE(PUMPDOWN_NAME);
    UNFREEZE_SM;

    WAIT(0.5); // Give time for transition

    assertStateIs(PUMPDOWN_NAME); // Ensure we have transitioned

    setBmsSoc(10); // Make our soc too low

    WAIT(0.5); // Give time for transition

    assertStateIs(NON_RUN_FAULT_NAME);
}

/**
 * Battery Voltage is too low during run
 * Start state: propulsion      Expected end state: run-fault
 */
TEST_F(StateTest, HV_Battery_Low_Voltage_Test) {
    FREEZE_SM;
    fprintf(stderr, "[LOG] GOING INTO PROPULSION\n");
    genericInit();
    GO_TO_STATE(PROPULSION_NAME);
    UNFREEZE_SM;
    WAIT(0.5);

    assertStateIs(PROPULSION_NAME);

    setBmsPackVoltage(200);

    WAIT(0.5);

    assertStateIs(RUN_FAULT_NAME);
}

/**
 * RMS overheats during run
 * Start state: propusion       Expected end state: run-fault
 */
TEST_F(StateTest, RMS_Overheat_Test) {
    FREEZE_SM;
    genericInit();
    fprintf(stderr, "[LOG] GOING INTO PROPULSION\n");
    GO_TO_STATE(PROPULSION_NAME);
    UNFREEZE_SM;

    WAIT(0.5);

    assertStateIs(PROPULSION_NAME);

    setRmsIgbtTemp(150);

    WAIT(0.5);

    assertStateIs(RUN_FAULT_NAME);
}

/**
 * Make sure the SM transitions into stopped after going past crawl threshold
 * Start state: crawl       Expected end state: stopped
 */
TEST_F(StateTest, Crawl_Timer_Test) {
    FREEZE_SM;
    genericInit();
    setPressurePrimTank(500);
    setPressurePrimLine(100);
    fprintf(stderr, "[LOG] GOING INTO CRAWL\n");
    GO_TO_STATE(CRAWL_NAME);
    UNFREEZE_SM;

    WAIT(0.5);

    assertStateIs(CRAWL_NAME);

    uint64_t offset = getuSTimestamp();

    for (int i = 0; i < (30 * 5); i++) {
        printf("crawl timer: %llu\r", getuSTimestamp() - getTimersCrawlTimer());
        fflush(stdin);
        usleep(200000);
    }

    WAIT(.2);

    return assertStateIs(STOPPED_NAME);
}

TEST_F(StateTest, Pressure_Vessel_Depressure_Test) {
    char* statesToTest[] = {
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

    for (i = 0; statesToTest[i] != NULL; i++) {
        FREEZE_SM;
        genericInit();
        fprintf(stderr, "[LOG] Going into %s\n", statesToTest[i]);
        GO_TO_STATE(statesToTest[i]);
        UNFREEZE_SM;
        WAIT(.2);


        setPressurePv(5);

        WAIT(.5);

        assertStateIsFault();
    }
}
/**
 * Test the nominal transition from idle to pumpdown
 * Start state: idle        Expected state: pumpdown
 */
TEST_F(StateTest, Idle_To_Pumpdown_Test) {
    FREEZE_SM;
    genericInit();
    fprintf(stderr, "[LOG] Going into Pumpdown\n");
    GO_TO_STATE(PUMPDOWN_NAME);
    UNFREEZE_SM;
    WAIT(.5);
    ASSERT_EQ(getFlagsShouldBrake(), false);

    debuggingHelper();

    assertStateIs(PUMPDOWN_NAME);
}
/**
 * Test the nominal transition from idle to pumpdown
 * Start state: pumpdown        Expected state: propulsion
 */
TEST_F(StateTest, Pumpdown_to_Propulsion_Test) {
    FREEZE_SM;
    genericInit();
    fprintf(stderr, "[LOG] Going into Propulsion\n");
    GO_TO_STATE(PUMPDOWN_NAME);
    GO_TO_STATE(PROPULSION_NAME);
    UNFREEZE_SM;
    WAIT(.5);
    
    debuggingHelper();
    ASSERT_EQ(getFlagsShouldBrake(), false);
    EXPECT_NE(getRmsCommandedTorque(), 0);
    assertStateIs(PROPULSION_NAME);
}
/**
 * Test the nominal transition from propulsion to braking
 * Start state: propulsion        Expected state: braking
 */
TEST_F(StateTest, Propulsion_to_Braking_Test) {
    FREEZE_SM;
    genericInit();
    fprintf(stderr, "[LOG] Going into Braking\n");
    GO_TO_STATE(PROPULSION_NAME);
    GO_TO_STATE(BRAKING_NAME);
    UNFREEZE_SM;
    WAIT(.5);

    debuggingHelper();
    ASSERT_EQ(getFlagsBrakePrimAct(), true); // check if brakes are primary actuated
    assertStateIs(BRAKING_NAME);
}
/**
 * Test the nominal transition from braking to stopped
 * Start state: braking        Expected state: stopped
 */
TEST_F(StateTest, Braking_To_Stopped_Test) {
    FREEZE_SM;
    genericInit();
    fprintf(stderr, "[LOG] Going into Stopped\n");
    GO_TO_STATE(BRAKING_NAME);
    GO_TO_STATE(STOPPED_NAME);
    UNFREEZE_SM;
    WAIT(.5);

    debuggingHelper();
    ASSERT_EQ(getFlagsBrakePrimRetr(), true);

    assertStateIs(STOPPED_NAME);
}
/**
 * Test the nominal transition from idle to pumpdown
 * Start state: stopped        Expected state: service precharge
 */
TEST_F(StateTest, Stopped_to_Service_Precharge_Test) {
    FREEZE_SM;
    genericInit();
    fprintf(stderr, "[LOG] Going into Stopped\n");
    GO_TO_STATE(STOPPED_NAME);
    GO_TO_STATE(SERV_PRECHARGE_NAME);
    UNFREEZE_SM;
    WAIT(.5);

    debuggingHelper();
    ASSERT_EQ(getFlagsShouldBrake(), false);
    assertStateIs(SERV_PRECHARGE_NAME);
}
/**
 * Test the nominal transition from idle to pumpdown
 * Start state: service precharge         Expected state: service propulsion
 */
TEST_F(StateTest, Service_Precharge_To_Service_Propulsion_Test) {
    FREEZE_SM;
    genericInit();
    fprintf(stderr, "[LOG] Going into Stopped\n");
    GO_TO_STATE(SERV_PRECHARGE_NAME);
    GO_TO_STATE(CRAWL_NAME);
    UNFREEZE_SM;
    WAIT(.5);

    debuggingHelper();
    ASSERT_EQ(getFlagsShouldBrake(), false);
    EXPECT_NE(getRmsCommandedTorque(), 0);
    assertStateIs(CRAWL_NAME);
}
/**
 * Test the nominal transition from idle to pumpdown
 * Start state: service propulsion        Expected state: post run
 */
TEST_F(StateTest, Service_Propulsion_to_Post_Run_Test) {
    FREEZE_SM;
    genericInit();
    fprintf(stderr, "[LOG] Going into Stopped\n");
    GO_TO_STATE(CRAWL_NAME);
    GO_TO_STATE(POST_RUN_NAME);
    UNFREEZE_SM;
    WAIT(.5);

    debuggingHelper();
    assertStateIs(POST_RUN_NAME);
}
/**
 * Test the nominal transition from idle to pumpdown
 * Start state: post run        Expected state: safe to approach
 */
TEST_F(StateTest, Post_Run_To_Safe_To_Approach_Test) {
    FREEZE_SM;
    genericInit();
    fprintf(stderr, "[LOG] Going into Stopped\n");
    GO_TO_STATE(POST_RUN_NAME);
    GO_TO_STATE(SAFE_TO_APPROACH_NAME);
    UNFREEZE_SM;
    WAIT(.5);
    debuggingHelper();

    assertStateIs(SAFE_TO_APPROACH_NAME);
}
