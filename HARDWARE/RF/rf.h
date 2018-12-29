#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include "data_map.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define RF_AUX PAin(8)
#define RF_M0 PAout(12)
#define RF_M1 PAout(11)
#define USART_REC_LEN  			50  	//定义最大接收字节数 200






				//超时时间
#define TIME_OUT  10
					//临时数组长度
#define BUFF_LENGTH 40

#define DEVICE_DATA_NUM 20


					//设备状态
#define DEVICEON 				0x0100
#define DEVICEUP 				0x0200
#define DEVICEDOWN 			0x0400
#define DEVICEOFFLINE 	0x0800


						//设备数量，按照数量，类型的格式
						//这个数组通过初始化函数填充
extern u16 DEVICE_NUM[DEVICE_DATA_NUM];


extern u8 EN_DATA[25];




//------------------------供外部调用的函数---------------------------


					//获取采集器数据首地址
u8 *GetCJDataAddr (void);

					//获取本机地址
u16 Get_MyAddr(void);

					//设置本机地址
void Set_MyAddr(u16 addr);

				//设置设备参数，格式是按照设备地址高8位，低8位，类型,len,设备个数
void Set_Device(u8 * data,u16 len);

				//设置ip地址
void Set_MyIP (u8 *data);
					//获取ip地址
u16 * Get_MyIP (void);





//------------------------供外部调用的函数End---------------------------







				//设置无线发送焦点
void RF_SetFocus(u8 focus);




void RF_Init(u32 bound);//初始化
u16 RF1_tx_bytes( uint8_t* TxBuffer, uint8_t Length );
u16 RF_GetCmd (uint8_t *buff,u16 len);


				//设置无线配置的时候用
u16 RF_Get (u8 *buff,u16 len);

u16  Get_Mycmd(u8 *data);
void Clear_Mycmd(void);

	//设置信道
u8 RF_SetChannel(u8 channel);



				//复制数据
void copy_data (u8 *from,u8 *to,u16 length);



#endif
