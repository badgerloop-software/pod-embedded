#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <math.h>
#include "i2c.h"

static const char MCP_ADRESS = 0x21; // default address for GPIO extender

i2c_settings * i2c;
i2c = (i2c_settings *) malloc(sizeof(i2c_settings));

setUpMCP(i2c, MCP_ADRESS);

if (getDir(i2c, B, 4) != 0) {
  fprintf(stderr, "GETDIRNOTWORKING");
}

if (getState(i2c, A, 7) != 0) {
  fprintf(stderr, "GETSTATENOTWORKING");
}

setDir(i2c, B, 0, 1);
if (getDir(i2c, B, 0) != 1){
  fprintf(stderr, "SETDIRNOTWORKING");
}

getDir(i2c, A, 2, 1);
if (getDir(i2c, A, 2) != 1) {
  fprintf(stderr, "SETSTATENOTWORKING");
}
