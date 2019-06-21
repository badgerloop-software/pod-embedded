#include <stdio.h>
#include <data.h>
#include <retro.h>
#include <imu.h>
#include <string.h>

#define ROLL "roll"
#define EXP  "exp"
#define RAW  "raw"

extern void initNav();

int main(int argc, char *argv[]) {
    initData();
    SetupIMU();
    initRetros();
    initNav();
    

    joinRetroThreads();
}
