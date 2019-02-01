
#ifndef __BEEP_H
#define __BEEP_H

#include "sys.h"
#include "stm32f10x_gpio.h"



#define BEEP PCout(13)

typedef struct 
{
	char tone[4];//声调，1_低音，1，中音，1~，高音 0，间歇
	char time[4];//1/8，8分之一音，1/4，四分之一音
} jianpu;


void BEEP_Init(void);

void Beep_Play (jianpu *jianpu_);

void set_jianpu(jianpu *j,char *tone_,char *time_);
void jianpu_end(jianpu *j);

void Load_song(void);		//敢问路在何方
void Load_song1(void);	//相思
void Load_song2(void);	//清明上河图
void Load_song3(void);	//琴师
void Load_ShenPiZi (void);//生僻字
	
void Load_up(void);
void Load_down(void);


#endif



