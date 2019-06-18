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

    if (argc != 2) {
        printf("Try with either 'roll' or 'exp' for different filters\n");
        rawTest();
    } else if (strcmp(argv[1], ROLL) == 0) 
        rollingTest();
    else if (strcmp(argv[1], EXP) == 0)
        expTest();
    else
        rawTest();
}

void rawTest() {
        
}

void rollingTest() {

}

void expTest() {

}

