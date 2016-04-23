#include "sys.h"
#include "usart.h"	  
#include "stm32f10x_dma.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif

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

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if 0
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef  NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
   // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}
#if EN_USART1_RX   //如果使能了接收
void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif	

#endif

//串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[200];     //接收缓冲,最大200个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  

//初始化IO 串口2
//bound:波特率

void USART2_IRQHandler(void)
{
	u8 res;	
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
	{	 
		res =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		USART_RX_BUF[0]=res;
		if (USART_RX_BUF[0] & 0X04)
	    {
		    USART_RX_STA = 1;
		}		 									     
	}
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	OSIntExit();  											 
#endif
} 


void uart_printer_init(u32 bound){
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_DeInit(USART2);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);                    //使能串口 

}


unsigned char Send_Buffer[SEND_BUFFER_SIZE];

void TimerDly(unsigned int Timer)
{
	unsigned int i=0;
	while(Timer)
	{
		for(i=0;i<700;i++);
		Timer--;
	}
}


unsigned char UART_TX_Buffer[128]={0x00,0xaa,0xA0,0xC0,0xD1,0xD2,0xD3,0xD4,0xCC,0xCC,0x55,0x00,0x00};
unsigned char UART_TX_Status=0;			//0 ready

unsigned char UART_RX_Buffer[UART_RX_BUFFER_SIZE];
unsigned short UART_Buffer_Top=0;
unsigned short UART_Buffer_Bom=0;
unsigned short m_UARTFIFO_PackedDatNum=0;


void uart_lcd_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure; 

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟

    DMA_DeInit(DMA1_Channel4); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR; 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)UART_TX_Buffer; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
	DMA_InitStructure.DMA_BufferSize = 128; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel4, &DMA_InitStructure); 

	DMA_DeInit(DMA1_Channel5); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR; 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)UART_RX_Buffer; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
	DMA_InitStructure.DMA_BufferSize = UART_RX_BUFFER_SIZE;; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel5, &DMA_InitStructure); 

	USART_DeInit(USART1);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    /* Enable USART1 DMA Rxrequest */ 
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	/* Enable DMA1 Channel4 */ 
	DMA_Cmd(DMA1_Channel4, ENABLE); 
	/* Enable DMA1 Channel5 */ 
	DMA_Cmd(DMA1_Channel5, ENABLE); 
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}

void UART_DMA_Tx_Enable(void)
{ 
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	//USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Channel4, DISABLE );
	//DMA_Cmd(DMA1_Channel5, DISABLE );
 	DMA_SetCurrDataCounter(DMA1_Channel4,128);
	//DMA_SetCurrDataCounter(DMA1_Channel5,UART_RX_BUFFER_SIZE);
 	DMA_Cmd(DMA1_Channel4, ENABLE);  
	//DMA_Cmd(DMA1_Channel5, ENABLE); 
}	 

void UART_DMA_Rx_Enable(void)
{
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE );
	DMA_SetCurrDataCounter(DMA1_Channel5,UART_RX_BUFFER_SIZE);
	DMA_Cmd(DMA1_Channel5, ENABLE); 
}

void UART_SendOneByte(unsigned char Dat)
{
	if(UART_TX_Status==1)
	{
		while((DMA1->ISR&0x00002000)==0);
		DMA1->IFCR |= 0x0000f000;
		DMA1_Channel4->CCR &= ~0x0001;
		UART_TX_Status = 0;
		while((USART1->SR&0x0080)==0) ;
	}
	else 
	{
		while((USART1->SR&0x0080)==0) ;
	}
	USART1->DR = Dat;
	while((USART1->SR&0x0080)==0) ;
}

void UART_SendPacked(unsigned char Command,unsigned char * p,unsigned char num)
{
	unsigned char Index=0;
	unsigned char check=0;
	if(UART_TX_Status==1)
	{
		while((DMA1->ISR&0x00002000)==0);
		DMA1->IFCR |= 0x0000f000;
		DMA1_Channel4->CCR &= ~0x0001;
	}
	UART_TX_Status = 1;
	UART_TX_Buffer[Index++] = 0x55;
	UART_TX_Buffer[Index++] = num+1;
	UART_TX_Buffer[Index++] = Command;
	check = Command;
	while(num--)
	{
		UART_TX_Buffer[Index++] = *p;
		check += *p++;
	}
	UART_TX_Buffer[Index++] = check;
	UART_TX_Buffer[Index++] = 0xaa;

	DMA1_Channel4->CNDTR = Index;
	DMA1_Channel4->CMAR = (u32)UART_TX_Buffer;
//	DMA1_Channel4->CCR |= 0x0002;

	USART1->CR3 |= 0x0080;						//使能UART1的DMA发送

	DMA1_Channel4->CCR |= 0x0001; 				//使能DMA1通道四,当数据发送完后,会进入DMA1通道四的中断,并在里面对UART_TX_Status清零
												//具体请参考stm32f10x_it.c文件中的代码
}

//用户可能需要根据所使用的MCU来修改该函数
//该函数应在用户程序中循环调用,在函数中会判断当前UART接收到模块发回的数据冲区中,是否有有效的
//数据包,如无有效数据包,则返回0,否则返回有效数据包中的指令标识
//UART_RX_Buffer[UART_RX_BUFFER_SIZE]为一个环形的数据缓冲区
//UART_Buffer_Top为环形缓冲区的当前数据区顶索引,它指明当前缓冲区中,当前开头的数据位置
//UART_Buffer_Bom为环形缓冲区的当前数据区底索引,它指明当前缓冲区中,最后一个数据的位置
//当UART_Buffer_Top==UART_Buffer_Bom时,表明,缓冲区中已无待读取的数据
//在使用中,UART每进来一个数据,则会保存在UART_Buffer_Bom所指明的位置,然后UART_Buffer_Bom会自加一,如
//超出了环形缓冲区的底部,则循环到缓冲区的顶部来保存数据。
//而对缓冲区的数据进行操作时，每提取一个字节的数据，则会从UART_Buffer_Top所指明的位置提取数据，然后
//UART_Buffer_Top进行自加一，如超出环形缓冲区底部，则循环到缓冲区的顶部。
unsigned short m_PackedDataNumber=0;	//用于记录当前解析的数据包中有效数据个数,该数据包含校验字节及帧尾
unsigned char UserTestUARTFIFOCommand(void)
{
	unsigned short usTemp;// = Buffer_But-Buffer_Top;
	unsigned char n_PackedDataNumber;
	unsigned char n_PackedDataCheck;
	unsigned short n_usTemp;
	unsigned short n_Buffer_Top;
	UART_Buffer_Bom = UART_RX_BUFFER_SIZE-DMA1_Channel5->CNDTR;//DMA_GetCurrDataCounter(DMA1_Channel5);
	usTemp = UART_Buffer_Bom-UART_Buffer_Top;
	usTemp = usTemp&(UART_RX_BUFFER_SIZE-1);
	if(usTemp==0) return 0;
	if(UART_RX_Buffer[UART_Buffer_Top]==0x55)
	{
		if(usTemp<4) return 0;					//数据包不符合要求
		n_Buffer_Top = UART_Buffer_Top+1;
		n_Buffer_Top = n_Buffer_Top&(UART_RX_BUFFER_SIZE-1);	//
		n_PackedDataNumber = UART_RX_Buffer[n_Buffer_Top++];
		n_Buffer_Top = n_Buffer_Top&(UART_RX_BUFFER_SIZE-1);
		if((n_PackedDataNumber+4)>usTemp) 
		{											//数据包不符合要求
			if(m_PackedDataNumber) 
			{
				m_PackedDataNumber--;
				UART_Buffer_Top++;
				UART_Buffer_Top = UART_Buffer_Top&(UART_RX_BUFFER_SIZE-1);
			}
			return 0;
		}
		if(n_PackedDataNumber>(UART_RX_BUFFER_SIZE-4))
		{	
			if(m_PackedDataNumber) 
				m_PackedDataNumber--;
			UART_Buffer_Top++;
			UART_Buffer_Top = UART_Buffer_Top&(UART_RX_BUFFER_SIZE-1);
			return 0;
		}
		n_usTemp = n_Buffer_Top+n_PackedDataNumber+1;
		n_usTemp = n_usTemp&(UART_RX_BUFFER_SIZE-1);
		if(UART_RX_Buffer[n_usTemp]==0xaa)			//检查数据缓冲区中是否在正确的位置中有数据包的帧尾标识
		{
			m_PackedDataNumber = n_PackedDataNumber+1;
			n_PackedDataCheck = 0;
			n_usTemp = n_Buffer_Top;
			//检查数据包的有效数据累加和值
			while(n_PackedDataNumber--)
			{
				n_PackedDataCheck += UART_RX_Buffer[n_usTemp++];
				n_usTemp = n_usTemp&(UART_RX_BUFFER_SIZE-1);
			}
			if(n_PackedDataCheck==UART_RX_Buffer[n_usTemp])
			{
				
				UART_Buffer_Top += 2;
				UART_Buffer_Top = UART_Buffer_Top&(UART_RX_BUFFER_SIZE-1);
				n_PackedDataCheck = UART_RX_Buffer[UART_Buffer_Top++];
				UART_Buffer_Top = UART_Buffer_Top&(UART_RX_BUFFER_SIZE-1);
				m_UARTFIFO_PackedDatNum = n_PackedDataNumber;
				return n_PackedDataCheck;			//返回数据包中的指令标识数据			 
			}
			else m_PackedDataNumber=0;
		}
		else
		{
			if(m_PackedDataNumber) 
				m_PackedDataNumber--;
			UART_Buffer_Top++;
			UART_Buffer_Top = UART_Buffer_Top&(UART_RX_BUFFER_SIZE-1);
			return 0;
		}				
	}
	if(m_PackedDataNumber) 
		m_PackedDataNumber--;
	UART_Buffer_Top++;
	UART_Buffer_Top = UART_Buffer_Top&(UART_RX_BUFFER_SIZE-1);
	return 0;		
}
//从UART接收数据缓冲区中提取一个字节数据
unsigned char UART_GetByte(void)
{
	unsigned char ucTemp;
	if(m_PackedDataNumber) 
		m_PackedDataNumber--;
	ucTemp = UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);
	return ucTemp;
}
//从UART接收数据缓冲区中提取一个WORD(16位)型数据
unsigned short UART_GetWord(void)
{
	unsigned short usTemp;
	if(m_PackedDataNumber>=2) m_PackedDataNumber-=2;
	else m_PackedDataNumber = 0;
	usTemp = UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);
	usTemp = usTemp<<8;
	usTemp += UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);	
	return usTemp;
}
//从UART接收数据缓冲区中提取一个有符INT(32位)型数据
signed int UART_GetSInt(void)
{
	signed int n_siTemp;
	if(m_PackedDataNumber>=4) m_PackedDataNumber-=4;
	else m_PackedDataNumber = 0;
//	unsigned char *p;
//	p = (unsigned char *)&n_siTemp;
	n_siTemp = UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);
	n_siTemp = n_siTemp<<8;
	n_siTemp |= UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);
	n_siTemp = n_siTemp<<8;
	n_siTemp |= UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);
	n_siTemp = n_siTemp<<8;
	n_siTemp |= UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);
	return n_siTemp;
}

//从UART接收数据缓冲区中提取一个有符float(32位)型数据
void UART_GetFloat(float *n_siTemp)
{
	//static float n_siTemp;
	unsigned char *p;
	p = (unsigned char *)n_siTemp;
	if(m_PackedDataNumber>=4) m_PackedDataNumber-=4;
	else m_PackedDataNumber = 0;
	p[3] = UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);
	p[2] = UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);
	p[1] = UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);
	p[0] = UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);
//	return n_siTemp;
}


