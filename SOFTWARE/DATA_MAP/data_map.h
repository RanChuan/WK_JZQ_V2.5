
#ifndef __DATA_MAP_H
#define __DATA_MAP_H

#include "stmflash.h"


					//保存屏幕配置的数据长度
#define LCD_DATA_NUM 20

					//无线配置数据长度
#define CONFIG_DATA_NUM 200


					//配置参数总长度
#define ALL_PARA_LENGTH (LCD_DATA_NUM+CONFIG_DATA_NUM)


//定义一个总的存储配置信息的变量数组
extern u16 ALL_PARAMETERS[ALL_PARA_LENGTH];

					//保存屏幕的配置
#define LCD_DATA (&ALL_PARAMETERS[0])

						//保存无线配置
#define EN_CONFIG (&ALL_PARAMETERS[LCD_DATA_NUM])



//我加的用于存储配置信息的函数
void Save_Config (void);
void Load_Config(void);
						//保存屏幕的数据
void Save_LCD (void);

void Load_LCD(void);





#endif






