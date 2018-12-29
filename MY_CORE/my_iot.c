

#include "my_iot.h"

#include "baidu_iot.h"

u8 MQTT_STATE=0;

void my_iot (void *t)
{
	u32 msg=0;
	u8 *buff=mymalloc(2048);
	u16 len=0;
	u16 temper=0;
	extern u8 DBG_IP[4];extern u16 DBG_PORT;
	extern u8 NET_S2_STATE;
	
	
	
	while(1)
	{
		delay_ms(5000);//�����ӵ��Ʒ�����
	}
	
	
	while(1)
	{
		if (MQTT_STATE)
		{
			msg=delay_ms(1000*30);
			if (net_get_comstate(2)!=SOCK_ESTABLISHED) 
			{
				MQTT_STATE=0;
				continue;
			}
			if (msg&1)//�����ж�
			{
				if (NET_S2_STATE&IR_RECV)//�����ݽ���
				{
					NET_S2_STATE&=~IR_RECV;
					udp_send(1,DBG_IP,DBG_PORT,"�յ���mqtt����\r\n",16);
					Read_SOCK_Data_Buffer(2,buff);
				}
			}
			if (msg&2)
			{
				//����������
				udp_send(1,DBG_IP,DBG_PORT,"����mqtt������\r\n",16);
				mqtt_keepalive();
			}
			if (msg&0x80000000)
			{
				
				udp_send(1,DBG_IP,DBG_PORT,"����mqtt����\r\n",14);
				len=creat_baidujson(buff,"temper",temper++);
				mqtt_publish("$baidu/iot/shadow/two/update",buff,len);
				continue;
			}
		}
		else
		{
			delay_ms(3000);
			udp_send(1,DBG_IP,DBG_PORT,"��������mqtt������\r\n",20);
			if (1==mqtt_connect("rahher9.mqtt.iot.gz.baidubce.com",1883,"two","rahher9/two","KGa5JL87iGCheRFF"))
			{
				MQTT_STATE=1;
				udp_send(1,DBG_IP,DBG_PORT,"����mqtt�������ɹ�\r\n",20);
			}
		}
		
	}
}







void IOT_Hander(void)
{
	static u8 i=0;
	static u16 sec=0;
	if (i<100)
	{
		i++;
	}
	else 
	{
		i=0;
		if (sec<60)
		{
			sec++;
		}
		else
		{
			sec=0;
			//���﻽��������������
			TaskIntSendMsg(10,2);
		}		
	}
	
}



