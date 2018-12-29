#ifndef __KEY_H
#define __KEY_H
#include "delay.h"
#include "sys.h"

//按键长按短按区分
#define PRESS_SHORT 1//短按弹起
#define PRESS_LONG 2	//达到长按
#define PRESS_DOWN 3//按下
#define PRESS_NONE 0




void Key_Init (void);

void TIME4_Init(void);

u8 Get_Key(u8 keynum);















#endif




