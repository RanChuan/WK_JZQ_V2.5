
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








