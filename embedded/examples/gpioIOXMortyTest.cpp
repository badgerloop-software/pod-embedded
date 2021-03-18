#include <iox_morty.h>

extern "C" {
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
}

iox_morty ioxMorty;
int main()
{
    initData();
    ioxMorty.init(false);
    i2c_settings iox = ioxMorty.getiox_mortyDev();
    printf("---Showing IOX_RICK---\n");
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