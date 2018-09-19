#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
#include "usart.h"

//ALIENTEKս��STM32������ʵ��12
//OLED��ʾʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
u16 Tem =0, Hum =0, Tem_OK =0, Hum_OK =0, eCO2=0, TVOC=0, CH2O=0;
void co2_handel(void);
	
 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	//LED_Init();			     //LED�˿ڳ�ʼ��
	OLED_Init();			//��ʼ��OLED 
  uart_init(9600);	 
	uart3_init(9600);   
	 
  OLED_ShowString(0,0, "Tem:");  
	OLED_ShowString(0,16,"Hum:");  
	OLED_ShowString(0,32,"eCO2:");  
 	OLED_ShowString(77,32,"TVOC:");  
 	OLED_ShowString(0,48,"CH2O:"); 
// OLED_ShowNum(0,0,5,1,16);//��ʾASCII�ַ�	 
 OLED_Refresh_Gram();	   
	 printf("is ok\n");
	while(1) 
	{
		co2_handel();
	}
}
 
int numLen(int num)
{
	int sum=0;
	while(num)
	{
		sum++;
		num/=10;
	}
	return sum;
}


void co2_handel(void)
{
	if( (USART3_RX_STA==17) && (USART3_RX_BUF[0]==0x42) && (USART3_RX_BUF[1]==0x4D))
	{
		printf("USART3_RX_STA= %d \r\n",USART3_RX_STA);
		printf("buf[1]= %d \r\n",USART3_RX_BUF[1]);
		Tem = (USART3_RX_BUF[5] << 8) | USART3_RX_BUF[6];
		Hum = (USART3_RX_BUF[7] << 8) | USART3_RX_BUF[8];
		eCO2 = (USART3_RX_BUF[9] << 8) | USART3_RX_BUF[10];
		TVOC = (USART3_RX_BUF[11] << 8) | USART3_RX_BUF[12];
		CH2O = (USART3_RX_BUF[13] << 8) | USART3_RX_BUF[14];
		Tem_OK = (-46.85 +175.72*Tem/65536);
		Hum_OK = (-6 + 125*(float)Hum/65536);
		OLED_ShowNum(32,0, Tem_OK,numLen(Tem_OK),16);//��ʾASCII�ַ�	
		OLED_ShowNum(32,16,Hum_OK,numLen(Hum_OK),16);//��ʾASCII�ַ�
		OLED_ShowNum(40,32,eCO2,numLen(eCO2),16);//��ʾASCII�ַ�
		OLED_ShowNum(117,32,TVOC,numLen(TVOC),16);//��ʾASCII�ַ�
		OLED_ShowNum(40,48,CH2O,numLen(CH2O),16);//��ʾASCII�ַ�
		OLED_Refresh_Gram();	
		
		printf("Tem = %f \r\n",(-46.85 +175.72*Tem/65536));
			printf("Hum = %f \r\n",(-6 + 125*(float)Hum/65536));
			printf("eCO2 = %d \r\n",eCO2);
			printf("TVOC = %d \r\n",TVOC);
			printf("CH2O = %d \r\n",CH2O);
		USART3_RX_STA=0;

	}
}

