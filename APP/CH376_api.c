#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CH376_api.h"

UINT16	total = 0;		/* ��¼��ǰ������FILE_DATA_BUF�е����ݳ��� */
UINT8	FileDataBuf[FILE_MEM_BUF_LEN] = {0};	/* �ļ�������̫С���ٶȑk*/


/* ��׼��д��U�̵��������ݽ��м��л���,��ϳɴ����ݿ�ʱ��ͨ��CH376����д��U�a*/
/* �������ĺô��p ����ٶ�(��Ϊ�����ݿ�д��ʱЧ�ʸ�), ����U����U���ڲ����ڴ���������,����Ƶ����д) */
int	mFlushBufferToDisk( UINT8 force)
/* force = 0 ���Զ�ˢ���黺�����е����ݳ���,����д��,��������ʱ���ڻ�����د, force != 0 ��ǿ��ˢ�岻�ܻ������е������ж��ٶ�д�aͨ����ϵͳ�ػ�ǰӦ��ǿ��д��) */
{
	UINT8	s = 0;
	UINT32 NewSize = 0;
	int ret = 1;

	if ( force ) {  /* ǿ��ˢ�� */
		s = ( total + DEF_SECTOR_SIZE - 1 ) / DEF_SECTOR_SIZE;  /* ���������е��ֽ���ת��Ϊ����������DEF_SECTOR_SIZE),���ȼ���DEF_SECTOR_SIZE-1����ȷ��д��������ͷ���� */
		if ( s ) {  /* ������*/
			s = CH376SecWrite( FileDataBuf, s, NULL );  /* ������Ϊ��λ���ļ�д������д�뻺�����е��������ݺ�������ͷ */
			ret = mStopIfError( s );
			if(!ret){
				return 0;
			}
/* ��ЩU�̿��ܻ�Ҫ����д���ݺ�ȴ�һ����ܼ�����ʹ���������ĳЩU���з������ݶ�ʧ����,������ÿ��д�����ݺ�������ʱ�ټ���*/
			delay_ms( 1 );  /* д����ʱ,��ѡ��,�����U�̲����]*/
			memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );  /* ���ղ���д��U�̵���ͷ���ݸ��Ƶ���������ͷ�d*/
			total &= DEF_SECTOR_SIZE - 1;  /* ��������ֻʣ�¸ղ���д��U�̵���ͷ����,���������ڻ���������Ϊ�˷����Ժ��������׷������ */
		}
		NewSize = CH376GetFileSize( );  /* ��ȡ��ǰ�ļ�����,���û����ͷ����,��ô�ļ�������DEF_SECTOR_SIZE�ı��� */
		if ( total ) NewSize -= DEF_SECTOR_SIZE - total;  /* ������Ϊ��λ,����ͷ���ݼ�����������ļ�����(��Ч���ݵĳ��� */
		CH376WriteVar32( VAR_FILE_SIZE, NewSize );  /* ����ȷ�ĵ�ǰ�ļ�����д��CH376�ڴ� */
		//printf( "Current file size is %ld\n", NewSize );
		s = CH376SecWrite( FileDataBuf, 0, NULL );  /* �ﳤ��,ʵ����ˢ���ļ�������CH376�ڴ��е�ǰ�ļ�������Ϣ����д��U�̻���SD���� */
		ret = mStopIfError( s );
		if(!ret){
			return 0;
		}
		s = CH376SecLocate( 0xFFFFFFFF );  /* ���»ص�ԭ�ļ���β��,������Ϊ��λ,���Ի�����ļ�β������ͷ�������������д�����ݽ�����β����ͷ����,��������ͷ������һ�ݸ��������ڻ�������,��������� */
		ret = mStopIfError( s );
		if(!ret){
			return 0;
		}else{
			return 1;
		}
	}
	else if ( total >= sizeof( FileDataBuf ) - DEF_SECTOR_SIZE ) {  /* �������е����ݿ�Ҫ��������Ӧ���Ƚ��������е�ԭ������д��U�a*/
		s = CH376SecWrite( FileDataBuf, (UINT8)( total / DEF_SECTOR_SIZE ), NULL );  /* ���������е��ֽ���ת��Ϊ����������DEF_SECTOR_SIZE),������ͷ�����Ȳ���������Ϊ��λ���ļ�д������*/
		ret = mStopIfError( s );
		if(!ret){
			return 0;
		}
		memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );  /* ���ղ�δд��U�̵���ͷ���ݸ��Ƶ���������ͷ�d*/
		total &= DEF_SECTOR_SIZE - 1;  /* ��������ֻʣ�¸ղ�δд��U�̵���ͷ���� */
/* ע��,�˴�U�̻���SD���е��ļ�������Ȼ����ǰ�Ă�����ʱͻȻ�Ϯm��ô��Ȼ�����ϵ���������U�aSD����,���ļ�����û�а�����Щ���ݵ��³���Ӧ�ó����޷�ȡ������ */
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
	CH376_RST	= 1;			/* ��λ */
	delay_ms( 20 );
	CH376_RST	= 0;				/* ��ֹ��λ */
	delay_ms( 100 );     /* ��ʱ100���� */
	
	s = mInitCH376Host();  /* ��ʼ��CH376 */
	ret = mStopIfError( s );
	if(!ret){
		return 0;
	}
	
	while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) {  /* ���U���Ƿ������ȴ�U�̲�϶���SD������ɵ�Ƭ��ֱ�Ӳ�ѯSD�����Ĳ��״̬��à*/
		ToWarn++;
		delay_ms( 100 );
		if(ToWarn == 20){
		/*??o?? p???U?��q ????*/
					
		}
	}

	delay_ms( 200 );  /* ��ʱ,��ѡ��ʹ�е�USB�洢����Ҫ��ʮ�������ʱ */
	
/* ���ڼ�⵽USB�豸�����ȏ�0*50mS */
	for ( s = 0; s < 10; s ++ ) {  /* ��ȴ�ʱ�]10*50mS */
		delay_ms( 50 );
		//printf( "Ready ?\n" );
		if ( CH376DiskMount( ) == USB_INT_SUCCESS ) break;  /* ��ʼ�����̲����Դ����Ƿ���� */
	}

/* ���U�̻���SD����ʣ��ռ� */
	//printf( "DiskQuery: " );
	s = CH376DiskQuery( (PUINT32)buf );  /* ��ѯ����ʣ��ռ���Ϣ,������*/
	ret = mStopIfError( s );
	if(!ret){
		return 0;
	}else{
		return 1;
	}
    //printf( "free cap = %ld MB\n", *(PUINT32)buf * DEF_SECTOR_SIZE / 1000000 );  /* δ������������������С��,����1M����Ϊ���ֽڵ�λ */
	//printf( "free cap = %ld MB\n", *(PUINT32)buf / ( 1000000 / DEF_SECTOR_SIZE ) );  /* ����һ�еļ�����ܻ����任��˳����� */
}

int OpenUSBFile(PUINT8 FileName)
{
	UINT8	s = 0;
	UINT32 NewSize = 0;
	int ret = 0;
	
	s = CH376FileOpen( FileName );  /* ���ļ�,���ļ��ڸ�Ŀ¼�� */
	if ( s == USB_INT_SUCCESS ) {  /* �ļ����ڲ����Ѿ�����,�ƶ��ļ�ָ�뵽β���Ա��������*/
		NewSize = CH376GetFileSize( );  /* ��ȡ��ǰ�ļ�����,���û����ͷ����,��ô�ļ�������DEF_SECTOR_SIZE�ı��� */
		//printf( "File size = %ld\n", NewSize );
		//printf( "Locate tail\n" );
		s = CH376SecLocate( 0xFFFFFFFF );  /* ������Ϊ��λ�Ƶ��ļ�β��,����ļ�β���в���һ����������ͷ�����򱻺���,�������������ô��ͷ���ݽ����ܱ�д�����ݸ��� */
		ret = mStopIfError( s );
		if(!ret){
			return 0;
		}
		total = NewSize & ( DEF_SECTOR_SIZE - 1 );  /* �ϴα����ļ�ʱ���β������ͷ����,��ô��ȡ����ͷ�ֽ���,�����������ȵ���������*/
		//printf( "Read last tail = %d Bytes\n", total );
		if ( total ) {  /* ԭβ������ͷ����,��ô�ȵ����ڌ\�Ա����������� */
			s = CH376SecRead( FileDataBuf, 1, NULL );  /* ���ļ���ȡβ����ͷ����*/
			ret = mStopIfError( s );
			if(!ret){
				return 0;
			}
			s = CH376SecLocate( 0xFFFFFFFF );  /* ���»ص�ԭ�ļ���β��,�������д�����ݽ�����ԭβ����ͷ����,����ԭ��ͷ���ݸղ��Ѿ��������ڴ�,��������� */
			ret = mStopIfError( s );
			if(!ret){
				return 0;
			}else{
				return 1;
			}
		}
	}
	else if ( s == ERR_MISS_FILE ) {  /* û���ҵ��ļ�,�����½��ļ� */
		//printf( "Create file\n" );
		s = CH376FileCreate( NULL );  /* �½��ļ�����,����ļ��Ѿ���������ɾ�������½�,�������ṩ�ļ���,�ղ��Ѿ��ṩ��CH376FileOpen */
		ret = mStopIfError( s );
		total = 0;  /* ��ǰû����ͷ���� */
		NewSize = 0;
		if(!ret){
			return 0;
		}else{
			return 1;
		}
	}
	else {
		ret = mStopIfError( s );  /* ���ļ�ʱ����*/
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
	total += sprintf( (char *)(FileDataBuf + total), "%s\xd\xa", "" );  /* ����������ӵ���������β�d�ۼƻ������ڵ����ݳ���*/
	mFlushBufferToDisk(1);  /* ǿ��ˢ�»�������ΪϵͳҪ�˳���,���Ա���ǿ��ˢ��*/
	//printf( "Close\n" );
	s = CH376FileClose( FALSE );  /* �ر��ļ�,����ģʽͨ�����Զ������ļ�������Ϊǿ��ˢ�»�����ʱ�Ѿ��������ļ������������ﲻ��Ҫ�Զ������ļ�����*/
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
/* ���XXX.TXT�ļ��Ѿ�������������ݵ�β��,������������½��ļ� */
	int ret = 0;

	//printf( "Write begin\n" );
	memset(FileDataBuf + total, 0, sizeof(FileDataBuf) - (total));
	total += sprintf( (char *)(FileDataBuf + total), "%s\xd\xa", DataBuf );  /* ����������ӵ���������β�d�ۼƻ������ڵ����ݳ���*/
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

	while ( (CH376DiskConnect( ) == USB_INT_SUCCESS) && (num < 10) ) {  /* ���U���Ƿ������ȴ�U�̰���*/
		delay_ms( 100 );
		num++;
	}
	//delay_ms( 200 );
}
