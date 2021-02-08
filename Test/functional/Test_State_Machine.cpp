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
    }

    void TearDown()
    {
        FREEZE_SM;
        GO_TO_STATE(IDLE_NAME);
        stateMachine.start = getuSTimestamp();
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
    EXPECT_NE(strcmp(getCurrState()->name, findState(test_name)->name), 0);
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

    setBmsSoc(50); // Make our soc too low

    WAIT(0.5); // Give time for transition

    assertStateIs(NON_RUN_FAULT_NAME);
}