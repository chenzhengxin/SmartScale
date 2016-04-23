#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CH376_api.h"

UINT16	total = 0;		/* 记录当前缓冲在FILE_DATA_BUF中的数据长度 */
UINT8	FileDataBuf[FILE_MEM_BUF_LEN] = {0};	/* 文件缓冲匿太小则速度慿*/


/* 将准备写入U盘的零碎数据进行集中缓冲,组合成大数据块时再通过CH376真正写入U盿*/
/* 这样做的好处昿 提高速度(因为大数据块写入时效率高), 减少U盘损耿U盘内部的内存寿命有限,不宜频繁擦写) */
int	mFlushBufferToDisk( UINT8 force)
/* force = 0 则自动刷斿检查缓冲区中的数据长度,满则写盘,不满则暂时放在缓冲区丿, force != 0 则强制刷斿不管缓冲区中的数据有多少都写盿通常在系统关机前应该强制写盘) */
{
	UINT8	s = 0;
	UINT32 NewSize = 0;
	int ret = 1;

	if ( force ) {  /* 强制刷新 */
		s = ( total + DEF_SECTOR_SIZE - 1 ) / DEF_SECTOR_SIZE;  /* 将缓冲区中的字节数转换为扇区敿除以DEF_SECTOR_SIZE),长度加上DEF_SECTOR_SIZE-1用于确保写入最后的零头数据 */
		if ( s ) {  /* 有数捿*/
			s = CH376SecWrite( FileDataBuf, s, NULL );  /* 以扇区为单位向文件写入数捿写入缓冲区中的所有数捿含最后的零头 */
			ret = mStopIfError( s );
			if(!ret){
				return 0;
			}
/* 有些U盘可能会要求在写数据后等待一会才能继续操使所仿如果在某些U盘中发生数据丢失现象,建议在每次写入数据后稍作延时再继绿*/
			delay_ms( 1 );  /* 写后延时,可选的,大多数U盘不需覿*/
			memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );  /* 将刚才已写入U盘的零头数据复制到缓冲区的头郿*/
			total &= DEF_SECTOR_SIZE - 1;  /* 缓冲区中只剩下刚才已写入U盘的零头数据,继续保留在缓冲区中是为了方便以后在其后面追加数据 */
		}
		NewSize = CH376GetFileSize( );  /* 读取当前文件长度,如果没有零头数据,那么文件长度是DEF_SECTOR_SIZE的倍数 */
		if ( total ) NewSize -= DEF_SECTOR_SIZE - total;  /* 以扇区为单位,有零头数捿计算出真正的文件长度(有效数据的长庿 */
		CH376WriteVar32( VAR_FILE_SIZE, NewSize );  /* 将正确的当前文件长度写入CH376内存 */
		//printf( "Current file size is %ld\n", NewSize );
		s = CH376SecWrite( FileDataBuf, 0, NULL );  /* 冿长度,实际是刷新文件长庿将CH376内存中当前文件长度信息真正写入U盘或者SD卡中 */
		ret = mStopIfError( s );
		if(!ret){
			return 0;
		}
		s = CH376SecLocate( 0xFFFFFFFF );  /* 重新回到原文件的尾部,以扇区为单位,所以会忽略文件尾部的零头数捿下面如果再写入数据将覆盖尾部零头数据,不过该零头数据有一份副本保留在缓冲区中,所以请放心 */
		ret = mStopIfError( s );
		if(!ret){
			return 0;
		}else{
			return 1;
		}
	}
	else if ( total >= sizeof( FileDataBuf ) - DEF_SECTOR_SIZE ) {  /* 缓冲区中的数据快要满亿所以应该先将缓冲区中的原有数据写入U盿*/
		s = CH376SecWrite( FileDataBuf, (UINT8)( total / DEF_SECTOR_SIZE ), NULL );  /* 将缓冲区中的字节数转换为扇区敿除以DEF_SECTOR_SIZE),最后的零头数据先不箿以扇区为单位向文件写入数捿*/
		ret = mStopIfError( s );
		if(!ret){
			return 0;
		}
		memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );  /* 将刚才未写入U盘的零头数据复制到缓冲区的头郿*/
		total &= DEF_SECTOR_SIZE - 1;  /* 缓冲区中只剩下刚才未写入U盘的零头数据 */
/* 注意,此处U盘或者SD卡中的文件长度仍然是以前的倿即此时突然断甿那么虽然物理上的数据已在U盿SD卡中,但文件长度没有包括这些数捿导致常归应用程序无法取得数据 */
		return 1;
	}

	return ret;
}

int InitUSB(void)
{
	UINT8	s = 0;
	UINT8	buf[64] = {0};
	UINT8   ToWarn = 0;
	int ret = 0;
	
	//delay_init();
	//NVIC_Configuration();// ?????????
	//uart_init(9600);	 //??????9600
	SPI2_Init();
	CH376_RST	= 1;			/* 复位 */
	delay_ms( 20 );
	CH376_RST	= 0;				/* 禁止复位 */
	delay_ms( 100 );     /* 延时100毫秒 */
	
	s = mInitCH376Host();  /* 初始化CH376 */
	ret = mStopIfError( s );
	if(!ret){
		return 0;
	}
	
	while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) {  /* 检查U盘是否连掿等待U盘插兿对于SD卿可以由单片机直接查询SD卡座的插拔状态引脿*/
		ToWarn++;
		delay_ms( 100 );
		if(ToWarn == 20){
		/*??o?? p???U?áq ????*/
					
		}
	}

	delay_ms( 200 );  /* 延时,可选操使有的USB存储器需要几十毫秒的延时 */
	
/* 对于检测到USB设备皿最多等徿0*50mS */
	for ( s = 0; s < 10; s ++ ) {  /* 最长等待时闿10*50mS */
		delay_ms( 50 );
		//printf( "Ready ?\n" );
		if ( CH376DiskMount( ) == USB_INT_SUCCESS ) break;  /* 初始化磁盘并测试磁盘是否就绪 */
	}

/* 检查U盘或者SD卡的剩余空间 */
	//printf( "DiskQuery: " );
	s = CH376DiskQuery( (PUINT32)buf );  /* 查询磁盘剩余空间信息,扇区敿*/
	ret = mStopIfError( s );
	if(!ret){
		return 0;
	}else{
		return 1;
	}
    //printf( "free cap = %ld MB\n", *(PUINT32)buf * DEF_SECTOR_SIZE / 1000000 );  /* 未用扇区数乘以扇区大小后,除以1M换算为兆字节单位 */
	//printf( "free cap = %ld MB\n", *(PUINT32)buf / ( 1000000 / DEF_SECTOR_SIZE ) );  /* 上面一行的计算可能会溢凿换个顺序计算 */
}

int OpenUSBFile(PUINT8 FileName)
{
	UINT8	s = 0;
	UINT32 NewSize = 0;
	int ret = 0;
	
	s = CH376FileOpen( FileName );  /* 打开文件,该文件在根目录下 */
	if ( s == USB_INT_SUCCESS ) {  /* 文件存在并且已经被打开,移动文件指针到尾部以便添加数捿*/
		NewSize = CH376GetFileSize( );  /* 读取当前文件长度,如果没有零头数据,那么文件长度是DEF_SECTOR_SIZE的倍数 */
		//printf( "File size = %ld\n", NewSize );
		//printf( "Locate tail\n" );
		s = CH376SecLocate( 0xFFFFFFFF );  /* 以扇区为单位移到文件尾部,如果文件尾部有不足一个扇区的零头数据则被忽略,如果不做处理那么零头数据将可能被写入数据覆盖 */
		ret = mStopIfError( s );
		if(!ret){
			return 0;
		}
		total = NewSize & ( DEF_SECTOR_SIZE - 1 );  /* 上次保存文件时如果尾部有零头数据,那么先取得零头字节数,不满扇区长度的零碎数捿*/
		//printf( "Read last tail = %d Bytes\n", total );
		if ( total ) {  /* 原尾部有零头数据,那么先调入内孿以便接着添加数据 */
			s = CH376SecRead( FileDataBuf, 1, NULL );  /* 从文件读取尾部零头数捿*/
			ret = mStopIfError( s );
			if(!ret){
				return 0;
			}
			s = CH376SecLocate( 0xFFFFFFFF );  /* 重新回到原文件的尾部,下面如果写入数据将覆盖原尾部零头数据,不过原零头数据刚才已经被读入内存,所以请放心 */
			ret = mStopIfError( s );
			if(!ret){
				return 0;
			}else{
				return 1;
			}
		}
	}
	else if ( s == ERR_MISS_FILE ) {  /* 没有找到文件,必须新建文件 */
		//printf( "Create file\n" );
		s = CH376FileCreate( NULL );  /* 新建文件并打开,如果文件已经存在则先删除后再新建,不必再提供文件名,刚才已经提供给CH376FileOpen */
		ret = mStopIfError( s );
		total = 0;  /* 此前没有零头数据 */
		NewSize = 0;
		if(!ret){
			return 0;
		}else{
			return 1;
		}
	}
	else {
		ret = mStopIfError( s );  /* 打开文件时出锿*/
		if(!ret){
			return 0;
		}else{
			return 1;
		}
	}

	return ret;
}

int CloseUSBFile(void)
{
	UINT8	s = 0;
	int ret = 0;

	memset(FileDataBuf + total, 0, sizeof(FileDataBuf) - (total));
	total += sprintf( (char *)(FileDataBuf + total), "%s\xd\xa", "" );  /* 将新数据添加到缓冲区的尾郿累计缓冲区内的数据长庿*/
	mFlushBufferToDisk(1);  /* 强制刷新缓冲匿因为系统要退出了,所以必须强制刷斿*/
	//printf( "Close\n" );
	s = CH376FileClose( FALSE );  /* 关闭文件,扇区模式通常不自动更新文件长庿因为强制刷新缓冲区时已经更新了文件长庿所以这里不需要自动更新文件长庿*/
	ret = mStopIfError( s );

	total = 0;
	memset(FileDataBuf, 0, sizeof(FileDataBuf));
	if(!ret){
		return 0;
	}else{
		return 1;
	}
}

int WriteDataBufToUSB(char *DataBuf)
{
/* 如果XXX.TXT文件已经存在则添加数据到尾部,如果不存在则新建文件 */
	int ret = 0;

	//printf( "Write begin\n" );
	memset(FileDataBuf + total, 0, sizeof(FileDataBuf) - (total));
	total += sprintf( (char *)(FileDataBuf + total), "%s\xd\xa", DataBuf );  /* 将新数据添加到缓冲区的尾郿累计缓冲区内的数据长庿*/
	ret = mFlushBufferToDisk(0);
	if(!ret){
		return 0;
	}else{
		delay_ms(50);
		return 1;
	}
}

void WaitTakeOutUSB(void)
{
	//printf( "Take out\n" );
	int num = 0;

	while ( (CH376DiskConnect( ) == USB_INT_SUCCESS) && (num < 10) ) {  /* 检查U盘是否连掿等待U盘拔凿*/
		delay_ms( 100 );
		num++;
	}
	//delay_ms( 200 );
}
