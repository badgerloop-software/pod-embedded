#include <braking.h>
#include <data.h>
#include <stdio.h>

int main()
{
    printf("INITTING DATA\n");
    initData();
    printf("INITTING PRESSURE MONITOR\n");
    initPressureMonitor();
    printf("JOINING PRESURE MONITOR\n");
    joinPressureMonitor();
}
