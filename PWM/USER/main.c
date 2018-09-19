#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "usart.h"
//ALIENTEK Mini STM32开发板范例代码8
//PWM输出实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司

unsigned int StrToInt(u8 *s)  // 
{
	unsigned int number=0;
	
	while(*s!='#')
	{
	number=number*10+*s-'0';
	s++;
	}
	return number;
}

 int main(void)
 {	
	uart_init(9600);
	TIM3_PWM_Init(1999,90);//不分频。PWM频率=72000/(1999+1) 
	
	//TIM_SetCompare2(TIM3,300);	

	while(1)
	{
				if(USART_RX_STA)
				{
					printf("USART_RX_BUF is %s \n",USART_RX_BUF);
					recvbuf = StrToInt(USART_RX_BUF);
					printf("recvfbuf is %d \n",recvbuf);
					USART_RX_STA = 0;
					if(recvbuf >1999)
								recvbuf = 1999;
					TIM_SetCompare2(TIM3,recvbuf);
		
					memset(USART_RX_BUF,0,200);
				}

	}
	 while(1);
}



