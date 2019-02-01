
#ifndef __BEEP_H
#define __BEEP_H

#include "sys.h"
#include "stm32f10x_gpio.h"



#define BEEP PCout(13)

typedef struct 
{
	char tone[4];//������1_������1��������1~������ 0����Ъ
	char time[4];//1/8��8��֮һ����1/4���ķ�֮һ��
} jianpu;


void BEEP_Init(void);

void Beep_Play (jianpu *jianpu_);

void set_jianpu(jianpu *j,char *tone_,char *time_);
void jianpu_end(jianpu *j);

void Load_song(void);		//����·�ںη�
void Load_song1(void);	//��˼
void Load_song2(void);	//�����Ϻ�ͼ
void Load_song3(void);	//��ʦ
void Load_ShenPiZi (void);//��Ƨ��
	
void Load_up(void);
void Load_down(void);


#endif



