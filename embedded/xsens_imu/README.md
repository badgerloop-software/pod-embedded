# XSens MTi-1 IMU Firmware for BeagleBone Black
*Developer: Rohan Daruwala*

### Platform: 
	BeagleBone Black
	
	
### How it works:
	
Start the IMU process by calling the SetupIMU function:
	
```
setupIMU();
```
	
Next, you can pull data by using any of the following functions:

```
void SetupIMU();
void *IMULoop();

void getDeltaVData(float *fData);
float getDeltaVX();
float getDeltaVY();
float getDeltaVZ();

void getAccelData(float *fData);
float getAccelX();
float getAccelY();
float getAccelZ();

void getDeltaOrientationData(float *fData);
float getOrientationQ0();
float getOrientationQ1();
float getOrientationQ2();
float getOrientationQ3();
```
	
An example using all of these functions has been provided. 
