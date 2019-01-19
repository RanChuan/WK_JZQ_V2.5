#ifndef __MY_RF_H
#define __MY_RF_H
#include "sys.h"
#include "rf.h"


/****************************************************************

		EN_CONFIG最后20位存储IP地址相关数据



*****************************************************************/

















				//找出设备数量
void Updata_DeviceNum (void);



	//返回指定地址的设备类型
u8 GetDeviceType (u16 addr);
					//返回指定地址的设备状态
u16 GetDeviceState(u16 addr);
					//返回指定类型的设备数量
u16 GetDeviceNum (u8 type);
			//返回第i个type类型的地址
u16 GetNextDeviceaddr (u8 type,u16 i);


				//获取配置信息的首地址
u16 *GetCfgData (void);




				//获取设备信息，recv是接收数组
u16 Cmd_0x01 (u16 addr ,u8 *recv);


					//控制设备
u16 Cmd_0x03 (u16 addr ,u8 type,u8 power,u8 state);

u16 Cmd_0x06 (u16 addr ,u8 state);

u16 Cmd_0x07 (u16 addr ,u8 type);



//处理消息循环
void my_rf_loop (void * t);

				//无线的数据处理
void my_rf_deal (void * t);


					//处理手动消息
void my_rf_hand (void * t);


void loushui_warn(u16 addr,u8 devtype);

void rf_cjq_deal(u8 *data);


u16 CheckId (u16 addr);//判断是否含有此设备id


					//设备运行状态指示，类型，开关量，状态（升降）
void Device_state_see(u8 type,u8 power,u8 state);

						//获取新的控制设备的状态
u16 get_newstate(u8 nowpower,u8 key,u16 addr);
							//获取一体机的新状态
u16 yt_newstate(u8 nowpower,u8 key,u16 addr);

									//更新集中器中记录的设备状态
u16 Updata_devicestate(u16 addr ,u8 power,u8 state);


		//加载默认配置信息
void load_test_cfg(void);


#endif


