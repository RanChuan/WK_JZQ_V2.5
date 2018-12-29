
#ifndef __MY_W5500_H
#define __MY_W5500_H

#include "sys.h"
#include "w5500.h"
#include "cjson.h"

void my_w5500 (void * t);

					//处理网口中断的任务函数
void my_w5500interrupt (void * t);


				//温控客户端函数接口
void wk_client(void);






#endif














