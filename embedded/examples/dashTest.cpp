#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <HVTelemetry_Loop.h>
#include <LVTelemetry_Loop.h>
extern "C" 
{
    #include "motor.h"
    #include "hv_iox.h"
    #include "motor.h"
    #include "proc_iox.h"
    #include "data.h"
    #include "can_devices.h"
    #include "state_machine.h"
}



int init() {
  initData();
  SetupHVTelemetry((char *) "127.0.0.1", 33333);
  // Lv Telemetry will seg fault, idk why.  
  // SetupLVTelemetry((char *) "127.0.0.1", 33335);
}
int main() {
init();
while (1);
}
