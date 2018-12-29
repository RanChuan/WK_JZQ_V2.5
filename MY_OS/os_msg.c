
#include "os_msg.h"
#include "includes.h"



INT8U os_msg_init (void)
{
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
		if (my_messeg[i].type==type) return 1;
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
		if (my_messeg[i].type==0) break;
	}
	if (i==MESSEG_NUM) return 1;//缓冲区已满
	my_messeg[i].type=type;
	for (t=0;t<MESSEG_DATA;t++)
	{
		my_messeg[i].data[t]=data[t];
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
			if (my_messeg[i].type==type) break;
		}
		if (i==MESSEG_NUM) return 1;//没找到该类型的
	for (t=0;t<MESSEG_DATA;t++)
	{
		data[t]=my_messeg[i].data[t];
		my_messeg[i].data[t]=0;
	}
	my_messeg[i].type=0;	//读取过后清空缓冲区
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
			if (my_messeg[i].type==type) break;
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
		data[t]=my_messeg[i].data[t];
		my_messeg[i].data[t]=0;
	}
	my_messeg[i].type=0;	//读取过后清空缓冲区
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



