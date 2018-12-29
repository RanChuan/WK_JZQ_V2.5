
#include "delay.h"
#include "my_w5500_old.h"
#include "includes.h"
#include "malloc.h"
#include "cJSON.h"

#include "my_rf.h"//��������ļ����ڻ�ȡ������ַ
#include "my_lcd.h"//��ȡ�ֶ��Զ�
/*******************************************

			msgλ���壺
			msg[0],1,���жϣ�2������Ϣ����.3��������
			msg[1],������Ϣʱ�����͵��豸����
			msg[2���豸״̬��0�����ߣ�1������
			������ʱ��
			msg3,id,��8λ
			msg4,id,��8λ
			msg5���豸״̬������״̬��1����
			msg6������״̬��1������2����
			�ɼ���ʱ��
			msg3~6���ɼ������ݻ�������ַ���ɸߵ���
			
		��ϸ����Ϣ����μ�my_messeg.h�ļ�

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
		if (S0_State==0)//��������Ѿ��ͳ�����������
		{
				W5500_ReInit();
		}
		if (W5500_Interrupt)
		{
			W5500_Interrupt_Process();
		}
		if (S0_Data&S_RECEIVE)//��������ݵȴ�����
		{
			S0_Data&=~S_RECEIVE;
			recv_json ();
		}
		if (get_messeg(WK_MESSEG,m_recv)==0)//�������Ϣ
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


//0,�ѷ��ͣ���=0��ʧ��
u8	send_json (u8 *msg)
{
	u8 ret=0;
	if (msg[0]!=2) return 1;//���Ƿ���ָ�
	if (msg[2]!=0) return 2;//�豸������,�����߲���������
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
			if (msg[5]==1)//�豸����ʱ���ͣ��ر�״̬������
				ret=send_json_kz(msg);
			break;
		default:
			return 3;//��֧�ֵ��豸
	}
	return ret;
}





				//�����������ݲ�����
void recv_json (void)
{
	u8 *data;
	data=mymalloc(2048);//����2k�ڴ�
	Read_SOCK_Data_Buffer(0, data);
	u8 pcbtype;
	cJSON *root = cJSON_Parse((const char *)data);
	pcbtype=cJSON_GetObjectItem(root, "pcbType")->valueint;
	
	
	
	
	switch(pcbtype)
	{
		case 11://���»���������

			break;
		case 12://�����豸
			recv_json_kz(root);
			break;
		case 13://�绰����
			break;
		default :
			break;

	}
	cJSON_Delete(root);
	myfree(data);
}


					//���������Ϳ��ư�ָ��
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
							
										//����json����
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
		m_send[0]=0xff;//����ָ����ַ���豸
	}
	m_send[1]=2;//�ֶ������豸
	m_send[2]=deviceid>>8;
	m_send[3]=deviceid;
	if (conditionswitch==1)
	{
		m_send[0]=2;//�յ�
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
	else if (humidity==1)//��ʪһ���
	{
		m_send[0]=6;//
		m_send[4]=1;
		m_send[5]=humiset;//�趨��ʪ��ֵ
	}
	if (cmd==2)//��Ϊ�Զ�����
	{
		m_send[0]=0;
		Lcd_SetHandstate(0);//�ı��Զ�״̬
	}
	else if (cmd==1)//�ֶ�
	{
		Lcd_SetHandstate(1);//�ı��ֶ��Զ�״̬
	}
									//���͸�����
	send_messeg(RF_MESSEG,m_send);	
}





				//���Ͳɼ�������
u8 send_json_cj (u8 *msg)
{
				//jsonʹ�õı���
	char *out;
  cJSON *root,*js_collect;
  root = cJSON_CreateObject();
	
	
				//��ʹ�õı���
	u8 * data;//�ɼ������ݻ����ַ
	u16 id;
//	char temp[6];//�¶�
//	char humi[6];//ʪ��
//	char pm2_5[6];
//	char tvoc[6];//tvoc
	u8 dd;
	
	data=(u8 *)((msg[3]<<24)|(msg[4]<<16)|(msg[5]<<8)|(msg[6]));//ȡ�õ�ַ
	
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


								
								//ȡ��json�ַ���
		out=cJSON_PrintUnformatted(root);	
		dd=mem_perused();
		cJSON_Delete(root);
		Write_SOCK_Data_Buffer(0, (u8*)out, strlen(out));//�������ݣ��Զ�д��س�����
		Write_SOCK_Data_Buffer(0, "\r\n", 2);//�������ݣ��Զ�д��س�����
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
	
	if (msg[1]==2)			//�յ�
	{
		cJSON_AddNumberToObject(js_control,"conditionswitch",msg[5]);
		cJSON_AddNumberToObject(js_control,"conditionstate",msg[6]);
		cJSON_AddNumberToObject(js_control,"humiswitch",0);
		cJSON_AddNumberToObject(js_control,"airswitch",0);
		cJSON_AddNumberToObject(js_control,"humidifier",0);
		cJSON_AddNumberToObject(js_control,"humidity",0);
		cJSON_AddNumberToObject(js_control,"humiset",0);
	}
	else if (msg[1]==3)			//��ʪ��
	{
		cJSON_AddNumberToObject(js_control,"humiswitch",msg[5]);
		cJSON_AddNumberToObject(js_control,"conditionstate",0);
		cJSON_AddNumberToObject(js_control,"conditionswitch",0);
		cJSON_AddNumberToObject(js_control,"airswitch",0);
		cJSON_AddNumberToObject(js_control,"humidifier",0);
		cJSON_AddNumberToObject(js_control,"humidity",0);
		cJSON_AddNumberToObject(js_control,"humiset",0);
	}
	else if (msg[1]==4)			//����������
	{
		cJSON_AddNumberToObject(js_control,"airswitch",msg[5]);
		cJSON_AddNumberToObject(js_control,"humiswitch",0);
		cJSON_AddNumberToObject(js_control,"conditionstate",0);
		cJSON_AddNumberToObject(js_control,"conditionswitch",0);
		cJSON_AddNumberToObject(js_control,"humidifier",0);
		cJSON_AddNumberToObject(js_control,"humidity",0);
		cJSON_AddNumberToObject(js_control,"humiset",0);
	}
	else if (msg[1]==5)			//��ʪ��
	{
		cJSON_AddNumberToObject(js_control,"humidifier",msg[5]);
		cJSON_AddNumberToObject(js_control,"humidity",0);
		cJSON_AddNumberToObject(js_control,"airswitch",0);
		cJSON_AddNumberToObject(js_control,"humiswitch",0);
		cJSON_AddNumberToObject(js_control,"conditionstate",0);
		cJSON_AddNumberToObject(js_control,"conditionswitch",0);
		cJSON_AddNumberToObject(js_control,"humiset",0);
	}
	else if (msg[1]==6)			//��ʪ��
	{
		cJSON_AddNumberToObject(js_control,"humidity",msg[5]);
		cJSON_AddNumberToObject(js_control,"humiset",msg[6]);
		cJSON_AddNumberToObject(js_control,"humidifier",0);
		cJSON_AddNumberToObject(js_control,"airswitch",0);
		cJSON_AddNumberToObject(js_control,"humiswitch",0);
		cJSON_AddNumberToObject(js_control,"conditionstate",0);
		cJSON_AddNumberToObject(js_control,"conditionswitch",0);
	}
	cJSON_AddNumberToObject(js_control,"cmd",Lcd_GetHandstate());//�ֶ�



							//ȡ��json�ַ���
	out=cJSON_PrintUnformatted(root);	cJSON_Delete(root);
	Write_SOCK_Data_Buffer(0, (u8*)out, strlen(out));//ָ��Socket(0~7)�������ݴ���,�˿�0����23�ֽ�����
		Write_SOCK_Data_Buffer(0, "\r\n", 2);//�������ݣ��Զ�д��س�����
	myfree(out);

	return 0;
}
























								//���жϾ͸�����������Ϣ
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


