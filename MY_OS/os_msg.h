#ifndef __OS_MSG_H
#define __OS_MSG_H

#include "includes.h"



		//命令缓冲区长度
#define MESSEG_NUM 30
		//数据位长度
#define MESSEG_DATA 9



#define  MSG_TYPE 		0xff000000
#define  SYS_MSG	 		0x00000000
#define  KEY_INPUT 		0x01000000
#define  GET_MSGTYPE(msg,type) (((msg)&(MSG_TYPE))==(type))//消息类型判定宏



typedef struct
{
	u8 type;			//发送给谁的，为0，没内容
	u8 data[MESSEG_DATA];				//消息的内容
} messeg,*pmesseg;


extern messeg my_messeg[MESSEG_NUM];



//发送消息，0，成功，1，失败
u8 send_messeg (u8 type,u8 *data);

//接收消息，0，成功，1，失败
u8 get_messeg (u8 type,u8 *data);
			//找该类型的消息是否存在，1，有，0，无
u8 find_messeg(u8 type);

//等待消息，0，成功，1，失败
u8 wait_messeg (u8 type,u8 *data);

u8 SetFocus(u8 pro);

u8 GetFocus (void);

INT8U os_msg_init (void);


#endif

