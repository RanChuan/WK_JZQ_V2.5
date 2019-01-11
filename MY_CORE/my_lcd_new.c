

#include "my_lcd_new.h"

#include "includes.h"
#include "relay.h"
#include "lcd.h"

/********************************************
*
*			����ļ�д�°洮��������������
*
*
*
*
*********************************************/



		//����һ��lcd�����ĺ���ָ��
void (*lcd_run)(void);
extern void num_to_str(u8 *str,u8 num1,u8 num2);
msgdata LCD_MSG;//���������͵�����������Ϣ
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




	//lcd���ؽ���
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
	
	sprintf ((char *)buff,"new ui �������½��棬%d",_i++);
	_len=strlen((char *)buff);
	mymemcpy(&buff2[6],buff,_len);
	buff2[2]=_len+3;
	_len=_len+6;
//	LCD_Send_Data(buff2,_len);
	lcd_change_str	(0x0048,(char *)buff,strlen((char *)buff));
	
	delay_ms(300);

	u8 jump=1;//Ҫ��ת��ҳ��
	SysCfgDef *cfgbuff=mymalloc(sizeof(SysCfgDef));
	if (load_sys_cfg(cfgbuff))
	{
		if (cfgbuff->ifUse==1)
		{
			jump=1;//��ҳ��
			lcd_run=lcd_main;//תΪ��ҳ��
		}
		else
		{
			jump=2;//��ҳ��
			lcd_run=lcd_wizard1;//תΪ��
		}
	}
	else
	{
		sprintf ((char *)buff,"�ش����FLASH���ʧ�ܣ����룺%04X",SPI_FLASH_TYPE);
		_len=strlen((char *)buff);
		mymemcpy(&buff2[6],buff,_len);
		buff2[2]=_len+3;
		_len=_len+6;
		LCD_Send_Data(buff2,_len);
		while(1)
		{
			delay_ms(1000);//ϵͳ�����ڴ�ѭ��
		}
	}
	myfree(buff);
	myfree(buff2);
	
	
	
	delay_ms(3000);
	uint8_t lcd_page[6]		={0x5A,0xA5,0x03,0x80,0x4F,0x01};//����λ��Ҫ��ת��ҳ��
	lcd_page[5]=jump;
	LCD_Send_Data(lcd_page,6);
	myfree(buff);
	myfree(buff2);
}



				//��1
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
				case 0x1601://�Զ���ʱ����
					get_syscfg()->autoGetTime=!get_syscfg()->autoGetTime;
					lcd_change_value(0x0048,get_syscfg()->autoGetTime);//�ı�־					
					break;
				case 0x1602://�豸ר������
					get_syscfg()->devExclusiveset=!get_syscfg()->devExclusiveset;
					lcd_change_value(0x0049,get_syscfg()->devExclusiveset);//�ı�־					
					break;
				case 0x1603://��һ������
					lcd_run=lcd_wizard2;
					lcd_turn_page(1);//��ת����һ����ҳ
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
				case 0x0010://��������
					mymemcpy(get_syscfg()->myName,temps,strlen((char *)temps));
					break;
				case 0x0025://�����ŵ�
					get_syscfg()->channel= str2num(temps);
					break;
				case 0x0036://����ʱ����
					break;
				case 0x0039://����ʱ����
					break;
				case 0x003c://����ʱ����
					break;
				case 0x003f://����ʱ��ʱ
					break;
				case 0x0042://����ʱ���
					break;
				case 0x0045://����ʱ����
					break;
				case 0x002b://��ַ��ͱ߽�
					get_syscfg()->addressDomainL= str2num(temps);
					break;
				case 0x0030://��ַ��߽߱�
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


				//��2
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
				case 0x1801://��ɰ���
					get_syscfg()->ifUse=1;
					lcd_run=lcd_main;
					lcd_turn_page(2);//��ת����ҳ
					break;
				case 0x1802://��һ������
					lcd_run=lcd_wizard1;
					lcd_turn_page(1);//��ת����һ����ҳ
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
				case 0x005f://�����豸����					
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
				//�����������Ϣ
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
			lcd_change_str(0x0069,buff,strlen(buff));//��ʾ�豸���
			lcd_change_str(0x006f,(char *)dbg_getdevname(get_devcfg(get_syscfg()->numberOfDevices-1)->devType),
				strlen((char *)dbg_getdevname(get_devcfg(get_syscfg()->numberOfDevices-1)->devType)));//��ʾ�豸����
			myfree(buff);
		}
	}
	myfree(temps);
	myfree(buff);

}

				//������
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
				case 0x0101://�ֶ���������
					lcd_run=lcd_operation;
					lcd_turn_page(3);//��ת���ֶ�
					break;
				case 0x0102://���ð���
					lcd_run=lcd_setchoose;
					lcd_turn_page(1);//
					break;
				case 0x0103:		//��Դ����
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
	
	
	
					//�������Ϣ����
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
			if (LCD_MSG.u8dat[0]==LCD_DATAUP)//�ϴ���������
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




				//�ֶ�����
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
				case 0x0701://��ɰ���
					firstin=1;
					lcd_run=lcd_main;
					lcd_turn_page(1);//��ת����ҳ
					break;
				case 0x0702://����
					get_DevStateByType(devTypeKT,&offline,&devpower,&devstate);
					if (offline==offlineNo)
					{
						lcd_msg_to_rf(RF_DEVICE_KT,RF_DEVICE_UP);
						lcd_change_value(0x00f0,3);
					}
					break;
				case 0x0703://��ʪ
					get_DevStateByType(devTypeJS,&offline,&devpower,&devstate);
					if (offline==offlineNo)
					{
						lcd_change_value(0x00f1,3);
					}
					break;
				case 0x0704://����
					get_DevStateByType(devTypeJH,&offline,&devpower,&devstate);
					if (offline==offlineNo)
					{
						lcd_change_value(0x00f2,3);
					}
					break;
				case 0x0705://����
					get_DevStateByType(devTypeKT,&offline,&devpower,&devstate);
					if (offline==offlineNo)
					{
						lcd_msg_to_rf(RF_DEVICE_KT,RF_DEVICE_DOWN);
						lcd_change_value(0x00f3,3);
					}
					break;
				case 0x0706://��ʪ
					get_DevStateByType(devTypeCS,&offline,&devpower,&devstate);
					if (offline==offlineNo)
					{
						lcd_msg_to_rf(RF_DEVICE_CS,RF_DEVICE_DOWN);
						lcd_change_value(0x00f4,3);
					}
					break;
				case 0x0707://�ֶ�
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
	
					//�������Ϣ����
	if (LCD_MSGSTATE.errType==msgNoneErr)
	{
		
	}
	
	
//	if (firstin)//����ת��
	{
		firstin=0;
		u8 offline;
		u8 devpower;
		u8 devstate;
		get_DevStateByType(devTypeKT,&offline,&devpower,&devstate);
		if (offline==offlineYes)
		{
			lcd_change_value(0x00f0,0);//���ɲ���
			delay_us(10);
			lcd_change_value(0x00f3,0);//���ɲ���
		}
		else
		{
			if (devpower==devPowerOff)
			{
				lcd_change_value(0x00f0,1);//��
				delay_us(10);
				lcd_change_value(0x00f3,1);//��
			}
			else
			{
				if (devstate==devStateUp)
				{
					lcd_change_value(0x00f0,2);//��
					delay_us(10);
					lcd_change_value(0x00f3,1);//��
				}
				else if (devstate==devStateDown)
				{
					lcd_change_value(0x00f0,1);//��
					delay_us(10);
					lcd_change_value(0x00f3,2);//��
				}
				else
				{
					lcd_change_value(0x00f0,0);//��
					delay_us(10);
					lcd_change_value(0x00f3,0);//��
				}
			}
		}
		get_DevStateByType(devTypeCS,&offline,&devpower,&devstate);
		if (offline==offlineYes)
		{
			lcd_change_value(0x00f4,0);//���ɲ���
		}
		else
		{
			if (devpower==devPowerOff)
			{
				lcd_change_value(0x00f4,1);//��
			}
			else
			{
				if (devstate==devStateDown)
					lcd_change_value(0x00f4,2);//��
			}
		}
		get_DevStateByType(devTypeJS,&offline,&devpower,&devstate);
		if (offline==offlineYes)
		{
			lcd_change_value(0x00f1,0);//���ɲ���
		}
		else
		{
			if (devpower==devPowerOff)
			{
				lcd_change_value(0x00f1,1);//��
			}
			else
			{
				if (devstate==devStateUp)
					lcd_change_value(0x00f1,2);//��
			}
		}
		get_DevStateByType(devTypeJH,&offline,&devpower,&devstate);
		if (offline==offlineYes)
		{
			lcd_change_value(0x00f2,0);//���ɲ���
		}
		else
		{
			if (devpower==devPowerOff)
			{
				lcd_change_value(0x00f2,1);//��
			}
			else
			{
				lcd_change_value(0x00f2,2);//��
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
				//����ѡ��
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
				case 0x0501://��������
					lcd_run=lcd_generalSet;
					lcd_turn_page(3);
					break;
				case 0x0502://��������
					lcd_run=lcd_energySaving;
					lcd_turn_page(4);
					break;
				case 0x0503://ʱ������
					lcd_run=lcd_timeSet;
					lcd_turn_page(5);
					break;
				case 0x0504://��������
					lcd_run=lcd_environmentalControl;
					lcd_turn_page(6);
					break;
				case 0x0505://�������
					lcd_run=lcd_passwordInterface;
					lcd_turn_page(7);
					break;
				case 0x0506://����
					lcd_run=lcd_about;
					lcd_turn_page(1);
					break;
				case 0x0507://����
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
				//��Դ
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
				case 0x1301://˯��
					break;
				case 0x1302://�ػ�
					break;
				case 0x1303://����
					SysPowerOff();
					break;
				case 0x1304://����
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


				//��������
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
				case 0x1105://����
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

				//��������
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
				case 0x1301://˯��
					break;
				case 0x1302://�ػ�
					break;
				case 0x1303://����
					SysPowerOff();
					break;
				case 0x2401://����
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

				//ʱ������
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
				case 0x1301://˯��
					break;
				case 0x1302://�ػ�
					break;
				case 0x1303://����
					SysPowerOff();
					break;
				case 0x2601://����
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

				//��������
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
				case 0x1301://˯��
					break;
				case 0x1302://�ػ�
					break;
				case 0x1303://����
					SysPowerOff();
					break;
				case 0x2801://����
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

				//�������
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
				case 0x1301://˯��
					break;
				case 0x1302://�ػ�
					break;
				case 0x1303://����
					SysPowerOff();
					break;
				case 0x3201://����
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

				//���ڽ���
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
				case 0x1301://˯��
					break;
				case 0x1302://�ػ�
					break;
				case 0x1303://����
					SysPowerOff();
					break;
				case 0x1304://����
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

				//�߼�����
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
				case 0x1301://˯��
					break;
				case 0x1302://�ػ�
					break;
				case 0x1303://����
					SysPowerOff();
					break;
				case 0x3401://����
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





/////////////////һЩ��������////////////////////////


//ת��ҳ��
void lcd_turn_page(u8 page)
{
	uint8_t lcd_page[6]		={0x5A,0xA5,0x03,0x80,0x4F,0x01};//����λ��Ҫ��ת��ҳ��
	lcd_page[5]=page;
	LCD_Send_Data(lcd_page,6);
	
}



//�ı��ַ�������ֵ����ַ�����ݣ����ݳ���
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



//�ı�һ������
void lcd_change_value(u16 addr,u16 value)
{
	u8 data[8]={0x5a,0xa5,0x05,0x82,0x00,0x00,0x00,0x00};
	
	data[4]=addr>>8;
	data[5]=addr;
	data[6]=value>>8;
	data[7]=value;
	LCD_Send_Data(data,8);
	
}



//���������ش��İ������ݣ����㣬�ɹ���0��ʧ��
u16 lcd_deal_key(u8 *data)
{
	if (data[3]!=0x83)
	{
		return 0;
	}
	
	if (data[6]==0x01)//����ֻ��һ������
	{
		return (data[7]<<8)|data[8];
	}
	else
	{
		return 0;
	}
}




//���������ش����ַ������ݣ����ص�ַ���ɹ���0��ʧ��
u16 lcd_deal_str(u8 *data,u8 *strout)
{ 
	u8 len=0;
	u8 *datain;
	len=data[2]-4;//��ȡ����
	datain=&data[7];
	if (data[3]!=0x83)
	{
		return 0;
	}
	
	
	if ((data[6]>=0x01))//���ص������������ַ�����
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
		strout[0]=0;//��β��0��
		return (data[4]<<8)|data[5];
	}
	else
	{
		return 0;
	}
}


		//���Ϳ����豸����豸
void lcd_msg_to_rf (u8 devtype,u8 key_state)
{
	msgdata msg={0};
	msg.u8dat[0]=devtype;
	msg.u8dat[1]=RF_CTRL_DEVICE;
	msg.u8dat[4]=RF_DEVICE_CH;
	msg.u8dat[5]=key_state;
	Msg_Send(3,&msg,MSG_TYPE_U8,MSG_FLAG_CEECK|MSG_FLAG_SEND,0,0);
}	







