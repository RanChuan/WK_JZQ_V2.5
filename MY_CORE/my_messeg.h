#ifndef __MY_MESSEG_H
#define __MY_MESSEG_H
#include "sys.h"
#include "os_msg.h"


/****************************************************************************
									/																													/
									/			这个文件里定义消息的具体参数代表的含义							/
									/			具体到各个任务之间的通信														/
									/			定义时间2018.11.16																	/
									/			在这个日期以前写的程序消息定义没有使用以下的宏定义	/
									/			导致程序的可读性比较差，以后写的程序消息传递部分使	/
									/			用以下的宏定义，提高程序可读性											/
									/			每个自定义任务都包含这个头文件，这个头文件包含了所	/
									/			有消息传递需要用到的参数														/
									/			
*****************************************************************************/



#define  KEY_MESSEG  1
#define  LCD_MESSEG  2
#define  LIT_MESSEG  4
#define  RF_MESSEG   5
#define  WK_MESSEG   7//网口
#define  CFG_MESSEG  9

//#define  ET_MESSEG   2




//RF通信中的错误类型定义

#define ERR_SUCCESS         0x0000			//成功
#define ERR_FAIL						0x0001			//未知错误
#define ERR_DATAVALE				0x0002			//数据错误
#define ERR_DATALENGTH			0x0003			//数据长度错误
#define ERR_WANTLENGTH			0x0004			//想要读取的数据长度错误
#define ERR_NULLCMD					0x0005			//不支持的命令类型，
#define ERR_NOCONNECT				0x0006			//没有连接，一切操作都要在通信开始之后
#define ERR_NOCJADDR				0x0007			//采集器的地址不是本机绑定的采集器地址
#define ERR_CANNOTCFG				0x0008			//在不是可配置状态下收到了配置命令
#define ERR_NOTMYTYPE				0x0009			//所要控制的设备与本机不符
#define ERR_TIMEOUT					0x000a   		//超时
#define ERR_OFFLINE         0x000b   		//设备不在线
#define ERR_NONEADDR				0x000c      //设备地址不对
#define ERR_ADDRCROSS				0x000d			//添加设备的时候地址超出了地址域





/*************************************************

			按键任务的消息定义：
			meg0，1改变按键状态，
								取值：KEY_CHANGE_STATE
			meg1，改变的按键状态位置
								取值：KEY_WENDU_UP,
											KEY_WENDU_DOWN,
											KEY_SHIDU_UP,
											KEY_SHIDU_DOWN,
											KEY_LCD_STATE,
											KEY_JINHUA
			meg2新状态
								取值：0~255
			

*****************************************************/

#define  KEY_CHANGE_STATE  		 1

#define  KEY_WENDU_UP  			0x01
#define  KEY_WENDU_DOWN 		0x02
#define  KEY_SHIDU_UP  			0x03
#define  KEY_SHIDU_DOWN 		0x04
#define  KEY_LCD_STATE 			0x05
#define  KEY_JINHUA  				0x06

/********************按键任务End******************************/




/*****************************************

		灯光任务的消息定义：
		meg[0],1,键盘上的灯，2，设备状态指示灯3，灯带，4，键盘上的灯闪烁，5,更新按键灯颜色显示
						取值：LIGHT_DEVICE_STATE
									LIGHT_WARN_STATE
									LIGHT_ROUND_LIGHT
									LIGHT_UPDATE_KEY
									
		meg[1],亮或灭,灯带闪烁的类型，1，正向流水，2，反向流水，3，正向双流水，4，反向双流水，5，纯色,
						键盘灯闪烁的时候闪烁的位置
						取值：在meg[0]=LIGHT_DEVICE_STATE或meg[0]=LIGHT_WARN_STATE时
											LIGHT_LIGHT_ON
											LIGHT_LIGHT_OFF
									在meg[0]=LIGHT_ROUND_LIGHT时
											LIGHT_LIGHT_R
											LIGHT_LIGHT_L
											LIGHT_LIGHT_DR
											LIGHT_LIGHT_DL
											LIGHT_LIGHT_COLOR
									在meg[0]=LIGHT_UPDATE_KEY时
											LIGHT_LIGHT_UPDATE
											LIGHT_LIGHT_UPDATEBY
											LIGHT_LIGHT_GETTO
		meg2，亮或灭第几个,灯带闪烁1,开始，0，停止，2，自动换色
						键盘灯闪烁的时候闪烁次数，
						取值：在meg[0]=LIGHT_DEVICE_STATE或meg[0]=LIGHT_WARN_STATE时
											LIGHT_WENDU_UP
											LIGHT_WENDU_DOWN
											LIGHT_SHIDU_UP
											LIGHT_SHIDU_DOWN
											LIGHT_LCD_STATE
											LIGHT_JINHUA
									在meg[0]=LIGHT_ROUND_LIGHT时
											LIGHT_LIGHT_START
											LIGHT_LIGHT_STCOR
											LIGHT_LIGHT_STOP
									在meg[0]=LIGHT_UPDATE_KEY时
											连续4个字节表示缓存区的地址
		meg3,灯带红色
						键盘灯闪烁的时候亮灯时间(单位，10ms)
		meg4，灯带绿色
						键盘灯闪烁的时候灭灯时间
		meg5，灯带蓝色
						键盘灯闪烁的时候启动还是停止

***************************************/


#define LIGHT_DEVICE_STATE  		1//键盘上的灯,设备开启状态指示
#define LIGHT_WARN_STATE  			2//按键周围的灯，报警状态
#define LIGHT_ROUND_LIGHT   		3//周围的环绕灯
#define LIGHT_UPDATE_KEY	   		5//更新按键灯颜色显示

#define LIGHT_LIGHT_ON					1//灯光，开
#define LIGHT_LIGHT_OFF					0//灯光，关
#define LIGHT_LIGHT_R						1//环绕灯光，正向流水
#define LIGHT_LIGHT_L						2//环饶灯光，反向流水
#define LIGHT_LIGHT_DR					3//环绕灯光，正向双流水
#define LIGHT_LIGHT_DL					4//环绕灯光，反向双流水
#define LIGHT_LIGHT_COLOR				5//环绕灯光，纯色
#define LIGHT_LIGHT_UPDATE			1//更新为自身缓存的颜色
#define LIGHT_LIGHT_UPDATEBY		2//更新为指定缓存地址的颜色
#define LIGHT_LIGHT_GETTO				3//获取自身缓存地址的颜色到指定缓存地址

#define  LIGHT_WENDU_UP  			0x01//位置，升温按钮处
#define  LIGHT_WENDU_DOWN 		0x02//位置，降温按钮处
#define  LIGHT_SHIDU_UP  			0x03//位置，加湿按钮处
#define  LIGHT_SHIDU_DOWN 		0x04//位置，除湿按钮处
#define  LIGHT_LCD_STATE 			0x05//位置，LCD屏按钮处
#define  LIGHT_JINHUA  				0x06//位置，空气净化按钮处
#define LIGHT_LIGHT_START				 1//环绕灯光，开始静态环绕
#define LIGHT_LIGHT_STCOR				 2//环绕灯光，开始自动换色环绕
#define LIGHT_LIGHT_STOP				 0//环绕灯光，停止


/********************灯光任务End******************************/



/*****************************************

		屏幕任务的消息定义：
		meg[0],1,显示报警信息，2，设备状态指示3，屏幕电源控制，4，显示温湿度5,发出比比声
						取值：LCD_SHOW_WARN
									LCD_DEVICE_STATE
									LCD_LCD_POWER
									LCD_DATAUP
									LCD_BEEP
		meg[1],开或关，控制屏幕电源时1，开，可控制屏幕亮度，0，电源控制,发出哔哔声时1，短，2，长
						取值：meg[0]=	LCD_DEVICE_STATE时
										LCD_STATE_ON
										LCD_STATE_OFF
									meg[0]=LCD_LCD_POWER时
										LCD_POWER_ON
										LCD_POWER_OFF
									meg[0]=LCD_BEEP时
										LCD_BEEP_SHORT
										LCD_BEEP_LONG
		meg2，亮或灭第几个,控制电源开的时候屏幕亮度
						取值：meg[0]=	LCD_DEVICE_STATE时
										LCD_WENDU_UP
										LCD_WENDU_DOWN
										LCD_SHIDU_UP
										LCD_SHIDU_DOWN
										LCD_JINHUA
										LCD_HAND
									meg[0]=LCD_LCD_POWER且meg[1]=LCD_POWER_ON时
										亮度：0~63
		meg3, 温控高8位
		meg4，温度低8位
		meg5，湿度高8位
		meg6，湿度低8位
		meg7，tvoc高8位
		meg8，tvoc低8位

***************************************/


#define LCD_SHOW_WARN  				1//显示报警信息
#define LCD_DEVICE_STATE     	2//更新设备状态指示
#define LCD_LCD_POWER					3//屏幕电源控制
#define LCD_DATAUP						4//数据上传，温湿度TVOC
#define LCD_BEEP							5//发出比比声

#define LCD_STATE_ON					1//状态开
#define LCD_STATE_OFF					0//状态关
#define LCD_POWER_ON					1//屏幕电源开
#define LCD_POWER_OFF					0//屏幕电源关
#define LCD_BEEP_SHORT				1//发出较短的哔哔声
#define LCD_BEEP_LONG					2//发出较长的哔哔声

#define  LCD_WENDU_UP  			0x01//位置，升温按键
#define  LCD_WENDU_DOWN 		0x02//位置，降温按键
#define  LCD_SHIDU_UP  			0x03//位置，加湿按键
#define  LCD_SHIDU_DOWN 		0x04//位置，除湿按键
#define  LCD_JINHUA  				0x05//位置，净化按键
#define  LCD_HAND  					0x06//位置，手动自动切换按键

/********************LCD任务End******************************/




/***********************************************************

		无线控制任务的消息定义：
		meg[0],设备类型,不能为0，0xff,控制指定地址的设备
				取值：RF_DEVICE_CJ
							RF_DEVICE_KT
							RF_DEVICE_CS
							RF_DEVICE_JH
							RF_DEVICE_JS
							RF_DEVICE_YT
							RF_DEVICE_ADDR
		meg[1],1获取设备状态，2，控制设备,3向集中器添加设备
				取值：RF_CTRL_DEVICE
							RF_GET_DEVICE
							RF_ADD_DEVICE
		meg[2],设备地址高把位
		meg[3],设备地址第八位
		meg[4],如果是控制，开关状态，0，关，1，开，2，反向
				取值：meg[1]=RF_CTRL_DEVICE 时
								RF_DEVICE_ON
								RF_DEVICE_OFF
								RF_DEVICE_CH
		meg[5],如果是控制，升降状态，反向时的按键，1，升，2，降
				取值：meg[1]=RF_CTRL_DEVICE 时
								RF_DEVICE_UP
								RF_DEVICE_DOWN

***********************************************************/

#define RF_DEVICE_CJ				1//设备类型采集器
#define RF_DEVICE_KT				2//设备类型空调控制器
#define RF_DEVICE_CS				3//设备类型除湿机控制器
#define RF_DEVICE_JH				4//设备类型净化机控制器
#define RF_DEVICE_JS				5//设备类型加湿机控制器
#define RF_DEVICE_YT				6//设备类型一体机控制器
#define RF_DEVICE_ADDR	 0xff//不定义设备类型，通过设备地址来确定

#define RF_CTRL_DEVICE			2//控制设备
#define RF_GET_DEVICE				1//获取设备状态
#define RF_ADD_DEVICE				3//向集中器添加设备

#define RF_DEVICE_ON				1//设备开
#define RF_DEVICE_OFF				0//设备关
#define RF_DEVICE_CH				2//反向，开转换为关，关装换为开

#define RF_DEVICE_UP				1//设备状态升
#define RF_DEVICE_DOWN			2//设备状态降

/********************RF任务End******************************/




/*******************************************

			网口W5500消息定义：
			msg[0],1,有中断，2，有消息发送.3重启网口
				取值：WK_INTERRUPT
							WK_SEND_DATA
							WK_REINIT
			msg[1],发送消息时，发送的设备类型
				取值：WK_DEVICE_CJ
							WK_DEVICE_KT
							WK_DEVICE_CS
							WK_DEVICE_JH
							WK_DEVICE_JS
							WK_DEVICE_YT
			msg[2]，设备状态，0，在线，1，离线
				取值：WK_DEVICE_ONLINE
							WK_DEVICE_OFFLINE
			控制器时：msg[1]！=WK_DEVICE_CJ
			msg3,id,高8位
			msg4,id,低8位
			msg5，设备状态，控制状态，1，开
				取值：WK_DEVICE_ON
							WK_DEVICE_OFF
			msg6，控制状态，1，升，2，降
				取值：WK_STATE_UP
							WK_STATE_DOWN
			采集器时：msg[1]=WK_DEVICE_CJ
			msg3~6，采集器数据缓存区地址，由高到低
			

*********************************************/


#define WK_INTERRUPT  1//有中断发生
#define WK_SEND_DATA	2//通过网口发送消息
#define WK_REINIT			3//网口重新初始化

#define WK_DEVICE_CJ				1//设备类型采集器
#define WK_DEVICE_KT				2//设备类型空调控制器
#define WK_DEVICE_CS				3//设备类型除湿机控制器
#define WK_DEVICE_JH				4//设备类型净化机控制器
#define WK_DEVICE_JS				5//设备类型加湿机控制器
#define WK_DEVICE_YT				6//设备类型一体机控制器

#define WK_DEVICE_ONLINE		0//设备在线
#define WK_DEVICE_OFFLINE		1//设备不在线

#define WK_DEVICE_ON				1//设备开启状态
#define WK_DEVICE_OFF				0//设备关闭状态

#define WK_STATE_UP					1//设备升
#define WK_STATE_DOWN				2//设备降

/********************网口任务End******************************/





/*********************************************

		配置任务消息数据位：
		msg0,1，进入配置模式，0，退出配置模式,2，转换当前的状态
				取值：CFG_ENTER_CFGMODE
							CFG_EXIT_CFGMODE
							CFG_CHANGE_CFGMODE

***********************************************/


#define CFG_ENTER_CFGMODE 		1//进入配置模式
#define CFG_EXIT_CFGMODE			0//退出配置模式
#define CFG_CHANGE_CFGMODE		2//转换当前状态，是配置模式时退出，不是时进入

/********************配置任务End******************************/




	//把错误转换为字符串
char *err_to_str(u16 err);





#endif
















