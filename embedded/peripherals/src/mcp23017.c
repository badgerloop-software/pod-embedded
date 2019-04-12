#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <math.h>
#include "i2c.h"

static const char IODIRA = 0x00; // Port A direction address
static const char IODIRB = 0x01; // Port B direction address
static const char GPIOA = 0x12; // Port A state address
static const char GPIOB = 0x13; // Port B state address

void setupMCP(i2c_settings * i2c, char mcp_address) {

  i2c->bus = 2;
  i2c->address = mcp_address;
  i2c->openMode = O_RDWR;

  if (i2c_begin(i2c) == -1) {
    fprintf(stderr, "Could not open i2c bus.\n");
    return;
  }

  // Set IODIRA/B to make all pins configured as output
  write_data_i2c(i2c, IODIRA, 0x00);
  write_data_i2c(i2c, IODIRB, 0x00);
}

// helper method for getting data from a specific address
unsigned uint8_t getFromAddress(i2c_settings * i2c, unsigned char address, uint8_t pin) {
	write_byte_i2c(i2c, address);

	unsigned uint8_t dataBuffer[1];
	read_i2c(i2c, dataBuffer, 1);

	return (dataBuffer[0] >> pin) & 1; // getting the digit corresponding to the pin
}

unsigned uint8_t getState(i2c_settings * i2c, char port, uint8_t pin) {
  unsigned char address;
  if (port == 'A') {
    return getFromAddress(i2c, GPIOA, pin);
  } else if (port == 'B') {
	return getFromAddress(i2c, GPIOB, pin);
  }
  fprintf(stderr, "Invalid port input getState\n");
  return 0;
}

void setState(i2c_settings * i2c, char port, uint8_t pin, uint8_t val) {
  unsigned char address;
  if (port == 'A') {
    address = GPIOA;
  } else if (port == 'B') {
    address = GPIOB;
  } else {
	fprintf(stderr, "Invalid port input. Try 'A' or 'B'\n");
    return;
  }

  unsigned uint8_t currentState = getState(i2c, port, pin);
  if (val == currentState) { // no changes to be made
    return;
  }

  unsigned uint8_t newState;
  if (val == 0) { // change it form 1 to 0
	write_data_i2c(i2c, address, currentState - pow(2, pin));
  } else if (val == 1) { // change it from 0 to 1
	write_data_i2c(i2c, address, currentState + pow(2, pin));
  } else {
    fprintf(stderr, "Invalid val arg setState");
  }
}

unsigned uint8_t getDir(i2c_settings * i2c, char port, uint8_t pin) {
  unsigned char address;
  if (port == 'A') {
	return getFromAddress(i2c, IODIRA, pin);
  } else if(port == 'B') {
	return getFromAddress(i2c, IODIRB, pin);
  }
  fprintf(stderr, "Invalid port input. Try 'A' or 'B'\n");
  return 0;
}

void setDir(i2c_settings * i2c, char port, uint8_t pin, uint8_t val) {
  unsigned char address;
  if (port == 'A') {
    address = IODIRA;
  } else if (port == 'B') {
    address = IODIRB;
  } else {
	fprintf(stderr, "Invalid port input. Try 'A' or 'B'\n");
    return;
  }

  unsigned uint8_t currentState = getDir(i2c, port, pin);
  if (val == currentState) {
    return;
  }

  unsigned uint8_t newState;
  if (val == 0) { // change it form 1 to 0
	write_data_i2c(i2c, address, currentState - pow(2, pin));
  } else if (val == 1) { // change it form 0 to 1
	write_data_i2c(i2c, address, currentState + pow(2, pin));
  } else {
    fprintf(stderr, "Invalid val arg setDir");
  }
}
