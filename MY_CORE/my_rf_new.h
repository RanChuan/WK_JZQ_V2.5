

#ifndef __MY_RF_NEW_H
#define __MY_RF_NEW_H

#include "includes.h"



				//��������1������
void deal_cmd_0x01 (u16 ret,u16 devid,u8 *rf_data);

void rf_deal_msg(u8 *msg);








////////////////////������һЩ��������//////////////////////////////


			//ͨ��ָ�뷢���ַ�����Ϣ��ע����Ŀ������ʹ��֮ǰ��Ҫ�����ⲿ�ֱ���
void msg_to_str (u8 type,char *str);



#endif






