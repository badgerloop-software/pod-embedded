#include <stdio.h>
#include <inttypes.h>
#include "imu.h"
#include "lib/core.h"
#include "lib/i2c.h"



int main(int argc, char *argv[]) {
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
		write_byte_i2c(i2c, 0x04);
		unsigned char res1[4];
		read_i2c(i2c, res1, 4);
		
		//Get buffer sizes
		//int notifSize = res1[0] | res1[1] << 8;
		int mSize = res1[2] | res1[3] << 8;
		
		
		//Notification buffer, ignoring for now
		/*
		write_byte_i2c(i2c, 0x05);
		unsigned char res2[notifSize];
		read_i2c(i2c, res2, notifSize);*/
		
		// Get Data
		write_byte_i2c(i2c, 0x06);
		unsigned char res3[mSize];
		read_i2c(i2c, res3, mSize);
		
		for(int i = 0; i < mSize; i++){
			//printf("0x%02x ", res3[i]);
			if(res3[i] == 0x40 && res3[i+1] == 0x10){
				//i+2 is length, which is 12
				//i+3, i+4, i+5, i+6 is one
				// i+7, i+8, i+9 i+10 is two
				// i+11, i+12, i+13, i+14 is three
				
				uint32_t tempx = (res3[i+3] << 24) | (res3[i+4] << 16) | (res3[i+5] << 8) | res3[i+6];
				uint32_t tempy = (res3[i+7] << 24) | (res3[i+8] << 16) | (res3[i+9] << 8) | res3[i+10];
				uint32_t tempz = (res3[i+11] << 24) | (res3[i+12] << 16) | (res3[i+13] << 8) | res3[i+14];
				
				//Time increment is 10ms
				float dVx = *((float*) &tempx);
				float dVy = *((float*) &tempy);
				float dVz = *((float*) &tempz);
				
				float accelX = dVx / 0.01;
				float accelY = dVy / 0.01;
				float accelZ = dVz / 0.01;
				
				printf("X: %f, Y: %f, Z: %f\n", accelX, accelY, accelZ);
			}
		}
		printf("\n");
		
		//Arbitrary sleep
		usleep(100000);
		
		
		
		
		
		
	}
		
	//free(readBuffer);
	free(i2c);
}