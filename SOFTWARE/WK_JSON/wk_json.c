
#include "my_rf.h"//包含这个文件用于获取本机地址
#include "my_lcd.h"//获取手动自动
#include "lcd.h"
#include "includes.h"
#include "my_debug.h"

#include "wk_json.h"
#include "w5500.h"
#include "enternet.h"
#include "dhcp.h"


//调试用，把S0的收发的数据复制到调试端口
u8 DBG_COPY_TO_S1CK=0;


				//数据发送到服务器接口函数
void server_send_data(u8 *databuff)
{
	extern u8 DBG_IP[4];
	extern u16 DBG_PORT;
	u16 size=strlen((const char *)databuff);
	tcp_send(0,databuff,size);	
	delay_us(500);//tcp需要等待发送完成
	tcp_send(0,"_#_",3);		

	delay_us(500);//短暂延时保证数据正常发送
	if (DBG_COPY_TO_S1CK==1)
	{
		udp_send(1,DBG_IP,DBG_PORT,"发送：",6);
		udp_send(1,DBG_IP,DBG_PORT,databuff,size);
		udp_send(1,DBG_IP,DBG_PORT,"\r\n",2);
	}
	else if (DBG_COPY_TO_S1CK==2)
	{
		tcp_send(2,"发送：",6);		
		delay_us(500);//tcp需要等待发送完成
		tcp_send(2,databuff,size);		
		delay_us(500);//tcp需要等待发送完成
		tcp_send(2,"\r\n",2);		
	}
	delay_us(300);//短暂延时保证数据正常发送
}


			//读取服务器发来的数据接口函数
void server_read_data(u8 *databuff)
{
	extern u8 DBG_IP[4];
	extern u16 DBG_PORT;


	delay_us(500);//延时一段时间，可能是因为没有延时造成接收数据不完整
	socket_recv(0,databuff);
	u16 size=strlen((const char *)databuff);
	delay_us(500);//短暂延时保证数据正常发送
	if (DBG_COPY_TO_S1CK==1)
	{
		udp_send(1,DBG_IP,DBG_PORT,"接收：",6);
		udp_send(1,DBG_IP,DBG_PORT,databuff,strlen((const char *)databuff));
		udp_send(1,DBG_IP,DBG_PORT,"\r\n",2);
	}
	else if (DBG_COPY_TO_S1CK==2)
	{
		tcp_send(2,"接收：",6);		
		delay_us(300);//tcp需要等待发送完成
		tcp_send(2,databuff,size);		
		delay_us(300);//tcp需要等待发送完成
		tcp_send(2,"\r\n",2);		
	}
	delay_us(300);//短暂延时保证数据正常发送
}




				//接收温控服务器的消息，用于检测是不是json数据，防止意外重启
u8 server_data(void)
{
	u8 *data;
	data=mymalloc(2048);//申请2k内存
	mymemset(data,0,2048);
	server_read_data( data);
	
	//udp数据有8个字节包头，tcp数据没有
	if (json_cheek((char *)data))//json数据
		recv_json(data);
	else if (data[0]=='-')
	{
		myfree(data);
		return 1;
	}
	
	 
	myfree(data);
	return 0;
}



			//查找服务器
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
			if (NET_S0_STATE&IR_RECV)//如果收到了消息
			{
				NET_S0_STATE&=~IR_RECV;
				data=mymalloc(2048);//申请2k内存
				server_read_data( data);
				if (json_cheek((char *)(data+8)))
				{
					cJSON *root = cJSON_Parse((const char *)(data+8));
					if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,"serverconf")==0)
					{
						//重新设置调试主机目标地址
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


//将数据封装成json再发送到服务器
//0,已发送，！=0，失败
u8	send_json (u8 *msg)
{
	u8 ret=0;
	if (msg[0]!=2) return 1;//不是发送指令，
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
			return 3;//不支持的设备
	}
	return ret;
}




				//json格式的数据调用这个函数解析
void recv_json (u8 *data) 
{
	cJSON *root = cJSON_Parse((const char *)data);
	
	if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,
		"setenv")==0)//设置环境上下限
	{
		json_setting (root);
	}
	else if (strcmp(cJSON_GetObjectItem(root, "cmd")->valuestring,
		"start")==0)//设置到指定值
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
		"cmdresult")==0)//上位机的返回信息，暂时不理会
	{
	//	json_version (root);
	}
	else
	{
		json_return (Get_MyAddr(),cJSON_GetObjectItem(root, "cmdNum")->valuestring,"NoneCmd");//不支持的命令
	}
	
	
	
	cJSON_Delete(root);
}




						//向上位机添加设备，已更改2018.12.8
u8 send_json_adddevice (u8 * msg)

{
	extern u8 NET_S0_STATE;
	if (msg[0]!=4) return 1;//如果不是发送设备上线消息就什么也不做
				//json使用的变量
	
	u16 *cfg=GetCfgData(); 
	u16 i=0;//发送上线设备计数
	u16 j=0;//等待消息回应超时计数
	u16 k=0;//发送上线消息重试次数
	for (i=0;cfg[i];)
	{
		char *out;
		cJSON *root,*js_collect;
		root = cJSON_CreateObject();
		u8 *strbuff=mymalloc(64);
		cJSON_AddNumberToObject(root,"devNum",cfg[i]);//设备编号
		cJSON_AddNumberToObject(root,"centerNum",Get_MyAddr());//集中器编号
		cJSON_AddStringToObject(root,"type","wk");//消息类型温控
		cJSON_AddStringToObject(root,"devType",json_get_devicname(cfg[i+1]&0x00ff));//设备类型
		cJSON_AddStringToObject(root,"cmd","online");//发送的是上线消息

		sprintf (strbuff,"%d",(cfg[i]<<6)+i);//利用集中器id的唯一性产生唯一的确认编码
		cJSON_AddStringToObject(root,"cmdNum",strbuff);//发送消息身份确认编码

		if (cfg[i+1]&0x0800)//离线
		{
			cJSON_AddStringToObject(root,"devState","offline");//		
		}
		else
		{
			if (cfg[i+1]&0x0100)//设备上线状态
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
		server_send_data((u8*)out);//发送数据，自动写入回车换行
		myfree(out);
		for (j=0;j<100;j++)
		{
			delay_ms(5);
			if (NET_S0_STATE&IR_RECV)//如果接收到了消息
			{
				NET_S0_STATE&=~IR_RECV;
				u8 *data;
				data=mymalloc(2048);//申请2k内存
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
						i+=2;//发送成功，发送下一个
						k=0;
						cJSON_Delete(root);
						myfree(data);
						break;//防止上位机返回多次
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
		myfree(strbuff);//无论收没收到上位机返回，都释放内存
		k++;
		if (k>10)
		{
			break ;
		}
	}
	if (k>10) return -1;
	return 0;
	
}



		//根据类型获取json类型字符串
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


					//执行上位机的命令之后结果返回
void json_return (u8 deviceid,const char * cmdNum,const char *errType)
{
	char *out;
  cJSON *root,*js_collect;
  root = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(root,"devNum",deviceid);//设备编号
	cJSON_AddNumberToObject(root,"centerNum",Get_MyAddr());//集中器编号
	cJSON_AddStringToObject(root,"type","wk");
	cJSON_AddStringToObject(root,"devtype","jzq");
	cJSON_AddStringToObject(root,"cmd","cmdresult");
	cJSON_AddStringToObject(root,"cmdNum",cmdNum);
	cJSON_AddItemToObject(root,"data", js_collect = cJSON_CreateObject());
	cJSON_AddStringToObject(js_collect,"errorType",errType);
	

							//取得json字符串
	out=cJSON_PrintUnformatted(root);	
	cJSON_Delete(root);
	server_send_data((u8*)out);//发送数据，自动写入回车换行
	myfree(out);
	
}



		//检验一个json字符串是否完整,0,不完整，1，完整
u8 json_cheek(char *buff)
{
	char *json;
	u8 num=0;
	json=buff;
	
	if (*json!='{')//不是json字符串
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
		if (num==0)//已经找到一个完整的json字符串
		{
			json[1]=0;//丢弃后面的
			return 1;
		}
		json++;
	}
	if (num)//json字符串不完整
	{
		return 0;
	}
}




