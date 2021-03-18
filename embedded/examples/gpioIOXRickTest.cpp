#include <iox_rick.h>

extern "C" {
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
}

iox_rick ioxRick;
int main()
{
    initData();
    ioxRick.init(false);
    i2c_settings iox = iox_rick.getiox_rickDev();
    printf("---Showing HV IOX---\n");
    for (int i = 0; i < 16; i++) {
        printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
    }

    initProcIox(true);
    iox = getProcIoxDev();
    printf("--Showing Proc IOX--\n");
    for (int i = 0; i < 16; i++) {
        printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
    }
    return 0;
}

