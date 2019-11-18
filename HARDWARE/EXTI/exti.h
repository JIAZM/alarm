#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"

//V1.1 20150118
//1,增加EXTI8_Init函数。
//2,增加EXTI9_5_IRQHandler函数											  
//////////////////////////////////////////////////////////////////////////////////  

void EXTIX_Init(void);	//外部中断初始化	
void EXTI8_Init(void);	
void NVIC_SPI_Configuration(void);

#endif

























