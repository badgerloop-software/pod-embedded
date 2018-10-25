#include <stdio.h>
#include <inttypes.h>
#include "imu.h"
#include "lib/core.h"
#include "lib/i2c.h"

int main(int argc, char *argv[]) {
	
	uint8_t setConfiguration[] = {0xFA, 0xFF, 0xC0, 0x28, 0x10, 0x20, 0xFF, 0xFF, 0x10, 0x60, 0xFF, 0xFF, 0x20, 0x10, 0x00, 0x64, 0x40, 0x20, 0x01, 0x90, 0x80, 0x20, 0x01, 0x90, 0xC0, 0x20, 0x00, 0x64, 
	0xE0, 0x20, 0xFF, 0xFF, 0x50, 0x22, 0x00, 0x64, 0xD0, 0x12, 0x00, 0x64, 0x73};
	
	uint8_t test1[] = {0xFA, 0xFF, 0x3E, 0x00, 0x01};
	uint8_t test2[] = {0xFA, 0xFF, 0x3E, 0x00, 0x18};
	uint8_t r[] = {0xFA, 0x01, 0x34, 0x00, 0xCB};
	
	uint8_t d[] = {0x03, 0x12, 0x00, 0xEF};
	
	
	init_bbc_lib();
	
	i2c_properties *i2c = malloc(sizeof(i2c_properties));
	i2c->i2cnr = 2;
	i2c->deviceAddress = 0x1D;
	i2c->openMode = O_RDWR;
	
	if (open_i2c(i2c) == -1) {
		syslog(LOG_ERR, "%s", "Could not open i2c bus.");
		return 0;
	}
	
	unsigned char c[20];

	
	uint8_t testConfig[] = {0x03, 0x12, 0x00, 0xEF};
	

	
	while(1){
//		for(int i = 0; i < 4; i++){
//			write_data_i2c(i2c, 0x00, testConfig[i]);
//			usleep(100);
//		}
		write_byte_i2c(i2c, 0x04);
		unsigned char res1[4];
		read_i2c(i2c, res1, 4);
		
		int notifSize = res1[0] | res1[1] << 8;
		int mSize = res1[2] | res1[3] << 8;
		
		printf("N Size: %i M Size: %i\n", notifSize, mSize);
		
		write_byte_i2c(i2c, 0x05);
		unsigned char res2[notifSize];
		read_i2c(i2c, res2, notifSize);
		
//		for(int i = 0; i < notifSize; i++){
//			printf("0x%02x ", res2[i]);
//		}
//		printf("\n");
		
		write_byte_i2c(i2c, 0x06);
		unsigned char res3[mSize];
		read_i2c(i2c, res3, mSize);
		
		for(int i = 0; i < mSize; i++){
			printf("0x%02x ", res3[i]);
			if(res3[i] == 0x40 && res3[i+1] == 0x10){
				printf("RIGHT HERE SIR");
			}
		}
		printf("\n");
			

//		for(int j = 0; j < 20; j++){
//			printf("0x%02x ", c[j]);
//		}
//		printf("\n");
		
		usleep(100000);
		
		
		
		
		
		
	}
		
	//free(readBuffer);
	free(i2c);
}