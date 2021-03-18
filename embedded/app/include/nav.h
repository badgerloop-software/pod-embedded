#ifndef __NAV_H__
#define __NAV_H__

void initNav(void);

void showNavData(void);

typedef struct rawMotionNode_t {
    float *data;
    int head;
    int size;
} rawMotionNode_t;

typedef struct rawMotion_t {
   rawMotionNode_t pos;
   rawMotionNode_t vel;
   rawMotionNode_t accel;
} rawMotion_t;






#endif
