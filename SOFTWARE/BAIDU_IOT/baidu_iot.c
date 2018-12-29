

#include "baidu_iot.h"



//mqtt服务器IP
//u8 MQTT_IP[4]={0xa3,0xb1,0x96,0x0b};
u8 MQTT_IP[4]={0};


//url，要连接的服务器地址，port，要连接的端口，clienntID，用户名，password，密码
u8 mqtt_connect(char *url,u16 port,char *clientID,char *username,char *password)
{ 
//	u8 serverip[4]={0xa3,0xb1,0x96,0x0b};
//	u8 serverip[4]={192,168,43,121};
	u8 i=0;
	if (!(MQTT_IP[0]||MQTT_IP[1]||MQTT_IP[2]||MQTT_IP[3]))
	{
		for (i=0;i<5;i++)
		{
			if (dns_query(2,(u8*)url,MQTT_IP))
			{
				udp_close(2);
				break;
			}
		}
		if (i>=4) return 200;
	}
	
	if (net_get_comstate(2)!=SOCK_ESTABLISHED) 
	{
		if (tcp_connect(2,666,MQTT_IP,port)==FALSE)
		{
			tcp_close(2);
			return 201;
		}
	}
	
	int len=0;
	MQTTPacket_connectData connect=MQTTPacket_connectData_initializer;
	u8 *sendbuff=mymalloc(512);//申请数据包内存
	connect.clientID.cstring=clientID;
	connect.clientID.lenstring.data=username;
	connect.username.cstring=username;
	connect.password.cstring=password;
	len=MQTTSerialize_connect(sendbuff,512,&connect);
	if (len==0) 
	{
		myfree(sendbuff);
		return 202;
	}
	tcp_send(2,sendbuff,len);
	myfree(sendbuff);
	
	
	
	extern u8 NET_S2_STATE;
	u8 *recvbuff=mymalloc(2048);
	u8 t1,t2;
	int ret=0;
	for (i=0;i<2;i++)
	{
		delay_ms(1000);
		if (NET_S2_STATE&IR_RECV)
		{
			NET_S2_STATE&=~IR_RECV;
			Read_SOCK_Data_Buffer(2,recvbuff);
			ret=MQTTDeserialize_connack(&t1,&t2,recvbuff,2048);
			if (ret==1) break;
		}
	}
	myfree(recvbuff);
	return ret;
}


//1,成功，0，失败
//发送心跳包
u8 mqtt_keepalive(void)
{


	if (net_get_comstate(2)!=SOCK_ESTABLISHED) return 0;
	
	u16 datalen;
	u8 *sendbuff=mymalloc(512);
	
	datalen=MQTTSerialize_pingreq(sendbuff,512);
	if (datalen!=0)
	{
		tcp_send(2,sendbuff,datalen);
	}
	myfree(sendbuff);



	u16 i=0;
	u8 ret=0;
	extern u8 NET_S2_STATE;
	u8 *recvbuff=mymalloc(2048);
	for (i=0;i<2;i++)
	{
		delay_ms(1000);
		if (NET_S2_STATE&IR_RECV)
		{
			NET_S2_STATE&=~IR_RECV;
			Read_SOCK_Data_Buffer(2,recvbuff);
			ret=1;
			if (ret==1) break;
		}
	}

	myfree(recvbuff);
	return ret;
}


//1,成功，0，失败
//发送消息，topic，话题，buff，要发送的数据，bufflen，数据长度
u8 mqtt_publish(char *topic,u8 *buff,u16 bufflen)
{
	
	
	if (net_get_comstate(2)!=SOCK_ESTABLISHED) return 0;
	u16 datalen;
	u8 *sendbuff=mymalloc(512);
	MQTTString mqtttopic={0};
	mqtttopic.cstring=topic;
	datalen=MQTTSerialize_publish(sendbuff,512,0,0,0,0,mqtttopic,buff,bufflen);
	if (datalen!=0)
	{
		tcp_send(2,sendbuff,datalen);
	}
	myfree(sendbuff);


	u16 i=0;
	u8 ret=0;
	extern u8 NET_S2_STATE;
	u8 *recvbuff=mymalloc(2048);
	for (i=0;i<100;i++)
	{
		if (NET_S2_STATE&IR_RECV)
		{
			NET_S2_STATE&=~IR_RECV;
			Read_SOCK_Data_Buffer(2,recvbuff);
			ret=1;
			if (ret==1) break;
		}
		delay_ms(20);
	}

	myfree(recvbuff);
	return ret;

	
}

		//生成百度格式的json,buff,输出缓冲区，返回值是长度
u16 creat_baidujson(u8 *buff,char *name,u16 value)
{
	char *out;
  cJSON *root,*js_data1,*js_data2;
	 root = cJSON_CreateObject();
	cJSON_AddItemToObject(root,"desired", js_data1 = cJSON_CreateObject());
	cJSON_AddItemToObject(root,"reported", js_data2 = cJSON_CreateObject());
	cJSON_AddNumberToObject(js_data2,name,value);


	
								//取得json字符串
	out=cJSON_PrintUnformatted(root);	
	cJSON_Delete(root);
	u16 len=0;
	len=strlen(out);
	mymemcpy(buff,out,len);
	myfree(out);

	return len;
	
}


