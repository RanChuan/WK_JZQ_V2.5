#ifndef  __LIGHT_H
#define  __LIGHT_H
#include "sys.h"
#include "stm32f10x_gpio.h"


#define LED1  0x10
#define LED2  0x20
#define LED3  0x04
#define LED4  0x08
#define LED5  0x02
#define LED6  0x01

#define LIGHT_LED 7
#define LIGHT1 1
#define LIGHT2 2
#define LIGHT3 3
#define LIGHT4 4
#define LIGHT5 5
#define LIGHT6 6

						//灯的个数
#define LED_NUM  1



void Light_init (void);
void HC595_SendData(void);
void on_light(u8 a,u8 b);
void off_light(u8 a,u8 b);
u8 GetLight_State(u8 lednum,u8 ic);
void Run_Light (void);
		//设置亮灯位
void Set_lightdata (u8 *data1);
		//hoqu亮灯位
void Get_lightdata (u8 *data1);


void sm16703_reset (void);
void sm16703_send (u8 red ,u8 green,u8 blue);

//设置灯光的颜色,0,成功，1，失败
u8 led_setcolor (u16 led_num,u8 red ,u8 green,u8 blue);
//获取灯光的颜色,0,成功，1，失败
u8 led_getcolor (u16 led_num,u8 *red ,u8 *green,u8 *blue);
void led_senddata (void);
void led_resetall (void);

void led_setall (u8 red,u8 green,u8 blue);


//新版集中器控制键盘灯的函数2018.11.5
void key_reset(void);
void key_send (u8 red ,u8 green,u8 blue);
u8 key_setcolor (u16 led_num,u8 red ,u8 green,u8 blue);
void key_light (u8 key_num,u8 state);//按键指示灯
void key_around (u8 key_num,u8 state);//按键报警灯
void key_senddata (void);
			//刷新刷新指定缓存区颜色
void key_senddataby (u8 **colors);


#endif


