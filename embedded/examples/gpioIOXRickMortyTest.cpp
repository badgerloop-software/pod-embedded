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
// sets every pin to ouput then changes vals specified by argv
int testDirs(int lvl, i2c_settings iox) {
    printf("IIC settings:\nbus: %d\ndeviceAddress: %X\nopenMode %d\n", iox.bus, iox.deviceAddress, iox.openMode);
    for (int i = 0; i < 16; i++) {
        setDir(&iox, i, MCP_DIR_OUT);
        setState(&iox, i, lvl);
    }
}
int main(int argc, char **argv)
{
    initData();
    ioxRick.init(false);
    i2c_settings iox = ioxRick.getiox_rickDev();
    if (argc == 2)
        testDirs(atoi(argv[1]), iox);
    printf("---Showing IOX_RICK---\n");
    for (int i = 0; i < 16; i++) {
        printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
    }
    ioxMorty.init(false);
    iox = ioxMorty.getiox_mortyDev();
    if (argc == 2)
        testDirs(atoi(argv[1]), iox);
    printf("---Showing IOX_MORTY---\n");
    for (int i = 0; i < 16; i++) {
        printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
    }

}

