
#ifndef __MY_LCD_NEW_H
#define __MY_LCD_NEW_H

#include "includes.h"

void my_lcd_new(void *t);




	//lcd加载界面
void lcd_load(void);
		//向导1
void lcd_wizard1(void);
		//向导2
void lcd_wizard2(void);
			//主页
void lcd_main(void);
				//手动稻作
void lcd_operation(void);
				//设置选择
void lcd_setchoose(void);
				//电源
void lcd_power(void);

				//常规设置
void lcd_generalSet(void);

				//节能设置
void lcd_energySaving(void);

				//时间设置
void lcd_timeSet(void);

				//环境控制
void lcd_environmentalControl(void);

				//密码界面
void lcd_passwordInterface(void);

				//关于界面
void lcd_about (void);

				//高级设置
void lcd_advancedSet(void);



/////////////////////一些辅助函数///////////////////////////////

//转换页面
void lcd_turn_page(u8 page);

//改变字符串变量值，地址，数据，数据长度
void lcd_change_str(u16 addr,char *data,u16 len);

//改变一个变量
void lcd_change_value(u16 addr,u16 value);


//处理串口屏回传的按键数据，非零，成功，0，失败
u16 lcd_deal_key(u8 *data);

//处理串口屏回传的字符串数据，1，成功，0，失败
u16 lcd_deal_str(u8 *data,u8 *strout);


		//发送控制设备命令到设备
void lcd_msg_to_rf (u8 devtype,u8 key_state);


#endif




