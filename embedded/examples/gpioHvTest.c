#include <stdio.h>
#include <hv_iox.h>
#include <mcp23017.h>
#include <proc_iox.h>
#include <i2c.h>
#include <data.h>

int main() {
	initData();
    initHVIox();
	i2c_settings iox = getHVIoxDev();
	printf("---Showing HV IOX---\n");
	for (int i = 0; i < 16; i++) {
		printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
	}

	initProcIox();
	iox = getProcIoxDev();
	printf("--Showing Proc IOX--\n");
	for (int i = 0; i < 16; i++) {
		printf("PIN: %d, VAL: %d, DIR: %d\n", i, getState(&iox, i), getDir(&iox, i));
	}
	return 0;
}
