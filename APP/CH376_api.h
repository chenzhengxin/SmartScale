#ifndef __CH376_API__
#define __CH376_API__

#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stm32f10x.h"
#include "FILE_SYS.h"
#include "CH376_SPI.h"
#include "CH376INC.H"
#include "DEBUG.H"

#define FILE_MEM_BUF_LEN 0x400

extern int InitUSB(void);
extern int WriteDataBufToUSB(char *DataBuf);
extern void WaitTakeOutUSB(void);
extern int OpenUSBFile(PUINT8 FileName);
extern int CloseUSBFile(void);

#endif
