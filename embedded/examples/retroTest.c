#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <bbgpio.h>
#include <retro.h>
#include <data.h>

static void initTest() {
    initData();
	/*data = malloc(sizeof(data_t));
	data->motion = malloc(sizeof(motion_t));
	data->motion->vel = 3168.0;	 //3168.0 in/s == 180 mph
	data->timers = malloc(sizeof(timers_t));
    int i = 0;
	for (i = 0; i < NUM_RETROS; i++) data->timers->lastRetros[i] = getuSTimestamp();
	data->motion->retroCount = 0;*/
}

static void setSpeed(float vel) {
	data->motion->vel = vel;
}

int main() {
	initTest();
	setSpeed(500);

	initRetros();
while(1);
   
    sleep(5);
	printf("Bumping speed to 500 in/s\n");
	setSpeed(500);
	sleep(5);
	printf("Bumping speed to 1000 in/s\n");
	setSpeed(1000);
	sleep(5);
	printf("Hitting max speed of 3168 in/s\n");
	setSpeed(3168);
	sleep(5);
	joinRetroThreads();
}
