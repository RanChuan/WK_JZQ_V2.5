

#ifndef __MY_RF_NEW_H
#define __MY_RF_NEW_H

#include "includes.h"

void my_rf_new_loop (void *t);

				//��������1������
void deal_cmd_0x01 (u16 ret,u16 devid,u8 *rf_data);

				//�����������Ϣ
void rf_deal_msg(msgdata *,msgerr *,u8 ,u8);


		//����豸��������
u16 rf_add_device(void);


u16 rf_ctrl_device(msgdata *msg);


////////////////////������һЩ��������//////////////////////////////


			//ͨ��ָ�뷢���ַ�����Ϣ��ע����Ŀ������ʹ��֮ǰ��Ҫ�����ⲿ�ֱ���
void msg_to_str (u8 type,char *str);



#endif






