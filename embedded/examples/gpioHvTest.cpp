#include <hv_iox.h>

extern "C" {
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
}

HVIox hv_iox;
int main()
{
    initData();
    hv_iox.init(false);
    i2c_settings iox = hv_iox.getHVIoxDev();
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
