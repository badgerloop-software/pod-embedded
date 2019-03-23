# XSens MTi-1 IMU Firmware for BeagleBone Black
*Developer: Rohan Daruwala*

### Platform: 
	BeagleBone Black
	
	
### How it works:
	
Start the IMU process by calling the SetupIMU function:
	
```
SetupIMU();
```
	
Next, you can pull data by using any of the following functions:

```
void getDeltaVData(float *fData);
float getDeltaVX();
float getDeltaVY();
float getDeltaVZ();

void getAccelData(float *fData);
float getAccelX();
float getAccelY();
float getAccelZ();

void getDeltaOrientationData(float *fData);
float getOrientationdDeltaQ0();
float getOrientationDeltaQ1();
float getOrientationDeltaQ2();
float getOrientationDeltaQ3();
```
	
An example using all of these functions has been provided. 


### Notes:

This currently assumes that you are running on the I2C-2 bus (Meaning that sensors are plugged in on the `P9_19` and `P9_20` pins).