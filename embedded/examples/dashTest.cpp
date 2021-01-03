#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <TelemetryLoop.h>
#include "hv_iox.h"
extern "C"
{
#include "motor.h"
#include "motor.h"
#include "proc_iox.h"
#include "data.h"
#include "can_devices.h"
#include "state_machine.h"
}

HVIox hv_iox;

int init()
{
  initData();
  SetupTelemetry((char *)"127.0.0.1", 33333);
}
int main()
{
  init();
  while (1)
    ;
}
