
#include "data_map.h"

/************************************

这个文件描述系统参数的存储空间分配
所有需要用到FLASH存储的参数
内存变量在这个文件定义
包括：
		屏幕设置参数
		无线配置参数
		网络配置参数

**************************************/

//定义一个总的存储配置信息的变量数组
u16 ALL_PARAMETERS[ALL_PARA_LENGTH];


//u16 LCD_DATA[LCD_DATA_NUM]={0};


			//环境变量配置项,集中器地址，采集器地址，控制器地址
			//按照[0]表示设备地址，[1]低8位表示设备类型格式，高8位是设备状态
			//设备状态位【0】表示开关状态，【1,2】位表示升降状态，【3】位表示离线状态,
			//设备地址按照集中器，采集器，控制器的顺序
//u16 EN_CONFIG[CONFIG_DATA_NUM]={1,0,2,1,3,3,4,2,5,4,6,5};

 
 
 
 
#define FLASH_SAVE_ADDR  (0x8040000-1024)
#define FLASH_LCD_ADDR FLASH_SAVE_ADDR
#define FLASH_CFG_ADDR (FLASH_SAVE_ADDR+LCD_DATA_NUM*2)
#define DEVICEOFFLINE 	0x0800


void Save_Config (void)
{
	STMFLASH_Write(FLASH_CFG_ADDR,EN_CONFIG,CONFIG_DATA_NUM);
}

void Load_Config(void)
{
	u16 i=0;
	STMFLASH_Read(FLASH_CFG_ADDR,EN_CONFIG,CONFIG_DATA_NUM);
	for (i=0;i<CONFIG_DATA_NUM;i++)
	{
		if (EN_CONFIG[i]==0xffff) EN_CONFIG[i]=0;
		if (i&1) {EN_CONFIG[i]&=0x00ff;//初始化之后，把设备状态为0，2018.8.18
			if (EN_CONFIG[i]&0x00ff) EN_CONFIG[i]|=DEVICEOFFLINE;//初始化为离线状态，2018.8.22
		}
	}
	EN_CONFIG[1]|=0x0100;//集中器初始化为开启
}
 
 
				//保存手动设置数据位
#define CHUSHI_STATE 			LCD_DATA[14]
#define KONGTIAO_STATE 		LCD_DATA[15]
#define JINHUA_STATE  		LCD_DATA[16]
#define HAND_STATE   			LCD_DATA[17]

						//保存屏幕的数据
void Save_LCD (void)
{
	STMFLASH_Write(FLASH_LCD_ADDR,LCD_DATA,LCD_DATA_NUM);
}


void Load_LCD(void)
{
	u16 i=0;
	u16 tt[4]={0};
	tt[0]=CHUSHI_STATE;
	tt[1]=KONGTIAO_STATE;
	tt[2]=JINHUA_STATE;
	tt[3]=HAND_STATE;
	STMFLASH_Read(FLASH_LCD_ADDR,LCD_DATA,LCD_DATA_NUM);
	for (i=0;i<LCD_DATA_NUM;i++)
	{
		if (LCD_DATA[i]==0xffff)
			LCD_DATA[i]=0;
	}
	CHUSHI_STATE=tt[0];
	KONGTIAO_STATE=tt[1];
	JINHUA_STATE =tt[2];
//	HAND_STATE  =tt[3];注释这一行手动自动模式掉电保存
	
}
 
 

