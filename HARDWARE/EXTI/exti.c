#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "beep.h"
#include "ov7670.h"

//V1.1 20150118
//1,增加EXTI8_Init函数。
//2,增加EXTI9_5_IRQHandler函数											  
//////////////////////////////////////////////////////////////////////////////////  

////外部中断0服务程序
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);	//消抖
//	if(KEY3==1)	//WK_UP按键
//	{				 
//		BEEP=!BEEP;	
//	}		 
//  EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位  
//}
 
////外部中断3服务程序
//void EXTI3_IRQHandler(void)
//{
//	delay_ms(10);	//消抖
//	if(KEY1==0)	 	//按键KEY1
//	{				 
//		LED1=!LED1;
//	}		 
//	EXTI_ClearITPendingBit(EXTI_Line3);  //清除EXTI0线路挂起位
//}

////外部中断4服务程序
//void EXTI4_IRQHandler(void)
//{
//	delay_ms(10);	//消抖
//	if(KEY0==0)	 	//按键KEY0
//	{
//		LED0=!LED0;
//	}		 
//	EXTI_ClearITPendingBit(EXTI_Line4);  //清除EXTI0线路挂起位
//}

//外部中断初始化程序
//初始化PA0/PE2/PE3/PE4为中断输入.
//void EXTIX_Init(void)
//{
//	EXTI_InitTypeDef EXTI_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

//  KEY_Init();	 //	按键端口初始化

// 

// //GPIOE.3	  中断线以及中断初始化配置 下降沿触发
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);

//  EXTI_InitStructure.EXTI_Line=EXTI_Line3;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

// //GPIOE.4	  中断线以及中断初始化配置  下降沿触发
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);

//  EXTI_InitStructure.EXTI_Line=EXTI_Line4;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


//  //GPIOA.0	  中断线以及中断初始化配置 上升沿触发
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

// 	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


//  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
//  NVIC_Init(&NVIC_InitStructure); 
// 
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键所在的外部中断通道
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
//  NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键所在的外部中断通道
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
//  NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
//}

u8 ov_sta;	//帧中断标记
 //外部中断5~9服务程序
void EXTI9_5_IRQHandler(void)
{		 		
	if(EXTI_GetITStatus(EXTI_Line8)==SET)	//是8线的中断
	{      
		OV7670_WRST=0;	//复位写指针		  		 
		OV7670_WRST=1;	
		OV7670_WREN=1;	//允许写入FIFO 	 
		ov_sta++;		//帧中断加1 
	}
	EXTI_ClearITPendingBit(EXTI_Line8);  //清除EXTI8线路挂起位						  
} 
//外部中断8初始化
void EXTI8_Init(void)
{												  
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);//PA8对中断线8
 	   	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级0 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器   
}

/*配置嵌套向量中断控制器NVIC*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //配置为第二种中断配置方式，即1bits配置抢占式，3bits配置响应式，这个函数在misc.c文件中，组别在misc.h文件中
  
  /* 配置P[A|B|C|D|E]4为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;      //外部中断0表示方法为EXTI0_IRQn，中断0~4表示方法类似。中断5~9表示方法则统一为EXTI9_5_IRQn。中断10~15表示方法则统一为EXTI15_10_IRQn
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占式优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //响应式优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能
  NVIC_Init(&NVIC_InitStructure);
}

/*配置 PE4 为线中断口，并设置中断优先级*/
void EXTI_PE4_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;     //定义一个GPIO_InitTypeDef类型的结构体
	EXTI_InitTypeDef EXTI_InitStructure;	

	/* config the extiline(PE4) clock and AFIO clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE);  //开启GPIOE和AFIO的外设时钟
	
	/* config the NVIC(PE4) */
	NVIC_Configuration();   //中断的配置
	
	/* EXTI line gpio config(PE4) */	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;    //选择要控制的引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;  //设置引脚模式为下拉输入
	GPIO_Init(GPIOE,&GPIO_InitStructure);    //调用库函数，初始化

	/*配置外部中断线的模式*/	
	/* EXTI line(PE4) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);  
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;                       //外部中断4
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	             //配置为中断模式 
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;   //上升沿和下降沿中断
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;                        //使能   
	EXTI_Init(&EXTI_InitStructure);                                //初始化
}

