#ifndef  __MY_LIGHT_H
#define  __MY_LIGHT_H

#include "light.h"







					//亮按键灯
void key_light (u8 key_num,u8 state);

			//按键周围的灯
void key_around (u8 key_num,u8 state);

void led_light (u16 num,u8 state,u8 red,u8 green,u8 blue);


			//流水
void led_run (u8 *meg);

						//键盘周围的灯闪烁
void light_run (u8 *msg);




			//灯带亮纯色
void led_lightall (u8 *meg);

		//灯光任务函数
void my_light (void * t);


#endif

