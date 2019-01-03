#include "includes.h"
#include "my_rf_new.h"




void my_rf_new_loop (void *t)
{
	u8 dev_now=0;
	u8 rf_recv[40]={0};
	u16 ret=0xffff;
	msgdata msg_recv={0};
	u8 msgtype=0;
	u8 msgflag=0;
	msgerr err={0};
	RF_Init(115200);
	RF_SetFocus(OSPrioHighRdy);
	RF_SetChannel(get_syscfg()->channel); 
	
	while(1)
	{
		delay_ms(200);
		if (get_syscfg()->numberOfDevices)
		{
			for (dev_now=1;dev_now<get_syscfg()->numberOfDevices;dev_now++)
			{
				err=Msg_Get(TASK_MSG,&msg_recv,&msgtype,&msgflag);
				if (err.errType==msgNoneErr)
				{
					rf_deal_msg(&msg_recv,&err,msgtype,msgflag);//处理消息
				} 
				ret=Cmd_0x01(get_devcfg(dev_now)->devId,rf_recv);
				deal_cmd_0x01(ret,get_devcfg(dev_now)->devId,rf_recv);
			}
		}
		else
		{
			err=Msg_Get(TASK_MSG,&msg_recv,&msgtype,&msgflag);
			if (err.errType==msgNoneErr)
			{
				rf_deal_msg(&msg_recv,&err,msgtype,msgflag);//处理消息
			} 
		}
	}
	
}



				//处理命令1的数据 
void deal_cmd_0x01 (u16 ret,u16 devid,u8 *rf_data)
{
	if (rf_data[4]!=0x81) return;//不是从机返回的命令
	msgdata msg={0};
	switch (ret)
	{
		case ERR_SUCCESS:
			if (rf_data[9]==devTypeCJQ)
			{
				get_devcfgbyid(devid)->offline=offlineNo;
				get_envirbyid(devid)->Humidity=rf_data[19]+rf_data[20]/10.;
				get_envirbyid(devid)->temperture=rf_data[17]+rf_data[18]/10.;
				get_envirbyid(devid)->tvoc=rf_data[21]+rf_data[22]/10.;
				
				if (get_syscfg()->collectorNumber==devid)
				{
					msg.voidptr=get_envirbyid(devid);//发送环境值结构体
					Msg_Send(2,&msg,MSG_TYPE_VOIDPTR,MSG_FLAG_CEECK|MSG_FLAG_SEND,0,0);
				}
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





void rf_deal_msg(msgdata *msg,msgerr *err,u8 msgtype,u8 msgflag)
{
	u16 ret=0;
	msgdata msgsend={0};
	char *errstr=0;
	switch (msgtype)
	{
		case MSG_TYPE_U8:
			switch (msg->u8dat[0])
			{
				case RF_ADD_DEVICE:
					ret=rf_add_device();
					if (msgflag&MSG_FLAG_ENRE)
					{
						
						errstr=err_to_str(ret);
						Msg_Send(err->msgfrom,&msgsend,MSG_TYPE_U8,MSG_FLAG_CEECK|MSG_FLAG_RECV,ret,errstr);
					}
					break;
				default:
					break;
			}
		break;
		default:
			break;
	}
}



		//添加设备到集中器
u16 rf_add_device(void)
{
	u16 ret=0;
	u16 addr=0;
	u8 devtype=0;
	ret=Cmd_0x06(0,1);
	if (ret) return ret;
	devtype=get_devcfg(get_syscfg()->numberOfDevices)->devType;
	addr=get_syscfg()->addressDomainL+get_syscfg()->numberOfDevices;
	if (addr>get_syscfg()->addressDomainH) return ERR_ADDRCROSS;//地址越界
	ret=Cmd_0x07(addr,devtype);
	if ( ret==ERR_SUCCESS)
	{
		ret=Cmd_0x06(0,0);
	}
	if (ret==ERR_SUCCESS)
	{
		get_devcfg(get_syscfg()->numberOfDevices)->devId=addr;
		get_devcfg(get_syscfg()->numberOfDevices)->offline=offlineYes;
		get_syscfg()->numberOfDevices++;//添加了一个设备
	}
	return ret;
}





			//通过指针发送字符串消息，注意在目标任务使用之前不要销毁这部分变量
void msg_to_str (u8 type,char *str)
{
	u8 msg[MESSEG_DATA]={0};
	*(u32 *)&msg=(u32)str;
	send_messeg(type,msg);
}





