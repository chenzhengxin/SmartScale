/* ���� */
/* �ṩprintf�ӳ��� */

#include "delay.h"
#include "usart.h"
#include "stm32f10x.h"
#include	"DEBUG.H"

/* ������״̬,�����������ʾ������벢ͣ��,Ӧ���滻Ϊʵ�ʵĴ�����ʩ,������ʾ������Ϣ,�ȴ��û�ȷ�Ϻ����Ե� */
int	mStopIfError( UINT8 iError )
{
	if ( iError == USB_INT_SUCCESS ) 
		return 1;  /* �����ɹ� */
	//printf("Error: %02X\n", (UINT16)iError );  /* ��ʾ���� */
	else
		return 0;  /*����ʧ��*/
	
}
