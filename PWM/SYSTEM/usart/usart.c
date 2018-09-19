#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
	  
 



/********************************/
/*					变量声明						*/
/********************************/
char recvbuf_temp[4] = {0};
unsigned int recvbuf = 0;
char recvflag = 0;
unsigned int recvend = 0;

char recvbuf_temp2[4] = {0};
char recvbuf2 = 0;
char recvflag2 = 0;
char recvend2 = 0;


//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

void Usart1Send(u8 *str, u32 len)
{      
	int i;
	for(i = 0; i < len; i++)
	{
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕 
		USART1->DR = str[i];
	} 
  
}
 
void Usart2Send(u8 *str, u32 len)
{      
	int i;
	for(i = 0; i < len; i++)
	{
		USART_SendData(USART2, (unsigned char) str[i]);
		while (!(USART2->SR & USART_FLAG_TXE));
//		while((USART2->SR&0X40)==0);//循环发送,直到发送完毕 
//		USART2->DR = (u8)str[i];
	}   
}

void USART2_Init(u32 My_BaudRate)  
{  
    GPIO_InitTypeDef GPIO_InitStrue;  
    USART_InitTypeDef USART_InitStrue;  
    NVIC_InitTypeDef NVIC_InitStrue;  
      
    // 外设使能时钟  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  
    USART_DeInit(USART2);  //复位串口2 -> 可以没有  
      
    // 初始化 串口对应IO口  TX-PA2  RX-PA3  
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA,&GPIO_InitStrue);  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;  
  GPIO_Init(GPIOA,&GPIO_InitStrue);  
      
    // 初始化 串口模式状态  
    USART_InitStrue.USART_BaudRate=My_BaudRate; // 波特率  
    USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // 硬件流控制  
    USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // 发送 接收 模式都使用  
    USART_InitStrue.USART_Parity=USART_Parity_No; // 没有奇偶校验  
    USART_InitStrue.USART_StopBits=USART_StopBits_1; // 一位停止位  
    USART_InitStrue.USART_WordLength=USART_WordLength_8b; // 每次发送数据宽度为8位  
    USART_Init(USART2,&USART_InitStrue);  
      
    USART_Cmd(USART2,ENABLE);//使能串口  
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启接收中断  
      
    // 初始化 中断优先级  
    NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;  
    NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;  
    NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;  
    NVIC_Init(&NVIC_InitStrue);  
}  
  
//void USART2_IRQHandler(void) // 串口2中断服务函数  
//{  
//    u8 res;  
//    if(USART_GetITStatus(USART2,USART_IT_RXNE)) // 中断标志  
// {  
//		res = USART_ReceiveData(USART2);	//读取接收到的数据
//		recvbuf_temp2[recvflag2++] = res;				
//		switch(recvflag2)												
//		{
//				case 1: if(recvbuf_temp2[0] != 0xAA )
//									{
//										recvflag2 = 0;						
//										return ;
//									}break;
//				case 3: if(recvbuf_temp2[2] != 0x0D )
//									{
//										recvflag2 = 0;						
//										return ;
//									}break;
//				case 4: if(recvbuf_temp2[3] != 0x0A )	
//									{
//										recvflag2 = 0;						
//										return ;
//									}
//									else											// 接收完成
//										{
//											recvend2 = 1;					
//											recvbuf2 = recvbuf_temp2[1];		
//											recvflag2 = 0;					
//											return ;
//										}
//									break;
//		}//end switch
//  }  
//}   
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		if((USART_RX_STA&0x8000)==0)//
			{
			if(USART_RX_STA&0x4000)//
				{
				if(Res!=0x0a)USART_RX_STA=0;//
				else USART_RX_STA|=0x8000;	//
				}
			else //
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	
	

