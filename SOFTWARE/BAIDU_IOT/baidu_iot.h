

#ifndef __BAIDU_IOT_H
#define __BAIDU_IOT_H

#include "includes.h"


/*
*				���дMQTTЭ�����ϰٶ��ƣ�����Խ���λ��Э��ܼ�
*				ֻ����ͣ�����дЭ��QAQ
*				2018.12.14
*/
					//mqtt������Ϣ
u8 mqtt_connect(char *url,u16 port,char *clientID,char *username,char *password);

u8 mqtt_keepalive(void);

u8 mqtt_publish(char *topic,u8 *buff,u16 bufflen);

u16 creat_baidujson(u8 *buff,char *name,u16 value);


#endif









