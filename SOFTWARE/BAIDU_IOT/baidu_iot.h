

#ifndef __BAIDU_IOT_H
#define __BAIDU_IOT_H

#include "includes.h"


/*
*				想编写MQTT协议连上百度云，结果对接上位机协议很急
*				只有暂停这个先写协议QAQ
*				2018.12.14
*/
					//mqtt连接消息
u8 mqtt_connect(char *url,u16 port,char *clientID,char *username,char *password);

u8 mqtt_keepalive(void);

u8 mqtt_publish(char *topic,u8 *buff,u16 bufflen);

u16 creat_baidujson(u8 *buff,char *name,u16 value);


#endif









