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
#include <data.h>
#include <state_machine.h>
#include <states.h>
}

#define WAIT(x) (usleep((x)*1000000.0))

/* So that we don't have any changes made to the statemachine reverted mid-setup */
#define FREEZE_SM (sem_wait(&smSem))
#define UNFREEZE_SM (sem_post(&smSem))

#define PASS_STR "\033[1;32m[PASS]\033[0m "
#define FAIL_STR "\033[1;31m[FAIL]\033[0m "

#define ASSERT_STATE_IS(x)                                 \
    (!strcmp((getCurrState()->name), (findState(x)->name)) \
            ? PASS                                         \
            : FAIL)

#define RUN_TEST(x) (x() == PASS     \
        ? printf(PASS_STR #x "\n\n") \
        : printf(FAIL_STR #x " @ state %s\n\n", getCurrState()->name))

#define BANNER(x) (printf("----STARTING %s----\n", (x)))

static pthread_t smThread;
static sem_t smSem;

static void genericInit(char* name);
static void goToState(char* name);
static int checkForChange(char* name);

class StateTest : public testing::Test {

    void SetUp()
    {
        // Executes before test
        initData();
        buildStateMachine();
        /*    genericInit();*/
        if (sem_init(&smSem, 0, 1) != 0) {
            FAIL();
        }
        if (pthread_create(&smThread, NULL, &StateTest::stateMachineLoop, NULL) != 0)
            FAIL();
    }

    void TearDown()
    {
        // Executes after test
    }

    void genericInit(char* name)
    {
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
    stateMachine_t stateMachine;
};

TEST_F(StateTest, CreateState)
{
}
