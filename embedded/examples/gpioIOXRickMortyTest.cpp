#include <iox_rick.h>
#include <iox_morty.h>

extern "C" {
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
}

iox_morty ioxMorty;
iox_rick ioxRick;
int testDirs(int dir, i2c_settings iox) {
    printf("IIC settings:\nbus: %d\ndeviceAddress: %c\nopenMode %d",dir, iox.bus, iox.deviceAddress, iox.openMode);
    printf("Setting dir to %d\n",dir);
    for (int i = 0; i < 16; i++) {
        setState(&iox, i, dir);
        setDir(&iox, i, dir);
    }
}
int main()
{
    initData();
    if (ioxRick.init(false) != 0)
            printf("ioxRick.init returned !0");
    i2c_settings iox = ioxRick.getiox_rickDev();
    testDirs(1, iox);
    printf("---Showing IOX_RICK---\n");
    for (int i = 0; i < 16; i++) {
        printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
    }



    if (ioxMorty.init(false))
            printf("ioxMorty return !0");
    iox = ioxMorty.getiox_mortyDev();
    testDirs(0, iox);
    printf("---Showing IOX_MORTY---\n");
    for (int i = 0; i < 16; i++) {
        printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
    }

}

