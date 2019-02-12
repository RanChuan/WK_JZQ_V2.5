
#include "includes.h"
#include "my_idle.h"


u32 IDLE=0;
u32 LASTTIME[32]={0};
u16 IDLETIMES[32]={0};

void idle_task (void *t)
{
	u8 i=0;
	#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
	while(1)
	{
		for (i=0;i<TASK_MAX_NUM;i++)
		{
			OS_ENTER_CRITICAL();
			if (getSysRunTime()- TCB_Table[i].LastTime>60*2)
			{
				if (TCB_Table[i].pTask)
				{
					IDLE|=0x80000000>>i;
					LASTTIME[i]=TCB_Table[i].LastTime;
					IDLETIMES[i]++;
					TaskRepend(i);
				}
			}
			OS_EXIT_CRITICAL();
		}
	}
}

 
u32 getIdleTask(void)
{
	return IDLE;
}


void getKilledTask(u32 *lasttime,u32 *dietimes,u8 pro)
{
	*lasttime=LASTTIME[pro];
	*dietimes=IDLETIMES[pro];
}




