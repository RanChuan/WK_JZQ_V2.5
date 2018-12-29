
#ifndef __OS_TASK_H
#define __OS_TASK_H
#include "includes.h"




/************************************************

				����������֧��32������
				

************************************************/


typedef struct
{
	OS_STK *pMYStack;//ջ��ָ��
	void (*pTask)(void *p_arg);//����ĵ�ַ
	INT32U MYDelay_ms;//��������ʱ��ʱ�䣬��λ��ms��
	INT32U MYWork;//�ҵĹ�������Ϊ0ʱϵͳ�����������
	INT32U Pend;	//֪�����
}OS_TCB,*pOS_TCB;

 


extern				INT32U  OSRunning    ;           // External referencesϵͳ�������б�־λ
extern        INT32U  OSPrioCur;
extern        INT32U  OSPrioHighRdy;
extern       pOS_TCB  OSTCBCur;							//����ṹ�壬��ջ��ַ����λ
extern       pOS_TCB OSTCBHighRdy;
extern        INT32U  OSIntNesting;
extern        INT32U  OSIntExit;

//extern INT32U TASK_THIS;//��¼��Ծ�Ľ���



#define TASK_MAX_NUM 32u



//��������ṹ��
extern OS_TCB TCB_Table[TASK_MAX_NUM];
  
								//��������ע���,
#if TASK_MAX_NUM<=32u
extern INT32U   TASK_Free;
#else
extern INT32U  TASK_Free[TASK_MAX_NUM/32+1];
#endif






INT8U CreateTask (void   (*task)(void *p_arg),void *p_arg,OS_STK  *ptos,INT8U prio)	;		

void  OSStart (void);
					//�������
INT8U TaskPend (INT32U prio);
INT8U TaskRepend (INT32U prio);




						//�����ջ��ʼ��
OS_STK *OSTaskStkInit (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt);




					//�����ṹ���ʼ��
INT8U OS_TCBInit (void (*task)(void *p_arg),OS_STK *ptos,INT8U prio);





				//������ȳ�ʼ��
void os_task_init (void);



//��������ͨ�����ȼ�ȷ��,��������жϵ��ÿ��Ի�������
void TaskIntSendMsg(u8 pro,INT32U msg);
			//Ч��ͬ�ϣ��������
u8 TaskSendMsg(u8 pro,INT32U msg);

void TaskMsgZero(void);

	//�ȴ���Ϣ����
INT32U TaskGetMsg(void);


void OS_Enter_Onlyme(void);

void OS_Exit_Onlyme(void);


			//һ��Ҫ�ɶ�ʹ��
#define OS_ENTER_ONLYME() OS_Enter_Onlyme( )

#define OS_EXIT_ONLYME() OS_Exit_Onlyme()

#endif


