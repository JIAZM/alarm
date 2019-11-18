#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"
#include "piclib.h"	
#include "string.h"		
#include "math.h"	 
#include "ov7670.h" 
#include "timer.h" 
#include "exti.h"

#include "output.h"
#include "rc522.h"
#define CLI() __set_PRIMASK(1)		    /* 关闭总中断 */  
#define SEI() __set_PRIMASK(0)				/* 开放总中断 */ 


extern u8 ov_sta;	//在exit.c里面定义
extern u8 ov_frame;	//在timer.c里面定义	  
extern unsigned char flag_trigger;

extern unsigned char CT[2];//卡类型
extern unsigned char SN[4]; //卡号
extern unsigned char RFID[16];			//存放RFID 
extern unsigned char lxl_bit;
extern u8 xx;
/*函数声明*/
void EXTI_PE4_Config();

void camera_new_pathname(u8 *pname);
void camera_refresh(void);

 int main(void)
 {	 
	u8 res;							 
	u8 *pname;				//带路径的文件名 
	u8 key;					//键值		   
	u8 i;						 
	u8 sd_ok=1;				//0,sd卡不正常;1,SD卡正常.
	
	unsigned char status;
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(9600);				
 	usmart_dev.init(72);		//初始化USMART		
	LCD_Init();			   		//初始化LCD    
 	my_mem_init(SRAMIN);		//初始化内部内存池
	exfuns_init();				//为fatfs相关变量申请内存 
 	OUTPUT_Init();			 //输出模块初始化
	InitRc522();				//初始化射频卡模块
	LED3=1;    //ID卡指示灯，刷卡闪灯
	LED2=1;    //报警，亮灯
	CLI();   // 关闭总中断
	SEI();   // 打开总中断 
	EXTI_PE4_Config();


 	f_mount(fs[0],"0:",1); 		//挂载SD卡 
 	f_mount(fs[1],"1:",1); 		//挂载FLASH. 
	POINT_COLOR=RED;      
 	Show_Str(30,50,200,16,"STM32F103ZET6",16,0);				    	 
	Show_Str(30,70,200,16,"Cammera",16,0);				    	 
	Show_Str(30,90,200,16,"KEY0:take photo",16,0);				    	 
	res=f_mkdir("0:/PHOTO");		//创建PHOTO文件夹
	if(res!=FR_EXIST&&res!=FR_OK) 	//发生了错误
	{		    
		Show_Str(30,150,240,16,"SD disk error!",16,0);
		delay_ms(200);				  
		Show_Str(30,170,240,16,"can not take photo!",16,0);
		sd_ok=0;  	
	}else
	{
		Show_Str(30,150,240,16,"SD disk OK!",16,0);
		delay_ms(200);				  
		Show_Str(30,170,240,16,"KEY_UP:take photo",16,0);
		sd_ok=1;  	  
	}										   						    
 	pname=mymalloc(SRAMIN,30);	//为带路径的文件名分配30个字节的内存		    
 	while(pname==NULL)			//内存分配出错
 	{	    
		Show_Str(30,190,240,16,"memory allocation error!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,190,240,146,WHITE);//清除显示	     
		delay_ms(200);				  
	}   											  
	while(OV7670_Init())//初始化OV7670
	{
		Show_Str(30,190,240,16,"OV7670 error!",16,0);
		delay_ms(200);
	    LCD_Fill(30,190,239,206,WHITE);
		delay_ms(200);
	}
 	Show_Str(30,190,200,16,"OV7670 OK",16,0);
	delay_ms(1500);	 		 
	TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断									  
	EXTI8_Init();						//使能定时器捕获
	OV7670_Window_Set(12,176,240,320);	//设置窗口	  
  	OV7670_CS=0;				    		    
	LCD_Clear(BLACK);
 	while(1)
	{	
		if(KEY1==0)   //按键S2!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		{
			SN[0]=0x00;
			SN[1]=0x00;
			SN[2]=0x00;	
			SN[3]=0x00;
			LED2=1;
			xx=0;    //标志位
		}
		status = PcdRequest(PICC_REQALL,CT);/*尋卡*/
		if(status==MI_OK)//尋卡成功
		{
			//LCD_ShowString(0,30,200,16,16,"PcdRequest_MI_OK");
			printf("PcdRequest_MI_OK   ");
			printf("ID:%02x %02x %02x %02x\n",SN[0],SN[1],SN[2],SN[3]);
			status=MI_ERR;
			status = PcdAnticoll(SN);/*防冲撞*/		
		}

		if (status==MI_OK)//防衝撞成功
		{
		/*选卡号*/
			if (((SN[0]==0x39)&&(SN[1]==0x76)&&
				(SN[2]==0x6b)&&(SN[3]==0x23))||
				((SN[0]==0x4e)&&(SN[1]==0x63)&&
				(SN[2]==0xff)&&(SN[3]==0x8a))||
				((SN[0]==0x1e)&&(SN[1]==0x67)&&
				(SN[2]==0x03)&&(SN[3]==0x8b))||
				((SN[0]==0xed)&&(SN[1]==0x7c)&&
				(SN[2]==0xb1)&&(SN[3]==0x01)))  //选卡号!!!!!!!!!!!!!!!
			{
				//LCD_ShowString(150,30,200,16,16,"PcdAnticoll_MI_OK");
				status=MI_ERR;		
				LED3=0;
				LED2=0;
				BEEP=0;
				xx=1;
				delay_ms(100);
			}
		}
		else
		{
			LED3 = 1;
		}
		if(flag_trigger)
		{
			if(sd_ok)
			{
				camera_new_pathname(pname);//得到文件名		    
				if(bmp_encode(pname,(lcddev.width-240)/2,(lcddev.height-320)/2,240,320,0))//拍照有误
				{
					Show_Str(40,130,240,12,"can not write file in!",12,0);		 
				}else 
				{
					Show_Str(40,130,240,12,"took a photo!",12,0);
					Show_Str(40,150,240,12,"save as:",12,0);
 					Show_Str(40+42,150,240,12,pname,12,0);		    
					delay_ms(100);
		 		}
			}else //提示SD卡错误
			{					    
				Show_Str(40,130,240,12,"SD disk error!",12,0);
 				Show_Str(40,150,240,12,"can not take photo!",12,0);			    
 			}
			delay_ms(500);//等待1.8秒钟
			LCD_Clear(BLACK);
		}else delay_ms(5);
 		camera_refresh();//更新显示
		i++;
		if(i==40)//DS0闪烁.
		{
			i=0;
 		}
		flag_trigger = 0;
	}	   										    
}

//更新LCD显示
void camera_refresh(void)
{
	u32 j;
 	u16 color;	 
	if(ov_sta)//有帧中断更新？
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右  
		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//将显示区域设置到屏幕中央
		else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK_H;
		for(j=0;j<76800;j++)
		{
			OV7670_RCK_L;
			color=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H; 
			color<<=8;  
			OV7670_RCK_L;
			color|=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H; 
			LCD->LCD_RAM=color;    
		}   							  
 		ov_sta=0;					//清零帧中断标记
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
	} 
}	   
//文件名自增（避免覆盖）
//组合成:形如"0:PHOTO/PIC13141.bmp"的文件名
void camera_new_pathname(u8 *pname)
{	 
	u8 res;					 
	u16 index=0;
	while(index<0XFFFF)
	{
		sprintf((char*)pname,"0:PHOTO/PIC%05d.bmp",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//尝试打开这个文件
		if(res==FR_NO_FILE)break;		//该文件名不存在=正是我们需要的.
		index++;
	}
}





