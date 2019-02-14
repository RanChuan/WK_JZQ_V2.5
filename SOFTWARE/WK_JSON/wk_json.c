
#include "my_rf.h"//��������ļ����ڻ�ȡ������ַ
#include "my_lcd.h"//��ȡ�ֶ��Զ�
#include "lcd.h"
#include "includes.h"
#include "my_debug.h"

#include "wk_json.h"
#include "w5500.h"
#include "enternet.h"
#include "dhcp.h"


//�����ã���S0���շ������ݸ��Ƶ����Զ˿�
u8 DBG_COPY_TO_S1CK=0;


				//���ݷ��͵��������ӿں���
void server_send_data(u8 *databuff)
{
	extern u8 DBG_IP[4];
	extern u16 DBG_PORT;
	u16 size=strlen((const char *)databuff);
	tcp_send(0,databuff,size);	
	delay_us(500);//tcp��Ҫ�ȴ��������
	tcp_send(0,"_#_",3);		

	delay_us(500);//������ʱ��֤������������
	if (DBG_COPY_TO_S1CK==1)
	{
		udp_send(1,DBG_IP,DBG_PORT,"���ͣ�",6);
		udp_send(1,DBG_IP,DBG_PORT,databuff,size);
		udp_send(1,DBG_IP,DBG_PORT,"\r\n",2);
	}
	else if (DBG_COPY_TO_S1CK==2)
	{
		tcp_send(2,"���ͣ�",6);		
		delay_us(500);//tcp��Ҫ�ȴ��������
		tcp_send(2,databuff,size);		
		delay_us(500);//tcp��Ҫ�ȴ��������
		tcp_send(2,"\r\n",2);		
	}
	delay_us(300);//������ʱ��֤������������
}


			//��ȡ���������������ݽӿں���
void server_read_data(u8 *databuff)
{
	extern u8 DBG_IP[4];
	extern u16 DBG_PORT;


	delay_us(500);//��ʱһ��ʱ�䣬��������Ϊû����ʱ��ɽ������ݲ�����
	socket_recv(0,databuff);
	u16 size=strlen((const char *)databuff);
	delay_us(500);//������ʱ��֤������������
	if (DBG_COPY_TO_S1CK==1)
	{
		udp_send(1,DBG_IP,DBG_PORT,"���գ�",6);
		udp_send(1,DBG_IP,DBG_PORT,databuff,strlen((const char *)databuff));
		udp_send(1,DBG_IP,DBG_PORT,"\r\n",2);
	}
	else if (DBG_COPY_TO_S1CK==2)
	{
		tcp_send(2,"���գ�",6);		
		delay_us(300);//tcp��Ҫ�ȴ��������
		tcp_send(2,databuff,size);		
		delay_us(300);//tcp��Ҫ�ȴ��������
		tcp_send(2,"\r\n",2);		
	}
	delay_us(300);//������ʱ��֤������������
}




				//�����¿ط���������Ϣ�����ڼ���ǲ���json���ݣ���ֹ��������
u8 server_data(void)
{
	u8 *data;
	data=mymalloc(2048);//����2k�ڴ�
	mymemset(data,0,2048);
	server_read_data( data);
	
	//udp������8���ֽڰ�ͷ��tcp����û��
	if (json_cheek((char *)data))//json����
		recv_json(data);
	else if (data[0]=='-')
	{
		myfree(data);
		return 1;
	}
	
	 
	myfree(data);
	return 0;
}



			//���ҷ�����
u8 serch_server(void)
{
//	static u8 serip=254;
	extern u8 Gateway_IP[4];
	extern u8 SERVER_IP[4];
	extern u16 SERVER_PORT;
	extern u8 NET_S0_STATE;
	u8 udpip[4]={255,255,255,255};
	u8 i=2;
	u8 ret=0;
	while (--i)
	{
		if (udp_init(0,4545)==FALSE)
		{
			return FALSE;
		}
		else
		{
			udp_send(0,udpip,6001,"{\"cmd\":\"search\"}_#_",19);
			u16 wait=10;
			while(wait--)
			{
				delay_ms(100);
				if (NET_S0_STATE&IR_RECV) break;
			}
			u8 *data=0;
			if (NET_S0_STATE&IR_RECV)//����յ�����Ϣ
			{
				NET_S0_STATE&=~IR_RECV;
				data=mymalloc(2048);//����2k�ڴ�
				server_read_data( data);
				if (json_cheek((char *)(data+8)))
				{
					cJSON *root = cJSON_Parse((const char *)(data+8));
					if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,"serverconf")==0)
					{
						//�������õ�������Ŀ���ַ
						mymemcpy(SERVER_IP,data,4);
						cJSON *js_controldata = cJSON_GetObjectItem(root, "data");
						SERVER_PORT=cJSON_GetObjectItem(js_controldata, "serverport")->valueint;
						ret= TRUE;
					}
					else
					{
						ret= FALSE;
					}
					cJSON_Delete(root);
				}
				else
				{
					ret= FALSE;
				}
				myfree(data);
			}
			if (ret==TRUE) break;
		}
	}
	return ret;
}


//�����ݷ�װ��json�ٷ��͵�������
//0,�ѷ��ͣ���=0��ʧ��
u8	send_json (u8 *msg)
{
	u8 ret=0;
	if (msg[0]!=2) return 1;//���Ƿ���ָ�
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
				ret=send_json_kz(msg);
			break;
		default:
			return 3;//��֧�ֵ��豸
	}
	return ret;
}




				//json��ʽ�����ݵ��������������
void recv_json (u8 *data) 
{
	cJSON *root = cJSON_Parse((const char *)data);
	
	if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,
		"setenv")==0)//���û���������
	{
		json_setting (root);
	}
	else if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,
		"start")==0)//���õ�ָ��ֵ
	{
		json_start (root);
	}
	else if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,
		"close")==0)
	{
		json_close (root);
	}
	else if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,
		"setmode")==0)
	{
		json_mode (root); 
	}
	else if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,
		"up")==0)
	{
		json_startup (root);
	}
	else if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,
		"down")==0)
	{
		json_startdown (root);
	}
	else if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,
		"version")==0)
	{
		json_version (root);
	}
	else if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,
		"cmdresult")==0)//��λ���ķ�����Ϣ����ʱ�����
	{
	//	json_version (root);
	}
	else
	{
		json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NoneCmd");//��֧�ֵ�����
	}
	
	
	
	cJSON_Delete(root);
}




						//����λ������豸���Ѹ���2018.12.8
u8 send_json_adddevice (u8 * msg)

{
	extern u8 NET_S0_STATE;
	if (msg[0]!=4) return 1;//������Ƿ����豸������Ϣ��ʲôҲ����
				//jsonʹ�õı���
	
	u16 *cfg=GetCfgData(); 
	u16 i=0;//���������豸����
	u16 j=0;//�ȴ���Ϣ��Ӧ��ʱ����
	u16 k=0;//����������Ϣ���Դ���
	for (i=0;cfg[i];)
	{
		char *out;
		cJSON *root,*js_collect;
		root = cJSON_CreateObject();
		u8 *strbuff=mymalloc(64);
		cJSON_AddNumberToObject(root,"devNum",cfg[i]);//�豸���
		cJSON_AddNumberToObject(root,"centerNum",Get_MyAddr());//���������
		cJSON_AddStringToObject(root,"type","wk");//��Ϣ�����¿�
		cJSON_AddStringToObject(root,"devType",json_get_devicname(cfg[i+1]&0x00ff));//�豸����
		cJSON_AddStringToObject(root,"cmd","online");//���͵���������Ϣ

		sprintf (strbuff,"%d",(cfg[i]<<6)+i);//���ü�����id��Ψһ�Բ���Ψһ��ȷ�ϱ���
		cJSON_AddStringToObject(root,"cmdNum",strbuff);//������Ϣ���ȷ�ϱ���

		if (cfg[i+1]&0x0800)//����
		{
			cJSON_AddStringToObject(root,"devState","offline");//		
		}
		else
		{
			if (cfg[i+1]&0x0100)//�豸����״̬
			{
				cJSON_AddStringToObject(root,"devState","on");//		
			}
			else
			{
				cJSON_AddStringToObject(root,"devState","off");//		
			}			
		}
		
		if (Lcd_GetHandstate())
		{
			cJSON_AddStringToObject(root,"mode","hand");//		
		}
		else
		{
			cJSON_AddStringToObject(root,"mode","auto");//		
		}
		
		out=cJSON_PrintUnformatted(root);	
		cJSON_Delete(root);
		server_send_data((u8*)out);//�������ݣ��Զ�д��س�����
		myfree(out);
		for (j=0;j<100;j++)
		{
			delay_ms(5);
			if (NET_S0_STATE&IR_RECV)//������յ�����Ϣ
			{
				NET_S0_STATE&=~IR_RECV;
				u8 *data;
				data=mymalloc(2048);//����2k�ڴ�
				mymemset(data,0,2048);
				server_read_data( data);
				if (!json_cheek((char *)data))
				{
					myfree(data);
					continue;
				}
				cJSON *root = cJSON_Parse((const char *)data);
				if (strcmp(cJSON_GetObjectItem(root, "cmdNum")->valuestring,strbuff)==0)
				{
//					if (strcmp(cJSON_GetObjectItem(root, "type")->valuestring,"wk")==0)
//					{
						i+=2;//���ͳɹ���������һ��
						k=0;
						cJSON_Delete(root);
						myfree(data);
						break;//��ֹ��λ�����ض��
//					}
				}
				else
				{
					cJSON_Delete(root);
					myfree(data);
				}

			}
			else
			{
			}
		}
		myfree(strbuff);//������û�յ���λ�����أ����ͷ��ڴ�
		k++;
		if (k>10)
		{
			break ;
		}
	}
	if (k>10) return -1;
	return 0;
	
}



		//�������ͻ�ȡjson�����ַ���
u8 * json_get_devicname(u8 devtype)
{
	switch (devtype)
	{
		case 0:
			return "jzq";
			break;
		case 1:
			return "cjq";
			break;
		case 2:
			return "znkt";
			break;
		case 3:
			return "csj";
			break;
		case 4:
			return "kqjhq";
			break;
		case 5:
			return "jiasj";
			break;
		case 6:
			return "hsj";
			break;
		default:
			return "nuknown";
			break;
	}
}


					//ִ����λ��������֮��������
void json_return (u8 deviceid,const char * cmdNum,const char *errType)
{
	char *out;
  cJSON *root,*js_collect;
  root = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(root,"devNum",deviceid);//�豸���
	cJSON_AddNumberToObject(root,"centerNum",Get_MyAddr());//���������
	cJSON_AddStringToObject(root,"type","wk");
	cJSON_AddStringToObject(root,"devtype","jzq");
	cJSON_AddStringToObject(root,"cmd","cmdresult");
	cJSON_AddStringToObject(root,"cmdNum",cmdNum);
	cJSON_AddItemToObject(root,"data", js_collect = cJSON_CreateObject());
	cJSON_AddStringToObject(js_collect,"errorType",errType);
	

							//ȡ��json�ַ���
	out=cJSON_PrintUnformatted(root);	
	cJSON_Delete(root);
	server_send_data((u8*)out);//�������ݣ��Զ�д��س�����
	myfree(out);
	
}



		//����һ��json�ַ����Ƿ�����,0,��������1������
u8 json_cheek(char *buff)
{
	char *json;
	u8 num=0;
	json=buff;
	
	if (*json!='{')//����json�ַ���
		return 0;
	while(*json)
	{
		if (*json=='{')
		{
			num++;
		}
		else if (*json=='}')
		{
			num--;
		}
		if (num==0)//�Ѿ��ҵ�һ��������json�ַ���
		{
			json[1]=0;//���������
			return 1;
		}
		json++;
	}
	if (num)//json�ַ���������
	{
		return 0;
	}
}







//Ӧ��λ�����Ҫ�󣬰���ֵ��Ϊ�ַ����͡�2019.1.18
					//��λ�����û���������
void json_setting (cJSON *root)
{
	u16 wdup;
	u16 sdup;
	u16 tvocup;
	u16 wddown;
	u16 sddown;
	cJSON *js_controldata = cJSON_GetObjectItem(root, "data");
	if (js_controldata==0)
	{
		json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NoDataItem");
		return;
	}
//	wdup= cJSON_GetObjectItem(js_controldata, "wdup")->valueint;
//	sdup= cJSON_GetObjectItem(js_controldata, "sdup")->valueint;
//	tvocup= cJSON_GetObjectItem(js_controldata, "tvocup")->valueint;
//	wddown= cJSON_GetObjectItem(js_controldata, "wddown")->valueint;
//	sddown= cJSON_GetObjectItem(js_controldata, "sddown")->valueint;

	
	wdup=str2num((u8 *)cJSON_GetObjectItem(js_controldata, "wdup")->valuestring);
	sdup=str2num((u8 *)cJSON_GetObjectItem(js_controldata, "sdup")->valuestring);
	tvocup=str2num((u8 *)cJSON_GetObjectItem(js_controldata, "tvocup")->valuestring);
	wddown=str2num((u8 *)cJSON_GetObjectItem(js_controldata, "wddown")->valuestring);
	sddown=str2num((u8 *)cJSON_GetObjectItem(js_controldata, "sddown")->valuestring);
	
	
	
	Lcd_SetLimitData(0,wdup);
	Lcd_SetLimitData(1,wddown);
	Lcd_SetLimitData(2,sdup);
	Lcd_SetLimitData(3,sddown);
	Lcd_SetLimitData(4,tvocup);
	Save_LCD();
	json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"0");
}
				//���õ�ָ��ֵ
void json_setto (cJSON *root)
{
}

			//�����豸
void json_start (cJSON *root)
{
	
	
	
	u16 devId;
	devId= cJSON_GetObjectItem(root, "devNum")->valueint;
	if (Lcd_GetHandstate()==0)//�Զ�״̬��ִ���ֶ�����
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
		delay_ms(200);//�ȴ��������
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
	
						//���͸������������
	m_send[0]=3;
	m_send[2]=(u32)phone>>24;
	m_send[3]=(u32)phone>>16;
	m_send[4]=(u32)phone>>8;
	m_send[5]=(u32)phone;
//	send_messeg(DX_MESSEG,m_send);��ʹ�ö���ģ��
	while (1)
	{
		delay_ms(200);//�ȴ��������
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
	if (Lcd_GetHandstate()==0)//�Զ�״̬��ִ���ֶ�����
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
		delay_ms(200);//�ȴ��������
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
	cJSON *js_controldata = cJSON_GetObjectItem(root, "data");
	if (js_controldata==0) 
	{
		json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NoDataItem");
		return ;
	}
	if (strcmp("hand",cJSON_GetObjectItem(js_controldata, "mode")->valuestring)==0)
	{
		Lcd_SetHandstate(1);
		json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"0");
	}
	else if (strcmp("auto",cJSON_GetObjectItem(js_controldata, "mode")->valuestring)==0)
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
	cJSON_AddStringToObject(js_collect1,"version",__DATE__);//�������ʱ��
	cJSON_AddStringToObject(js_collect1,"versiontime",__TIME__);//�������ʱ��
	
							//ȡ��json�ַ���
	out1=cJSON_PrintUnformatted(root1);	
	cJSON_Delete(root1);
	server_send_data((u8*)out1);//�������ݣ��Զ�д��س�����
	myfree(out1);
	
}




void json_startup (cJSON *root)
{
	
	
	u16 devId;
	devId= cJSON_GetObjectItem(root, "devNum")->valueint;
	if (Lcd_GetHandstate()==0)//�Զ�״̬��ִ���ֶ�����
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
		delay_ms(200);//�ȴ��������
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
				break;//�˳�ѭ��
			}
		}
	}

}


void json_startdown (cJSON *root)
{
	
	
	u16 devId;
	devId= cJSON_GetObjectItem(root, "devNum")->valueint;
	if (Lcd_GetHandstate()==0)//�Զ�״̬��ִ���ֶ�����
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
		delay_ms(200);//�ȴ��������
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
























				//���Ͳɼ�������,��Ϊ��Э��2018.12.14
u8 send_json_cj (u8 *msg)
{
				//jsonʹ�õı���
	char *out;
  cJSON *root,*js_collect;
  root = cJSON_CreateObject();
	
	
				//��ʹ�õı���
	u8 * data;//�ɼ������ݻ����ַ
	u16 id;
	char temp[6];//�¶�
	char humi[6];//ʪ��
	char pm2_5[6];
	char tvoc[6];//tvoc
	u8 dd;
	
	data=(u8 *)((msg[3]<<24)|(msg[4]<<16)|(msg[5]<<8)|(msg[6]));//ȡ�õ�ַ
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
	  cJSON_AddStringToObject(root,"devType","cjq");//�豸���Ͳɼ���
	  cJSON_AddStringToObject(root,"cmd","swap");//���ݽ���

		if (Lcd_GetHandstate()==0)
		{
			cJSON_AddStringToObject(root,"mode","auto");
		}
		else
		{
			cJSON_AddStringToObject(root,"mode","hand");
		}
		

		if (msg[2])//����
		{
			cJSON_AddStringToObject(root,"devState","offline");
			cJSON_AddItemToObject(root,"data", js_collect = cJSON_CreateObject());
		}
		else	//����
		{
			cJSON_AddStringToObject(root,"devState","on");
			cJSON_AddItemToObject(root,"data", js_collect = cJSON_CreateObject());
			cJSON_AddNumberToObject(js_collect,"wd",data[17]+data[18]/10.0);
			cJSON_AddNumberToObject(js_collect,"sd",data[19]+data[20]/10.0);
			cJSON_AddNumberToObject(js_collect,"pm25",0);
			cJSON_AddNumberToObject(js_collect,"tvoc",data[21]+data[22]/10.0);
		}
	


								
								//ȡ��json�ַ���
		out=cJSON_PrintUnformatted(root);	
		dd=mem_perused();
		cJSON_Delete(root);
		server_send_data((u8*)out);//�������ݣ��Զ�д��س�����
		myfree(out);
		dd=mem_perused();

		
		
		
//	delay_ms(500);	
//	if (NET_S0_STATE&IR_RECV)//��������ݵȴ�����
//	{
//		NET_S0_STATE&=~IR_RECV;
//		server_data ();
//	}
		
		
	return 0;
}




					//��Ϊ��Э�飬2018.12.14
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
	cJSON_AddStringToObject(root,"devType",(char *)json_get_devicname(msg[1]));//�豸���Ͳɼ���
	cJSON_AddStringToObject(root,"cmd","swap");//���ݽ���
	
		if (msg[2])//����
		{
			cJSON_AddStringToObject(root,"devState","offline");
			cJSON_AddItemToObject(root,"data", js_control = cJSON_CreateObject());
		}
		else	//����
		{
			if (Lcd_GetHandstate()==0)
			{
				cJSON_AddStringToObject(root,"mode","auto");
			}
			else
			{
				cJSON_AddStringToObject(root,"mode","hand");
			}
			if (msg[5])//��
			{
				if (msg[6]==1)//����
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


							//ȡ��json�ַ���
	out=cJSON_PrintUnformatted(root);	cJSON_Delete(root);
	server_send_data((u8*)out );//ָ��Socket(0~7)�������ݴ���,�˿�0����23�ֽ�����
	myfree(out);

		
		
//	delay_ms(500);	
//	if (NET_S0_STATE&IR_RECV)//��������ݵȴ�����
//	{
//		NET_S0_STATE&=~IR_RECV;
//		server_data ();
//	}
		
		
		
		
	return 0;
}

/*
		��Ϣ��ʽ��
		msg0=7��������Ϣ
		msg1=�������ͣ�1���¶ȹ��ߣ�2���¶ȹ��ͣ�3ʪ�ȹ��ߣ�4ʪ�ȹ��ͣ�5tvoc���꣬6©ˮ������7���豸����
		msg2=�豸����
		msg3��msg4���豸��ַ
		msg5������ֵ��ʵ�ʵĻ���ֵ
*/
extern u32 Transaction;
#define CMD_NUM (*MAC+Transaction++)



const char warn_char[][8]={"wdup","wddown","sdup","sddown","tvoc","loushui","deverr"};



				//��Ϊ��Э��2018.12.14
u8 send_json_warn (u8 *msg)
{
	char *out;
	cJSON *root,*js_control;
	root = cJSON_CreateObject();
	
	u16 id;
	id=(msg[3]<<8)|msg[4];


	
	  cJSON_AddNumberToObject(root,"devNum",id);//�����豸��ַ
		cJSON_AddNumberToObject(root,"centerNum",Get_MyAddr());
		cJSON_AddStringToObject(root,"type","wk");
		cJSON_AddStringToObject(root,"devType",(char *)json_get_devicname(msg[2]));
		cJSON_AddStringToObject(root,"cmd","alarm");
	
	
		char *buff=mymalloc(64);//��ϢΨһ��ʶ
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



							//ȡ��json�ַ���
	out=cJSON_PrintUnformatted(root);	cJSON_Delete(root);
	server_send_data((u8*)out);//ָ��Socket(0~7)�������ݴ���,�˿�0����23�ֽ�����
	myfree(out);
	myfree(buff);

//	delay_ms(500);	
//	if (NET_S0_STATE&IR_RECV)//��������ݵȴ�����
//	{
//		NET_S0_STATE&=~IR_RECV;
//		server_data ();
//	}


		return 0;
}





















				//��Ϊ��Э��2018.12.14
u8  send_json_limit (u8 *msg)
{
				//jsonʹ�õı���
	char *out;
  cJSON *root,*js_collect;
  root = cJSON_CreateObject();

	cJSON_AddNumberToObject(root,"devNum",Get_MyAddr());
	cJSON_AddNumberToObject(root,"centerNum",Get_MyAddr());
	cJSON_AddStringToObject(root,"type","wk");//�ϴ��������������¿�
	cJSON_AddStringToObject(root,"devType","jzq");
	cJSON_AddStringToObject(root,"cmd","setenv");//���û����޶�
	if (Lcd_GetHandstate())
	{
		cJSON_AddStringToObject(root,"mode","hand");
	}
	else
	{
		cJSON_AddStringToObject(root,"mode","auto");
	}
	char *buff=mymalloc(64);//��ϢΨһ��ʶ
	sprintf(buff,"%d",CMD_NUM); 
	cJSON_AddStringToObject(root,"cmdNum",buff);
	cJSON_AddStringToObject(root,"devState","on");
	
	
	cJSON_AddItemToObject(root,"data", js_collect = cJSON_CreateObject());
	cJSON_AddNumberToObject(js_collect,"wdup",Lcd_GetLimitData(0));
	cJSON_AddNumberToObject(js_collect,"wddown",Lcd_GetLimitData(1));
	cJSON_AddNumberToObject(js_collect,"sdup",Lcd_GetLimitData(2));
	cJSON_AddNumberToObject(js_collect,"sddown",Lcd_GetLimitData(3));
	cJSON_AddNumberToObject(js_collect,"tvocup",Lcd_GetLimitData(4));


								//ȡ��json�ַ���
		out=cJSON_PrintUnformatted(root);	
		cJSON_Delete(root);
		server_send_data((u8*)out);//�������ݣ��Զ�д��س�����
	
	
	
		myfree(out);
		myfree(buff);


//	delay_ms(500);	
//	if (NET_S0_STATE&IR_RECV)//��������ݵȴ�����
//	{
//		NET_S0_STATE&=~IR_RECV;
//		server_data ();
//	}



return 0;
	
}
	





