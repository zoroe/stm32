#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
//ALIENTEKս��STM32������ʵ��12
//OLED��ʾʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
	
 int main(void)
 {	
//	 u8 t;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	LED_Init();			     //LED�˿ڳ�ʼ��
//	OLED_Init();			//��ʼ��OLED      
// 	OLED_ShowString(0,0, "0.96' OLED TEST");  
// 	OLED_ShowString(0,16,"ATOM@ALIENTEK");  
// 	OLED_ShowString(0,32,"2010/06/3");  
// 	OLED_ShowString(0,48,"ASCII:");  
// 	OLED_ShowString(63,48,"CODE:");  
//	OLED_Refresh_Gram();	 
//	t=' ';  
//	while(1) 
//	{		
//		OLED_ShowChar(48,48,t,16,1);//��ʾASCII�ַ�	   
//		OLED_Refresh_Gram();
//		t++;
//		if(t>'~')t=' ';
//		OLED_ShowNum(103,48,t,3,16);//��ʾASCII�ַ�����ֵ 
//		delay_ms(300);
//		LED0=!LED0;
//	}	  
	while(1)
	{
		LED0=!LED0;
		LED1=!LED1;
		delay_ms(100);
	}
	
}

