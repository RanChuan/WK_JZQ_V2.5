

#include "my_lcd_new.h"

#include "includes.h"
#include "relay.h"
#include "lcd.h"

/********************************************
*
*			这个文件写新版串口屏界面操作相关
*
*
*
*
*********************************************/



		//定义一个lcd操作的函数指针
void (*lcd_run)(void);
extern void num_to_str(u8 *str,u8 num1,u8 num2);
msgdata LCD_MSG;//其他任务发送到串口屏的消息
u8      LCD_MSGTYPE;
u8			LCD_MSGFLAG;
msgerr      LCD_MSGSTATE;
		//
void my_lcd_new(void *t)
{
	u8 buf[30]={0};u16 reclen;
	u16 i=0;
	Relay_Init();
	LCD_Init(9600);
	my_lcd_init();
	LCD_SetFocus(OSPrioHighRdy); 
	
	lcd_run=lcd_load;
	
	while(1)
	{
		delay_ms(1000);
		LCD_MSGSTATE=Msg_Get(TASK_MSG,&LCD_MSG,&LCD_MSGTYPE,&LCD_MSGFLAG);
		lcd_run();
	}
}




	//lcd加载界面
void lcd_load(void)
{
	u8 *buff=mymalloc(512);
	u8 *buff2=mymalloc(512);
	buff2[0]=0x5a;
	buff2[1]=0xa5;
	buff2[3]=0x82;
	buff2[4]=0x00;
	buff2[5]=0x48;
	u16 _i=0;
	u16 _len=0;
	
	sprintf ((char *)buff,"new ui 现在是新界面，%d",_i++);
	_len=strlen((char *)buff);
	mymemcpy(&buff2[6],buff,_len);
	buff2[2]=_len+3;
	_len=_len+6;
//	LCD_Send_Data(buff2,_len);
	lcd_change_str	(0x0048,(char *)buff,strlen((char *)buff));
	
	delay_ms(300);

	u8 jump=1;//要跳转的页面
	SysCfgDef *cfgbuff=mymalloc(sizeof(SysCfgDef));
	if (load_sys_cfg(cfgbuff))
	{
		if (cfgbuff->ifUse==1)
		{
			jump=1;//主页面
			lcd_run=lcd_main;//转为主页面
		}
		else
		{
			jump=2;//向导页面
			lcd_run=lcd_wizard1;//转为向导
		}
	}
	else
	{
		sprintf ((char *)buff,"重大错误：FLASH检测失败，代码：%04X",SPI_FLASH_TYPE);
		_len=strlen((char *)buff);
		mymemcpy(&buff2[6],buff,_len);
		buff2[2]=_len+3;
		_len=_len+6;
		LCD_Send_Data(buff2,_len);
		while(1)
		{
			delay_ms(1000);//系统错误，在此循环
		}
	}
	myfree(buff);
	myfree(buff2);
	
	
	
	delay_ms(3000);
	uint8_t lcd_page[6]		={0x5A,0xA5,0x03,0x80,0x4F,0x01};//第五位是要跳转的页面
	lcd_page[5]=jump;
	LCD_Send_Data(lcd_page,6);
	myfree(buff);
	myfree(buff2);
}



				//向导1
void lcd_wizard1(void)
{
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x1601://自动对时按键
					get_syscfg()->autoGetTime=!get_syscfg()->autoGetTime;
					lcd_change_value(0x0048,get_syscfg()->autoGetTime);//改标志					
					break;
				case 0x1602://设备专属按键
					get_syscfg()->devExclusiveset=!get_syscfg()->devExclusiveset;
					lcd_change_value(0x0049,get_syscfg()->devExclusiveset);//改标志					
					break;
				case 0x1603://下一步按键
					lcd_run=lcd_wizard2;
					lcd_turn_page(1);//跳转到下一个向导页
					break;
				default:
					break;
			}
		}
		addr=lcd_deal_str(buff,temps);
		if (addr)
		{
			switch (addr)
			{
				case 0x0010://本机名称
					mymemcpy(get_syscfg()->myName,temps,strlen((char *)temps));
					break;
				case 0x0025://无线信道
					get_syscfg()->channel= str2num(temps);
					break;
				case 0x0036://本机时间年
					break;
				case 0x0039://本机时间月
					break;
				case 0x003c://本机时间日
					break;
				case 0x003f://本机时间时
					break;
				case 0x0042://本机时间分
					break;
				case 0x0045://本机时间秒
					break;
				case 0x002b://地址域低边界
					get_syscfg()->addressDomainL= str2num(temps);
					break;
				case 0x0030://地址域高边界
					get_syscfg()->addressDomainH= str2num(temps);
					break;
				default:
					break;
			}
		}
	}
	else
	{
	}
	myfree(temps);
	myfree(buff);
}


				//向导2
void lcd_wizard2(void)
{
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	msgdata msg={0};
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x1801://完成按键
					get_syscfg()->ifUse=1;
					lcd_run=lcd_main;
					lcd_turn_page(2);//跳转到主页
					break;
				case 0x1802://上一步按键
					lcd_run=lcd_wizard1;
					lcd_turn_page(1);//跳转到上一个向导页
					break;
				default:
					break;
			}
		}
		addr=lcd_deal_str(buff,temps);
		if (addr)
		{
			switch (addr)
			{
				case 0x005f://设置设备类型					
					get_devcfg(get_syscfg()->numberOfDevices)->devType=str2num(temps);
					msg.u8dat[1]=(u8)RF_ADD_DEVICE;
					Msg_Send(3,&msg,MSG_TYPE_U8,MSG_FLAG_CEECK|MSG_FLAG_SEND|MSG_FLAG_ENRE,0,0);
					lcd_change_str(0x0076,"_",1);
					delay_us(10);
					lcd_change_str(0x0069,"_",1);
					delay_us(10);
					lcd_change_str(0x006f,"_",1);
					break;
				default:
					break;
			}
		}
	}
	else
	{
	}
				//处理任务间消息
	if (LCD_MSGSTATE.errType!=msgNoneMsg)
	{
		if (LCD_MSGSTATE.errType==msgMoreErr)
		{
			lcd_change_str(0x0076,LCD_MSGSTATE.extErrStr,strlen(LCD_MSGSTATE.extErrStr));
		}
		else 
		{
			lcd_change_str(0x0076,LCD_MSGSTATE.errStr,strlen(LCD_MSGSTATE.errStr));
		}
		if (LCD_MSGSTATE.errType==msgNoneErr)
		{
			char *buff=mymalloc(64);
			sprintf(buff,"%d",get_devcfg(get_syscfg()->numberOfDevices-1)->devId);
			lcd_change_str(0x0069,buff,strlen(buff));//显示设备编号
			lcd_change_str(0x006f,(char *)dbg_getdevname(get_devcfg(get_syscfg()->numberOfDevices-1)->devType),
				strlen((char *)dbg_getdevname(get_devcfg(get_syscfg()->numberOfDevices-1)->devType)));//显示设备类型
			myfree(buff);
		}
	}
	myfree(temps);
	myfree(buff);

}

				//主界面
void lcd_main(void)
{
	u8 *buff=mymalloc(64);
	char *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x0101://手动操作按键
					lcd_run=lcd_operation;
					lcd_turn_page(3);//跳转到手动
					break;
				case 0x0102://设置按键
					lcd_run=lcd_setchoose;
					lcd_turn_page(1);//
					break;
				case 0x0103:		//电源按键
					lcd_run=lcd_power;
					lcd_turn_page(2);//
				default:
					break;
			}
		}
	}
	else
	{
	}
	
	
	
					//任务间消息处理
	if (LCD_MSGSTATE.errType==msgNoneErr)
	{
		
		if (LCD_MSGTYPE==MSG_TYPE_VOIDPTR)
		{
			u8 num[2]={0};
			num[0]=((EnvirDef*)LCD_MSG.voidptr)->temperture;
			num[1]=(u8)((EnvirDef*)LCD_MSG.voidptr)->temperture*10%10;
			num_to_str(temps,num[0],num[1]);
			lcd_change_str(0x0080,temps,strlen((char *)temps));
			delay_us(100);
			num[0]=((EnvirDef*)LCD_MSG.voidptr)->Humidity;
			num[1]=(u8)((EnvirDef*)LCD_MSG.voidptr)->Humidity*10%10;
			num_to_str(temps,num[0],num[1]);
			lcd_change_str(0x0086,temps,strlen((char *)temps));
			delay_us(100);
			num[0]=((EnvirDef*)LCD_MSG.voidptr)->tvoc;
			num[1]=(u8)((EnvirDef*)LCD_MSG.voidptr)->tvoc*10%10;
			num_to_str(temps,num[0],num[1]);
			lcd_change_str(0x008b,temps,strlen((char *)temps));

		}
		else if (LCD_MSGTYPE==MSG_TYPE_U8)
		{
			if (LCD_MSG.u8dat[0]==LCD_DATAUP)//上传环境数据
			{
				u8 num[2]={0};
				num[0]=((LCD_MSG.u8dat[3]<<8)|LCD_MSG.u8dat[4])/10;
				num[1]=((LCD_MSG.u8dat[3]<<8)|LCD_MSG.u8dat[4])%10;
				num_to_str(temps,num[0],num[1]);
				lcd_change_str(0x0080,temps,strlen((char *)temps));
				delay_us(100);
				num[0]=((LCD_MSG.u8dat[5]<<8)|LCD_MSG.u8dat[6])/10;
				num[1]=((LCD_MSG.u8dat[5]<<8)|LCD_MSG.u8dat[6])%10;
				num_to_str(temps,num[0],num[1]);
				lcd_change_str(0x0086,temps,strlen((char *)temps));
				delay_us(100);
				num[0]=((LCD_MSG.u8dat[7]<<8)|LCD_MSG.u8dat[8])/10;
				num[1]=((LCD_MSG.u8dat[7]<<8)|LCD_MSG.u8dat[8])%10;
				num_to_str(temps,num[0],num[1]);
				lcd_change_str(0x008b,temps,strlen((char *)temps));
			}
		}
	}
	
	myfree(temps);
	myfree(buff);


}




				//手动稻作
void lcd_operation(void)
{
	static u8 firstin=1;
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			u8 offline;
			u8 devpower;
			u8 devstate;
			switch (key)
			{
				case 0x0701://完成按键
					firstin=1;
					lcd_run=lcd_main;
					lcd_turn_page(1);//跳转到主页
					break;
				case 0x0702://升温
					get_DevStateByType(devTypeKT,&offline,&devpower,&devstate);
					if (offline==offlineNo)
					{
						lcd_msg_to_rf(RF_DEVICE_KT,RF_DEVICE_UP);
						lcd_change_value(0x00f0,3);
					}
					break;
				case 0x0703://加湿
					get_DevStateByType(devTypeJS,&offline,&devpower,&devstate);
					if (offline==offlineNo)
					{
						lcd_change_value(0x00f1,3);
					}
					break;
				case 0x0704://净化
					get_DevStateByType(devTypeJH,&offline,&devpower,&devstate);
					if (offline==offlineNo)
					{
						lcd_change_value(0x00f2,3);
					}
					break;
				case 0x0705://降温
					get_DevStateByType(devTypeKT,&offline,&devpower,&devstate);
					if (offline==offlineNo)
					{
						lcd_msg_to_rf(RF_DEVICE_KT,RF_DEVICE_DOWN);
						lcd_change_value(0x00f3,3);
					}
					break;
				case 0x0706://除湿
					get_DevStateByType(devTypeCS,&offline,&devpower,&devstate);
					if (offline==offlineNo)
					{
						lcd_msg_to_rf(RF_DEVICE_CS,RF_DEVICE_DOWN);
						lcd_change_value(0x00f4,3);
					}
					break;
				case 0x0707://手动
					get_syscfg()->handMode=!get_syscfg()->handMode;
					if (get_syscfg()->handMode)
					{
						lcd_change_value(0x00f5,2);
					}
					else
					{
						lcd_change_value(0x00f5,1);
					}
					break;
				default:
					break;
			}
		}
	}
	else
	{
	}
	
					//任务间消息处理
	if (LCD_MSGSTATE.errType==msgNoneErr)
	{
		
	}
	
	
//	if (firstin)//初次转入
	{
		firstin=0;
		u8 offline;
		u8 devpower;
		u8 devstate;
		get_DevStateByType(devTypeKT,&offline,&devpower,&devstate);
		if (offline==offlineYes)
		{
			lcd_change_value(0x00f0,0);//不可操作
			delay_us(10);
			lcd_change_value(0x00f3,0);//不可操作
		}
		else
		{
			if (devpower==devPowerOff)
			{
				lcd_change_value(0x00f0,1);//关
				delay_us(10);
				lcd_change_value(0x00f3,1);//关
			}
			else
			{
				if (devstate==devStateUp)
				{
					lcd_change_value(0x00f0,2);//开
					delay_us(10);
					lcd_change_value(0x00f3,1);//关
				}
				else if (devstate==devStateDown)
				{
					lcd_change_value(0x00f0,1);//关
					delay_us(10);
					lcd_change_value(0x00f3,2);//开
				}
				else
				{
					lcd_change_value(0x00f0,0);//关
					delay_us(10);
					lcd_change_value(0x00f3,0);//关
				}
			}
		}
		get_DevStateByType(devTypeCS,&offline,&devpower,&devstate);
		if (offline==offlineYes)
		{
			lcd_change_value(0x00f4,0);//不可操作
		}
		else
		{
			if (devpower==devPowerOff)
			{
				lcd_change_value(0x00f4,1);//关
			}
			else
			{
				if (devstate==devStateDown)
					lcd_change_value(0x00f4,2);//开
			}
		}
		get_DevStateByType(devTypeJS,&offline,&devpower,&devstate);
		if (offline==offlineYes)
		{
			lcd_change_value(0x00f1,0);//不可操作
		}
		else
		{
			if (devpower==devPowerOff)
			{
				lcd_change_value(0x00f1,1);//关
			}
			else
			{
				if (devstate==devStateUp)
					lcd_change_value(0x00f1,2);//开
			}
		}
		get_DevStateByType(devTypeJH,&offline,&devpower,&devstate);
		if (offline==offlineYes)
		{
			lcd_change_value(0x00f2,0);//不可操作
		}
		else
		{
			if (devpower==devPowerOff)
			{
				lcd_change_value(0x00f2,1);//关
			}
			else
			{
				lcd_change_value(0x00f2,2);//开
			}
		}
		if (get_syscfg()->handMode)
		{
			lcd_change_value(0x00f5,2);
		}
		else
		{
			lcd_change_value(0x00f5,1);
		}
	}
	
	myfree(temps);
	myfree(buff);

}
				//设置选择
void lcd_setchoose(void)
{
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x0501://常规设置
					lcd_run=lcd_generalSet;
					lcd_turn_page(3);
					break;
				case 0x0502://节能设置
					lcd_run=lcd_energySaving;
					lcd_turn_page(4);
					break;
				case 0x0503://时间设置
					lcd_run=lcd_timeSet;
					lcd_turn_page(5);
					break;
				case 0x0504://环境设置
					lcd_run=lcd_environmentalControl;
					lcd_turn_page(6);
					break;
				case 0x0505://密码界面
					lcd_run=lcd_passwordInterface;
					lcd_turn_page(7);
					break;
				case 0x0506://关于
					lcd_run=lcd_about;
					lcd_turn_page(1);
					break;
				case 0x0507://返回
					lcd_run=lcd_main;
					lcd_turn_page(1);
					break;
				default:
					lcd_run=lcd_main;
					lcd_turn_page(1);
					break;
			}
		}
	}
	else
	{
	}
	myfree(temps);
	myfree(buff);

}
				//电源
void lcd_power(void)
{
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x1301://睡眠
					break;
				case 0x1302://关机
					break;
				case 0x1303://重启
					SysPowerOff();
					break;
				case 0x1304://返回
					lcd_run=lcd_main;
					lcd_turn_page(1);
					break;
				default:
					break;
			}
		}
	}
	else
	{
	}
	myfree(temps);
	myfree(buff);
	
}


				//常规设置
void lcd_generalSet(void)
{
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x1105://返回
					lcd_run=lcd_setchoose;
					lcd_turn_page(1);
					break;
				default:
					lcd_run=lcd_setchoose;
					lcd_turn_page(1);
					break;
			}
		}
	}
	else
	{
	}
	myfree(temps);
	myfree(buff);
	
}

				//节能设置
void lcd_energySaving(void)
{
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x1301://睡眠
					break;
				case 0x1302://关机
					break;
				case 0x1303://重启
					SysPowerOff();
					break;
				case 0x2401://返回
					lcd_run=lcd_main;
					lcd_turn_page(1);
					break;
				default:
					break;
			}
		}
	}
	else
	{
	}
	myfree(temps);
	myfree(buff);
	
}

				//时间设置
void lcd_timeSet(void)
{
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x1301://睡眠
					break;
				case 0x1302://关机
					break;
				case 0x1303://重启
					SysPowerOff();
					break;
				case 0x2601://返回
					lcd_run=lcd_main;
					lcd_turn_page(1);
					break;
				default:
					break;
			}
		}
	}
	else
	{
	}
	myfree(temps);
	myfree(buff);
	
}

				//环境控制
void lcd_environmentalControl(void)
{
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x1301://睡眠
					break;
				case 0x1302://关机
					break;
				case 0x1303://重启
					SysPowerOff();
					break;
				case 0x2801://返回
					lcd_run=lcd_main;
					lcd_turn_page(1);
					break;
				default:
					break;
			}
		}
	}
	else
	{
	}
	myfree(temps);
	myfree(buff);
	
}

				//密码界面
void lcd_passwordInterface(void)
{
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x1301://睡眠
					break;
				case 0x1302://关机
					break;
				case 0x1303://重启
					SysPowerOff();
					break;
				case 0x3201://返回
					lcd_run=lcd_setchoose;
					lcd_turn_page(1);
					break;
				default:
					break;
			}
		}
	}
	else
	{
	}
	myfree(temps);
	myfree(buff);
	
}

				//关于界面
void lcd_about (void)
{
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x1301://睡眠
					break;
				case 0x1302://关机
					break;
				case 0x1303://重启
					SysPowerOff();
					break;
				case 0x1304://返回
					lcd_run=lcd_main;
					lcd_turn_page(1);
					break;
				default:
					break;
			}
		}
	}
	else
	{
	}
	myfree(temps);
	myfree(buff);
	
}

				//高级设置
void lcd_advancedSet(void)
{
	u8 *buff=mymalloc(64);
	u8 *temps=mymalloc(64);
	u16 reclen=0;
	u16 key=0;
	u16 addr=0;
	LCD_Receive_Data(buff,&reclen);
	if (reclen)
	{
		reclen=0;
		key=lcd_deal_key(buff);
		if (key)
		{
			switch (key)
			{
				case 0x1301://睡眠
					break;
				case 0x1302://关机
					break;
				case 0x1303://重启
					SysPowerOff();
					break;
				case 0x3401://返回
					lcd_run=lcd_setchoose;
					lcd_turn_page(1);
					break;
				default:
					break;
			}
		}
	}
	else
	{
	}
	myfree(temps);
	myfree(buff);
	
}





/////////////////一些辅助函数////////////////////////


//转换页面
void lcd_turn_page(u8 page)
{
	uint8_t lcd_page[6]		={0x5A,0xA5,0x03,0x80,0x4F,0x01};//第五位是要跳转的页面
	lcd_page[5]=page;
	LCD_Send_Data(lcd_page,6);
	
}



//改变字符串变量值，地址，数据，数据长度
void lcd_change_str(u16 addr,char *data,u16 len)
{
	u8 *buff2=mymalloc(64);
	buff2[0]=0x5a;
	buff2[1]=0xa5;
	buff2[3]=0x82;
	buff2[4]=addr>>8;
	buff2[5]=addr;
	
	mymemcpy(&buff2[6],data,len);
	buff2[2]=len+3;
	len=len+6;
	LCD_Send_Data(buff2,len);
	myfree(buff2);
}



//改变一个变量
void lcd_change_value(u16 addr,u16 value)
{
	u8 data[8]={0x5a,0xa5,0x05,0x82,0x00,0x00,0x00,0x00};
	
	data[4]=addr>>8;
	data[5]=addr;
	data[6]=value>>8;
	data[7]=value;
	LCD_Send_Data(data,8);
	
}



//处理串口屏回传的按键数据，非零，成功，0，失败
u16 lcd_deal_key(u8 *data)
{
	if (data[3]!=0x83)
	{
		return 0;
	}
	
	if (data[6]==0x01)//按键只有一个长度
	{
		return (data[7]<<8)|data[8];
	}
	else
	{
		return 0;
	}
}




//处理串口屏回传的字符串数据，返回地址，成功，0，失败
u16 lcd_deal_str(u8 *data,u8 *strout)
{ 
	u8 len=0;
	u8 *datain;
	len=data[2]-4;//获取长度
	datain=&data[7];
	if (data[3]!=0x83)
	{
		return 0;
	}
	
	
	if ((data[6]>=0x01))//返回的数据类型是字符串型
	{
		while(len--)
		{
			if (*datain!=0xff)
			{
				*strout++=*datain++;
			}
			else
			{
				*strout++=0;
				break;
			}
		}
		strout[0]=0;//结尾加0；
		return (data[4]<<8)|data[5];
	}
	else
	{
		return 0;
	}
}


		//发送控制设备命令到设备
void lcd_msg_to_rf (u8 devtype,u8 key_state)
{
	msgdata msg={0};
	msg.u8dat[0]=devtype;
	msg.u8dat[1]=RF_CTRL_DEVICE;
	msg.u8dat[4]=RF_DEVICE_CH;
	msg.u8dat[5]=key_state;
	Msg_Send(3,&msg,MSG_TYPE_U8,MSG_FLAG_CEECK|MSG_FLAG_SEND,0,0);
}	







