#include <stdio.h>
#include <stdlib.h>
#include <bbgpio.h>
#include <retro.h>
#include <data.h>
#include <sys/time.h>

extern data_t *data;

static void initTest() {
	data = malloc(sizeof(data_t));
	data->motion = malloc(sizeof(motion_t));
	data->motion->vel = 3168.0;	/* 3168.0 in/s == 180 mph */
	data->timers = malloc(sizeof(timers_t));


	gettimeofday(&(data->timers->lastRetro1), NULL);
	gettimeofday(&(data->timers->lastRetro2), NULL);
	gettimeofday(&(data->timers->lastRetro3), NULL);

	data->motion->retroCount = 0;

}

int main() {
	initTest();
	initRetros();
	sleep(100);
	joinRetroThreads();
}
