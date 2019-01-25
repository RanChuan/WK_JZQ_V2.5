#include "my_cfg.h"
#include "rf.h"
#include "crc8_16.h"
#include "my_rf.h"
#include "delay.h"
#include "includes.h"
#include "my_messeg.h"

 u8 IN_CFG=0;//默认没有进入配置模式
#define CFG_MODE   		1
#define NOT_CFGMODE		0

void cfg_0x01 (u8 * data);
void cfg_0x06 (u8 * data);
void cfg_0x07 (u8 * data);
void cfg_0x08 (u8 * data);
void cfg_0x09 (u8 * data);
void cfg_retern (u8 cmd,u16 err);


/*********************************************

		消息数据位：
		msg0,1，进入配置模式，0，退出配置模式,2，转换当前的状态

		详细的消息定义参见my_messeg.h文件


***********************************************/


			//配置功能任务函数
void my_cfg (void * t)
{
	u8 m_send[MESSEG_NUM]={0};
	u8 rf_recv[50]={0};
	u8 crc[2]={0};
	if (t){IN_CFG=1;}else{IN_CFG=0;}
	while (1)
	{
		
		if (get_messeg(CFG_MESSEG,m_send)==0)
		{
			if (m_send[0]==CFG_CHANGE_CFGMODE)
			{
				IN_CFG=!IN_CFG;
				if (IN_CFG==NOT_CFGMODE)
				{
					exit_cfg();
				}
				else if (IN_CFG==CFG_MODE)
				{
					into_cfg();
				}
			}
		}
		delay_ms(200);		
		Get_Mycmd(rf_recv);
		if (rf_recv[0]==0) continue;
		Get_Crc16(rf_recv,rf_recv[6]+7,crc);
		if (crc[0]==rf_recv[rf_recv[6]+7]&&crc[1]==rf_recv[rf_recv[6]+8])//crc校验通过
		{
			if ((((rf_recv[2]<<8)|rf_recv[3])==Get_MyAddr())||(rf_recv[4]>=6&&rf_recv[4]<=8)||(rf_recv[4]==0x01))//地址符合
			{
				switch(rf_recv[4])
				{
					case 1:
						cfg_0x01 (rf_recv);
						break;
					case 2:
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						break;
					case 6:
						cfg_0x06 (rf_recv);
						break;
					case 7:
						cfg_0x07 (rf_recv);
						break;
					case 8:
						cfg_0x08 (rf_recv);
						break;
					case 9:
						cfg_0x09 (rf_recv);
						break;
					default:
						cfg_retern (rf_recv[4],ERR_NULLCMD);
						break;
				}
			}
		}
	}
}








		//获取设备信息，
void cfg_0x01 (u8 * data)
{
	u8 ret_[40]={0};
	u8 crc[2]={0};
	u8 *date=(u8*)__DATE__;
	u8 *time=(u8*)__TIME__;
	u8 *temp;//临时指针
	u8 datelen=strlen((const char*)date);
	u8 timelen=strlen((const char*)time);
	u16 length=datelen+timelen+4+2;
	ret_[0]=0xff;
	ret_[1]=0xff;
	ret_[2]=data[2];
	ret_[3]=data[3];
	ret_[4]=0x80|data[4];
	ret_[5]=length>>8;
	ret_[6]=length;
	temp=&ret_[9];
	copy_data(date,temp,datelen);
	temp+=datelen;
	*temp++=' ';
	*temp++='-';
	*temp++=' ';
	copy_data(time,temp,timelen);
	temp+=timelen;
	*temp=' ';

	Get_Crc16(ret_,ret_[6]+7,crc);
	ret_[ret_[6]+7]=crc[0];
	ret_[ret_[6]+7+1]=crc[1];
	RF1_tx_bytes(ret_,ret_[6]+7+2);
	
}










		//进入配置模式
void cfg_0x06 (u8 * data)
{
	u8 ret_[20]={0};
	u8 crc[2]={0};
	if (data[7]>1) 
	{
		cfg_retern(data[4],ERR_DATAVALE);
		return;
	}
	IN_CFG=data[7];

	if (IN_CFG==CFG_MODE)
	{
		into_cfg();
	}
	else
	{
		exit_cfg();
	}








	
	ret_[0]=0xff;
	ret_[1]=0xff;
	ret_[2]=data[2];
	ret_[3]=data[3];
	ret_[4]=0x80|data[4];
	ret_[5]=0x00;
	ret_[6]=0x03;
	ret_[7]=0x00;
	ret_[8]=0x00;
	ret_[9]=IN_CFG;
	
	Get_Crc16(ret_,ret_[6]+7,crc);
	ret_[ret_[6]+7]=crc[0];
	ret_[ret_[6]+7+1]=crc[1];
	RF1_tx_bytes(ret_,ret_[6]+7+2);
	
}


		//设置地址
void cfg_0x07 (u8 * data)
{
	
	if (data[9]!=0x00)//类型不是集中器
	{
		cfg_retern(data[4],ERR_DATAVALE);
		return;
	}
	if (IN_CFG==1)
	{
		Set_MyAddr((data[7]<<8)|data[8]);
		Save_Config();
		cfg_retern(data[4],ERR_SUCCESS);
	}
	else
	{
		cfg_retern(data[4],ERR_CANNOTCFG);
		return;
	}
	
	
	
	
	
}



void cfg_0x08 (u8 * data)
{
	u8 m_send[MESSEG_NUM]={0};
	
	if (IN_CFG!=1)
	{
		cfg_retern(data[4],ERR_CANNOTCFG);
		return;
	}
			
	if (data[7]==1)//设备地址
	{
		Set_Device(&data[8],(((data[5]<<8)|data[6])-1)/3);
		Save_Config();
		Updata_DeviceNum ( );
		cfg_retern(data[4],ERR_SUCCESS);
	}
	else if (data[7]==3)//网口配置
	{
		Set_MyIP(&data[8]);
		Save_Config();
		cfg_retern(data[4],ERR_SUCCESS);
		m_send[0]=3;//重启网口
		send_messeg(WK_MESSEG,m_send);
	}
	else
	{
		cfg_retern(data[4],ERR_DATAVALE);
		return;
	}
}





void cfg_0x09 (u8 *data)
{
	if (data[7]==2)
	{
		cfg_retern(data[4],ERR_SUCCESS);
		delay_ms(100);
		NVIC_SystemReset();
	}
	else
	{
		cfg_retern(data[4],ERR_DATAVALE);
	}
}




					//返回错误码
void cfg_retern (u8 cmd,u16 err)
{
	u8 data[20]={0};
	u8 crc[2]={0};
	u16 addr=0;
	data[0]=0xff;
	data[1]=0xff;
	addr=Get_MyAddr();
	data[2]=addr>>8;
	data[3]=addr;
	data[4]=0x80|cmd;
	data[5]=0x00;
	data[6]=0x02;
	data[7]=err>>8;
	data[8]=err;
	
	Get_Crc16(data,data[6]+7,crc);
	data[data[6]+7]=crc[0];
	data[data[6]+7+1]=crc[1];
	RF1_tx_bytes(data,data[6]+7+2);
}


//进入配置模式
void into_cfg(void)
{
	u8 meg[MESSEG_DATA]={0};
	RF_SetFocus(OSPrioHighRdy);
	TaskPend(5);//挂起消息循环
	TaskPend(3);//挂起消息循环
	meg[0]=1;meg[1]=1;//灯
	meg[2]=4;//4
	send_messeg(LIT_MESSEG,meg);//
	meg[2]=3;//3
	send_messeg(LIT_MESSEG,meg);//
//	RF_SetChannel(0);
}

//退出配置模式
void exit_cfg(void)
{
	u8 meg[MESSEG_DATA]={0};
	TaskRepend(5);//恢复消息循环
	RF_SetFocus(3);
	TaskRepend(3);//恢复消息循环
	meg[0]=1;meg[1]=0;//灯
	meg[2]=4;//4
	send_messeg(LIT_MESSEG,meg);//
	meg[2]=3;//3
	send_messeg(LIT_MESSEG,meg);//
//	RF_SetChannel(10);
}



