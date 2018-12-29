#ifndef __OS_MSG_H
#define __OS_MSG_H

#include "includes.h"



		//命令缓冲区长度
#define MESSEG_NUM 30
		//数据位长度
#define MESSEG_DATA 9


					//系统消息类型
#define  MSG_TYPE 		0xff000000
#define  SYS_MSG	 		0x01000000
#define  TASK_MSG 		0x02000000
#define  GET_MSGTYPE(msg,type) (((msg)&(MSG_TYPE))==(type))//消息类型判定宏



//错误类型结构体
typedef struct
{
	u8 msgfrom;
	u8 msgto;
	u16 errType;//返回错误类型
	u16 extErrType;//扩展错误类型
	char *errStr;//返回错误类型字符串
	char *extErrStr;//扩展错误类型字符串
} msgerr;


enum 
{
	msgNoneErr=0,				//没有错误
	msgUnknownErr=1,				//未知的错误
	msgQueueIsFull=2,				//队列已满
	msgAddrErr=3,				//目标地址错误
	msgMoreErr=4,				//因其他执行结果造成的错误
	msgSameType=5,			//已有同类消息在队列中
};





			//消息内容联合体
typedef union 
{
	u8 u8dat[MESSEG_DATA];
	u8 *u8ptr;
	u16 *u16ptr;
	u32 *u32ptr;
	void *voidptr;
} msgdata;



#define MSG_FLAG_SEND 0x08			//发送的消息
#define MSG_FLAG_RECV 0x80			//根据需要发送的执行结果返回消息
#define MSG_FLAG_ENRE	0x01			//此消息需要返回执行结果
#define MSG_FLAG_CEECK 0x02			//发送时检查队列中是否存在发送到该地址的消息，如果有返回错误

#define MSG_TYPE_U8  			0x01
#define MSG_TYPE_U8PTR 		0x02
#define MSG_TYPE_U16PTR 	0x03
#define MSG_TYPE_U32PTR 	0x04
#define MSG_TYPE_VOIDPTR 	0x05

typedef struct
{
	u8 msgto;			//发送给谁的，为0，没内容
	u8 msgfrom;		//消息源头
	u8 msgflag;		//消息标志，是否等待返回等
	u8 msgtype;		//消息类型，
	msgdata data;				//消息的内容，消息内容根据具体的使用者确定
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


///////////////新增的消息函数2018.12.29////////////////////////

//获取错误类型的字符串形式
char *msgErrToStr (u16 errType);

		//复制消息结构体
void msgCopyMsg(msgdata *from,msgdata *to,u8 msgtype);

msgerr Msg_Wait (u32 msg_type,msgdata *msg,u8 *datatype,u8 *msgflag);

msgerr Msg_Send (u8 to_task_pro,msgdata *msg,u8 msgtype, u8 msgflag);//,u16 reterr,char *errstr)

msgerr Msg_Get (u32 msg_type,msgdata *msg,u8 *datatype,u8 *msgflag);

#endif

