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


	 
/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ	
__align(8) static OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	
 			   
//������
//�����������ȼ�
#define MAIN_TASK_PRIO       			6 
//���������ջ��С
#define MAIN_STK_SIZE  					256
//�����ջ��8�ֽڶ���	
__align(8) static OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//������
void main_task(void *pdata);

float g_ad_show_pi = 0.0;


//ϵͳ��ʼ��
void system_init(void)
{
    NVIC_Configuration();
	delay_init();	    	 //��ʱ������ʼ��
	uart_printer_init(9600);
	InitRfid();				 //��ʼ��RFID	
	SPI_Flash_Init();	     //W25Qxx��ʼ��
	uart_lcd_init(115200);
	mem_init();				 //�ڴ�س�ʼ��
	exfuns_init();		     //Ϊfatfs��ر��������ڴ�
	RTC_Init();
	f_mount(fs[0],"0:",1);	//����SD�� 
 	f_mount(fs[1],"1:",1); 	//����FLASH
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
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);//������ʼ����
	OSStart();	
	return 0;
}
#endif

#if 0
int main()
{
	int i = 0;
	//char buf1[] = "����           ����             ����             ������               �ܽ��\r\n";
	//char buf2[] = "T12035        �޺���            ����             100�н�            10000�����\r\n";
	unsigned char *buf3 = "/���ܳ�.TXT"; /*�ļ���ǰ��� / �ַ�����ʡ�ԣ���ʾ��Ŀ¼�������ļ���*/

	/*InitUSB()Ҳ�����˶�STM32��һЩӲ���ĳ�ʼ���������ظ��ɽ���ɾ������SPI2�ĳ�ʼ������ɾ����
	 *InitUSB()�������滹��һ����Ҫ���� �������U�̣��� ����ʾ�򣬾���λ�ú��������б�ע
	*/
	InitUSB();
	OpenUSBFile((PUINT8)buf3);

	/*���µ�forѭ����ģ���SD������flash�ж�ȡһ���ļ����ݻ���һ��Ա�����ݼ�¼��Ȼ���ٽ���ȡ��������д��U����*/
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
	/*�����WaitTakeOutUSB()����֮�󣩵��� �����ݿ�����ɣ���γ�U�̣��� ����ʾ��*/
	WaitTakeOutUSB();

	return 0;
}
#endif
	  
//��ʼ����
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��) 
	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);   						   	 				   
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}



//������
void main_task(void *pdata)
{ 
#if 1
	while(1)
	{
	    //TimerDly(10000);
	    switch(UserTestUARTFIFOCommand())
	    {
		    case 0xEB:      //0xEB�ķ������ݰ�Ϊģ��GUI����������Ϣ
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
