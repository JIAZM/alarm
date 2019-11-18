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
#define CLI() __set_PRIMASK(1)		    /* �ر����ж� */  
#define SEI() __set_PRIMASK(0)				/* �������ж� */ 


extern u8 ov_sta;	//��exit.c���涨��
extern u8 ov_frame;	//��timer.c���涨��	  
extern unsigned char flag_trigger;

extern unsigned char CT[2];//������
extern unsigned char SN[4]; //����
extern unsigned char RFID[16];			//���RFID 
extern unsigned char lxl_bit;
extern u8 xx;
/*��������*/
void EXTI_PE4_Config();

void camera_new_pathname(u8 *pname);
void camera_refresh(void);

 int main(void)
 {	 
	u8 res;							 
	u8 *pname;				//��·�����ļ��� 
	u8 key;					//��ֵ		   
	u8 i;						 
	u8 sd_ok=1;				//0,sd��������;1,SD������.
	
	unsigned char status;
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);				
 	usmart_dev.init(72);		//��ʼ��USMART		
	LCD_Init();			   		//��ʼ��LCD    
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	exfuns_init();				//Ϊfatfs��ر��������ڴ� 
 	OUTPUT_Init();			 //���ģ���ʼ��
	InitRc522();				//��ʼ����Ƶ��ģ��
	LED3=1;    //ID��ָʾ�ƣ�ˢ������
	LED2=1;    //����������
	CLI();   // �ر����ж�
	SEI();   // �����ж� 
	EXTI_PE4_Config();


 	f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����FLASH. 
	POINT_COLOR=RED;      
 	Show_Str(30,50,200,16,"STM32F103ZET6",16,0);				    	 
	Show_Str(30,70,200,16,"Cammera",16,0);				    	 
	Show_Str(30,90,200,16,"KEY0:take photo",16,0);				    	 
	res=f_mkdir("0:/PHOTO");		//����PHOTO�ļ���
	if(res!=FR_EXIST&&res!=FR_OK) 	//�����˴���
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
 	pname=mymalloc(SRAMIN,30);	//Ϊ��·�����ļ�������30���ֽڵ��ڴ�		    
 	while(pname==NULL)			//�ڴ�������
 	{	    
		Show_Str(30,190,240,16,"memory allocation error!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,190,240,146,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}   											  
	while(OV7670_Init())//��ʼ��OV7670
	{
		Show_Str(30,190,240,16,"OV7670 error!",16,0);
		delay_ms(200);
	    LCD_Fill(30,190,239,206,WHITE);
		delay_ms(200);
	}
 	Show_Str(30,190,200,16,"OV7670 OK",16,0);
	delay_ms(1500);	 		 
	TIM6_Int_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�									  
	EXTI8_Init();						//ʹ�ܶ�ʱ������
	OV7670_Window_Set(12,176,240,320);	//���ô���	  
  	OV7670_CS=0;				    		    
	LCD_Clear(BLACK);
 	while(1)
	{	
		if(KEY1==0)   //����S2!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		{
			SN[0]=0x00;
			SN[1]=0x00;
			SN[2]=0x00;	
			SN[3]=0x00;
			LED2=1;
			xx=0;    //��־λ
		}
		status = PcdRequest(PICC_REQALL,CT);/*����*/
		if(status==MI_OK)//�����ɹ�
		{
			//LCD_ShowString(0,30,200,16,16,"PcdRequest_MI_OK");
			printf("PcdRequest_MI_OK   ");
			printf("ID:%02x %02x %02x %02x\n",SN[0],SN[1],SN[2],SN[3]);
			status=MI_ERR;
			status = PcdAnticoll(SN);/*����ײ*/		
		}

		if (status==MI_OK)//���nײ�ɹ�
		{
		/*ѡ����*/
			if (((SN[0]==0x39)&&(SN[1]==0x76)&&
				(SN[2]==0x6b)&&(SN[3]==0x23))||
				((SN[0]==0x4e)&&(SN[1]==0x63)&&
				(SN[2]==0xff)&&(SN[3]==0x8a))||
				((SN[0]==0x1e)&&(SN[1]==0x67)&&
				(SN[2]==0x03)&&(SN[3]==0x8b))||
				((SN[0]==0xed)&&(SN[1]==0x7c)&&
				(SN[2]==0xb1)&&(SN[3]==0x01)))  //ѡ����!!!!!!!!!!!!!!!
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
				camera_new_pathname(pname);//�õ��ļ���		    
				if(bmp_encode(pname,(lcddev.width-240)/2,(lcddev.height-320)/2,240,320,0))//��������
				{
					Show_Str(40,130,240,12,"can not write file in!",12,0);		 
				}else 
				{
					Show_Str(40,130,240,12,"took a photo!",12,0);
					Show_Str(40,150,240,12,"save as:",12,0);
 					Show_Str(40+42,150,240,12,pname,12,0);		    
					delay_ms(100);
		 		}
			}else //��ʾSD������
			{					    
				Show_Str(40,130,240,12,"SD disk error!",12,0);
 				Show_Str(40,150,240,12,"can not take photo!",12,0);			    
 			}
			delay_ms(500);//�ȴ�1.8����
			LCD_Clear(BLACK);
		}else delay_ms(5);
 		camera_refresh();//������ʾ
		i++;
		if(i==40)//DS0��˸.
		{
			i=0;
 		}
		flag_trigger = 0;
	}	   										    
}

//����LCD��ʾ
void camera_refresh(void)
{
	u32 j;
 	u16 color;	 
	if(ov_sta)//��֡�жϸ��£�
	{
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������  
		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//����ʾ�������õ���Ļ����
		else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//����ʾ�������õ���Ļ����
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK_H;
		for(j=0;j<76800;j++)
		{
			OV7670_RCK_L;
			color=GPIOC->IDR&0XFF;	//������
			OV7670_RCK_H; 
			color<<=8;  
			OV7670_RCK_L;
			color|=GPIOC->IDR&0XFF;	//������
			OV7670_RCK_H; 
			LCD->LCD_RAM=color;    
		}   							  
 		ov_sta=0;					//����֡�жϱ��
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
	} 
}	   
//�ļ������������⸲�ǣ�
//��ϳ�:����"0:PHOTO/PIC13141.bmp"���ļ���
void camera_new_pathname(u8 *pname)
{	 
	u8 res;					 
	u16 index=0;
	while(index<0XFFFF)
	{
		sprintf((char*)pname,"0:PHOTO/PIC%05d.bmp",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//���Դ�����ļ�
		if(res==FR_NO_FILE)break;		//���ļ���������=����������Ҫ��.
		index++;
	}
}





