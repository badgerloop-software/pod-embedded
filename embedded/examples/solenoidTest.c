#include <stdio.h>
#include <lv_iox.h>
#include <braking.h>
#include <data.h>
#include <string.h>

int whichSolenoid(int argc, char *argv[]) {
    if (argc <= 1) return -1;
    return atoi(argv[1]);
}

int whatToDo(int argc, char *argv[]) {
    if (argc <= 2) return -1;

    if (strcmp(argv[2], "set") == 0) {
        return 1;
    } else if (strcmp(argv[2], "clr") == 0) {
        return 0;
    }

    return -1;
}

bool isHard(int argc, char *argv[]) {
    if (argc > 1) {
        return strcmp(argv[1], "-h") == 0;
    }
    return false;
}

int convSolenoid(int num) {
    switch(num) {
    case 0:
        return SOLENOID_0;
    case 1:
        return SOLENOID_1;
    case 2:
        return SOLENOID_2;
    case 3:
        return SOLENOID_3;
    case 4:
        return SOLENOID_4;
    case 5:
        return SOLENOID_5;
    case 6:
        return SOLENOID_6;
    case 7:
        return SOLENOID_7;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    initData();
    initLVIox(isHard(argc, argv));

    int sol = convSolenoid(whichSolenoid(argc, argv));
    printf("SOL: %d\n", sol);
    if (sol < 0) return -1;

    int state = whatToDo(argc, argv);
    printf("STATE: %d\n", state);
    if (state < 0) return -1;
    solenoidSet(sol, state);
    return 0;
}
