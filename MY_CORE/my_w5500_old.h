
#ifndef __MY_W5500_OLD_H
#define __MY_W5500_OLD_H

#include "sys.h"
#include "w5500.h"
#include "cjson.h"
void my_w5500 (void * t);


				//������ת����json��ʽ�����͸�����
u8	send_json (u8 *msg);

				//�����������ݲ�����
void recv_json (void);
					//�ַ����ư�ָ��
void recv_json_kz (cJSON *root);


				//���Ͳɼ�������
u8 send_json_cj (u8 *msg);
u8 send_json_kz (u8 *msg);


#endif














