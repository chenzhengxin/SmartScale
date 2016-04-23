#include "CH376_SPI.h"
#include "delay.h"
#include "usart.h"

//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C							  
//SPI口初始化
//这里针是对SPI2的初始化
void SPI2_Init(void)	
{
 
	//配置SPI2管脚

	RCC->APB2ENR|=1<<0;
	RCC->APB2ENR|=1<<3;

	GPIOB->CRH&=0X000FFFFF;
	GPIOB->CRH|=0XB8B00000;

	GPIOB->CRL&=0XFFFFFF00;		 //PB.1 复位 推挽输出
	GPIOB->CRL|=0X00000033;		//PB.0 片选 推挽输出  

    RCC->APB2ENR|=1<<4;  //PORTC 时钟使能  
    GPIOC->CRL&=0XFF0FFFFF;//PC5   
    GPIOC->CRL|=0X00800000;//PC5--INT   
    GPIOC->IDR|=1<<5;//PC5上拉输入  
	 	

	RCC->APB1ENR|=1<<14;
	SPI2->CR1|=0<<10;//全双工模式	
	SPI2->CR1|=1<<9; //软件nss管理
	SPI2->CR1|=1<<8;  
	SPI2->CR1|=1<<2; //SPI主机
	SPI2->CR1|=0<<11;//8bit数据格式	
	SPI2->CR1|=1<<1; //空闲模式下SCK为0 CPOL=0
	SPI2->CR1|=1<<0; //数据采样从第一个时间边沿开始,CPHA=0  
	SPI2->CR1|=1<<3; //Fsck=Fcpu/4
	SPI2->CR1|=0<<7; //MSBfirst   
	SPI2->CR1|=1<<6; //SPI设备使能
	//SPIx_ReadWriteByte(0xff);//启动传输    
}

UINT8 SPI2_ReadWriteByte(UINT8 TxData)
{		
	while((SPI2->SR&0X02)==0);		//等待发送区空	  
	SPI2->DR=TxData;	 	//发送一个byte   
	while((SPI2->SR&0X01)==0);//等待接收完一个byte  
	return SPI2->DR;          	     //返回收到的数据  				    
}

/*******************************************************************************
* 函  数  名      : CH376_PORT_INIT
* 描      述      : 由于使用软件模拟SPI读写时序,所以进行初始化.
*                   如果是硬件SPI接口,那么可使用mode3(CPOL=1&CPHA=1)或
*                   mode0(CPOL=0&CPHA=0),CH376在时钟上升沿采样输入,下降沿输出,数
*                   据位是高位在前.
*******************************************************************************/
void	CH376_PORT_INIT( void )
{
	CH376_SPI_SCS = 1;  /* 禁止SPI片选 */
   CH376_INT_WIRE=1;   /* 默认为高电平,SPI模式3,也可以用SPI模式0,但模拟程序可能需稍做修改 */
/* 对于双向I/O引脚模拟SPI接口,那么必须在此设置SPI_SCS,SPI_SCK,SPI_SDI为输出方向,
*  SPI_SDO为输入方向 */
}

/*******************************************************************************
* 函  数  名      : xEndCH376Cmd   结束命令.
*******************************************************************************/
void	xEndCH376Cmd( void )
{ 
	CH376_SPI_SCS = 1; 	/* SPI片选无效,结束CH376命令,仅用于SPI接口方式 */
}
/*******************************************************************************
SPI输出8个位数据.    * 发送: UINT8 d:要发送的数据.
*******************************************************************************/
void	Spi376OutByte( UINT8 d )
{  
   SPI2_ReadWriteByte(d);	 
}

/*******************************************************************************
* 描      述      : SPI接收8个位数据.  UINT8 d:接收到的数据.
*******************************************************************************/
UINT8	Spi376InByte( void )
{
/* 如果是硬件SPI接口,应该是先查询SPI状态寄存器以等待SPI字节传输完成,然后从SPI数据寄存器读出数据 */
	while((SPI2->SR&1<<0)==0);    //RXEN=0 接收缓冲区为空						    
	return SPI2->DR;  //返回收到的数据	
}

/*******************************************************************************
* 描      述      : 向CH376写  命令.
* 输      入      : UINT8 mCmd:要发送的命令.
*******************************************************************************/
void	xWriteCH376Cmd( UINT8 mCmd )
{
	CH376_SPI_SCS = 1;    /* 防止之前未通过xEndCH376Cmd禁止SPI片选 */
	delay_us(10 );
	delay_us(10 );
/* 对于双向I/O引脚模拟SPI接口,那么必须确保已经设置SPI_SCS,SPI_SCK,SPI_SDI为输出
*  方向,SPI_SDO为输入方向 */
	CH376_SPI_SCS = 0;      /* SPI片选有效 */
	Spi376OutByte( mCmd );  /* 发出命令码 */
	delay_us(10 ); 
	delay_us(10 ); 
	delay_us(10 );   /* 延时1.5uS确保读写周期大于1.5uS,或者用上面一行的状态查询代替 */
	delay_us(10 ); 
}

/*******************************************************************************
* 描      述      : 向CH376写   数据.
* 输      入      : UINT8 mData:
*                   要发送的数据.
*******************************************************************************/
void	xWriteCH376Data( UINT8 mData )
{
	Spi376OutByte( mData );
	delay_us(10 );  /* 确保读写周期大于0.6uS */
}

/*******************************************************************************
* 函  数  名      : xReadCH376Data
* 描      述      : 从CH376读数据.
*******************************************************************************/
UINT8	xReadCH376Data( void )
{
	UINT8 i;
	delay_us(10);
	i = SPI2_ReadWriteByte( 0xFF );
	return( i );
}

/*******************************************************************************
* 描      述      : 查询CH376中断(INT#低电平).
* 返      回      : FALSE:无中断.       TRUE:有中断.
*******************************************************************************/
UINT8	Query376Interrupt( void )
{
	UINT8 i;
	i = (CH376_INT_WIRE ? FALSE : TRUE ); 	/* 如果连接了CH376的中断引脚则直接查询中断引脚 */
	return( i ); 
}

/*******************************************************************************
* 描      述      : 初始化CH376.
* 返      回      : FALSE:无中断.  TRUE:有中断.
*******************************************************************************/
UINT8	mInitCH376Host( void )
{
	UINT8	res;	
	delay_ms(200);
	delay_ms(200);
	delay_ms(200);

	CH376_PORT_INIT( );           /* 接口硬件初始化 */
	xWriteCH376Cmd( CMD11_CHECK_EXIST );    /* 测试单片机与CH376之间的通讯接口 */
	xWriteCH376Data( 0x55 );
	res = xReadCH376Data( );
	//printf("[CMD11_CHECK_EXIST] res =%02x \n",(unsigned short)res);
	xEndCH376Cmd( );
	if ( res != 0xAA ) return( ERR_USB_UNKNOWN );  /* 通讯接口不正常,可能原因有:接口连接异常,其它设备影响(片选不唯一),串口波特率,一直在复位,晶振不工作 */
	xWriteCH376Cmd( CMD11_SET_USB_MODE ); /* 设备USB工作模式 */
	xWriteCH376Data( 0x06 ); //06H=已启用的主机方式并且自动产生SOF包  ???
	delay_us( 20 );
	res = xReadCH376Data( );
	//printf("[CMD11_SET_USB_MODE] res =%02x \n",(unsigned short)res);
	xEndCH376Cmd( );

	if ( res == CMD_RET_SUCCESS )  //RES=51  命令操作成功
	{
	    return( USB_INT_SUCCESS ); //USB事务或者传输操作成功 
	}
	else 
	{
	    return( ERR_USB_UNKNOWN );/* 设置模式错误 */
	}
}
/************************************ End *************************************/
