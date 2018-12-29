

#ifndef __WK_JSON_H
#define __WK_JSON_H

#include "sys.h"
#include "cjson.h"
#include "includes.h"

				//������ת����json��ʽ�����͸�����
u8	send_json (u8 *msg);
u8  send_json_limit (u8 *msg);
u8  send_json_warn(u8 *msg);
				//����json���ݲ�����
void recv_json (u8 *data);

					//��λ���·�����������
void json_setting (cJSON *root);
void json_setto (cJSON *root);
void json_start (cJSON *root);
void json_alarm (cJSON *root);
void json_close (cJSON *root);
void json_mode (cJSON *root);
void json_version (cJSON *root);
void json_startup (cJSON *root);
void json_startdown (cJSON *root);


					//ִ����λ��������֮��������
void json_return (u8 deviceid,const char * cmdNum,const char *errType);

				//��������
void server_read_data(u8 *databuff);
void server_send_data(u8 *databuff);

				//���Ͳɼ�������
u8 send_json_cj (u8 *msg);
u8 send_json_kz (u8 *msg);
						//����������Ϣ
u8 send_json_adddevice (u8 *msg);


			//���ҷ�����
u8 serch_server(void);

				//�����¿ط���������Ϣ
u8 server_data(void);



				/*һЩ��������*/
		//�������ͻ�ȡjson�����ַ���
u8 * json_get_devicname(u8 devtype);
		//����һ��json�ַ����Ƿ�����,0,��������1������
u8 json_cheek(char *buff);


#endif

















