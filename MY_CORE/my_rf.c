#include "my_rf.h"
#include "crc8_16.h"
#include "my_light.h"
#include "delay.h"
#include "includes.h"
#include "stmflash.h"






/***********************************************************

		meg每个位的含义
		meg[0],设备类型,0切换为自动控制，0xff,控制指定地址的设备
		meg[1],1获取设备状态，2，手动控制设备,
		meg[2],设备地址高把位
		meg[3],设备地址第八位
		meg[4],如果是控制，开关状态，0，关，1，开，2，反向
		meg[5],如果是控制，升降状态，反向时的按键，1，升，2，降

		详细的消息定义参见my_messeg.h文件

***********************************************************/


//1使用旧版，0使用新版，旧版串口屏设置是以1mg/m3为单位，新版以0.1mg/m3为单位
#define __USE_OLD   1

u8 HANDING=0;//手动操作是否在进行

//处理消息循环
void my_rf_loop (void * t)
{
	u8 rf_recv[40]={0};
	u16 i;//循环变量
	u16 ret;//返回值
	extern u8 IN_CFG; 
	
	RF_M0=0;
	RF_M1=0;
	RF_Init(115200);
	
	RF_SetFocus(OSPrioHighRdy);
	
	
	Load_Config();
	load_test_cfg(); 
	RF_SetChannel(Get_MyChanel()); 
	//write_config(); //写入配置到文件
	Updata_DeviceNum();
	while (1)
	{
		for (i=1;EN_CONFIG[i*2+1];i++)
		{
			delay_ms(1000);//采集器延时1s，防止下一个冲突
			ret=Cmd_0x01 (EN_CONFIG[i*2] ,rf_recv);
			if (ret==0)//成功
			{
				if ((EN_CONFIG[i*2+1]&0x00ff)==1)
//				if (rf_recv[9]==1)//类型是采集器
				{
					EN_CONFIG[i*2+1]|=DEVICEON;
					
					//模拟收到了消息
					rf_recv[0]=0xff;
					rf_recv[1]=0xff;
					rf_recv[2]=EN_CONFIG[i*2]>>8;
					rf_recv[3]=EN_CONFIG[i*2];
					
					rf_cjq_deal(rf_recv);
					if (EN_DATA[1]!=0xff)
						copy_data(rf_recv,EN_DATA,rf_recv[6]+7);
				}
				else 
				{
					
									//设备开关状态
					if (rf_recv[7+9]==1) 
					{
						EN_CONFIG[i*2+1]|=DEVICEON;
					}
					else 
					{
						EN_CONFIG[i*2+1]&=~DEVICEON;
					}
					
					if (GetDeviceType(EN_CONFIG[i*2])!=6)//不是恒湿机
					{
										//设备升降状态
						if (rf_recv[8+9]==1) 
						{
							EN_CONFIG[i*2+1]|=DEVICEUP;
							EN_CONFIG[i*2+1]&=~DEVICEDOWN;
						}
						else if (rf_recv[8+9]==2)
						{
							EN_CONFIG[i*2+1]|=DEVICEDOWN;
							EN_CONFIG[i*2+1]&=~DEVICEUP;
						}
					}else
					{
					}
					if(rf_recv[9+11]) //发送漏水报警
						loushui_warn(EN_CONFIG[i*2],GetDeviceType(EN_CONFIG[i*2]));
				}
				EN_CONFIG[i*2+1]&=~0xf800;//在线
			}
			else if (ret==ERR_TIMEOUT)
			{
				EN_CONFIG[i*2+1]+=0x1000;//离线计数器
				if ((EN_CONFIG[i*2+1]>>12)>5)//连续5次超时，确认离线
				{
					EN_CONFIG[i*2+1]&=0x00ff;//离线了
					EN_CONFIG[i*2+1]|=DEVICEOFFLINE;//离线了
				}
			}
			else
			{
					EN_CONFIG[i*2+1]&=0x00ff;//离线了
					EN_CONFIG[i*2+1]|=DEVICEOFFLINE;//离线了
			}
		}
	}
}
 

void loushui_warn(u16 addr,u8 devtype)
{
	u8 send[MESSEG_DATA]={0};
	send[0]=7;
	send[1]=6;
	send[2]=devtype;
	send[3]=addr>>8;
	send[4]=addr;
	send[5]=1;
	if (find_messeg(WK_MESSEG)==0)
		send_messeg(WK_MESSEG,send);//发送给网口
	
}


			//发送采集器数据
void rf_cjq_deal(u8 *data) 
{
	u16 temp=0;
	u8 send[MESSEG_DATA]={0};
	
	
		temp=data[17]*10+data[18];//原数据*10,温度
		send[0]=4;send[3]=temp>>8;send[4]=temp;
#if __USE_OLD == 0
		temp=data[19]*10+data[20];//原数据，湿度
#else
		temp=data[19];//原数据，湿度
#endif
		send[5]=temp>>8;send[6]=temp;
		temp=data[21]*10+data[22];
		send[7]=temp>>8;send[8]=temp;
		send_messeg(LCD_MESSEG,send);//发送给屏幕显示
		
		send[0]=2;send[1]=1;send[2]=0;//发送采集器的数据
		send[3]=((u32 )data>>24);	//设置地址
		send[4]=((u32 )data>>16);
		send[5]=((u32 )data>>8);
		send[6]=((u32 )data);
		if (find_messeg(WK_MESSEG)==0)
			send_messeg(WK_MESSEG,send);//发送给网口
		data[0]=0;

}




		//加载默认配置信息
void load_test_cfg(void)
{
	
	//如果没有配置，加载默认的
	if (EN_CONFIG[0]==0)
	{
		EN_CONFIG[0]=1;
		EN_CONFIG[1]=0;
		EN_CONFIG[2]=2;
		EN_CONFIG[3]=1;
		EN_CONFIG[4]=3;
		EN_CONFIG[5]=3;
		EN_CONFIG[6]=33;
		EN_CONFIG[7]=1;
		EN_CONFIG[8]=34;
		EN_CONFIG[9]=1;
		EN_CONFIG[10]=35;
		EN_CONFIG[11]=1;
		EN_CONFIG[12]=36;
		EN_CONFIG[13]=1;
		
		//本机地址
		Get_MyIP()[0]=192;
		Get_MyIP()[1]=168;
		Get_MyIP()[2]=2;
		Get_MyIP()[3]=13;
		
		//服务器地址
		Get_MyIP()[6]=192;
		Get_MyIP()[7]=168;
		Get_MyIP()[8]=2;
		Get_MyIP()[9]=56;

		Get_MyIP()[10]=6000>>8;
		Get_MyIP()[11]=6000&0x00ff;
		
		//网关IP
		Get_MyIP()[12]=192;
		Get_MyIP()[13]=168;
		Get_MyIP()[14]=2;
		Get_MyIP()[15]=1;

		Save_Config();
	}
	

}








					//处理手动消息
void my_rf_hand (void * t)
{
	u8 msg[MESSEG_DATA]={0};
	u8 m_send[MESSEG_DATA]={0};//用来给网口返回控制信息
	u16 my_i=0;
	u16 m=0;//操作失败次数；
	u16 ret;
	u16 id=0;
	u16 newret=0;
	u8 newpower;
	u8 newstate_;//newstate这个变量名可能是系统关键字，会自动改变
	while(1)
	{
		delay_ms(200);
		if (get_messeg(RF_MESSEG,msg))//没有消息
		{
		}
		else
		{
			RF_SetFocus(OSPrioHighRdy);
			TaskPend(3);
			if ((msg[0]!=0)&&(msg[1]==2)) 
			{
				m=0;
				id=(msg[2]<<8)|msg[3];
				if (id==0)				//地址为0，控制全部同类设备
				{
					my_i=0;
					while(my_i<GetDeviceNum(msg[0]))
					{
						if (GetDeviceState(GetNextDeviceaddr(msg[0],my_i))&DEVICEOFFLINE)
						{
							my_i++;//设备不在线
							continue;
						}
						delay_ms(200);			
												//获取新的控制设备的状态,高8位开关，低8位状态
						newret=get_newstate(msg[4],msg[5],GetNextDeviceaddr(msg[0],my_i));newstate_=newret;newpower=(newret>>8);
																//设备地址，设备类型，开关状态，附加参数
						ret=Cmd_0x03 (GetNextDeviceaddr(msg[0],my_i),msg[0],newpower,newstate_);
						if (ret==0)
						{
							Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,newstate_);
							Device_state_see(msg[0],newpower,newstate_);
							my_i++;//控制成功，控制下一个
						}
						else
						{
							m++;if (m>10){m=0;my_i++;}//重试10次
						}
					}
					//加一个控制恒湿机的循环
					if (msg[0]==3||msg[0]==5)
					{
						my_i=0;
						msg[0]=6;
						while(my_i<GetDeviceNum(6))//一体机的个数
						{
							if (GetDeviceState(GetNextDeviceaddr(msg[0],my_i))&DEVICEOFFLINE)
							{
								my_i++;//设备不在线
								continue;
							}
							delay_ms(200);	
							newret=yt_newstate(msg[4],msg[5],GetNextDeviceaddr(msg[0],my_i));newpower=(newret>>8);
							newstate_=newret;
																	//设备地址，设备类型，开关状态，附加参数
							ret=Cmd_0x03 (GetNextDeviceaddr(msg[0],my_i),msg[0],newpower,newstate_);
							if (ret==0)
							{
								if (newstate_==99)
								{
									Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,1);
									Device_state_see(5,newpower,1);
								}
								else if (newstate_==1)
								{
									Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,2);
									Device_state_see(3,newpower,2);
								}
								else
								{
									Updata_devicestate(GetNextDeviceaddr(msg[0],my_i) ,newpower,0);
									Device_state_see(3,newpower,newstate_);
									Device_state_see(5,newpower,newstate_);
								}
								my_i++;//控制成功，控制下一个
							}
							else
							{
								m++;if (m>10){m=0;my_i++;}//重试10次
							}
						}
					}
				}
				else //控制指定地址的设备
				{
					while (1)
					{
						delay_ms(200);			
						if (CheckId (id)==0)
						{
							//给网口返回执行结果，不存在指定的ip地址
							m_send[0]=5;m_send[1]=ERR_NONEADDR>>8;m_send[2]=ERR_NONEADDR;
							send_messeg(WK_MESSEG,m_send);
							break;
						}
														//设备地址，设备类型，开关状态，附加参数
						ret=Cmd_0x03 (id,GetDeviceType(id),msg[4],msg[5]);
						if (ret==0)
						{
							Updata_devicestate(id ,msg[4],msg[5]);
							Device_state_see(GetDeviceType(id),msg[4],msg[5]);
							//给网口返回执行结果，成功
							m_send[0]=5;m_send[1]=ret>>8;m_send[2]=ret;
							send_messeg(WK_MESSEG,m_send);
							
							break;//控制成功，退出
						}
						else if (GetDeviceState(id)&DEVICEOFFLINE)
						{
							//给网口返回执行结果，设备不在线
							m_send[0]=5;m_send[1]=ERR_OFFLINE>>8;m_send[2]=ERR_OFFLINE;
							send_messeg(WK_MESSEG,m_send);
							break;//设备不在线
						}
						else
						{
							m++;if (m>10){
								//给网口返回执行结果，执行结果
							m_send[0]=5;m_send[1]=ret>>8;m_send[2]=ret;
							send_messeg(WK_MESSEG,m_send);
								
							break;}//重试10次
						}
					}
				}
			}
			RF_SetFocus(3);
			TaskRepend(3);
		}
	}
}


 
u16 CheckId (u16 addr)
{
	u16 i=0;
	for (i=1;EN_CONFIG[i*2];i++)//修复集中器地址为0时，控制器只能开不能关的问题，2018.8.17
	{
		if (EN_CONFIG[i*2]==addr)
		{
			return 1;
		}
	}
	return 0;//没有找到
}





				//找出设备数量
void Updata_DeviceNum (void)
{
	u16 i=0;u16 j=0;
	
	for (i=0;i<DEVICE_DATA_NUM;i++)
	{
		DEVICE_NUM[i]=0;
	}
	DEVICE_NUM[1]=1;//采集器
	DEVICE_NUM[3]=2;//空调
	DEVICE_NUM[5]=3;//除湿机
	DEVICE_NUM[7]=4;//净化器
	DEVICE_NUM[9]=5;//加湿机
	DEVICE_NUM[11]=6;//恒湿机
	for (i=0;i<DEVICE_DATA_NUM;i+=2)
	{
		for (j=2;(EN_CONFIG[j+1]&0x00ff);j+=2)
		{
			if ((DEVICE_NUM[i+1]&0x00ff)==(0x00ff&EN_CONFIG[j+1]))
			{
				DEVICE_NUM[i]++;
			}
		}
	}
}





									//更新集中器中记录的设备状态
u16 Updata_devicestate(u16 addr ,u8 power,u8 state)
{
	u16 i=0;
	u16 state_=0;
	for (i=0;EN_CONFIG[i*2];i++)
	{
		if (EN_CONFIG[i*2]==addr)
		{
			state_= EN_CONFIG[i*2+1]&0x00ff;
			if (power==1)
			{
				state_|=DEVICEON;
			}
			if (state==1)
			{
				state_|=DEVICEUP;
			}
			else if (state==2)
			{
				state_|=DEVICEDOWN;
			}
			EN_CONFIG[i*2+1]=state_;
		}
	}
	return 0;
	
}














						//获取新的控制设备的状态,高8位开关，低8位状态
u16 get_newstate(u8 nowpower,u8 key,u16 addr)
{
	u16 state;
	u8 newpower;
	u8 newstate_;
							//根据协议，当控制状态为2时反向，不为2时输出原状态
	if (nowpower==2)
	{
		state=GetDeviceState(addr);
		if (state&DEVICEON)
		{
			if (state&DEVICEUP)
			{
				if (key==1)//key==1，升按键
				{						//改为关
					newpower=0;newstate_=0;
				} 
				else if (key==2)  //key==2,降按键
				{						//改为降
					newpower=1;newstate_=2;
				}
			}
			else if (state&DEVICEDOWN)
			{
				if (key==1)//key==1，升按键
				{						//改为升
					newpower=1;newstate_=1;
				} 
				else if (key==2)  //key==2,降按键
				{						//改为关
					newpower=0;newstate_=0;
				}				
			}
		}
		else //关着的，改为开
		{
			newpower=1;newstate_=key;
		}
		
	}
	else 
	{
		return (nowpower<<8)|key;
	}
	return (newpower<<8)|newstate_ ;
}


u16 yt_newstate(u8 nowpower,u8 key,u16 addr)
{
	u16 state;
	u8 newpower;
	u8 newstate_;
							//根据协议，当控制状态为2时反向，不为2时输出原状态
	if (nowpower==2)
	{
		state=GetDeviceState(addr);
		if (state&DEVICEON)
		{
			if (state&DEVICEUP)
			{
				if (key==1)//key==1，升按键
				{						//改为关
					newpower=0;newstate_=0;
				} 
				else if (key==2)  //key==2,降按键
				{						//改为降
					newpower=1;newstate_=1;
				}
			}
			else if (state&DEVICEDOWN)
			{
				if (key==1)//key==1，升按键
				{						//改为升
					newpower=1;newstate_=99;
				} 
				else if (key==2)  //key==2,降按键
				{						//改为关
					newpower=0;newstate_=0;
				}				
			}
			else//上位机设置了指定湿度之后是没有升温降温标识的
			{
				if (key==1)
				{
					newstate_=99;
				}
				else if (key==2)
				{
					newstate_=1;
				}
			}
		}
		else //关着的，改为开
		{
			newpower=1;
			if (key==1)
			{
				newstate_=99;
			}
			else if (key==2)
			{
				newstate_=1;
			}
		}
		
	}
	else //不是反向（即是自动控制或者上位机传来的控制），把1改为99,2改为1，实现除湿加湿功能2018.11.17
	{
		if (key==1) newstate_=99;
		else if (key==2) newstate_=1;
		return (nowpower<<8)|newstate_;
	}
	return (newpower<<8)|newstate_ ;
}







					//设备运行状态指示，类型，开关量，状态（升降）
void Device_state_see(u8 type,u8 power,u8 state)
{
	u8 m_send[MESSEG_DATA]={0};//发送的消息
	
						//在这里执行设备状态指示
	m_send[0]=2;//设备状态指示
	if (type==2)//空调
	{
		if (power)
		{
			if (state==1)
			{
				m_send[1]=1;
				m_send[2]=1;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=2;
				send_messeg(LIT_MESSEG,m_send);
			}
			else if (state==2)
			{
				m_send[1]=1;
				m_send[2]=2;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=1;
				send_messeg(LIT_MESSEG,m_send);
			}
		}
		else
		{
				m_send[1]=0;
				m_send[2]=1;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=2;
				send_messeg(LCD_MESSEG,m_send);//改正屏幕的显示不正常bug，2018.10.11
				send_messeg(LIT_MESSEG,m_send);
		}
	}
	else if (type==3||type==5)//除湿机或加湿机
	{
		if (power)
		{
			if (state==1)
			{
				m_send[1]=1;
				m_send[2]=3;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=4;
				send_messeg(LIT_MESSEG,m_send);
			}
			else if (state==2)
			{
				m_send[1]=1;
				m_send[2]=4;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=3;
				send_messeg(LIT_MESSEG,m_send);
			}
		}
		else
		{
				m_send[1]=0;
				m_send[2]=4;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
				m_send[1]=0;
				m_send[2]=3;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);//改正屏幕的显示不正常bug，2018.10.11
		}
	}
	else if (type==4)
	{
		if (power)
		{
				m_send[1]=1;
				m_send[2]=6;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
		}
		else
		{
				m_send[1]=0;
				m_send[2]=6;
				send_messeg(LIT_MESSEG,m_send);
				send_messeg(LCD_MESSEG,m_send);
		}
		
	}

}

















				//无线的数据处理
void my_rf_deal (void * t)
{
	u8 send[MESSEG_DATA]={0};
	u16 temp=0;
	u16 i=0;
	while (1)
	{
		delay_ms(300);//防止程序死在这里								
									//控制器数据
		for (i=1;EN_CONFIG[i*2+1];i++)
		{
			if ((EN_CONFIG[i*2+1]&0x00ff)==1)//是采集器
			{
				//发送采集器离线消息，在线时的消息在消息循环里发送
				if ((EN_CONFIG[i*2+1]&DEVICEOFFLINE))
				{
					send[0]=2;send[1]=1;send[2]=1;//发送采集器的数据
					send[3]=((u32 )EN_DATA>>24);	//设置地址
					send[4]=((u32 )EN_DATA>>16);
					send[5]=((u32 )EN_DATA>>8);
					send[6]=((u32 )EN_DATA);
					send[7]=EN_CONFIG[i*2]>>8;//地址
					send[8]=EN_CONFIG[i*2];
					if (find_messeg(WK_MESSEG)==0)
						send_messeg(WK_MESSEG,send);//发送给网口
				}
			}
			else
			{
				{
					send[0]=2;//发送消息
					send[1]=EN_CONFIG[i*2+1];//类型
					if ((EN_CONFIG[i*2+1]&DEVICEOFFLINE)==0)//设备是在线的
						send[2]=0;//设备在线
					else 
						send[2]=1;
					send[3]=EN_CONFIG[i*2]>>8;//地址
					send[4]=EN_CONFIG[i*2];
					if ((EN_CONFIG[i*2+1]&DEVICEON)==DEVICEON)
					{
						send[5]=1;//设备开
						if ((EN_CONFIG[i*2+1]&DEVICEUP)==DEVICEUP)
						{
							send[6]=1;//升
						}
						else if ((EN_CONFIG[i*2+1]&DEVICEUP)==DEVICEUP)
						{
							send[6]=2;//降
						}
						else
						{
							send[6]=0;
						}
					}
					else
					{
						send[5]=0;		//设备是关的
						send[6]=0;
					}
					if (find_messeg(WK_MESSEG)==0)
						send_messeg(WK_MESSEG,send);//发送给网口
				}
			}
			delay_ms(1000);
		}
	}
	
}



				//获取设备信息，recv是接收数组
u16 Cmd_0x01 (u16 addr ,u8 *recv)
{
	u8 i=0;
	u8 data[12]={0};
	u8 crc[2]={0};
	data[0]=0xff;
	data[1]=0xff;
	data[2]=addr>>8;
	data[3]=addr;
	data[4]=0x01;
	data[5]=0x00;
	data[6]=0x02;
	data[7]=0x00;
	data[8]=16;//想要获取15位数据
	Get_Crc16(data,9,crc);
	data[9]=crc[0];
	data[10]=crc[1];
	RF1_tx_bytes(data,11);
	
	for(i=0;i<5;i++)
	{
		delay_ms(200);
		if (RF_GetCmd (recv,16+9))
		{
			Get_Crc16(recv,recv[6]+7,crc);
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crc校验通过
			{
				if ((((recv[2]<<8)|recv[3])==addr)&&(recv[4]==(data[4]|0x80)))//地址和命令符合
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
		
	}
	return ERR_TIMEOUT;
}

					//控制设备
u16 Cmd_0x03 (u16 addr ,u8 type,u8 power,u8 state)
{
	u16 i=0;
	u8 recv[30];
	u8 data[13]={0};
	u8 crc[2]={0};
	data[0]=0xff;
	data[1]=0xff;
	data[2]=addr>>8;
	data[3]=addr;
	data[4]=0x03;
	data[5]=0x00;
	data[6]=0x03;
	data[7]=type;
	data[8]=power;
	data[9]=state;
	Get_Crc16(data,10,crc);
	data[10]=crc[0];
	data[11]=crc[1];
	RF1_tx_bytes(data,12);
	for(i=0;i<5;i++)
	{
		delay_ms(200);
		if (RF_GetCmd (recv,15+9))
		{
			Get_Crc16(recv,recv[6]+7,crc);
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crc校验通过
			{
				if ((((recv[2]<<8)|recv[3])==addr)&&(recv[4]==(data[4]|0x80)))//地址和命令符合
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
		
	}
	return ERR_TIMEOUT;

}



					//进入配置模式
u16 Cmd_0x06 (u16 addr ,u8 state)
{
	u16 i=0;
	u8 recv[30];
	u8 data[13]={0};
	u8 crc[2]={0};
	data[0]=0xff;
	data[1]=0xff;
	data[2]=addr>>8;
	data[3]=addr;
	data[4]=0x06;
	data[5]=0x00;
	data[6]=0x01;
	data[7]=state;
	Get_Crc16(data,data[6]+7,crc);
	data[8]=crc[0];
	data[9]=crc[1];
	RF1_tx_bytes(data,data[6]+7+2);
	for(i=0;i<5;i++)
	{
		delay_ms(200);
		if (RF_GetCmd (recv,15+9))
		{
			Get_Crc16(recv,recv[6]+7,crc);
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crc校验通过
			{
				if ((recv[4]==(data[4]|0x80)))//命令符合
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
		
	}
	return ERR_TIMEOUT;

}

			//配置基本参数，类型，地址等
u16 Cmd_0x07 (u16 addr ,u8 type)
{
	u16 i=0;
	u8 recv[30];
	u8 data[13]={0};
	u8 crc[2]={0};
	data[0]=0xff;
	data[1]=0xff;
	data[2]=0;
	data[3]=0;
	data[4]=0x07;
	data[5]=0x00;
	data[6]=0x04;
	data[7]=addr>>8;
	data[8]=addr;
	data[9]=type;
	data[10]=0x03;//控制器空调的类型为自定义
	Get_Crc16(data,data[6]+7,crc);
	data[data[6] + 7]=crc[0];
	data[data[6] + 8]=crc[1];
	RF1_tx_bytes(data,data[6]+7+2);
	for(i=0;i<5;i++)
	{
		delay_ms(200);
		if (RF_GetCmd (recv,15+9))
		{
			Get_Crc16(recv,recv[6]+7,crc);
			if (crc[0]==recv[recv[6]+7]&&crc[1]==recv[recv[6]+8])//crc校验通过
			{
				if (recv[4]==(data[4]|0x80))//命令符合
				{
					return (recv[7]<<8)|recv[8];
				}
			}
		}
		
	}
	return ERR_TIMEOUT;

}





			//返回第i个type类型的地址
u16 GetNextDeviceaddr (u8 type,u16 i)
{
	u16 t=0;
	u16 a=0;
	for(t=1;EN_CONFIG[t*2+1];t++)
	{
		if ((EN_CONFIG[t*2+1]&0x00ff)==type)
		{
			if (a==i)
			{
				return EN_CONFIG[t*2];
			}a++;
		}
	}
	return 0;
}

					//返回指定类型的设备数量
u16 GetDeviceNum (u8 type)
{
	u16 i=0;
	for (i=0;DEVICE_NUM[i*2+1];i++)
	{
		if (type==(DEVICE_NUM[i*2+1]&0x00ff))
		{
			return DEVICE_NUM[i*2];
		}
	}
	return  0;
}

	//返回指定地址的设备类型
u8 GetDeviceType (u16 addr)
{
	u16 i=0;
	for (i=0;EN_CONFIG[i*2];i++)
	{
		if (EN_CONFIG[i*2]==addr)
		{
			return EN_CONFIG[i*2+1]&0x00ff;
		}
	}
	return 0xff;
}



					//返回指定地址的设备状态
u16 GetDeviceState(u16 addr)
{
	u16 i=0;
	for (i=1;EN_CONFIG[i*2];i++)//修复集中器地址为0时，控制器只能开不能关的问题，2018.8.17
	{
		if (EN_CONFIG[i*2]==addr)
		{
			return EN_CONFIG[i*2+1]&0xff00;
		}
	}
	return 0xff;
	
}



u16 *GetCfgData (void)
{
	return EN_CONFIG;
}




/////////////////////文件操作相关代码/////////////////////


			//见"file.c"文件



