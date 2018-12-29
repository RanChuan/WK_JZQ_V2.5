

#ifndef __MY_RF_NEW_H
#define __MY_RF_NEW_H

#include "includes.h"



				//处理命令1的数据
void deal_cmd_0x01 (u16 ret,u16 devid,u8 *rf_data);

void rf_deal_msg(u8 *msg);








////////////////////以下是一些辅助函数//////////////////////////////


			//通过指针发送字符串消息，注意在目标任务使用之前不要销毁这部分变量
void msg_to_str (u8 type,char *str);



#endif






