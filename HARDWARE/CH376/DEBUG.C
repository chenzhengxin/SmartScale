/* 调试 */
/* 提供printf子程序 */

#include "delay.h"
#include "usart.h"
#include "stm32f10x.h"
#include	"DEBUG.H"

/* 检查操作状态,如果错误则显示错误代码并停机,应该替换为实际的处理措施,例如显示错误信息,等待用户确认后重试等 */
int	mStopIfError( UINT8 iError )
{
	if ( iError == USB_INT_SUCCESS ) 
		return 1;  /* 操作成功 */
	//printf("Error: %02X\n", (UINT16)iError );  /* 显示错误 */
	else
		return 0;  /*操作失败*/
	
}

