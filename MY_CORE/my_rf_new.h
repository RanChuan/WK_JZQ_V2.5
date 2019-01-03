

#ifndef __MY_RF_NEW_H
#define __MY_RF_NEW_H

#include "includes.h"

void my_rf_new_loop (void *t);

				//处理命令1的数据
void deal_cmd_0x01 (u16 ret,u16 devid,u8 *rf_data);

				//处理任务间消息
void rf_deal_msg(msgdata *,msgerr *,u8 ,u8);


		//添加设备到集中器
u16 rf_add_device(void);





////////////////////以下是一些辅助函数//////////////////////////////


			//通过指针发送字符串消息，注意在目标任务使用之前不要销毁这部分变量
void msg_to_str (u8 type,char *str);



#endif






