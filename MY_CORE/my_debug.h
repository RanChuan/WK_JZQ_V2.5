

#ifndef __MY_DEBUG_H
#define __MY_DEBUG_H

#include "sys.h"
#include "my_w5500.h"
#include "cjson.h"


/***************************************************

					调试时使用网口输出调试信息

***************************************************/



extern unsigned char Gateway_IP[4];	//网关IP地址 
extern unsigned char Sub_Mask[4];	//子网掩码 
extern unsigned char Phy_Addr[6];	//物理地址(MAC) 
extern unsigned char IP_Addr[4];	//本机IP地址 

			//温控数据上传服务器地址
extern u8 SERVER_IP[4];
extern u16 SERVER_PORT;


/***************************************************

					调试时文件系统输出调试信息

***************************************************/


extern u8 DBG_FATS;


/***************************************************

					输出内存占用信息信息

***************************************************/

extern u32 memsize    ;							//内存总大小


/***************************************************

					输出网络连接状态信息

***************************************************/

extern u8 DBG_INTER_STATE;


extern u16 SPI_FLASH_TYPE;

extern u16 Get_MyAddr(void);

//复制与上位机的通信
extern u8 DBG_COPY_TO_S1CK;

//根据类型获取名称
typedef struct 
{
	u8 objtype;
	u8 *name;
}objname;



//调试函数调用入口
void my_debug (void);

void dbg_Interpreter(void);

	//板子信息
void dbg_info (void);

void dbg_err (u8 errtype);

void dbg_devconfig(void);

void dbg_help(void);

void dbg_reboot (void);

void dbg_oche (u8 *);

void dbg_booting(void);

	//复制数据到调试端口
void dbg_copydata (u8 *);


				//使用DNS获取域名的IP地址
void dbg_getip(u8 *buff);


				//测试mqtt连接
void dbg_mqtt(u8 *buff);





/**********一些辅助函数****************/

			//比较字符串，相同为1，不同为0
u8 samestr(u8 *str1,u8 *str2);
				//字符串转为整形数字
u16 str2num(u8 *str);

			//提取字符串中的数字，用分隔符point分开,有length个数字
u8 getnumfstr(u8 *out,u8 *strin,u8 point,u8 length);


//根据设备类型获取名称
u8 *dbg_getdevname(u8 devtype);



#endif



















