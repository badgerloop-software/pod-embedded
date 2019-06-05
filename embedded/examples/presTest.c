#include <stdio.h>
#include <braking.h>
#include <data.h>

int main() {
    initData();
    initPressureMonitor();
    joinPressureMonitor();
}
