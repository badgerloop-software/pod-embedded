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

int main(int argc, char *argv[]) {
    initData();
    initLVIox(isHard(argc, argv));

    int sol = whichSolenoid(argc, argv);
    if (sol < 0) return -1;
    int state = whatToDo(argc, argv);
    if (state < 0) return -1;
    solenoidSet(sol, state);
    return 0;
}
