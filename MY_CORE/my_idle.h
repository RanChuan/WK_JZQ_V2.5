
#ifndef __MY_IDLE_H
#define __MY_IDLE_H


void idle_task(void *t);


u32 getIdleTask(void);

void getKilledTask(u32 *lasttime,u32 *dietimes,u8 pro);


#endif


