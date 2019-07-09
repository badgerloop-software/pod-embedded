extern "C"
{
#include <stdio.h>
#include <data.h>
#include <retro.h>
#include <imu.h>
#include <string.h>
}
#include <LVTelemetry_Loop.h>

#define ROLL "roll"
#define EXP "exp"
#define RAW "raw"

extern void initNav();

int main(int argc, char *argv[])
{
  initData();
  SetupIMU();
  initRetros();
  initNav();
  SetupLVTelemetry((char *)"127.0.0.1", 33333);

  joinRetroThreads();
}
