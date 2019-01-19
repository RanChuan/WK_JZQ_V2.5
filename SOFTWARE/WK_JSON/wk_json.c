
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




