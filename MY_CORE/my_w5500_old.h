
#ifndef __MY_W5500_OLD_H
#define __MY_W5500_OLD_H

#include "sys.h"
#include "w5500.h"
#include "cjson.h"
void my_w5500 (void * t);


				//把数据转化成json格式并发送给网口
u8	send_json (u8 *msg);

				//接收网口数据并解析
void recv_json (void);
					//分发控制板指令
void recv_json_kz (cJSON *root);


				//发送采集器数据
u8 send_json_cj (u8 *msg);
u8 send_json_kz (u8 *msg);


#endif














