#include "includes.h"
#include "my_rf_new.h"




void my_rf_new_loop (void *t)
{
	u8 dev_now=0;
	u8 rf_recv[40]={0};
	u8 msg_recv[MESSEG_DATA]={0};
	u16 ret=0xffff;
	RF_M0=0;
	RF_M1=0; 
	RF_Init(115200);
	RF_SetFocus(OSPrioHighRdy);
	RF_SetChannel(10); //暂时不用设置信道2018.10.12
	
	while(1)
	{
		delay_ms(200);
		for (dev_now=1;dev_now<get_syscfg()->numberOfDevices;dev_now++)
		{
			if (get_messeg(OSPrioHighRdy,msg_recv)==0)
			{
				rf_deal_msg(msg_recv);//处理消息
			}
			ret=Cmd_0x01(get_devcfg(dev_now)->devId,rf_recv);
			deal_cmd_0x01(ret,get_devcfg(dev_now)->devId,rf_recv);
		}
	}
	
}



				//处理命令1的数据 
void deal_cmd_0x01 (u16 ret,u16 devid,u8 *rf_data)
{
	if (rf_data[4]!=0x81) return;//不是从机返回的命令
	switch (ret)
	{
		case ERR_SUCCESS:
			if (rf_data[9]==devTypeCJQ)
			{
				get_devcfgbyid(devid)->offline=offlineNo;
				get_envirbyid(devid)->Humidity=rf_data[19]+rf_data[20]/10.;
				get_envirbyid(devid)->temperture=rf_data[17]+rf_data[18]/10.;
				get_envirbyid(devid)->tvoc=rf_data[21]+rf_data[22]/10.;
			}
			else
			{
				get_devcfgbyid(devid)->offline=offlineNo;
				get_devcfgbyid(devid)->devPower=rf_data[7+9];
				if (get_devcfgbyid(devid)->devType!=devTypeYT)
				{
					get_devcfgbyid(devid)->devState=rf_data[8+9];
				}
				else
				{
					get_devcfgbyid(devid)->devData=rf_data[8+9];
				}
			}
			break;
		case ERR_TIMEOUT:
			if (get_devcfgbyid(devid)->numberOfFailures<5)
			{
				get_devcfgbyid(devid)->offline=offlineYes;
			}
			else
			{
				get_devcfgbyid(devid)->numberOfFailures++;
			}
			break;
		default:
			break;
	}
}

				//操作设备
void operatingEquipment (u8 *msg)
{
	if (msg[0])
	{
		switch (msg[0])
		{
			case RF_DEVICE_ADDR:
				break;
			case RF_DEVICE_KT:
			case RF_DEVICE_CS:
			case RF_DEVICE_JH:
			case RF_DEVICE_JS:
				break;
			case RF_DEVICE_YT:
				break;
			default:
				break;
		}
	}
}





void rf_deal_msg(u8 *msg)
{
	u16 ret=0;
	switch (msg[1])
	{
		case RF_ADD_DEVICE:
			ret=Cmd_0x06(0,1);
			if (ret!=0)
			{
//				msg_to_str(
			}
			break;
		default:
			break;
	}
}




			//通过指针发送字符串消息，注意在目标任务使用之前不要销毁这部分变量
void msg_to_str (u8 type,char *str)
{
	u8 msg[MESSEG_DATA]={0};
	*(u32 *)&msg=(u32)str;
	send_messeg(type,msg);
}





