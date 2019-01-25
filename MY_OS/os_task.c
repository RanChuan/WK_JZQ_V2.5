
#include "os_task.h"
#include "includes.h"


/****************************************************

		�й�����Ĳ�������
		�����л���4�֣�
								1.�����ȼ������ͷ�CPU������������������������������
								2.�����л�������������������ѵ����ȼ��ȵ�ǰ����ߣ�������������
								3.�жϻ���������������ж������ѵ��������ȼ��ȵ�ǰ����ߣ�������������
									�жϵ��µ�������ȿ��ܷ������κεط�����CPU��ִ�в��ɴ�ϵĲ���ʱҪ��ʱ����
									��ֹ��������Ȳ��������⣬�ڲ������������Ͻ���һ�ε���
									ʹ��OS_ENTER_ONLYME()����������������ʹ��OS_EXIT_ONLYME()���˳����ҽ���һ�ε�4�ֵ���
								4.����Ļ��������ж��з����ģ������ڻ��ѵ�ʱ��CPU����ִ�в��ɴ�ϵĲ��������޷�����
									�ڲ��ɴ�ϵĲ���ִ����֮������и������ȼ��ľ������񣬻�����������

******************************************************/

INT32U  OSRunning=0    ;       //���������������1�����У�0��û����    // External references
INT32U  OSPrioCur=3;				
INT32U  OSPrioHighRdy=4;//��ߵ����ȼ�
pOS_TCB  OSTCBCur;
pOS_TCB  OSTCBHighRdy;//���ȼ���ߵ�TCBָ��
INT32U  OSIntNesting=7;   //�����жϵĲ���
INT32U  OSIntExit=8;			//�ж��˳�����

INT8U  OS_ONLYME=0;//��ֹ���ȱ�־

 
//��������ṹ��
OS_TCB TCB_Table[TASK_MAX_NUM]={0};
  
								//��������ע���,��ע����е������ǿ��Ե��ȵ�
#if TASK_MAX_NUM<=32u
INT32U   TASK_Free=0;
#else
INT32U  TASK_Free[TASK_MAX_NUM/32+1]={0};
#endif
 


/*************************************

		�������ã���ʼ��ϵͳ��ʱ����ʼ��֮��������ȲŻ�����

**************************************/

void os_task_init (void)
{
	delay_init();	    	 //��ʱ������ʼ��
}



INT8U CreateTask (void   (*task)(void *p_arg),//�����׵�ַ
                     void    *p_arg,					//�������
                     OS_STK  *ptos,						//�����ջ��ַ������������
                     INT8U    prio)						//�������ȼ�
{
#if TASK_MAX_NUM<=32u
	INT32U myprio=0x80000000>>prio;
#endif
	OS_STK *psp;
	
#if TASK_MAX_NUM<=32u
	if (TASK_Free&myprio)
	{
		return 1;//���ش��󣬸����ȼ��Ѿ�ע���
	}
	else
	{
		TASK_Free|=myprio;
	}
#endif
	
	psp=OSTaskStkInit(task,p_arg,ptos,0);//��ʼ�������ջ��������ջ��
	OS_TCBInit (task,psp,prio);
	return 0;
}


			//��ʼ�������ջ�ṹ�壬
INT8U OS_TCBInit (void (*task)(void *p_arg),OS_STK *ptos,INT8U prio)
{
	TCB_Table[prio].pMYStack=ptos;
	TCB_Table[prio].pTask=task;
	TCB_Table[prio].MYWork=0x80000000;//�������ʼ����ʱ�����һ��
	return 0;
}



void  OSStart (void)
{
    if (OSRunning == 0) {
				OSPrioHighRdy=GetZeroNum(TASK_Free);
        OSPrioCur     = OSPrioHighRdy;
        OSTCBHighRdy  = &TCB_Table[OSPrioHighRdy]; /* Point to highest priority task ready to run    */
        OSTCBCur      = OSTCBHighRdy;
        OSStartHighRdy();                            /* Execute target specific code to start task     */
    }
}





u8 Task_AddFree (u8 pro)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	if (pro>=TASK_MAX_NUM)
		return 0;
	OS_ENTER_CRITICAL(); 
	if (TCB_Table[pro].pTask)
	{
		TASK_Free|=0x80000000>>pro;//�����л�Ϊ����״̬
		TCB_Table[pro].LastTime=getSysRunTime();
	}
	OS_EXIT_CRITICAL();
	return 1;
}


u8 Task_DelFree(u8 pro)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	if (pro>=TASK_MAX_NUM)
		return 0;
	OS_ENTER_CRITICAL(); 
	if (TCB_Table[pro].pTask)
	{
		TASK_Free&=~(0x80000000>>pro);//������������־
//		TCB_Table[pro].LastTime=getSysRunTime();
	}
	OS_EXIT_CRITICAL();
	return 1;
}






					//�������
INT8U TaskPend (INT32U prio)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	if (prio>=TASK_MAX_NUM)
		return 0;
	OS_ENTER_CRITICAL(); 
	TCB_Table[prio].Pend++;
	Task_DelFree(prio);
	OS_EXIT_CRITICAL();
	return 1;
}
			//����ָ�
INT8U TaskRepend (INT32U prio)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	if (prio>=TASK_MAX_NUM)
		return 0;
	OS_ENTER_CRITICAL(); 
	if (TCB_Table[prio].Pend)
		TCB_Table[prio].Pend--;
	if (TCB_Table[prio].Pend==0)
		Task_AddFree(prio);
	OS_EXIT_CRITICAL();
	return 1;
}

			//���������л�����
void ToggleTasks(void)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	INT32U tt=TASK_MAX_NUM;
	OS_ENTER_CRITICAL(); 
	tt=GetZeroNum(TASK_Free);
	if (tt!=OSPrioHighRdy)//�����������������
	{
		if (tt<TASK_MAX_NUM)
		{
			OSPrioHighRdy=tt;
			OSTCBHighRdy=&TCB_Table[tt];
			OSCtxSw();
		}
	}
	OS_EXIT_CRITICAL();
}




//��������ͨ�����ȼ�ȷ��,��������жϵ��ÿ��Ի�������
//���ж��е��ã���Ҫ�Ӳ��ɵ��ȱ�����
void TaskIntSendMsg(u8 pro,INT32U msg)
{
	if (pro>=TASK_MAX_NUM) return;
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	INT32U tt=TASK_MAX_NUM;
	OS_ENTER_CRITICAL(); 
	if (TCB_Table[pro].pTask)
	{
		TCB_Table[pro].MYWork|=msg;
		if (!(TCB_Table[pro].Pend))
			Task_AddFree(pro);
	}
	OS_EXIT_CRITICAL();
	
	
	//������Ϣ֮������������
	if (OS_ONLYME) return;//��ʱ���ɽ��е���
	OS_ENTER_CRITICAL();
	tt=GetZeroNum(TASK_Free);
	if (tt<OSPrioHighRdy)//�����ǰ�������ȼ���ߣ�ǿ����ת
	{
		if (tt<TASK_MAX_NUM)
		{
			OSPrioHighRdy=tt;
			OSTCBHighRdy=&TCB_Table[tt];
			OSIntCtxSw();
		}
	}
	OS_EXIT_CRITICAL();
}

			//��������������Ϣ�������������������Ի�����������
			//���������߳�״̬�µ��õģ���Ҫ�Ӳ��ɵ��ȱ���
			//0,ʧ�ܣ�1���ɹ�
u8 TaskSendMsg(u8 pro,INT32U msg)
{
	if (pro>=TASK_MAX_NUM) return 0;
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	INT32U tt=TASK_MAX_NUM;
	OS_ENTER_CRITICAL(); 
	if (TCB_Table[pro].pTask)
	{
		TCB_Table[pro].MYWork|=msg;
		if (!(TCB_Table[pro].Pend))
			Task_AddFree(pro);
	}
	OS_EXIT_CRITICAL();
 
	
	//������Ϣ֮������������
	ToggleTasks();
	return 1;

}





	//�����ڲ���������Ϣ֮��������������ʼ��Ϣ
void TaskMsgZero(void)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	OS_ENTER_CRITICAL(); 
	TCB_Table[OSPrioHighRdy].MYWork=0;
	OS_EXIT_CRITICAL();
}


	//�ȴ���Ϣ����
INT32U TaskGetMsg(void)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	INT32U tt=TASK_MAX_NUM;
	INT32U mypro=TASK_MAX_NUM;
	OS_ENTER_CRITICAL(); 
	mypro=OSPrioHighRdy;
	TCB_Table[OSPrioHighRdy].MYWork=0;//�����Լ�
	Task_DelFree(OSPrioHighRdy);
	OS_EXIT_CRITICAL();
	
					//�����ȼ����������ͷ�CPU���������������ת
	ToggleTasks();
	while(!TASK_Free);
	return TCB_Table[OSPrioHighRdy].MYWork;
}

u8 ONLYME_PRO=TASK_MAX_NUM;//��¼���벻�ɵ���״̬������

void OS_Enter_Onlyme(void)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	OS_ENTER_CRITICAL(); 
	OS_ONLYME++;
	ONLYME_PRO=OSPrioHighRdy;					//�ҳ����CPU��ռ������
	OS_EXIT_CRITICAL();
	
}

void OS_Exit_Onlyme(void)
{
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
	 OS_CPU_SR  cpu_sr;
	#endif
	INT32U tt=TASK_MAX_NUM;
	OS_ENTER_CRITICAL(); 
	if (OS_ONLYME) OS_ONLYME--;
	if (!OS_ONLYME)
		ONLYME_PRO=TASK_MAX_NUM;
	OS_EXIT_CRITICAL();
	
	
	ToggleTasks();
}


