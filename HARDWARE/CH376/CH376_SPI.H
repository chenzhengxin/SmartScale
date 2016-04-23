#include"CH376INC.H"
#include "sys.h"
//#define	CH376_SPI_SDI			PAout(7)     /* PA7����      SDI   */
//#define	CH376_SPI_SDO			PAin(6)     /* PA6����      SDO   */
//#define	CH376_SPI_SCK			PAout(5)	    /* PA5����      SCK   */
#define  CH376_SPI_SCS			PBout(0)     /* PB0�������  CS       */
#define  CH376_RST            	PBout(1)     /* PB1�������  RST   */
#define	CH376_INT_WIRE      	PCin(5)      /* PC5��������  INT   */
/* ���ӵ�USB����״̬���� */
#define		ERR_USB_UNKNOWN		0xFA	                                                /* δ֪����,��Ӧ�÷��������,����Ӳ�����߳������ */


void 	CH376_PORT_INIT( void );  		                                                /* CH376ͨѶ�ӿڳ�ʼ�� */
void	xEndCH376Cmd( void );															/* ����SPI���� */
void 	xWriteCH376Cmd( UINT8 mCmd );	                                                /* ��CH376д���� */
void 	xWriteCH376Data( UINT8 mData );	                                                /* ��CH376д���� */
u8 	xReadCH376Data( void );			                                                /* ��CH376������ */
u8 	Query376Interrupt( void );		                                                /* ��ѯCH376�ж�(INT#����Ϊ�͵�ƽ) */
u8 	mInitCH376Host( void );			                                                /* ��ʼ��CH376 */
						  	    													  
void SPI2_Init(void);			 //��ʼ��SPI��  
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�

