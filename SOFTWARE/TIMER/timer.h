
#ifndef __TIMER_H
#define __TIMER_H

#include "includes.h"


void TIME4_Init(void);//按键扫描，系统运行时间

					//定时器3输出调光的PWM
void TIME3_Init(void);


void TIME2_Init(void);//蜂鸣器播放


void RunTime_IRQHandler(void);

			//获取系统运行时间
u32 getSysRunTime(void);


#endif



