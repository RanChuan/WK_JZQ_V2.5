
#ifndef __TIMER_H
#define __TIMER_H

#include "includes.h"


void TIME4_Init(void);
					//定时器3输出调光的PWM
void TIME3_Init(void);


void RunTime_IRQHandler(void);

			//获取系统运行时间
u32 getSysRunTime(void);


#endif



