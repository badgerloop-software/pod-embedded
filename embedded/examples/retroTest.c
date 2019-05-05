#include <stdio.h>
#include <stdlib.h>
#include <bbgpio.h>
#include <retro.h>
#include <data.h>

extern data_t *data;

static void initTest() {
	data = malloc(sizeof(data_t));
	data->motion = malloc(sizeof(motion_t));
	data->timers = malloc(sizeof(timers_t));

	data->timers->lastRetro1 = time(NULL);
	data->timers->lastRetro2 = time(NULL);
	data->timers->lastRetro3 = time(NULL);

	data->motion->retroCount = 0;

}

int main() {
	initTest();
	initRetro();
}
