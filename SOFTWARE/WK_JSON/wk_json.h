

#ifndef __WK_JSON_H
#define __WK_JSON_H

#include "sys.h"
#include "cjson.h"
#include "includes.h"

				//把数据转化成json格式并发送给网口
u8	send_json (u8 *msg);
u8  send_json_limit (u8 *msg);
u8  send_json_warn(u8 *msg);
				//接收json数据并解析
void recv_json (u8 *data);

					//上位机下发下来的命令
void json_setting (cJSON *root);
void json_setto (cJSON *root);
void json_start (cJSON *root);
void json_alarm (cJSON *root);
void json_close (cJSON *root);
void json_mode (cJSON *root);
void json_version (cJSON *root);
void json_startup (cJSON *root);
void json_startdown (cJSON *root);


					//执行上位机的命令之后结果返回
void json_return (u8 deviceid,const char * cmdNum,const char *errType);

				//交换数据
void server_read_data(u8 *databuff);
void server_send_data(u8 *databuff);

				//发送采集器数据
u8 send_json_cj (u8 *msg);
u8 send_json_kz (u8 *msg);
						//发送上线消息
u8 send_json_adddevice (u8 *msg);


			//查找服务器
u8 serch_server(void);

				//接收温控服务器的消息
u8 server_data(void);



				/*一些辅助函数*/
		//根据类型获取json类型字符串
u8 * json_get_devicname(u8 devtype);
		//检验一个json字符串是否完整,0,不完整，1，完整
u8 json_cheek(char *buff);


#endif

















