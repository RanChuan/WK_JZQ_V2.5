
#ifndef __TIMER_H
#define __TIMER_H

#include "includes.h"


void TIME4_Init(void);//����ɨ�裬ϵͳ����ʱ��

					//��ʱ��3��������PWM
void TIME3_Init(void);


void TIME2_Init(void);//����������


void RunTime_IRQHandler(void);

			//��ȡϵͳ����ʱ��
u32 getSysRunTime(void);


#endif



