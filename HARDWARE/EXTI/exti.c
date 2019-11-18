#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "beep.h"
#include "ov7670.h"

//V1.1 20150118
//1,����EXTI8_Init������
//2,����EXTI9_5_IRQHandler����											  
//////////////////////////////////////////////////////////////////////////////////  

////�ⲿ�ж�0�������
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);	//����
//	if(KEY3==1)	//WK_UP����
//	{				 
//		BEEP=!BEEP;	
//	}		 
//  EXTI_ClearITPendingBit(EXTI_Line0);  //���EXTI0��·����λ  
//}
 
////�ⲿ�ж�3�������
//void EXTI3_IRQHandler(void)
//{
//	delay_ms(10);	//����
//	if(KEY1==0)	 	//����KEY1
//	{				 
//		LED1=!LED1;
//	}		 
//	EXTI_ClearITPendingBit(EXTI_Line3);  //���EXTI0��·����λ
//}

////�ⲿ�ж�4�������
//void EXTI4_IRQHandler(void)
//{
//	delay_ms(10);	//����
//	if(KEY0==0)	 	//����KEY0
//	{
//		LED0=!LED0;
//	}		 
//	EXTI_ClearITPendingBit(EXTI_Line4);  //���EXTI0��·����λ
//}

//�ⲿ�жϳ�ʼ������
//��ʼ��PA0/PE2/PE3/PE4Ϊ�ж�����.
//void EXTIX_Init(void)
//{
//	EXTI_InitTypeDef EXTI_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

//  KEY_Init();	 //	�����˿ڳ�ʼ��

// 

// //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش���
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);

//  EXTI_InitStructure.EXTI_Line=EXTI_Line3;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

// //GPIOE.4	  �ж����Լ��жϳ�ʼ������  �½��ش���
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);

//  EXTI_InitStructure.EXTI_Line=EXTI_Line4;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


//  //GPIOA.0	  �ж����Լ��жϳ�ʼ������ �����ش���
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

// 	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


//  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//  NVIC_Init(&NVIC_InitStructure); 
// 
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//  NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//  NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
//}

u8 ov_sta;	//֡�жϱ��
 //�ⲿ�ж�5~9�������
void EXTI9_5_IRQHandler(void)
{		 		
	if(EXTI_GetITStatus(EXTI_Line8)==SET)	//��8�ߵ��ж�
	{      
		OV7670_WRST=0;	//��λдָ��		  		 
		OV7670_WRST=1;	
		OV7670_WREN=1;	//����д��FIFO 	 
		ov_sta++;		//֡�жϼ�1 
	}
	EXTI_ClearITPendingBit(EXTI_Line8);  //���EXTI8��·����λ						  
} 
//�ⲿ�ж�8��ʼ��
void EXTI8_Init(void)
{												  
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);//PA8���ж���8
 	   	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�0 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�0 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���   
}

/*����Ƕ�������жϿ�����NVIC*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //����Ϊ�ڶ����ж����÷�ʽ����1bits������ռʽ��3bits������Ӧʽ�����������misc.c�ļ��У������misc.h�ļ���
  
  /* ����P[A|B|C|D|E]4Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;      //�ⲿ�ж�0��ʾ����ΪEXTI0_IRQn���ж�0~4��ʾ�������ơ��ж�5~9��ʾ������ͳһΪEXTI9_5_IRQn���ж�10~15��ʾ������ͳһΪEXTI15_10_IRQn
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռʽ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��Ӧʽ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ��
  NVIC_Init(&NVIC_InitStructure);
}

/*���� PE4 Ϊ���жϿڣ��������ж����ȼ�*/
void EXTI_PE4_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;     //����һ��GPIO_InitTypeDef���͵Ľṹ��
	EXTI_InitTypeDef EXTI_InitStructure;	

	/* config the extiline(PE4) clock and AFIO clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE);  //����GPIOE��AFIO������ʱ��
	
	/* config the NVIC(PE4) */
	NVIC_Configuration();   //�жϵ�����
	
	/* EXTI line gpio config(PE4) */	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;    //ѡ��Ҫ���Ƶ�����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;  //��������ģʽΪ��������
	GPIO_Init(GPIOE,&GPIO_InitStructure);    //���ÿ⺯������ʼ��

	/*�����ⲿ�ж��ߵ�ģʽ*/	
	/* EXTI line(PE4) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);  
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;                       //�ⲿ�ж�4
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	             //����Ϊ�ж�ģʽ 
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;   //�����غ��½����ж�
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;                        //ʹ��   
	EXTI_Init(&EXTI_InitStructure);                                //��ʼ��
}

