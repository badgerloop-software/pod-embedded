# Peripherals
*Developers: Ezra Boley, Rohan Daruwala*

### Platform:
BeagleBone Black

## XSens MTi-1 IMU Firmware for BeagleBone Black
*Developer: Rohan Daruwala*
	
### How it works:
	
Start the IMU process by calling the SetupIMU function:
	
```
SetupIMU();
```
	
Next, you can pull data by using any of the following functions:

```C
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


## Retroreflective Sensor API for Beaglebone Black
*Developer: Ezra Boley*

### How it works:

All that needs to be run is:
```C
int initRetros();
```

Then a number of threads will be kicked off based on how many retro
reflective sensors are configured in `peripherals/include/retro.h`.

After that if there is a need to stop listening for interrupts on those
pins, the retro sensors have their own join function:

```C
int joinRetroThreads();
```

This will signal all of the threads that they should stop and then block
    until that is the case.


### Notes:

To use the BeagleBones sysfs gpio interface, the calling program must be run
    with sudo permissions.

