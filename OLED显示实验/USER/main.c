#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
//ALIENTEK战舰STM32开发板实验12
//OLED显示实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
	
 int main(void)
 {	
//	 u8 t;
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	LED_Init();			     //LED端口初始化
//	OLED_Init();			//初始化OLED      
// 	OLED_ShowString(0,0, "0.96' OLED TEST");  
// 	OLED_ShowString(0,16,"ATOM@ALIENTEK");  
// 	OLED_ShowString(0,32,"2010/06/3");  
// 	OLED_ShowString(0,48,"ASCII:");  
// 	OLED_ShowString(63,48,"CODE:");  
//	OLED_Refresh_Gram();	 
//	t=' ';  
//	while(1) 
//	{		
//		OLED_ShowChar(48,48,t,16,1);//显示ASCII字符	   
//		OLED_Refresh_Gram();
//		t++;
//		if(t>'~')t=' ';
//		OLED_ShowNum(103,48,t,3,16);//显示ASCII字符的码值 
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

