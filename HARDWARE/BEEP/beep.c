#include "beep.h"
#include "malloc.h"




u8 BEEP_BUSY=0;//蜂鸣器正在播放，




void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能GPIOB端口时钟

	
	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //BEEP-->PB.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	 //pc13只能是2M
 GPIO_Init(GPIOC, &GPIO_InitStructure);	 //根据参数初始化GPIOB.8
	RCC_LSEConfig(RCC_LSE_OFF);
	
 GPIO_ResetBits(GPIOC,GPIO_Pin_13);//输出0，关闭蜂鸣器输出

//	TIME3_Init( );
}






//	装载一个歌曲
void Load_song(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*200);
	jianpu *jj=j;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"2","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"3","2/1");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"1","3/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"6_","5/2");
	


	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"2","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"3","2/1");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"3","2/1");
	
	
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","2/1");
	jj++;
	set_jianpu(jj,"0","1/1");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"2","3/2");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"6_","5/2");


	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"7","1/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"7","1/1");
	jj++;
	set_jianpu(jj,"1~","1/1");
	jj++;
	set_jianpu(jj,"2~","2/1");
	jj++;
	set_jianpu(jj,"7","6/1");


	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"7","3/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/3");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"3","2/1");
	
	
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"7","1/1");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"3","3/1");
	
	
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"7","3/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"3","5/2");


	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"3~","1/1");
	jj++;
	set_jianpu(jj,"2~","3/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"1~","2/1");
	jj++;
	set_jianpu(jj,"7","1/1");
	jj++;
	set_jianpu(jj,"6","6/1");

	jj++;
	jianpu_end(jj);

	
	Beep_Play(j);
	myfree(j);
}





void Load_song1(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*200);
	jianpu *jj=j;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"3","3/2");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"1","7/2");


	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"6_","1/1");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"2","3/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"6_","7/2");


	jj++;
	set_jianpu(jj,"3","2/1");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"3","2/1");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","2/1");


	jj++;
	set_jianpu(jj,"1","3/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2","4/1");


	jj++;
	set_jianpu(jj,"6_","1/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6_","1/1");
	jj++;
	set_jianpu(jj,"1","3/1");
	jj++;
	set_jianpu(jj,"3","1/1");


	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","4/1");


	jj++;
	set_jianpu(jj,"3","2/1");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","2/1");


	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"2","3/1");
	jj++;
	set_jianpu(jj,"3","1/1");


	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"5_","1/1");
	jj++;
	set_jianpu(jj,"6_","3/1");
	jj++;
	set_jianpu(jj,"3","1/1");


	jj++;
	set_jianpu(jj,"5","3/1");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/1");
	jj++;
	set_jianpu(jj,"6","4/1");


	jj++;
	set_jianpu(jj,"1~","2/1");
	jj++;
	set_jianpu(jj,"7","3/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"5","5/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3","4/1");


	jj++;
	set_jianpu(jj,"1~","2/1");
	jj++;
	set_jianpu(jj,"7","3/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"5","5/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3","4/1");


	jj++;
	set_jianpu(jj,"6_","1/1");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"2","1/1");
	jj++;
	set_jianpu(jj,"3","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"2","3/1");


	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"7_","1/2");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"5_","1/1");
	jj++;
	set_jianpu(jj,"6_","4/1");


	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"2","3/1");
	

	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"5","2/1");
	jj++;
	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"7","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/1");
	jj++;
	set_jianpu(jj,"6","7/1");
	jj++;
	jianpu_end(jj);
	
	Beep_Play(j);
	myfree(j);
}


void Load_song2(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*200);
	jianpu *jj=j;
	
	/*
	set_jianpu(jj,"6_","2/2");
	jj++;
	set_jianpu(jj,"7_","1/4");
	jj++;
	set_jianpu(jj,"1","9/4");
	jj++;
	set_jianpu(jj,"2","3/4");
	jj++;
	set_jianpu(jj,"5_","3/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"7_","1/4");
	jj++;
	set_jianpu(jj,"6_","4/2");
	jj++;

	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6_","2/2");
	jj++;
	set_jianpu(jj,"7_","1/4");
	jj++;
	set_jianpu(jj,"1","9/4");
	jj++;
	set_jianpu(jj,"2","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;

	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"4","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","2/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"5_","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","3/2");
	jj++;


	set_jianpu(jj,"0","1/4");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"2","3/4");
	jj++;
	set_jianpu(jj,"0","1/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"7_","5/4");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"6_","9/4");
	jj++;
*/

//	set_jianpu(jj,"0","5/2");
//	jj++;
	set_jianpu(jj,"6_","2/2");
	jj++;
	set_jianpu(jj,"7_","1/4");
	jj++;
	set_jianpu(jj,"1","5/4");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"7_","2/2");
	jj++;
	set_jianpu(jj,"5_","1/4");
	jj++;
	set_jianpu(jj,"7_","3/4");
	jj++;
	set_jianpu(jj,"6_","4/2");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6_","2/2");
	jj++;
	set_jianpu(jj,"7_","1/4");
	jj++;
	set_jianpu(jj,"1","5/4");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;


	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"2","2/2");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"2","3/4");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6_","1/2");
	jj++;
	set_jianpu(jj,"4","3/2");
	jj++;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","1/4");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"5_","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;
	set_jianpu(jj,"2","3/4");
	jj++;
	set_jianpu(jj,"1","5/4");
	jj++;
	set_jianpu(jj,"0","1/4");
	jj++;


	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"2","2/2");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"1","1/4");
	jj++;
	set_jianpu(jj,"6_","1/4");
	jj++;
	set_jianpu(jj,"1","2/2");
	jj++;
	set_jianpu(jj,"2","1/4");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","8/4");
	jj++;
	set_jianpu(jj,"3","5/4");
	jj++;


	set_jianpu(jj,"0","1/2");//绫罗飘起遮住日落西
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3~","2/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/4");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;


	set_jianpu(jj,"0","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/4");
	jj++;
	set_jianpu(jj,"6","5/4");
	jj++;
	
	
	set_jianpu(jj,"0","1/2");//抬起画面如此的魅力
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3~","2/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/4");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	

	set_jianpu(jj,"0","1/4");//
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"5","1/4");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","5/4");
	jj++;


	set_jianpu(jj,"0","1/2");//淡淡胭脂遮住了思绪
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3~","2/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/4");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	
	set_jianpu(jj,"0","1/4");//
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/4");
	jj++;
	set_jianpu(jj,"3","5/4");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3~","3/4");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/4");
	jj++;
	set_jianpu(jj,"6","3/4");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","3/4");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	



	jianpu_end(jj);
	
	Beep_Play(j);
	myfree(j);
}





void Load_song3(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*300);
	jianpu *jj=j;
	
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","2/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"3~","1/4");
	jj++;
	set_jianpu(jj,"5~","1/4");
	jj++;
	set_jianpu(jj,"2~","4/2");
	jj++;

	set_jianpu(jj,"0","4/2");
	jj++;
	set_jianpu(jj,"6","3/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"6","2/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;

	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"6","2/2");
	jj++;
	set_jianpu(jj,"5","2/2");
	jj++;
	set_jianpu(jj,"6","4/2");
	jj++;

	set_jianpu(jj,"0","2/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","3/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","2/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;


	set_jianpu(jj,"1~","1/2");//哼着陌生乡音走在宫闱里
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","3/4");
	jj++;
	set_jianpu(jj,"3~","4/2");
	jj++;


	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","3/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"2~","1/4");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"3","2/2");
	jj++;


	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","4/2");
	jj++;


	set_jianpu(jj,"0","2/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"6~","1/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","3/2");
	jj++;


	set_jianpu(jj,"0","1/2");//送回多少离人唏嘘
	jj++;
	set_jianpu(jj,"3~","1/4");
	jj++;
	set_jianpu(jj,"5~","1/4");
	jj++;
	set_jianpu(jj,"6~","2/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","4/2");
	jj++;


	set_jianpu(jj,"0","2/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"6~","2/2");
	jj++;
	set_jianpu(jj,"5~","1/2");
	jj++;
	set_jianpu(jj,"3~","1/2");
	jj++;
	set_jianpu(jj,"2~","2/2");
	jj++;
	set_jianpu(jj,"1~","2/2");
	jj++;
	set_jianpu(jj,"7","2/2");
	jj++;
	set_jianpu(jj,"5","2/2");
	jj++;
	set_jianpu(jj,"1~","2/2");
	jj++;


	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","2/2");
	jj++;
	set_jianpu(jj,"0","1/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"7","1/2");
	jj++;
	set_jianpu(jj,"1~","1/4");
	jj++;
	set_jianpu(jj,"7","1/4");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","6/2");
	jj++;


	jianpu_end(jj);
	Beep_Play(j);
	myfree(j);
}



//生僻字
void Load_ShenPiZi (void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*400);
	jianpu *jj=j;
	set_jianpu(jj,"1","2/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","2/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;

	set_jianpu(jj,"5","3/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"5","3/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;

	set_jianpu(jj,"1","3/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"5","3/2");
	jj++;

	set_jianpu(jj,"3","1/1");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;

	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"1~","1/1");
	jj++;
	set_jianpu(jj,"7","1/1");
	jj++;
	set_jianpu(jj,"6","1/1");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	set_jianpu(jj,"6","2/1");
	jj++;
	
	//啊！
	set_jianpu(jj,"0","3/2");
	jj++;
	set_jianpu(jj,"1~","1/2");
	jj++;
	set_jianpu(jj,"6","1/2");
	jj++;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"5","3/1");
	jj++;
	set_jianpu(jj,"5","2/1");
	jj++;


	jianpu_end(jj);
	Beep_Play(j);
	myfree(j);

}




void Load_up(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*200);
	jianpu *jj=j;
	set_jianpu(jj,"1","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"5","1/1");
	jj++;
	jianpu_end(jj);
	Beep_Play(j);
	myfree(j);
}

void Load_down(void)
{
	jianpu *j=mymalloc(sizeof(jianpu)*200);
	jianpu *jj=j;
	set_jianpu(jj,"5","1/2");
	jj++;
	set_jianpu(jj,"4","1/2");
	jj++;
	set_jianpu(jj,"3","1/2");
	jj++;
	set_jianpu(jj,"2","1/2");
	jj++;
	set_jianpu(jj,"1","1/1");
	jj++;
	jianpu_end(jj);
	Beep_Play(j);
	myfree(j);
}


//最高8分之一音，音调的频率
u16 *sond=0;

//time,8是一个全音，16是两个全音
void set_jianpu(jianpu *j,char *tone_,char *time_)
{
	u8 i=0;
	while(tone_[i])
	{
		j->tone[i]=tone_[i];
		i++;
	}
	j->tone[i]=0;//加上数据结尾
	i=0;
	while(time_[i])
	{
		j->time[i]=time_[i];
		i++;
	}
	j->time[i]=0;//加上数据结尾
}

//简谱终了
void jianpu_end(jianpu *j)
{
	j->time[0]=0;
	j->tone[0]=0;
}


void Beep_Play (jianpu *jianpu_)
{
	u16 i=0;
	u16 frequency[8]={0,2615/3,2935/3,3295/3,3490/3,3920/3,4400/3,4940/3};
//	u16 frequency[8]={0,2615*2,2935*2,3295*2,3490*2,3920*2,4400*2,4940*2};
	
	if (BEEP_BUSY==1) return;//正在播放，返回
	BEEP_BUSY=1;//设置为忙
	sond=mymalloc(300*2);
	if (sond==0) return;//没有分配到内存，退出
	while(jianpu_->tone[0])
	{
		sond[i*2]=frequency[jianpu_->tone[0]-'0'];
		if (jianpu_->tone[1]=='_')
		{
			sond[i*2]=sond[i*2]/2;
		}
		else if (jianpu_->tone[1]=='~')
		{
			sond[i*2]=sond[i*2]*2;
		}
		sond[i*2+1]=8/(jianpu_->time[2]-'0');
		sond[i*2+1]=sond[i*2+1]*(jianpu_->time[0]-'0');

		jianpu_++;
		i++;
	}
	sond[i*2+1]=0;//标记时长为0，这时播放停止
	TIM_Cmd(TIM3,ENABLE); 
}



	
void Beep_Run(void)
{
	static u16 i=0;
	static u32 time =0;//音调长度
	static u32 sondtime=0;//音调调整时长
	u32 temp=100000/sond[i*2];
	sondtime++;
	time++;
	if (temp)
	{
		if (sondtime>temp-5)//((temp)>25?(temp-10):(temp/2)))
			BEEP=1;
		else 
			BEEP=0;
	}
	else//支持间歇
	{
		BEEP=0;
	}
	if (sondtime>=100000/sond[i*2])
	{
		sondtime=0;
	}
	if (time>=sond[i*2+1]*10000)
	{
		i++;
		time=0;
		if (sond[i*2+1]==0)
		{
			i=0;
			time=0;
			sondtime=0;
			myfree(sond);
			BEEP=0;
			BEEP_BUSY=0;
			TIM_Cmd(TIM3, DISABLE);  //关闭定时器	
		}
	}
}






