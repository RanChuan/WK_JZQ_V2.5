
#include "delay.h"
#include "my_w5500_old.h"
#include "includes.h"
#include "malloc.h"
#include "cJSON.h"

#include "my_rf.h"//包含这个文件用于获取本机地址
#include "my_lcd.h"//获取手动自动
/*******************************************

			msg位定义：
			msg[0],1,有中断，2，有消息发送.3重启网口
			msg[1],发送消息时，发送的设备类型
			msg[2，设备状态，0，在线，1，离线
			控制器时：
			msg3,id,高8位
			msg4,id,低8位
			msg5，设备状态，控制状态，1，开
			msg6，控制状态，1，升，2，降
			采集器时：
			msg3~6，采集器数据缓存区地址，由高到低
			
		详细的消息定义参见my_messeg.h文件

*********************************************/



u8 W5500_Interrupt=0;


void my_w5500 (void * t)
{
	u8 m_recv[MESSEG_DATA]={0};
	cJSON_Hooks a;
	W5500_Initialization();	
	a.malloc_fn=mymalloc;
	a.free_fn=myfree;
	cJSON_InitHooks(&a);
	while(1)
	{
		if (S0_State==0)//如果连接已经就尝试重新连接
		{
				W5500_ReInit();
		}
		if (W5500_Interrupt)
		{
			W5500_Interrupt_Process();
		}
		if (S0_Data&S_RECEIVE)//如果有数据等待接收
		{
			S0_Data&=~S_RECEIVE;
			recv_json ();
		}
		if (get_messeg(WK_MESSEG,m_recv)==0)//如果有消息
		{
			if (m_recv[0]==2)
			{
				send_json (m_recv);
			}
			else if (m_recv[0]==3)
			{
				W5500_ReInit();
			}
		}
		delay_ms(100);
	}
}


//0,已发送，！=0，失败
u8	send_json (u8 *msg)
{
	u8 ret=0;
	if (msg[0]!=2) return 1;//不是发送指令，
	if (msg[2]!=0) return 2;//设备不在线,不在线不发送数据
	switch (msg[1])
	{
		case 1:
			ret=send_json_cj(msg);
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			if (msg[5]==1)//设备开启时发送，关闭状态不发送
				ret=send_json_kz(msg);
			break;
		default:
			return 3;//不支持的设备
	}
	return ret;
}





				//接收网口数据并解析
void recv_json (void)
{
	u8 *data;
	data=mymalloc(2048);//申请2k内存
	Read_SOCK_Data_Buffer(0, data);
	u8 pcbtype;
	cJSON *root = cJSON_Parse((const char *)data);
	pcbtype=cJSON_GetObjectItem(root, "pcbType")->valueint;
	
	
	
	
	switch(pcbtype)
	{
		case 11://更新环境上下限

			break;
		case 12://控制设备
			recv_json_kz(root);
			break;
		case 13://电话号码
			break;
		default :
			break;

	}
	cJSON_Delete(root);
	myfree(data);
}


					//解析并发送控制板指令
void recv_json_kz (cJSON *root)
{
	u16 deviceid=0x0000;
	u8 pcbtype;
	u8 conditionswitch;
	u8 conditionstate;
	u8 humiswitch;
	u8 airswitch;
	u8 humidifier;
	u8 humidity;
	u8 humiset;
	u8 cmd;
	u8 m_send[MESSEG_DATA]={0};
							
										//解析json数据
	cJSON *js_controldata = cJSON_GetObjectItem(root, "data");
	deviceid = cJSON_GetObjectItem(root, "deviceId")->valueint;
	pcbtype = cJSON_GetObjectItem(root, "pcbType")->valueint;

	conditionswitch = cJSON_GetObjectItem(js_controldata, "conditionswitch")->valueint;
	conditionstate = cJSON_GetObjectItem(js_controldata, "conditionstate")->valueint;
	humiswitch = cJSON_GetObjectItem(js_controldata, "humiswitch")->valueint;
	airswitch = cJSON_GetObjectItem(js_controldata, "airswitch")->valueint;
	humidifier=	cJSON_GetObjectItem(js_controldata,"humidifier")->valueint;
	humidity=	cJSON_GetObjectItem(js_controldata,"humidity")->valueint;
	humiset=	cJSON_GetObjectItem(js_controldata,"humiset")->valueint;
	cmd = cJSON_GetObjectItem(js_controldata, "cmd")->valueint;

	if (deviceid)
	{
		m_send[0]=0xff;//控制指定地址的设备
	}
	m_send[1]=2;//手动控制设备
	m_send[2]=deviceid>>8;
	m_send[3]=deviceid;
	if (conditionswitch==1)
	{
		m_send[0]=2;//空调
		m_send[4]=1;
		m_send[5]=conditionstate;
	}
	else if (humiswitch==1)
	{
		m_send[0]=3;//
		m_send[4]=1;
		m_send[5]=2;
	}
	else if (airswitch==1)
	{
		m_send[0]=4;//
		m_send[4]=1;
		m_send[5]=2;
	}
	else if (humidifier==1)
	{
		m_send[0]=5;//
		m_send[4]=1;
		m_send[5]=1;
	}
	else if (humidity==1)//恒湿一体机
	{
		m_send[0]=6;//
		m_send[4]=1;
		m_send[5]=humiset;//设定的湿度值
	}
	if (cmd==2)//改为自动控制
	{
		m_send[0]=0;
		Lcd_SetHandstate(0);//改变自动状态
	}
	else if (cmd==1)//手动
	{
		Lcd_SetHandstate(1);//改变手动自动状态
	}
									//发送给无线
	send_messeg(RF_MESSEG,m_send);	
}





				//发送采集器数据
u8 send_json_cj (u8 *msg)
{
				//json使用的变量
	char *out;
  cJSON *root,*js_collect;
  root = cJSON_CreateObject();
	
	
				//我使用的变量
	u8 * data;//采集器数据缓存地址
	u16 id;
//	char temp[6];//温度
//	char humi[6];//湿度
//	char pm2_5[6];
//	char tvoc[6];//tvoc
	u8 dd;
	
	data=(u8 *)((msg[3]<<24)|(msg[4]<<16)|(msg[5]<<8)|(msg[6]));//取得地址
	
	id=(data[2]<<8)|data[3];
	
	
//	num_to_str((u8*)temp,data[17],data[18]);
//	num_to_str((u8*)humi,data[19],data[20]);
//	num_to_str((u8*)pm2_5,0,0);
//	num_to_str((u8*)tvoc,data[21],data[22]);

		cJSON_AddNumberToObject(root,"dataType",2);
	  cJSON_AddNumberToObject(root,"deviceId",id);
    cJSON_AddNumberToObject(root,"pcbType",11);
		cJSON_AddNumberToObject(root,"centerId",Get_MyAddr());
		

    cJSON_AddItemToObject(root,"data", js_collect = cJSON_CreateObject());
	
    cJSON_AddNumberToObject(js_collect,"temp",data[17]+data[18]/10.0);
		cJSON_AddNumberToObject(js_collect,"humi",data[19]+data[20]/10.0);
		cJSON_AddNumberToObject(js_collect,"pm25",0);
		cJSON_AddNumberToObject(js_collect,"tvoc",data[21]+data[22]/10.0);


								
								//取得json字符串
		out=cJSON_PrintUnformatted(root);	
		dd=mem_perused();
		cJSON_Delete(root);
		Write_SOCK_Data_Buffer(0, (u8*)out, strlen(out));//发送数据，自动写入回车换行
		Write_SOCK_Data_Buffer(0, "\r\n", 2);//发送数据，自动写入回车换行
		myfree(out);
		dd=mem_perused();

	return 0;
}





u8 send_json_kz (u8 *msg)
{
	char *out;
	cJSON *root,*js_control;
	root = cJSON_CreateObject();
	
	u16 id;
	id=(msg[3]<<8)|msg[4];


	cJSON_AddNumberToObject(root,"dataType",2);
	cJSON_AddNumberToObject(root,"deviceId",id);
	cJSON_AddNumberToObject(root,"pcbType",12);
	cJSON_AddNumberToObject(root,"centerId",Get_MyAddr());
	
	cJSON_AddItemToObject(root,"data", js_control = cJSON_CreateObject());
	
	if (msg[1]==2)			//空调
	{
		cJSON_AddNumberToObject(js_control,"conditionswitch",msg[5]);
		cJSON_AddNumberToObject(js_control,"conditionstate",msg[6]);
		cJSON_AddNumberToObject(js_control,"humiswitch",0);
		cJSON_AddNumberToObject(js_control,"airswitch",0);
		cJSON_AddNumberToObject(js_control,"humidifier",0);
		cJSON_AddNumberToObject(js_control,"humidity",0);
		cJSON_AddNumberToObject(js_control,"humiset",0);
	}
	else if (msg[1]==3)			//除湿机
	{
		cJSON_AddNumberToObject(js_control,"humiswitch",msg[5]);
		cJSON_AddNumberToObject(js_control,"conditionstate",0);
		cJSON_AddNumberToObject(js_control,"conditionswitch",0);
		cJSON_AddNumberToObject(js_control,"airswitch",0);
		cJSON_AddNumberToObject(js_control,"humidifier",0);
		cJSON_AddNumberToObject(js_control,"humidity",0);
		cJSON_AddNumberToObject(js_control,"humiset",0);
	}
	else if (msg[1]==4)			//空气净化机
	{
		cJSON_AddNumberToObject(js_control,"airswitch",msg[5]);
		cJSON_AddNumberToObject(js_control,"humiswitch",0);
		cJSON_AddNumberToObject(js_control,"conditionstate",0);
		cJSON_AddNumberToObject(js_control,"conditionswitch",0);
		cJSON_AddNumberToObject(js_control,"humidifier",0);
		cJSON_AddNumberToObject(js_control,"humidity",0);
		cJSON_AddNumberToObject(js_control,"humiset",0);
	}
	else if (msg[1]==5)			//加湿机
	{
		cJSON_AddNumberToObject(js_control,"humidifier",msg[5]);
		cJSON_AddNumberToObject(js_control,"humidity",0);
		cJSON_AddNumberToObject(js_control,"airswitch",0);
		cJSON_AddNumberToObject(js_control,"humiswitch",0);
		cJSON_AddNumberToObject(js_control,"conditionstate",0);
		cJSON_AddNumberToObject(js_control,"conditionswitch",0);
		cJSON_AddNumberToObject(js_control,"humiset",0);
	}
	else if (msg[1]==6)			//恒湿机
	{
		cJSON_AddNumberToObject(js_control,"humidity",msg[5]);
		cJSON_AddNumberToObject(js_control,"humiset",msg[6]);
		cJSON_AddNumberToObject(js_control,"humidifier",0);
		cJSON_AddNumberToObject(js_control,"airswitch",0);
		cJSON_AddNumberToObject(js_control,"humiswitch",0);
		cJSON_AddNumberToObject(js_control,"conditionstate",0);
		cJSON_AddNumberToObject(js_control,"conditionswitch",0);
	}
	cJSON_AddNumberToObject(js_control,"cmd",Lcd_GetHandstate());//手动



							//取得json字符串
	out=cJSON_PrintUnformatted(root);	cJSON_Delete(root);
	Write_SOCK_Data_Buffer(0, (u8*)out, strlen(out));//指定Socket(0~7)发送数据处理,端口0发送23字节数据
		Write_SOCK_Data_Buffer(0, "\r\n", 2);//发送数据，自动写入回车换行
	myfree(out);

	return 0;
}
























								//有中断就给网口任务发消息
void EXTI4_IRQHandler(void)
{
//	u8 m_send[MESSEG_DATA]={0};
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		W5500_Interrupt=1;
//		m_send[0]=1;
//		send_messeg(WK_MESSEG,m_send);
	}
}


