#include "sys.h"
#include "usart.h"	  
#include "stm32f10x_dma.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
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
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef  NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
   // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}
#if EN_USART1_RX   //���ʹ���˽���
void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
#endif	

#endif

//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[200];     //���ջ���,���200���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  

//��ʼ��IO ����2
//bound:������

void USART2_IRQHandler(void)
{
	u8 res;	
#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
	{	 
		res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
		USART_RX_BUF[0]=res;
		if (USART_RX_BUF[0] & 0X04)
	    {
		    USART_RX_STA = 1;
		}		 									     
	}
#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	OSIntExit();  											 
#endif
} 


void uart_printer_init(u32 bound){
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_DeInit(USART2);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

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
    //GPIO�˿�����
    GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure; 

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

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

	USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    /* Enable USART1 DMA Rxrequest */ 
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	/* Enable DMA1 Channel4 */ 
	DMA_Cmd(DMA1_Channel4, ENABLE); 
	/* Enable DMA1 Channel5 */ 
	DMA_Cmd(DMA1_Channel5, ENABLE); 
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

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

	USART1->CR3 |= 0x0080;						//ʹ��UART1��DMA����

	DMA1_Channel4->CCR |= 0x0001; 				//ʹ��DMA1ͨ����,�����ݷ������,�����DMA1ͨ���ĵ��ж�,���������UART_TX_Status����
												//������ο�stm32f10x_it.c�ļ��еĴ���
}

//�û�������Ҫ������ʹ�õ�MCU���޸ĸú���
//�ú���Ӧ���û�������ѭ������,�ں����л��жϵ�ǰUART���յ�ģ�鷢�ص����ݳ�����,�Ƿ�����Ч��
//���ݰ�,������Ч���ݰ�,�򷵻�0,���򷵻���Ч���ݰ��е�ָ���ʶ
//UART_RX_Buffer[UART_RX_BUFFER_SIZE]Ϊһ�����ε����ݻ�����
//UART_Buffer_TopΪ���λ������ĵ�ǰ������������,��ָ����ǰ��������,��ǰ��ͷ������λ��
//UART_Buffer_BomΪ���λ������ĵ�ǰ������������,��ָ����ǰ��������,���һ�����ݵ�λ��
//��UART_Buffer_Top==UART_Buffer_Bomʱ,����,�����������޴���ȡ������
//��ʹ����,UARTÿ����һ������,��ᱣ����UART_Buffer_Bom��ָ����λ��,Ȼ��UART_Buffer_Bom���Լ�һ,��
//�����˻��λ������ĵײ�,��ѭ�����������Ķ������������ݡ�
//���Ի����������ݽ��в���ʱ��ÿ��ȡһ���ֽڵ����ݣ�����UART_Buffer_Top��ָ����λ����ȡ���ݣ�Ȼ��
//UART_Buffer_Top�����Լ�һ���糬�����λ������ײ�����ѭ�����������Ķ�����
unsigned short m_PackedDataNumber=0;	//���ڼ�¼��ǰ���������ݰ�����Ч���ݸ���,�����ݰ���У���ֽڼ�֡β
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
		if(usTemp<4) return 0;					//���ݰ�������Ҫ��
		n_Buffer_Top = UART_Buffer_Top+1;
		n_Buffer_Top = n_Buffer_Top&(UART_RX_BUFFER_SIZE-1);	//
		n_PackedDataNumber = UART_RX_Buffer[n_Buffer_Top++];
		n_Buffer_Top = n_Buffer_Top&(UART_RX_BUFFER_SIZE-1);
		if((n_PackedDataNumber+4)>usTemp) 
		{											//���ݰ�������Ҫ��
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
		if(UART_RX_Buffer[n_usTemp]==0xaa)			//������ݻ��������Ƿ�����ȷ��λ���������ݰ���֡β��ʶ
		{
			m_PackedDataNumber = n_PackedDataNumber+1;
			n_PackedDataCheck = 0;
			n_usTemp = n_Buffer_Top;
			//������ݰ�����Ч�����ۼӺ�ֵ
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
				return n_PackedDataCheck;			//�������ݰ��е�ָ���ʶ����			 
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
//��UART�������ݻ���������ȡһ���ֽ�����
unsigned char UART_GetByte(void)
{
	unsigned char ucTemp;
	if(m_PackedDataNumber) 
		m_PackedDataNumber--;
	ucTemp = UART_RX_Buffer[UART_Buffer_Top++];
	UART_Buffer_Top &= (UART_RX_BUFFER_SIZE-1);
	return ucTemp;
}
//��UART�������ݻ���������ȡһ��WORD(16λ)������
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
//��UART�������ݻ���������ȡһ���з�INT(32λ)������
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

//��UART�������ݻ���������ȡһ���з�float(32λ)������
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


