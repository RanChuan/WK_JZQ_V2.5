
#include "delay.h"
#include "my_w5500.h"
#include "my_messeg.h"
#include "malloc.h"
#include "cJSON.h"

#include "my_rf.h"//��������ļ����ڻ�ȡ������ַ
#include "my_lcd.h"//��ȡ�ֶ��Զ�
#include "lcd.h"
#include "includes.h"
#include "my_debug.h"
#include "dhcp.h"
#include "enternet.h"
#include "wk_json.h"

/*******************************************

			msgλ���壺
			msg[0],1,���жϣ�2��������״̬��Ϣ����.3�������ڣ�4�����豸������Ϣ��5��ִ�н�����أ�6�����ͻ��������ޣ�7�����ͱ���
			msg[1],������Ϣʱ�����͵��豸����	��
													����ִ�н��ʱ����8λ
													
			msg[2���豸״̬��0�����ߣ�1�����ߣ�
													����ִ�н��ʱ����8λ
													
			�����豸����״̬��Ϣʱ
			������ʱ��
			msg3,id,��8λ
			msg4,id,��8λ
			msg5���豸״̬������״̬��1����
			msg6������״̬��1������2����
			�ɼ���ʱ��
			msg3~6���ɼ������ݻ�������ַ���ɸߵ���,
			msg7-8,����ʱ�ĵ�ַ
			

*********************************************/


//����״̬��־λ,ip��ַ��ͻ������û�����ӵ�
u8 NET_STATE=0;
u8 NET_S0_STATE=0;
u8 NET_S1_STATE=0;
u8 NET_S2_STATE=0;

u8 WK_FUCOS=TASK_MAX_NUM;//���ڽ���
extern u8 DNS_SERVER[4];

//��������״̬�������ã�0��δ���ӣ�1�������������أ�2���������Ϸ�����
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
		if (net_get_phycstate()==0) continue;//����û�����ӣ�ִ����һ��ѭ��
		if (net_check_parameters()==0)//����ip��ַ���Ϸ�
		{
			DBG_INTER_STATE=dhcp_retry();//�Զ���ȡIP��ַ
			
		}
		if (NET_STATE&CONFLICT)//ip��ͻ
		{
			DBG_INTER_STATE=dhcp_retry();//�Զ���ȡIP��ַ
		}
		NET_STATE=0;
		my_debug ( );		//������Ϣ���
		wk_client();
		
	}
}



		//�¿ؿͻ����¼�����
void wk_client(void)
{
	u8 m_send[MESSEG_DATA]={0};
	u8 m_recv[MESSEG_DATA]={0};
	static u8 so_state_old=0;
	
	/*�ж���Ϣ����*/
	so_state_old|=NET_S0_STATE;
	if (so_state_old&IR_CON)//���ӳɹ�����������Ϣ
	{
		so_state_old&=~IR_CON;
			m_send[0]=4;//ģ���յ��˷���������Ϣ
			send_json_adddevice(m_send);
	}
	if (so_state_old&IR_RECV)//��������ݵȴ�����
	{
		so_state_old&=~IR_RECV;
		server_data ();
	}
	NET_S0_STATE=0;
	
	
			/*�˿�0������ϴ���*/
	if ((net_get_comstate(0)!=SOCK_ESTABLISHED))//������ӶϿ�
	{		
		if (tcp_connect(0,4545,SERVER_IP,SERVER_PORT)==FALSE)
		{
			if (net_check_gateway()==TRUE)
			{
				DBG_INTER_STATE=1;//����������
				serch_server();//���ҷ�����
			}
			else
			{
				DBG_INTER_STATE=0;
				DBG_INTER_STATE=dhcp_retry();//�Զ���ȡIP��ַ
			}
		}
		else
		{
			DBG_INTER_STATE=2;//�����Ϸ�����
		}
	}



	//ϵͳ����Ϣ����
	if (get_messeg(WK_MESSEG,m_recv)==0)//�������Ϣ
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











					//���������жϵ�������
void my_w5500interrupt (void * t)
{
	while (1)
	{
		delay_ms(4);
	}
}

 







								//���жϾ͸�����������Ϣ
void EXTI4_IRQHandler(void)
{
	u8 temp=0;u8 sntemp=0;
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);


 
	do
	{
		temp = Read_W5500_1Byte(IR);//��ȡ�жϱ�־�Ĵ���
		Write_W5500_1Byte(IR, (temp&0xf0));//��д����жϱ�־
		NET_STATE=temp;


		temp=Read_W5500_1Byte(SIR);//��ȡ�˿��жϱ�־�Ĵ���	
		if((temp & S0_INT) == S0_INT)//Socket0�¼����� 
		{
			sntemp=Read_W5500_SOCK_1Byte(0,Sn_IR);//��ȡSocket0�жϱ�־�Ĵ���
			Write_W5500_SOCK_1Byte(0,Sn_IR,sntemp);
			NET_S0_STATE|=sntemp;
		}
		if((temp & S1_INT) == S1_INT)//Socket1�¼����� 
		{
			sntemp=Read_W5500_SOCK_1Byte(1,Sn_IR);//��ȡSocket1�жϱ�־�Ĵ���
			Write_W5500_SOCK_1Byte(1,Sn_IR,sntemp);
			NET_S1_STATE|=sntemp;
		}
		if((temp & S2_INT) == S2_INT)//Socket1�¼����� 
		{
			sntemp=Read_W5500_SOCK_1Byte(2,Sn_IR);//��ȡSocket1�жϱ�־�Ĵ���
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








