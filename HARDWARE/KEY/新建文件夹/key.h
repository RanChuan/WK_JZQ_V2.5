#ifndef __KEY_H
#define __KEY_H
#include "delay.h"
#include "sys.h"

//���������̰�����
#define PRESS_SHORT 1
#define PRESS_LONG 2
#define PRESS_NONE 0




void Key_Init (void);

void TIME4_Init(void);

u8 Get_Key(u8 keynum);















#endif




