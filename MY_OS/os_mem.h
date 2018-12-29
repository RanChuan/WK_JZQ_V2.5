
#include "sys.h"
#include "malloc.h"



					//内存管理初始化
void os_mem_init (void);


void myfree(void *ptr);  			//内存释放
void *mymalloc(u32 size);			//内存分配
void *myrealloc(void *ptr,u32 size);//重新分配内存


