#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "MAX30100.h"
#include "MAX30100_PulseOximeter.h"
#include "MAX30100_SpO2Calculator.h"
#include "MAX30100_Filters.h"
#include "timer3.h"
#include "myiic.h"
/************************************************
  ALIENTEK精英STM32开发板  
 作者：唯恋殊雨 
 CSDN博客：https://blog.csdn.net/tichimi3375 
 SCL-PB12
 SDA-PB11
************************************************/
 int main(void)
 {		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
 	LED_Init();			     //LED端口初始化
	//KEY_Init();          //初始化与按键连接的硬件接口
	TIM3_Int_Init(100-1,720-1);//??1ms??
	IIC_Init();	
	SPO2_Init();
 	while(1)
	{
		POupdate();//更新FIFO数据 血氧数据 心率数据 
		delay_ms(10);
	}	 
 }

