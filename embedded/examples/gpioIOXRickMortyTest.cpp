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
    printf("IIC settings:\nbus: %d\ndeviceAddress: %X\nopenMode %d",dir, iox.bus, iox.deviceAddress, iox.openMode);
    printf("Setting dir to %d\n",dir);
    for (int i = 0; i < 16; i++) {
        setDir(&iox, i, MCP_DIR_OUT);
        setState(&iox, i, dir);
    }
}
int main()
{
    initData();
    if (ioxRick.init(false) != 0)
            printf("ioxRick.init returned !0\n");
    i2c_settings iox = ioxRick.getiox_rickDev();
    printf("Writing all vals in Rick to low...");
    testDirs(0, iox);
    printf("Writing all vals in Rick to high...");
    testDirs(1, iox);
    printf("---Showing IOX_RICK---\n");
    for (int i = 0; i < 16; i++) {
        printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
    }



    if (ioxMorty.init(false))
            printf("ioxMorty.init returned !0\n");
    iox = ioxMorty.getiox_mortyDev();
    printf("Writing all vals in Morty to low...");
    testDirs(0, iox);
    printf("Writing all vals in Morty to high...");
    testDirs(1, iox);
    printf("---Showing IOX_MORTY---\n");
    for (int i = 0; i < 16; i++) {
        printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
    }

}

