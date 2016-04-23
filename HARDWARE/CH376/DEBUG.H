/* 调试 */
/* 提供printf子程序 */

#include	"CH376INC.H"

#ifndef	__DEBUG_H__
#define __DEBUG_H__


/* 检查操作状态,如果错误则显示错误代码并停机,应该替换为实际的处理措施,例如显示错误信息,等待用户确认后重试等 */
int	mStopIfError( UINT8 iError );


#endif
