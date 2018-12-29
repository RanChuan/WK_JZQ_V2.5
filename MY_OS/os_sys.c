
#include "os_sys.h"
#include "includes.h"


/***********************************

		函数作用：系统初始化
							初始化任务分配
							初始化动态内存分配
							初始化文件系统等

**************************************/

			
INT8U os_init (void)
{
	os_mem_init();
	os_task_init();
	os_msg_init();
	return os_fats_init();

}






