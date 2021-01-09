extern "C"
{
#include <stdio.h>
#include <data.h>
#include <retro.h>
#include <string.h>
}
#include <imu.h>
#include <nav.h>
#include <TelemetryLoop.h>
#include "connStat.h"
#define ROLL "roll"
#define EXP "exp"
#define RAW "raw"

extern void initNav();

int main(int argc, char *argv[])
{
  initData();
/*  SetupIMU();*/
  initRetros();
  initNav();
  
  SetupTelemetry((char *)DASHBOARD_IP, 33333);
  joinRetroThreads();

  while(1);
}
