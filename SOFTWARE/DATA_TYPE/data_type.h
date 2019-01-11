
#ifndef __DATA_TYPE_H
#define __DATA_TYPE_H
#include "includes.h"



/******以下是环境控制相关的数据类型**********/
 

typedef struct 
{
	u16 collectorId;//采集器的地址
	float temperture;
	float Humidity;
	float tvoc;
} EnvirDef; //环境值变量


typedef struct 
{
	float tempertureH;
	float tempertureL;
	float HumidityH;
	float HumidityL;
	float TVOCH;
	
} CtrllimitDef;//环境控制限度





/******环境控制相关的数据类型End**********/




/*******设备类型相关数据类型*************/

typedef struct
{
	u16 devId;					//设备编号
	u8 devType;					//设备类型
	u8 devPower;				//设备电源
	u8 devState;				//设备状态
	u8 devCmdState;			//控制状态 ，自动控制中的动态调整
	u8 devHand;					//是否打开手动
	u8 channel;					//设备信道
	u8 offline;					//离线
	u8 devError;				//是否有错误
	u8 numberOfFailures;//失败次数
	u8 devData;					//设备扩展数据
} DeviceDef;

		//定义设备类型
enum
{
	devTypeJZQ=0,
	devTypeCJQ=1,
	devTypeKT=2,
	devTypeCS=3,
	devTypeJH=4,
	devTypeJS=5,
	devTypeYT=6,
};


enum
{
	devPowerOff=0,			//设备电源关
	devPowerOn=1,				//设备电源开
};

enum
{
	devStateNone=0,			//设备无状态
	devStateUp=1,				//设备升
	devStateDown=2,			//设备降
	devStateOther=3,		//其他状态
};



enum
{
	devHandOff=0,
	devHandOn=1,
};

enum
{
	offlineNo=0,			//在线
	offlineYes=1,			//不在线
};



			//定义设备错误类型
enum
{
	devErrorLeaking=1,//漏水报警
	devErrorDevice=2,//设备错误
	devErrorSleep=3,//设备休眠中
};




typedef struct
{
	u16 ImplementerIP;			//执行者IP地址
	u8 cmdType;					//命令类型,自动控制，手动控制，动态调整控制等
	u8 cmdSource;				//命令来源，来自上位机，集中器，手动控制
	u8 cmdPermissions;	//命令权限 0，最高权限
	u8 cmdScopes ;			//命令作用域
	u8 cmd[3];					//命令参数
} CtrlCmdDef;


enum
{
	cmdTypeAuto=0,								//命令类型自动
	cmdTypeHand=1,								//命令类型手动
	cmdTypeAdjustment=2,					//命令类型动态调整
};

enum
{
	cmdSourceJZQAuto=0,				//集中器自动
	cmdSourceJZQHand=1,				//集中器手动
	cmdSourceServerAuto=2,				//服务器自动
	cmdSourceServerHand=3,			//服务器手动
};

enum
{
	cmdPermissionsAdmin=0,			//管理员，这个权限的命令强制执行
	cmdPermissionsUser=1,				//用户，这个权限可以中断动态调整中的自动设备
	cmdPermissionsAuto=2,						//自动，这个权限不能控制已经处于自动模式的设备
	cmdPermissionsVisitor=3,			//游客，这个权限只能控制处于手动模式的设备			
};


enum
{
	cmdScopesOnly=0,				//命令只作用于执行者
	cmdScopesSame=1,				//命令作用于与执行者同类型的设备
	cmdScopesJZQ=2,					//命令作用于集中器之下所有设备
};



/********设备类型相关数据类型End**********/




/********以下是系统配置相关的数据类型*******/


typedef struct
{
	u8 myName[20];//本机名称
	u16 ifUse;				//是否设置过
	u16 channel;			//无线信道
	u16 addressDomainL;//地址域低边界
	u16 addressDomainH;//地址域到边界
	u16 autoGetTime;//自动获取时间标志
	u16 devExclusiveset;//每个设备用专属环境上下限设置
	
	u16 numberOfDevices;//设备数量，包括采集器
	u16 collectorNumber;//采集器数量
	
	u16 alarmInterval; //报警时间间隔
	u16 controlInterval;//控制间隔
	u16 deployBeginH;//布防开始时
	u16 deployBeginM;//布防开始分
	u16 deployEndH;//布防结束时
	u16 deployEndM;//布防结束分
	u16 serverMode;//服务器模式
	u16 offAll;//关机时关闭所有下属设备
	u16 noticeSynchronous;//公告是否同步到服务器
	u8  notice[20];//公告
	
	u16 screenBrightness;//屏幕亮度
	u16 keyboardBrightness;//键盘亮度
	u16 alarmBrightness;//警示灯亮度
	u16 hibernationTime;//休眠时间
	u16 alarmModeInNormal;//正常时警示灯模式，0，关，1，开，2，闪烁
	u16 alarmModeInWarning;//有警告时警示灯模式
	u16 alarmModeInError;//有错误时警示灯模式
	u16 keyLightMode;//按键灯模式
	
	u16 timeSynchronous;//是否时间同步
	
	u8 serverIP[4];//默认服务器IP地址
	u16 serverCOM;//默认服务器端口
	u16 debugMode;//调试模式
	u16 postSelfTest;//开机自检
	u16 linkToYun;//连接到物联网
	u16 findServer;//是否自动寻找服务器
	u16 executingScripts;//是否执行脚本
	u16 mainCollectorId;//主要的采集器ID
	
	u16 handMode;//手动模式
} SysCfgDef;



/********系统配置相关的数据类型End**********/





//获取系统配置文件0,失败
u8 load_sys_cfg(SysCfgDef *buff);
u8 save_sys_cfg(SysCfgDef *buff);
u8 load_dev_cfg(DeviceDef *buff,u8 num);
u8 save_dev_cfg(DeviceDef *buff,u8 num);
u8 load_env_cfg(CtrllimitDef *buff,u8 num);
u8 save_env_cfg(CtrllimitDef *buff,u8 num);

void sys_cfg_init(void);
SysCfgDef *get_syscfg(void);
DeviceDef *get_devcfg(u8 num);
CtrllimitDef *get_envcfg(u8 num);
u8 get_collectorNumber(void);
DeviceDef *get_devcfgbyid(u16 id);
EnvirDef *get_envirbyid (u16 devid);
			//获取指定类型设备的数量
u8 get_DevNumberByType(u8 devtype);
//获取指定类型设备的状态
void get_DevStateByType(u8 devtype,u8 *offline,u8 *power,u8 *state);

u8 get_OnLineDevIdListByType (u8 devtype,u16 *idbuff);

#endif


