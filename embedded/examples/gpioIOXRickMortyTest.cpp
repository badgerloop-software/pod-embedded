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
int main()
{
    initData();
    if (ioxRick.init(false) != 0)
        std::count << "ioxRick doesn't return 0" << std::endl;
    i2c_settings iox = ioxRick.getiox_rickDev();
    printf("---Showing IOX_RICK---\n");
    for (int i = 0; i < 16; i++) {
        printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
    }

    ioxMorty.init(false);
    iox = ioxMorty.getiox_mortyDev();
    printf("---Showing IOX_MORTY---\n");
    for (int i = 0; i < 16; i++) {
        printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
    }

}

