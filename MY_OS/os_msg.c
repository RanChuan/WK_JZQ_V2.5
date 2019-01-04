
#include "os_msg.h"
#include "includes.h"



INT8U os_msg_init (void)
{
	msgInit();
	return 0;
}




messeg my_messeg[MESSEG_NUM];



/**********************************************

		任务结构体中，MYWork不为0表示收到了消息
		最高字节为0x00,收到的是系统消息
							0x01,输入消息


**********************************************/

			//找该类型的消息是否存在，1，有，0，无
u8 find_messeg(u8 type)
{
	u16 i=0;
	u16 t=0;
	for (i=0;i<MESSEG_NUM;i++)
	{
		if (my_messeg[i].msgto==type) return 1;
	}
	return 0;//没找到该类型的
	
}

//发送消息，0，成功，1，失败
u8 send_messeg (u8 type,u8 *data)
{
	u16 i=0;
	u16 t=0;
	for (i=0;i<MESSEG_NUM;i++)//查找有没有空位
	{
		if (my_messeg[i].msgto==TASK_MAX_NUM) break;
	}
	if (i==MESSEG_NUM) return 1;//缓冲区已满
	my_messeg[i].msgto=type;
	
	my_messeg[i].msgfrom=TASK_MAX_NUM;//加入这两项可以使用新的消息接收函数接收
	my_messeg[i].msgtype=MSG_TYPE_U8;
	
	for (t=0;t<MESSEG_DATA;t++)
	{
		my_messeg[i].data.u8dat[t]=data[t];
	}
	TaskSendMsg(type,2);
	return 0;
}


//查找消息，0，成功，1，失败
u8 get_messeg (u8 type,u8 *data)
{
	u16 i=0;
	u16 t=0;
	for (i=0;i<MESSEG_DATA;i++)
	{
		data[i]=0;
	}
	
		for (i=0;i<MESSEG_NUM;i++)
		{
			if (my_messeg[i].msgto==type) break;
		}
		if (i==MESSEG_NUM) return 1;//没找到该类型的
	for (t=0;t<MESSEG_DATA;t++)
	{
		data[t]=my_messeg[i].data.u8dat[t];
		my_messeg[i].data.u8dat[t]=0;
	}
	my_messeg[i].msgto=TASK_MAX_NUM;	//读取过后清空缓冲区
	return 0;
}



//等待消息，0，成功，1，失败
u8 wait_messeg (u8 type,u8 *data)
{
	u16 i=0;
	u16 t=0;
	for (i=0;i<MESSEG_DATA;i++)
	{
		data[i]=0;
	}
	
	while(1)
	{
		for (i=0;i<MESSEG_NUM;i++)
		{
			if (my_messeg[i].msgto==type) break;
		}
		if (i==MESSEG_NUM) //return 1;//没找到该类型的
		{
			TaskGetMsg();
		}
		else
		{
			break;
		}
	}
	for (t=0;t<MESSEG_DATA;t++)
	{
		data[t]=my_messeg[i].data.u8dat[t];
		my_messeg[i].data.u8dat[t]=0;
	}
	my_messeg[i].msgto=TASK_MAX_NUM;	//读取过后清空缓冲区
	return 0;
}









u8 INPUT_FOCUS=TASK_MAX_NUM;//输入焦点设置为一个不可能的值
u8 SetFocus(u8 pro)
{
	if (pro<TASK_MAX_NUM)
	{
		INPUT_FOCUS=pro;
		return 0;
	}
	else
	{
		return 1;
	}
}

u8 GetFocus (void)
{
	return INPUT_FOCUS;
}


///////////////添加一些新的消息处理函数2018.12.29//////////////////////

/*************************************************************
	函数名：发送消息函数
	参数：to_task_pro 消息目的任务优先级
	参数：msg 已填充好的消息内容
	参数：msgtype 消息内容的类型 只能有以下取值MSG_TYPE_U8，MSG_TYPE_U8PTR，MSG_TYPE_U16PTR，MSG_TYPE_U32PTR，MSG_TYPE_VOIDPTR
	参数：msgflag 消息标志 可以为以下参数的组合MSG_FLAG_SEND，MSG_FLAG_RECV，MSG_FLAG_ENRE，MSG_FLAG_CEECK
	参数：reterr 自定义错误类型标志，在msgflag为MSG_FLAG_RECV时需要用到
	参数：errstr 自定义的错误类型
	返回值：消息错误类型结构体
**************************************************************/
msgerr Msg_Send (u8 to_task_pro,msgdata *msg,u8 msgtype, u8 msgflag,u16 reterr,char *errstr)
{
	u16 i=0;
	u16 t=0;
	for (i=0;i<MESSEG_NUM;i++)//查找有没有空位
	{
		if (my_messeg[i].msgto==TASK_MAX_NUM) break;
	}
	if (i==MESSEG_NUM) 
	{
		msgerr err;
		err.errType=msgQueueIsFull;//缓冲区已满
		err.errStr=msgErrToStr(err.errType);
		err.msgfrom=OSPrioHighRdy;//消息源头
		err.msgto=to_task_pro;//消息送达地
		err.extErrType=my_messeg[MESSEG_NUM-1].msgto;//一般消息阻塞是因为某个任务没有接收，所以获取最后一个消息的目的地
		return err;
	}


	if (msgflag&MSG_FLAG_CEECK)			//要进行消息阻塞检查
	{
		for (t=0;t<MESSEG_NUM;t++)
		{
			if (my_messeg[t].msgto==to_task_pro) break;
		}
		if (t!=MESSEG_NUM) //有同类消息，返回错误
		{
			msgerr err;
			err.errType=msgSameType;
			err.errStr=msgErrToStr(err.errType);
			err.msgfrom=OSPrioHighRdy;//消息源头
			err.msgto=to_task_pro;//消息送达地
			err.extErrType=t;//同类消息的位置
			return err;
		}
	}


	my_messeg[i].msgto=to_task_pro;
	my_messeg[i].msgfrom=OSPrioHighRdy;
	my_messeg[i].msgtype=msgtype;
	my_messeg[i].msgflag=msgflag;
	my_messeg[i].exterr=reterr;
	my_messeg[i].exterrstr=errstr;
	//复制消息内容
	msgCopyMsg(msg,&my_messeg[i].data,msgtype);
	
	
	
	TaskSendMsg(to_task_pro,TASK_MSG);
				//判断消息标志
	
	msgerr err;
	err.errType=msgNoneErr;
	err.errStr=msgErrToStr(err.errType);
	err.msgfrom=my_messeg[i].msgfrom;
	err.msgto=my_messeg[i].msgto;
	return err;
	
}


/****************************************************************

			函数名：等待消息,没有消息就在这里无限等待
			参数：msg_type 消息类型，可以为以下值的任意组合，SYS_MSG，TASK_MSG 
			参数：msg 消息内容，用来存储接收的消息
			参数：datatype 消息内容类型，用来获得接收的消息的类型
			参数：msgflag  消息标志位
****************************************************************/
msgerr Msg_Wait (u32 msg_type,msgdata *msg,u8 *datatype,u8 *msgflag)
{
	u16 i=0;
	u16 t=0;
	msgerr err={0};
	for (;;)
	{
		u32 task_msg=TaskGetMsg();
		if (msg_type&task_msg)
		{
			switch (task_msg)
			{
				case SYS_MSG:
					break;
				case TASK_MSG:
					for (i=0;i<MESSEG_NUM;i++)//查找有没有空位
					{
						if (my_messeg[i].msgto==OSPrioHighRdy) break;
					}
					*msgflag=my_messeg[i].msgflag;
					*datatype=my_messeg[i].msgtype;
					msgCopyMsg(&my_messeg[i].data,msg,*datatype);
					my_messeg[i].msgto=TASK_MAX_NUM;//去除消息目的地，清除消息
					break;
				default :
					break;
			}
			if (my_messeg[i].exterr)//有执行错误
			{
				err.errType=msgMoreErr;
				err.errStr=msgErrToStr(err.errType);
				err.extErrType=my_messeg[i].exterr;
				err.extErrStr=my_messeg[i].exterrstr;
				err.msgfrom=my_messeg[i].msgfrom;
				err.msgto=my_messeg[i].msgto;
			}
			else
			{
				err.errType=msgNoneErr;
				err.errStr=msgErrToStr(err.errType);
				err.msgfrom=my_messeg[i].msgfrom;
				err.msgto=my_messeg[i].msgto;
			}
			return err;
		}
	}
}




/****************************************************************

			函数名：查询消息,有消息就复制，没有消息就跳过
			参数：msg_type 消息类型，可以为以下值的任意组合，SYS_MSG，TASK_MSG 
			参数：msg 消息内容，用来存储接收的消息
			参数：datatype 消息内容类型，用来获得接收的消息的类型
			参数：msgflag  消息标志位
****************************************************************/

msgerr Msg_Get (u32 msg_type,msgdata *msg,u8 *datatype,u8 *msgflag)
{
	u16 i=0;
	u16 t=0;
	msgerr err={0};
	for (i=0;i<MESSEG_NUM;i++)//查找有没有空位
	{
		if (my_messeg[i].msgto==OSPrioHighRdy) break;
	}
	if (i<MESSEG_NUM)
	{
		*msgflag=my_messeg[i].msgflag;
		*datatype=my_messeg[i].msgtype;
		msgCopyMsg(&my_messeg[i].data,msg,*datatype);
		my_messeg[i].msgto=TASK_MAX_NUM;//去除消息目的地，清除消息

		if (my_messeg[i].exterr)//有执行错误
		{
			err.errType=msgMoreErr;
			err.errStr=msgErrToStr(err.errType);
			err.extErrType=my_messeg[i].exterr;
			err.extErrStr=my_messeg[i].exterrstr;
			err.msgfrom=my_messeg[i].msgfrom;
			err.msgto=my_messeg[i].msgto;
		}
		else
		{
			err.errType=msgNoneErr;
			err.errStr=msgErrToStr(err.errType);
			err.msgfrom=my_messeg[i].msgfrom;
			err.msgto=my_messeg[i].msgto;
		}
		return err;
	}else
	{
		err.errType=msgNoneMsg;
		err.errStr=msgErrToStr(err.errType);
		return err;
	}
}




			//初始化消息结构体
void msgInit(void)
{
	u16 i=0;
	for (i=0;i<MESSEG_NUM;i++)
	{
		my_messeg[i].msgto=TASK_MAX_NUM;
	}
}





		//复制消息结构体
void msgCopyMsg(msgdata *from,msgdata *to,u8 msgtype)
{
	u16 t=0;
				//根据消息类型复制消息
	switch (msgtype)
	{
		case MSG_TYPE_U8:
			for (t=0;t<MESSEG_DATA;t++)
			{
				to->u8dat[t]=from->u8dat[t];
			}
		break;
		case MSG_TYPE_U8PTR:
			to->u8ptr=from->u8ptr;
		break;
		case MSG_TYPE_U16PTR:
			to->u16ptr=from->u16ptr;
		break;
		case MSG_TYPE_U32PTR:
			to->u32ptr=from->u32ptr;
		break;
		case MSG_TYPE_VOIDPTR:
			to->voidptr=from->voidptr;
		default:
			break;
	}

}





//获取错误类型的字符串形式
char *msgErrToStr (u16 errType)
{
	switch (errType)
	{
		case msgNoneErr:
			return "成功";
		break;
		case msgUnknownErr:
			return "未知的错误";
		break;
		case msgQueueIsFull:
			return "队列已满造成的错误";
		break;
		case msgSameType:
			return "已有同类消息在队列中";
		break;
		case msgAddrErr:
			return "目标地址错误";
		break;
		case msgMoreErr:
			return "因其他执行结果造成的错误";
		break;
		default:
			return "其他错误";
		break;
	}
}



