
#include "delay.h"
#include "my_w5500.h"
#include "my_messeg.h"
#include "malloc.h"
#include "cJSON.h"

#include "my_rf.h"//包含这个文件用于获取本机地址
#include "my_lcd.h"//获取手动自动
#include "lcd.h"
#include "includes.h"
#include "my_debug.h"
#include "dhcp.h"
#include "enternet.h"
#include "wk_json.h"

/*******************************************

			msg位定义：
			msg[0],1,有中断，2，有运行状态消息发送.3重启网口，4发送设备上线消息，5，执行结果返回，6，发送环境上下限，7，发送报警
			msg[1],发送消息时，发送的设备类型	，
													返回执行结果时，高8位
													
			msg[2，设备状态，0，在线，1，离线，
													返回执行结果时，低8位
													
			发送设备运行状态消息时
			控制器时：
			msg3,id,高8位
			msg4,id,低8位
			msg5，设备状态，控制状态，1，开
			msg6，控制状态，1，升，2，降
			采集器时：
			msg3~6，采集器数据缓存区地址，由高到低,
			msg7-8,离线时的地址
			

*********************************************/


//网络状态标志位,ip地址冲突，网线没有连接等
u8 NET_STATE=0;
u8 NET_S0_STATE=0;
u8 NET_S1_STATE=0;
u8 NET_S2_STATE=0;

u8 WK_FUCOS=TASK_MAX_NUM;//网口焦点
extern u8 DNS_SERVER[4];

//网络连接状态，调试用，0，未连接，1，已连接上网关，2，已连接上服务器
u8 DBG_INTER_STATE=0;



void my_w5500 (void * t)
{
	cJSON_Hooks a;
	W5500_Initialization();	
	net_test();
	net_sysboot_init();
	a.malloc_fn=mymalloc;
	a.free_fn=myfree;
	cJSON_InitHooks(&a);

	
	while(1)
	{
		delay_ms(100);
//		TaskGetMsg();
		if (net_get_phycstate()==0) continue;//网线没有连接，执行下一次循环
		if (net_check_parameters()==0)//本机ip地址不合法
		{
			DBG_INTER_STATE=dhcp_retry();//自动获取IP地址
			
		}
		if (NET_STATE&CONFLICT)//ip冲突
		{
			DBG_INTER_STATE=dhcp_retry();//自动获取IP地址
		}
		NET_STATE=0;
		my_debug ( );		//调试信息输出
		wk_client();
		
	}
}



		//温控客户端事件处理
void wk_client(void)
{
	u8 m_send[MESSEG_DATA]={0};
	u8 m_recv[MESSEG_DATA]={0};
	static u8 so_state_old=0;
	
	/*中断消息处理*/
	so_state_old|=NET_S0_STATE;
	if (so_state_old&IR_CON)//连接成功发送上线消息
	{
		so_state_old&=~IR_CON;
			m_send[0]=4;//模拟收到了发送上线消息
			send_json_adddevice(m_send);
	}
	if (so_state_old&IR_RECV)//如果有数据等待接收
	{
		so_state_old&=~IR_RECV;
		server_data ();
	}
	NET_S0_STATE=0;
	
	
			/*端口0网络故障处理*/
	if ((net_get_comstate(0)!=SOCK_ESTABLISHED))//如果连接断开
	{		
		if (tcp_connect(0,4545,SERVER_IP,SERVER_PORT)==FALSE)
		{
			if (net_check_gateway()==TRUE)
			{
				DBG_INTER_STATE=1;//连接上网关
				serch_server();//查找服务器
			}
			else
			{
				DBG_INTER_STATE=0;
				DBG_INTER_STATE=dhcp_retry();//自动获取IP地址
			}
		}
		else
		{
			DBG_INTER_STATE=2;//连接上服务器
		}
	}



	//系统内消息处理
	if (get_messeg(WK_MESSEG,m_recv)==0)//如果有消息
	{
		if (DBG_INTER_STATE==2) 
		{
			switch (m_recv[0])
			{
				case 2:
					send_json (m_recv);
					break;
				case 3:
					net_test();
					net_sysboot_init();
					break;
				case 4:
					send_json_adddevice(m_recv);
					break;
				case 6:
					send_json_limit (m_recv);
					break;
				case 7:
					send_json_warn(m_recv); 
					break;
				default:
					break;
			}
		}
	}

}











					//处理网口中断的任务函数
void my_w5500interrupt (void * t)
{
	while (1)
	{
		delay_ms(4);
	}
}

 





























					//上位机设置环境上下限
void json_setting (cJSON *root)
{
	u16 wdup;
	u16 sdup;
	u16 tvocup;
	u16 wddown;
	u16 sddown;
	cJSON *js_controldata = cJSON_GetObjectItem(root, "data");
	wdup= cJSON_GetObjectItem(js_controldata, "wdup")->valueint;
	sdup= cJSON_GetObjectItem(js_controldata, "sdup")->valueint;
	tvocup= cJSON_GetObjectItem(js_controldata, "tvocup")->valueint;
	wddown= cJSON_GetObjectItem(js_controldata, "wddown")->valueint;
	sddown= cJSON_GetObjectItem(js_controldata, "sddown")->valueint;

	Lcd_SetLimitData(0,wdup);
	Lcd_SetLimitData(1,wddown);
	Lcd_SetLimitData(2,sdup);
	Lcd_SetLimitData(3,sddown);
	Lcd_SetLimitData(4,tvocup);
	Save_LCD();
	json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"0");
}
				//设置到指定值
void json_setto (cJSON *root)
{
}

			//开启设备
void json_start (cJSON *root)
{
	
	
	
	u16 devId;
	devId= cJSON_GetObjectItem(root, "devNum")->valueint;
	if (Lcd_GetHandstate()==0)//自动状态不执行手动命令
	{
		json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NotHandMode");
		return ;
	}
	u8 m_send[MESSEG_DATA]={0};
	
	m_send[0]=0xff;
	m_send[1]=0x02;
	m_send[2]=devId>>8;
	m_send[3]=devId;
	m_send[4]=1;
	m_send[5]=0;
	send_messeg(RF_MESSEG,m_send);
	while (1)
	{
		delay_ms(200);//等待操作结果
		if (get_messeg(WK_MESSEG,m_send)==0)
		{
			if (m_send[0]==5)
			{
				u16 err=(m_send[1]<<8)|m_send[2];
				switch (err)
				{
					case ERR_SUCCESS:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"0");
						break;
					case ERR_TIMEOUT:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"TimeOut");
						break;
					case ERR_OFFLINE:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"OffLine");
						break;
					case ERR_NONEADDR:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NoAddr");
						break;
					default:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"OtherErr");
						break;
				}
				break;
			}
		}
	}
}


void json_alarm (cJSON *root)
{
	
	u8 m_send[MESSEG_DATA]={0};
	u8 *phone;
	u8 sms=0;
	u8 call=0;
	cJSON *js_controldata = cJSON_GetObjectItem(root, "data");
	phone=cJSON_GetObjectItem(js_controldata, "phone")->valuestring;
	sms=cJSON_GetObjectItem(js_controldata, "sms")->valueint;
	call=cJSON_GetObjectItem(js_controldata, "call")->valueint;
	
						//发送给短信任务程序
	m_send[0]=3;
	m_send[2]=(u32)phone>>24;
	m_send[3]=(u32)phone>>16;
	m_send[4]=(u32)phone>>8;
	m_send[5]=(u32)phone;
//	send_messeg(DX_MESSEG,m_send);不使用短信模块
	while (1)
	{
		delay_ms(200);//等待操作结果
//		if (get_messeg(DX_MESSEG,m_send)==0)
		{
			if (m_send[0]==5)
			{
				u16 err=(m_send[1]<<8)|m_send[2];
				switch (err)
				{
					case ERR_SUCCESS:
						json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"0");
						break;
					case ERR_FAIL:
						json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"Fail");
						break;
					default:
						json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NoneDevice");
						break;
				}
				break;
			}
		}
	}
}


void json_close (cJSON *root)
{
	
	
	u16 devId;
	devId= cJSON_GetObjectItem(root, "devNum")->valueint;
	if (Lcd_GetHandstate()==0)//自动状态不执行手动命令
	{
		json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NotHandMode");
		return ;
	}
	u8 m_send[MESSEG_DATA]={0};
	
	m_send[0]=0xff;
	m_send[1]=0x02;
	m_send[2]=devId>>8;
	m_send[3]=devId;
	m_send[4]=0;
	m_send[5]=0;
	send_messeg(RF_MESSEG,m_send);
	while (1)
	{
		delay_ms(200);//等待操作结果
		if (get_messeg(WK_MESSEG,m_send)==0)
		{
			if (m_send[0]==5)
			{
				u16 err=(m_send[1]<<8)|m_send[2];
				switch (err)
				{
					case ERR_SUCCESS:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"0");
						break;
					case ERR_TIMEOUT:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"TimeOut");
						break;
					case ERR_OFFLINE:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"OffLine");
						break;
					case ERR_NONEADDR:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NoAddr");
						break;
					default:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"OtherErr");
						break;
				}
				break;
			}
		}
	}

}

void json_mode (cJSON *root)
{
	if (strcmp("hand",cJSON_GetObjectItem(root, "mode")->valuestring)==0)
	{
		Lcd_SetHandstate(1);
		json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"0");
	}
	else if (strcmp("auto",cJSON_GetObjectItem(root, "mode")->valuestring)==0)
	{
		Lcd_SetHandstate(0);
		json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"0");
	}
	else
	{
		json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"ValueErr");
	}

}



void json_version (cJSON *root)
{	
	
	char *out1;
  cJSON *root1,*js_collect1;
  root1 = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(root1,"centerId",Get_MyAddr());
	cJSON_AddStringToObject(root1,"cmdNum",cJSON_GetObjectItem(root, "cmdNum")->valuestring);
	cJSON_AddStringToObject(root1,"errorType","0");
	cJSON_AddNumberToObject(root1,"type",5);
	
	cJSON_AddItemToObject(root1,"data", js_collect1 = cJSON_CreateObject());
	cJSON_AddStringToObject(js_collect1,"version",__DATE__);//加入编译时间
	cJSON_AddStringToObject(js_collect1,"versiontime",__TIME__);//加入编译时间
	
							//取得json字符串
	out1=cJSON_PrintUnformatted(root1);	
	cJSON_Delete(root1);
	server_send_data((u8*)out1);//发送数据，自动写入回车换行
	myfree(out1);
	
}




void json_startup (cJSON *root)
{
	
	
	u16 devId;
	devId= cJSON_GetObjectItem(root, "devNum")->valueint;
	if (Lcd_GetHandstate()==0)//自动状态不执行手动命令
	{
		json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NotHandMode");
		return ;
	}
	u8 m_send[MESSEG_DATA]={0};
	
	m_send[0]=0xff;
	m_send[1]=0x02;
	m_send[2]=devId>>8;
	m_send[3]=devId;
	m_send[4]=1;
	m_send[5]=1;
	send_messeg(RF_MESSEG,m_send);
	while (1)
	{
		delay_ms(200);//等待操作结果
		if (get_messeg(WK_MESSEG,m_send)==0)
		{
			if (m_send[0]==5)
			{
				u16 err=(m_send[1]<<8)|m_send[2];
				switch (err)
				{
					case ERR_SUCCESS:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"0");
						break;
					case ERR_TIMEOUT:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"TimeOut");
						break;
					case ERR_OFFLINE:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"OffLine");
						break;
					case ERR_NONEADDR:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NoAddr");
						break;
					default:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"OtherErr");
						break;
				}
				break;//退出循环
			}
		}
	}

}


void json_startdown (cJSON *root)
{
	
	
	u16 devId;
	devId= cJSON_GetObjectItem(root, "devNum")->valueint;
	if (Lcd_GetHandstate()==0)//自动状态不执行手动命令
	{
		json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NotHandMode");
		return ;
	}
	u8 m_send[MESSEG_DATA]={0};
	
	m_send[0]=0xff;
	m_send[1]=0x02;
	m_send[2]=devId>>8;
	m_send[3]=devId;
	m_send[4]=1;
	m_send[5]=2;
	send_messeg(RF_MESSEG,m_send);
	while (1)
	{
		delay_ms(200);//等待操作结果
		if (get_messeg(WK_MESSEG,m_send)==0)
		{
			if (m_send[0]==5)
			{
				u16 err=(m_send[1]<<8)|m_send[2];
				switch (err)
				{
					case ERR_SUCCESS:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"0");
						break;
					case ERR_TIMEOUT:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"TimeOut");
						break;
					case ERR_OFFLINE:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"OffLine");
						break;
					case ERR_NONEADDR:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NoAddr");
						break;
					default:
						json_return (devId,cJSON_GetObjectItem(root, "cmdNum")->valuestring,"OtherErr");
						break;
				}
				break;
			}
		}
	}
	
}
























				//发送采集器数据,改为新协议2018.12.14
u8 send_json_cj (u8 *msg)
{
				//json使用的变量
	char *out;
  cJSON *root,*js_collect;
  root = cJSON_CreateObject();
	
	
				//我使用的变量
	u8 * data;//采集器数据缓存地址
	u16 id;
	char temp[6];//温度
	char humi[6];//湿度
	char pm2_5[6];
	char tvoc[6];//tvoc
	u8 dd;
	
	data=(u8 *)((msg[3]<<24)|(msg[4]<<16)|(msg[5]<<8)|(msg[6]));//取得地址
	if (msg[2]==1)
	{
		id=(msg[7]<<8)|msg[8];
	}
	else
	{
		id=(data[2]<<8)|data[3];
	}
	

		cJSON_AddNumberToObject(root,"devNum",id);
		cJSON_AddNumberToObject(root,"centerNum",Get_MyAddr());
	  cJSON_AddStringToObject(root,"type","wk");
	  cJSON_AddStringToObject(root,"devType","cjq");//设备类型采集器
	  cJSON_AddStringToObject(root,"cmd","swap");//数据交换

		if (Lcd_GetHandstate())
		{
			cJSON_AddStringToObject(root,"mode","auto");
		}
		else
		{
			cJSON_AddStringToObject(root,"mode","hand");
		}
		

		if (msg[2])//离线
		{
			cJSON_AddStringToObject(root,"devState","offline");
			cJSON_AddItemToObject(root,"data", js_collect = cJSON_CreateObject());
		}
		else	//在线
		{
			cJSON_AddStringToObject(root,"devState","on");
			cJSON_AddItemToObject(root,"data", js_collect = cJSON_CreateObject());
			cJSON_AddNumberToObject(js_collect,"wd",data[17]+data[18]/10.0);
			cJSON_AddNumberToObject(js_collect,"sd",data[19]+data[20]/10.0);
			cJSON_AddNumberToObject(js_collect,"pm25",0);
			cJSON_AddNumberToObject(js_collect,"tvoc",data[21]+data[22]/10.0);
		}
	


								
								//取得json字符串
		out=cJSON_PrintUnformatted(root);	
		dd=mem_perused();
		cJSON_Delete(root);
		server_send_data((u8*)out);//发送数据，自动写入回车换行
		myfree(out);
		dd=mem_perused();

		
		
		
//	delay_ms(500);	
//	if (NET_S0_STATE&IR_RECV)//如果有数据等待接收
//	{
//		NET_S0_STATE&=~IR_RECV;
//		server_data ();
//	}
		
		
	return 0;
}




					//改为新协议，2018.12.14
u8 send_json_kz (u8 *msg)
{
	char *out;
	cJSON *root,*js_control;
	root = cJSON_CreateObject();
	
	u16 id;
	id=(msg[3]<<8)|msg[4];


	cJSON_AddNumberToObject(root,"devNum",id);
	cJSON_AddNumberToObject(root,"centerNum",Get_MyAddr());
	cJSON_AddStringToObject(root,"type","wk");
	cJSON_AddStringToObject(root,"devType",(char *)json_get_devicname(msg[1]));//设备类型采集器
	cJSON_AddStringToObject(root,"cmd","swap");//数据交换
	
		if (msg[2])//离线
		{
			cJSON_AddStringToObject(root,"devState","offline");
			cJSON_AddItemToObject(root,"data", js_control = cJSON_CreateObject());
		}
		else	//在线
		{
			if (msg[5])//开
			{
				if (msg[6]==1)//升温
				{
					cJSON_AddStringToObject(root,"devState","up");
				}
				else if (msg[6]==2)
				{
					cJSON_AddStringToObject(root,"devState","down");
				}
				else
				{
					cJSON_AddStringToObject(root,"devState","on");
				}
			}
			else
			{
				cJSON_AddStringToObject(root,"devState","off");
			}
			cJSON_AddItemToObject(root,"data", js_control = cJSON_CreateObject());
		}


							//取得json字符串
	out=cJSON_PrintUnformatted(root);	cJSON_Delete(root);
	server_send_data((u8*)out );//指定Socket(0~7)发送数据处理,端口0发送23字节数据
	myfree(out);

		
		
//	delay_ms(500);	
//	if (NET_S0_STATE&IR_RECV)//如果有数据等待接收
//	{
//		NET_S0_STATE&=~IR_RECV;
//		server_data ();
//	}
		
		
		
		
	return 0;
}

/*
		消息格式：
		msg0=7，报警消息
		msg1=报警类型，1，温度过高，2，温度过低，3湿度过高，4湿度过低，5tvoc超标，6漏水报警，7，设备故障
		msg2=设备类型
		msg3，msg4，设备地址
		msg5，报警值，实际的环境值
*/
extern u32 Transaction;
#define CMD_NUM (*MAC+Transaction++)



const char warn_char[][8]={"wdup","wddown","sdup","sddown","tvoc","loushui","deverr"};



				//改为新协议2018.12.14
u8 send_json_warn (u8 *msg)
{
	char *out;
	cJSON *root,*js_control;
	root = cJSON_CreateObject();
	
	u16 id;
	id=(msg[3]<<8)|msg[4];


	
	  cJSON_AddNumberToObject(root,"devNum",id);//报警设备地址
		cJSON_AddNumberToObject(root,"centerNum",Get_MyAddr());
		cJSON_AddStringToObject(root,"type","wk");
		cJSON_AddStringToObject(root,"devType",(char *)json_get_devicname(msg[2]));
		cJSON_AddStringToObject(root,"cmd","alarm");
	
	
		char *buff=mymalloc(64);//消息唯一标识
		sprintf(buff,"%d",CMD_NUM); 
		cJSON_AddStringToObject(root,"cmdNum",buff);
	
		if (Lcd_GetHandstate())
		{
			cJSON_AddStringToObject(root,"mode","hand");
		}
		else
		{
			cJSON_AddStringToObject(root,"mode","auto");
		}
	
		cJSON_AddItemToObject(root,"data", js_control = cJSON_CreateObject());
	
		cJSON_AddStringToObject(js_control,"warnType",warn_char[msg[1]-1]);
		cJSON_AddNumberToObject(js_control,"value",msg[5]);



							//取得json字符串
	out=cJSON_PrintUnformatted(root);	cJSON_Delete(root);
	server_send_data((u8*)out);//指定Socket(0~7)发送数据处理,端口0发送23字节数据
	myfree(out);
	myfree(buff);

//	delay_ms(500);	
//	if (NET_S0_STATE&IR_RECV)//如果有数据等待接收
//	{
//		NET_S0_STATE&=~IR_RECV;
//		server_data ();
//	}


		return 0;
}





















				//改为新协议2018.12.14
u8  send_json_limit (u8 *msg)
{
				//json使用的变量
	char *out;
  cJSON *root,*js_collect;
  root = cJSON_CreateObject();

	cJSON_AddNumberToObject(root,"devNum",Get_MyAddr());
	cJSON_AddNumberToObject(root,"centerNum",Get_MyAddr());
	cJSON_AddStringToObject(root,"type","wk");//上传的数据类型是温控
	cJSON_AddStringToObject(root,"devType","jzq");
	cJSON_AddStringToObject(root,"cmd","setenv");//设置环境限度
	if (Lcd_GetHandstate())
	{
		cJSON_AddStringToObject(root,"mode","hand");
	}
	else
	{
		cJSON_AddStringToObject(root,"mode","auto");
	}
	char *buff=mymalloc(64);//消息唯一标识
	sprintf(buff,"%d",CMD_NUM); 
	cJSON_AddStringToObject(root,"cmdNum",buff);
	cJSON_AddStringToObject(root,"devState","on");
	
	
	cJSON_AddItemToObject(root,"data", js_collect = cJSON_CreateObject());
	cJSON_AddNumberToObject(js_collect,"wdup",Lcd_GetLimitData(0));
	cJSON_AddNumberToObject(js_collect,"wddown",Lcd_GetLimitData(1));
	cJSON_AddNumberToObject(js_collect,"sdup",Lcd_GetLimitData(2));
	cJSON_AddNumberToObject(js_collect,"sddown",Lcd_GetLimitData(3));
	cJSON_AddNumberToObject(js_collect,"tvocup",Lcd_GetLimitData(4));


								//取得json字符串
		out=cJSON_PrintUnformatted(root);	
		cJSON_Delete(root);
		server_send_data((u8*)out);//发送数据，自动写入回车换行
	
	
	
		myfree(out);
		myfree(buff);


//	delay_ms(500);	
//	if (NET_S0_STATE&IR_RECV)//如果有数据等待接收
//	{
//		NET_S0_STATE&=~IR_RECV;
//		server_data ();
//	}



return 0;
	
}
	


								//有中断就给网口任务发消息
void EXTI4_IRQHandler(void)
{
	u8 temp=0;u8 sntemp=0;
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);


 
	do
	{
		temp = Read_W5500_1Byte(IR);//读取中断标志寄存器
		Write_W5500_1Byte(IR, (temp&0xf0));//回写清除中断标志
		NET_STATE=temp;


		temp=Read_W5500_1Byte(SIR);//读取端口中断标志寄存器	
		if((temp & S0_INT) == S0_INT)//Socket0事件处理 
		{
			sntemp=Read_W5500_SOCK_1Byte(0,Sn_IR);//读取Socket0中断标志寄存器
			Write_W5500_SOCK_1Byte(0,Sn_IR,sntemp);
			NET_S0_STATE|=sntemp;
		}
		if((temp & S1_INT) == S1_INT)//Socket1事件处理 
		{
			sntemp=Read_W5500_SOCK_1Byte(1,Sn_IR);//读取Socket1中断标志寄存器
			Write_W5500_SOCK_1Byte(1,Sn_IR,sntemp);
			NET_S1_STATE|=sntemp;
		}
		if((temp & S2_INT) == S2_INT)//Socket1事件处理 
		{
			sntemp=Read_W5500_SOCK_1Byte(2,Sn_IR);//读取Socket1中断标志寄存器
			Write_W5500_SOCK_1Byte(2,Sn_IR,sntemp);
			NET_S2_STATE|=sntemp;
		}

	}while(Read_W5500_1Byte(SIR) != 0) ;
	if (NET_S0_STATE||NET_S1_STATE)
	{
		TaskIntSendMsg(7,1);
	}
	if (NET_S2_STATE)
	{
		TaskIntSendMsg(10,1);
	}




	}
}








