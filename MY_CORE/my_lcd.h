#ifndef __MY_LCD_H
#define __MY_LCD_H
#include "lcd.h"





#define  PAGE_MAIN    0x01		//主页
#define  PAGE_GSET    0x02		//高级设置
#define  PAGE_PASS    0x03		//密码





			//屏幕初始化
void my_lcd_init (void);


			//进入一个页面
void lcd_page (u8 page);

				//改变一个开关的状态
void lcd_chan_state (u8 type,u8 state);


void Set_Init(void);//设置初始化

void Hand_Set_Init(void);//手动设置初始化按键

void lcd_set_data (u8 type,u16 data);

void Set_Save(void);//设置页面保存

void Auto_sleep();//节能时间段
					//屏幕消息处理，02
void lcd_messeg_02 (u8 *meg);
void	lcd_messeg_03 (u8 *meg);
			//发出比比声
void lcd_beep(u8 l);

				//屏幕上传温湿度数据
void lcd_dataUp (u8 *);

						//屏幕睡眠按键
void lcd_sleep(void);

void my_lcd (void *);

void Time_Set();//休眠时间设置保存


void BUT_UP_TEMP(void);//手动设置升温按键
void BUT_DOWN_TEMP(void);//手动设置降温按键
void BUT_UP_HUMI(void);
void BUT_DOWN_HUMI(void);
void BUT_PM(void);
				//手动自动转换
void AutoCtr(void);

void Into_Senior(u8 *buf);//进入高级设置

void lcd_timetosleep(void);//自动开关屏，大约5秒判断一次


				//立即睡眠
void LCD_tosleep (void);

				//立即亮屏
void LCD_tolight (void);

//u16 Lcd_GetLimitData (u16 type);

//u16 Lcd_SetLimitData (u16 type,u16 newlimit);

#endif


