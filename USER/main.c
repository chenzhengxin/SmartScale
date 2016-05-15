#include "delay.h"
#include "sys.h"
#include "includes.h" 
#include "usart.h"
#include "malloc.h"
#include "rc522.h"
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"
#include "HX711.h"
#include "rtc.h"
#include "flash.h"
#include "usart.h"
#include "EzUI_Ctrl.h"
#include "fileopt.h"
#include "CH376_api.h"


	 
/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
__align(8) static OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	
 			   
//主任务
//设置任务优先级
#define MAIN_TASK_PRIO       			6 
//设置任务堆栈大小
#define MAIN_STK_SIZE  					256
//任务堆栈，8字节对齐	
__align(8) static OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//任务函数
void main_task(void *pdata);

float g_ad_show_pi = 0.0;


//系统初始化
void system_init(void)
{
    NVIC_Configuration();
	delay_init();	    	 //延时函数初始化
	uart_printer_init(9600);
	InitRfid();				 //初始化RFID	
	SPI_Flash_Init();	     //W25Qxx初始化
	uart_lcd_init(115200);
	mem_init();				 //内存池初始化
	exfuns_init();		     //为fatfs相关变量申请内存
	RTC_Init();
	f_mount(fs[0],"0:",1);	//挂载SD卡 
 	f_mount(fs[1],"1:",1); 	//挂载FLASH
	AD_GPIO_Configuration();
	fileopt_init();
	lcdopt_init();
}

#if  1
int main(void)
{		
	system_init();	 
	//tmp();
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);//创建起始任务
	OSStart();	
	return 0;
}
#endif

#if 0
int main()
{
	int i = 0;
	//char buf1[] = "工号           姓名             物种             总重量               总金额\r\n";
	//char buf2[] = "T12035        罗海斌            海蛎             100市斤            10000人民币\r\n";
	unsigned char *buf3 = "/智能秤.TXT"; /*文件名前面的 / 字符不得省略（表示根目录创建该文件）*/

	/*InitUSB()也包含了对STM32的一些硬件的初始化，如有重复可将其删除（对SPI2的初始化切勿删除）
	 *InitUSB()函数里面还有一处需要弹出 “请插入U盘！” 的提示框，具体位置函数里面有标注
	*/
	InitUSB();
	OpenUSBFile((PUINT8)buf3);

	/*以下的for循环是模拟从SD卡或者flash中读取一行文件内容或者一个员工数据记录，然后再将读取到的内容写入U盘中*/
	/*for(i = 0; i < 2; i++)
	{
		if(i == 0){
			WriteDataBufToUSB("ncdsvbfdiubv");	
		}else{
		    WriteDataBufToUSB("1233");
		}
	}*/
	WriteDataBufToUSB("1234566");	
	WriteDataBufToUSB("qwweetrysyusdu");
	CloseUSBFile();
	/*在这里（WaitTakeOutUSB()函数之后）弹出 “数据拷贝完成，请拔出U盘！” 的提示框*/
	WaitTakeOutUSB();

	return 0;
}
#endif
	  
//开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断) 
	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);   						   	 				   
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}



//主任务
void main_task(void *pdata)
{ 
#if 1
	while(1)
	{
	    //TimerDly(10000);
	    switch(UserTestUARTFIFOCommand())
	    {
		    case 0xEB:      //0xEB的返回数据包为模块GUI服务引擎消息
			{
		    	lcdopt_main();
				//UART_DMA_Rx_Enable();
			    break;
		    }
		    default:
				break;
	    }

		lcdopt_printf_card_id();
		lcdopt_printf_weight();
		fileopt_copy_sd_to_flash();
	}
#endif
//tmp();

}
