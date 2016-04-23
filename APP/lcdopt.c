#include "rtc.h"
#include "usart.h"
#include "lcdopt.h"
#include "fileopt.h"
#include "EzUI_Ctrl.h"
#include "CH376_api.h"


#define CONTROL_NUM  44

void lcdopt_click_bootui_icon_work(void);
void lcdopt_click_bootui_icon_admin(void);
void lcdopt_click_work_button_goods_choose(void);
void lcdopt_click_work_button_print(void);
void lcdopt_click_work_button_flay(void);
void lcdopt_click_work_button_zero(void);
void lcdopt_click_work_button_total(void);
void lcdopt_click_work_button_quit(void);
void lcdopt_click_work_frame_print_sure(void);
void lcdopt_click_goods_opt_button_choose(void);
void lcdopt_click_goods_opt_button_return(void);
void lcdopt_click_admin_button_sure(void);
void lcdopt_click_admin_button_return(void);
void lcdopt_click_boot_admin_ui_icon_add_card(void);
void lcdopt_click_boot_admin_ui_icon_del_card(void);
void lcdopt_click_boot_admin_ui_icon_goods(void);
void lcdopt_click_boot_admin_ui_icon_setting(void);
void lcdopt_click_boot_admin_ui_icon_root_setting(void);
void lcdopt_click_boot_admin_ui_icon_pwd_manage(void);
void lcdopt_click_boot_admin_ui_icon_return(void);
void lcdopt_click_add_card_button_add(void);
void lcdopt_click_add_card_button_readd(void);
void lcdopt_click_add_card_button_return(void);
void lcdopt_click_add_card_control_worker_num(void);
void lcdopt_click_del_card_control_worker_num(void);
void lcdopt_click_del_card_button_del(void);
void lcdopt_click_del_card_button_return(void);
void lcdopt_click_goods_button_sure(void);
void lcdopt_click_goods_button_cancle(void);
void lcdopt_click_goods_button_find(void);
void lcdopt_click_goods_button_return(void);
void lcdopt_click_goods_del_button_del(void);
void lcdopt_click_goods_del_button_return(void);
void lcdopt_click_setting_button_sure(void);
void lcdopt_click_setting_button_restore(void);
void lcdopt_click_setting_button_return(void);
void lcdopt_click_root_auth_button_sure(void);
void lcdopt_click_root_auth_button_return(void);
void lcdopt_click_root_opt_button_sure(void);
void lcdopt_click_root_opt_button_list_export(void);
void lcdopt_click_root_opt_button_bill_export(void);
void lcdopt_click_root_opt_button_quit(void);
void lcdopt_click_pwd_manage_button_sure(void);
void lcdopt_click_pwd_manage_button_return(void);


typedef struct
{
    u16 control_id;
    void (*CONTROL_CALLBACK_FUNC) (void);
}lcdopt_control_opt_str;

enum
{
    LCDOPT_BOOL_TRUE   = 0,
    LCDOPT_BOOL_FALSE = 1,
};

enum
{
    LCDOPT_GONGJIN = 0,
    LCDOPT_JIN = 1,
};

enum
{
    LCDOPT_GOODS_TRUE  = 0,
	LCDOPT_GOODS_FALSE = 1,
};

lcdopt_control_opt_str opt_str[CONTROL_NUM] = {
{BOOT_ICON_WORK,              lcdopt_click_bootui_icon_work},
{BOOT_ICON_ADMIN,			  lcdopt_click_bootui_icon_admin},
{WORK_BUTTON_GOODS_CHOOSE,	  lcdopt_click_work_button_goods_choose},
{WORK_BUTTON_PRINT, 		  lcdopt_click_work_button_print},
{WORK_BUTTON_FLAY,			  lcdopt_click_work_button_flay},
{WORK_BUTTON_ZERO,			  lcdopt_click_work_button_zero},
{WORK_BUTTON_TOTAL, 		  lcdopt_click_work_button_total},
{WORK_BUTTON_QUIT,			  lcdopt_click_work_button_quit},
{BALL_FRAME_PRINT_SURE,       lcdopt_click_work_frame_print_sure},
{GOODS_OPT_BUTTON_CHOOSE,	  lcdopt_click_goods_opt_button_choose},
{GOODS_OPT_BUTTON_RETURN,	  lcdopt_click_goods_opt_button_return},
{ADMIN_BUTTON_SURE, 		  lcdopt_click_admin_button_sure},
{ADMIN_BUTTON_RETURN,		  lcdopt_click_admin_button_return},
{BOOT_ADMIN_ICON_ADD_CARD,    lcdopt_click_boot_admin_ui_icon_add_card},
{BOOT_ADMIN_ICON_DEL_CARD,    lcdopt_click_boot_admin_ui_icon_del_card},
{BOOT_ADMIN_ICON_ADD_GOODS,   lcdopt_click_boot_admin_ui_icon_goods},
{BOOT_ADMIN_ICON_SETTING,     lcdopt_click_boot_admin_ui_icon_setting},
{BOOT_ADMIN_ICON_ROOT_SETTING,lcdopt_click_boot_admin_ui_icon_root_setting},
{BOOT_ADMIN_ICON_PWD_MANAGE,  lcdopt_click_boot_admin_ui_icon_pwd_manage},
{BOOT_ADMIN_BUTTON_RETURN,    lcdopt_click_boot_admin_ui_icon_return},
{CARD_ADD_BUTTON_ADD,		  lcdopt_click_add_card_button_add},
{CARD_ADD_BUTTON_READD,       lcdopt_click_add_card_button_readd},
{CARD_ADD_BUTTON_RETURN,	  lcdopt_click_add_card_button_return},
{CARD_ADD_CONTROL_WORKER_NUM, lcdopt_click_add_card_control_worker_num},
{CARD_DEL_CONTROL_WORKER_NUM, lcdopt_click_del_card_control_worker_num},
{CARD_DEL_BUTTON_DEL,		  lcdopt_click_del_card_button_del},
{CARD_DEL_BUTTON_RETURN,	  lcdopt_click_del_card_button_return},
{GOODS_ADD_BUTTON_SURE, 	  lcdopt_click_goods_button_sure},
{GOODS_ADD_BUTTON_CANCLE,	  lcdopt_click_goods_button_cancle},
{GOODS_ADD_BUTTON_LOOK, 	  lcdopt_click_goods_button_find},
{GOODS_ADD_BUTTON_RETURN,	  lcdopt_click_goods_button_return},
{GOODS_DEL_BUTTON_DEL,        lcdopt_click_goods_del_button_del},
{GOODS_DEL_BUTTON_RETURN,     lcdopt_click_goods_del_button_return},
{SETTING_BUTTON_SURE,         lcdopt_click_setting_button_sure},
{SETTING_BUTTON_DATA_RESTORE, lcdopt_click_setting_button_restore},
{SETTING_BUTTON_RETURN,       lcdopt_click_setting_button_return},
{ROOT_AUTH_BUTTON_SURE,       lcdopt_click_root_auth_button_sure},
{ROOT_AUTH_BUTTON_RETURN,     lcdopt_click_root_auth_button_return},
{ROOT_OPT_BUTTON_SURE,        lcdopt_click_root_opt_button_sure},
{ROOT_OPT_BUTTON_LIST_EXPORT, lcdopt_click_root_opt_button_list_export},
{ROOT_OPT_BUTTON_BILL_EXPORT, lcdopt_click_root_opt_button_bill_export},
{ROOT_OPT_BUTTON_QUIT,	      lcdopt_click_root_opt_button_quit},
{PWD_MANAGE_BUTTON_SURE,      lcdopt_click_pwd_manage_button_sure},
{PWD_MANAGE_BUTTON_RETURN,    lcdopt_click_pwd_manage_button_return},
};

lcdopt_mgr_t lcdopt_mgr_str;
extern u16 USART_RX_STA;
extern _calendar_obj calendar;


void lcdopt_show_del_goods_list()
{
	int i = 0;
	int j = 0;
	goods_list_t *list;

	list = (goods_list_t *) fileopt_get_goods_list();

	for (i=0; i < list->total_num; i++)
	{
        while((j < 4) && (list->goods[j].price == 0))
       	{
			j++;
		}
		EzUI_StringEdit_SetString(GOODS_DEL_CONTROL_GOODS_TYPE_1+i,list->goods[j].goods_name);
		EzUI_NumberEdit_SetVarFloat(GOODS_DEL_CONTROL_GOODS_UNIT_PRICE_1+i,list->goods[j].price);
		j++;
	}
}

void lcdopt_show_choose_goods_list()
{
	int i = 0;
	int j = 0;
	goods_list_t *list;

	list = (goods_list_t *) fileopt_get_goods_list();

	for (i=0; i < list->total_num; i++)
	{
        while((j < 4) && (list->goods[j].price == 0))
       	{
			j++;
		}
		EzUI_StringEdit_SetString(GOODS_OPT_CONTROL_GOODS_TYPE_1+i,list->goods[j].goods_name);
		EzUI_NumberEdit_SetVarFloat(GOODS_OPT_CONTROL_GOODS_UNIT_PRICE_1+i,list->goods[j].price);
		j++;
	}
}


void lcdopt_clear_del_goods_list()
{
	EzUI_StringEdit_SetString(GOODS_DEL_CONTROL_GOODS_TYPE_1," ");
	EzUI_StringEdit_SetString(GOODS_DEL_CONTROL_GOODS_TYPE_2," ");
	EzUI_StringEdit_SetString(GOODS_DEL_CONTROL_GOODS_TYPE_3," ");
	EzUI_StringEdit_SetString(GOODS_DEL_CONTROL_GOODS_TYPE_4," ");
	EzUI_NumberEdit_SetVarFloat(GOODS_DEL_CONTROL_GOODS_UNIT_PRICE_1,0.00);
	EzUI_NumberEdit_SetVarFloat(GOODS_DEL_CONTROL_GOODS_UNIT_PRICE_2,0.00);
	EzUI_NumberEdit_SetVarFloat(GOODS_DEL_CONTROL_GOODS_UNIT_PRICE_3,0.00);
	EzUI_NumberEdit_SetVarFloat(GOODS_DEL_CONTROL_GOODS_UNIT_PRICE_4,0.00);
	EzUI_Combox_SetSel(GOODS_DEL_CONTROL_GOODS_DEL_FLAG_1,LCDOPT_GOODS_FALSE);
	EzUI_Combox_SetSel(GOODS_DEL_CONTROL_GOODS_DEL_FLAG_2,LCDOPT_GOODS_FALSE);
	EzUI_Combox_SetSel(GOODS_DEL_CONTROL_GOODS_DEL_FLAG_3,LCDOPT_GOODS_FALSE);
	EzUI_Combox_SetSel(GOODS_DEL_CONTROL_GOODS_DEL_FLAG_4,LCDOPT_GOODS_FALSE);
}

void lcdopt_clear_choose_goods_list()
{
	EzUI_StringEdit_SetString(GOODS_OPT_CONTROL_GOODS_TYPE_1," ");
	EzUI_StringEdit_SetString(GOODS_OPT_CONTROL_GOODS_TYPE_2," ");
	EzUI_StringEdit_SetString(GOODS_OPT_CONTROL_GOODS_TYPE_3," ");
	EzUI_StringEdit_SetString(GOODS_OPT_CONTROL_GOODS_TYPE_4," ");
	EzUI_NumberEdit_SetVarFloat(GOODS_OPT_CONTROL_GOODS_UNIT_PRICE_1,0.00);
	EzUI_NumberEdit_SetVarFloat(GOODS_OPT_CONTROL_GOODS_UNIT_PRICE_2,0.00);
	EzUI_NumberEdit_SetVarFloat(GOODS_OPT_CONTROL_GOODS_UNIT_PRICE_3,0.00);
	EzUI_NumberEdit_SetVarFloat(GOODS_OPT_CONTROL_GOODS_UNIT_PRICE_4,0.00);
	EzUI_Combox_SetSel(GOODS_OPT_CONTROL_GOODS_CHOOSE_FLAG_1,LCDOPT_GOODS_FALSE);
	EzUI_Combox_SetSel(GOODS_OPT_CONTROL_GOODS_CHOOSE_FLAG_2,LCDOPT_GOODS_FALSE);
	EzUI_Combox_SetSel(GOODS_OPT_CONTROL_GOODS_CHOOSE_FLAG_3,LCDOPT_GOODS_FALSE);
	EzUI_Combox_SetSel(GOODS_OPT_CONTROL_GOODS_CHOOSE_FLAG_4,LCDOPT_GOODS_FALSE);
}


void lcdopt_del_goods_flag_check()
{
	int i = 0;
	char sel_buf = 0;

	while(i < 4)
	{
    	EzUI_Combox_ReadSel(GOODS_DEL_CONTROL_GOODS_DEL_FLAG_1+i,&sel_buf);
		if (sel_buf == LCDOPT_GOODS_TRUE)
		{
			lcdopt_mgr_str.del_goods_flag[lcdopt_mgr_str.del_goods_num] = GOODS_DEL_CONTROL_GOODS_DEL_FLAG_1 + i;
			lcdopt_mgr_str.del_goods_num ++;
		}
		i++;
	}
}

void lcdopt_choose_goods_flag_check()
{
	int i = 0;
	char sel_buf = 0;

	while(i < 4)
	{
    	EzUI_Combox_ReadSel(GOODS_OPT_CONTROL_GOODS_CHOOSE_FLAG_1+i,&sel_buf);
		if (sel_buf == LCDOPT_GOODS_TRUE)
		{
			lcdopt_mgr_str.choose_goods_num ++;
			lcdopt_mgr_str.choose_goods_flag = GOODS_OPT_CONTROL_GOODS_CHOOSE_FLAG_1 + i;
		}
		i++;
	}
}


/*boot ui icon click callback func*/
void lcdopt_click_bootui_icon_work(void)
{
    EzUI_StringEdit_SetString(WORK_CONTROL_WEIGHT_UNIT,lcdopt_mgr_str.weight_unit);
    EzUI_StringEdit_SetString(WORK_CONTROL_GOODS,lcdopt_mgr_str.cur_goods);
    EzUI_NumberEdit_SetVarFloat(WORK_CONTROL_UNIT_PRICE,lcdopt_mgr_str.cur_unit_price);
	UART_DMA_Tx_Enable();
    lcdopt_mgr_str.cur_icon_index = WORK_UI_INDEX;
}

void lcdopt_click_bootui_icon_admin(void)
{
    lcdopt_mgr_str.cur_icon_index = ADMIN_ENTER_UI_INDEX;
}

/*work ui opt callback func*/

void lcdopt_click_work_button_goods_choose(void)
{
    lcdopt_show_choose_goods_list();
    lcdopt_mgr_str.cur_icon_index = GOODS_OPT_UI_INDEX;
	UART_DMA_Tx_Enable();
}

void lcdopt_click_work_button_print(void)
{
    int i = 0;
	int j = 0;
	unsigned char nopaper[5] = {0x1B,0x76,0x00};


	while(j<5)
    {
		for(i = 0; i< 3; i++)
		{
		    USART2->DR=nopaper[i];
		    while((USART2->SR&0X40)==0);
		}
		j++;
	}


	if (!USART_RX_STA) 
	{
	    EzUI_SetNowActiveGui(PRINT_WARNING_INDEX);
	}
	else 
	{
		EzUI_SetNowActiveGui(NO_PAPER_WARNING_INDEX);
		USART_RX_STA = 0;
	}
	UART_DMA_Tx_Enable();
}

void lcdopt_click_work_button_flay(void)
{
    lcdopt_mgr_str.ad_show_pi = Weight_QuPi();
}

void lcdopt_click_work_button_zero(void)
{
	EzUI_StringEdit_SetString(WORK_CONTROL_WORKER_NAME," ");
	EzUI_NumberEdit_SetVarInt(WORK_CONTROL_WORKER_NUM,0);
	UART_DMA_Tx_Enable();
}

void lcdopt_click_work_button_total(void)
{
    
}

void lcdopt_click_work_button_quit(void)
{
    EzUI_NumberEdit_SetVarFloat(WORK_CONTROL_WEIGHT_VAL,0.00);
	EzUI_NumberEdit_SetVarFloat(WORK_CONTROL_TOTAL_PRICE,0.00);
	EzUI_StringEdit_SetString(WORK_CONTROL_WORKER_NAME," ");
	EzUI_NumberEdit_SetVarInt(WORK_CONTROL_WORKER_NUM,0);
	UART_DMA_Tx_Enable();
}

void lcdopt_click_work_frame_print_sure(void)
{
    int i = 0; 
	int len = 0;
	char paste_buf[20] = {0};

	sprintf(paste_buf," 智能称\n");
    len = strlen(paste_buf);
	for(i=0;i<len;i++)
    {
        USART2->DR=paste_buf[i];
	    while((USART2->SR&0X40)==0);
    } 
}

/*goods opt ui callback func*/
void lcdopt_click_goods_opt_button_choose(void)
{
    int i = 0;
	float cur_price = 0.0;
	char goods_name[10] = {0};
	unsigned char len = 0;

	lcdopt_choose_goods_flag_check();

	if (lcdopt_mgr_str.choose_goods_num != 1)
   	{
		EzUI_SetNowActiveGui(OPT_FAIL_WARNING_INDEX);
		return ;
	}

	while (i < 4)
	{
		if (lcdopt_mgr_str.choose_goods_flag == GOODS_OPT_CONTROL_GOODS_CHOOSE_FLAG_1 + i)
		{
			EzUI_StringEdit_ReadString(GOODS_OPT_CONTROL_GOODS_TYPE_1 +i, goods_name, &len);
			EzUI_NumberEdit_ReadVarFloat(GOODS_OPT_CONTROL_GOODS_UNIT_PRICE_1+i, &cur_price);
			strcpy(lcdopt_mgr_str.cur_goods, goods_name);
			lcdopt_mgr_str.cur_unit_price = cur_price;
			EzUI_SetNowActiveGui(OPT_OK_WARNING_INDEX);
			break;
		}
		i++;
	}
	lcdopt_mgr_str.choose_goods_num = 0;
	lcdopt_mgr_str.choose_goods_flag = 0;
	UART_DMA_Tx_Enable();
}

void lcdopt_click_goods_opt_button_return(void)
{
    lcdopt_clear_choose_goods_list();
	EzUI_StringEdit_SetString(WORK_CONTROL_GOODS,lcdopt_mgr_str.cur_goods);
    EzUI_NumberEdit_SetVarFloat(WORK_CONTROL_UNIT_PRICE,lcdopt_mgr_str.cur_unit_price);
    lcdopt_mgr_str.cur_icon_index = WORK_UI_INDEX;
	lcdopt_mgr_str.choose_goods_flag = 0;
	lcdopt_mgr_str.choose_goods_num = 0;
	UART_DMA_Tx_Enable();
}

/*admin enter ui callback func*/
void lcdopt_click_admin_button_sure(void)
{
	char buf[5] = {0};
    unsigned char len = 0;
	unsigned char auth_buf[20] = {0};
	unsigned char admin_passwd_buf[20] = {0};
	unsigned char root_passwd_buf[20] = {0};

    fileopt_get_passwd(admin_passwd_buf, 20, ADMIN_PASSWD);
    fileopt_get_passwd(root_passwd_buf, 20, ROOT_PASSWD);

    EzUI_StringEdit_ReadString(ADMIN_CONTROL_AUTH, auth_buf, &len);
	EzUI_StringEdit_SetString(ADMIN_CONTROL_AUTH, buf);
	
	if (!strcmp(auth_buf, admin_passwd_buf)) {
	    lcdopt_mgr_str.user = LCDOPT_ADMIN;
	    lcdopt_mgr_str.cur_icon_index = ADMIN_BOOT_UI_INDEX;
		EzUI_SetNowActiveGui(ADMIN_BOOT_UI_INDEX);
	}
	else if(!strcmp(auth_buf,root_passwd_buf)) {
	    lcdopt_mgr_str.user = LCDOPT_ROOT;
	    lcdopt_mgr_str.cur_icon_index = ADMIN_BOOT_UI_INDEX;
		EzUI_SetNowActiveGui(ADMIN_BOOT_UI_INDEX);
	}
	else
	{
        EzUI_SetNowActiveGui(PWD_ERR_WARNING_INDEX);
	}

	
	UART_DMA_Tx_Enable();
}

void lcdopt_click_admin_button_return(void)
{
	char buf[5] = {0};
	EzUI_StringEdit_SetString(ADMIN_CONTROL_AUTH, buf);
	UART_DMA_Tx_Enable();
	lcdopt_mgr_str.cur_icon_index = BOOT_UI_INDEX;
}

/*boot admin ui icon callback func*/
void lcdopt_click_boot_admin_ui_icon_add_card(void)
{
    lcdopt_mgr_str.cur_worker_num = fileopt_get_work_id();
    EzUI_NumberEdit_SetVarInt(CARD_ADD_CONTROL_WORKER_NUM,lcdopt_mgr_str.cur_worker_num);
    lcdopt_mgr_str.cur_icon_index = ADD_CARD_UI_INDEX;
}

void lcdopt_click_boot_admin_ui_icon_del_card(void)
{
    lcdopt_mgr_str.cur_icon_index = DEL_CARD_UI_INDEX;
}

void lcdopt_click_boot_admin_ui_icon_goods(void)
{
    lcdopt_mgr_str.cur_icon_index = GOODS_ADD_UI_INDEX;
}

void lcdopt_click_boot_admin_ui_icon_setting(void)
{
	int s_year = 0;
	int s_month = 0;
	int s_date = 0;
	int s_hour = 0;
	int s_min = 0;
	int s_sec = 0;
	
	if (RTC_Get() == 0)
	{	
		s_year  = (int) calendar.w_year;
		s_month = (int) calendar.w_month;
		s_date  = (int) calendar.w_date;
		s_hour  = (int) calendar.hour;
		s_min   = (int) calendar.min;
		s_sec   = (int) calendar.sec;	
	}
	
	EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_YEAR,   s_year);
	EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_MONTH,  s_month);
	EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_DAY,    s_date);
	EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_HOURS,  s_hour);
	EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_MINUTE, s_min);
	EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_SECOND, s_sec);
    lcdopt_mgr_str.cur_icon_index = SETTING_UI_INDEX;

	UART_DMA_Tx_Enable();
}

void lcdopt_click_boot_admin_ui_icon_root_setting(void)
{
    if (lcdopt_mgr_str.user == LCDOPT_ADMIN) {
        lcdopt_mgr_str.cur_icon_index = ADMIN_ENTER_UI_2_INDEX;
        EzUI_SetNowActiveGui(ADMIN_ENTER_UI_2_INDEX);
	} else if (lcdopt_mgr_str.user == LCDOPT_ROOT) {
		float water_rate = 0.0;

		fileopt_get_water_rate(&water_rate);
		EzUI_NumberEdit_SetVarFloat(ROOT_OPT_CONTROL_RATE,water_rate);
        lcdopt_mgr_str.cur_icon_index = ROOT_OPT_UI_INDEX;
        EzUI_SetNowActiveGui(ROOT_OPT_UI_INDEX);
	}

	UART_DMA_Tx_Enable();
}

void lcdopt_click_boot_admin_ui_icon_pwd_manage(void)
{
    lcdopt_mgr_str.cur_icon_index = PWD_MANAGE_UI_INDEX;
}

void lcdopt_click_boot_admin_ui_icon_return(void)
{
    lcdopt_mgr_str.cur_icon_index = BOOT_UI_INDEX;
	lcdopt_mgr_str.user == LCDOPT_NONE;
}

/*add card ui opt callback func*/
void lcdopt_click_add_card_button_add(void)
{
    unsigned char ret = 0;
    unsigned char worker_name[20] = {0};
	unsigned char card_id[20] = {0};
	unsigned char worker_name_len = 0;
	unsigned char card_id_len = 0;

    EzUI_StringEdit_ReadString(CARD_ADD_CONTROL_WORKER_NAME, worker_name, &worker_name_len);
	EzUI_StringEdit_ReadString(CARD_ADD_CONTROL_CARD_ID, card_id, &card_id_len);

	if (!strcmp(card_id, " ")) {
		EzUI_SetNowActiveGui(ADD_FAIL_WARNING_INDEX);
	} else if (fileopt_create_card(card_id,worker_name)) {
        EzUI_SetNowActiveGui(ADD_FAIL_WARNING_INDEX);
	} else {
        EzUI_SetNowActiveGui(ADD_OK_WARNING_INDEX);
	}

	lcdopt_mgr_str.cur_worker_num = fileopt_get_work_id();
	EzUI_StringEdit_SetString(CARD_ADD_CONTROL_WORKER_NAME," ");
    EzUI_NumberEdit_SetVarInt(CARD_ADD_CONTROL_WORKER_NUM,lcdopt_mgr_str.cur_worker_num);
	EzUI_StringEdit_SetString(CARD_ADD_CONTROL_CARD_ID," ");
	 
	UART_DMA_Tx_Enable();
}

void lcdopt_click_add_card_button_readd(void)
{
    int worker_num = 0;
	char worker_id[10] = {0};
	unsigned char card_id[20] = {0};
	unsigned char card_id_len = 0;

	EzUI_NumberEdit_ReadVarInt(CARD_ADD_CONTROL_WORKER_NUM,&worker_num);
	EzUI_StringEdit_ReadString(CARD_ADD_CONTROL_CARD_ID, card_id, &card_id_len);
	
    if (worker_num) {
		sprintf(worker_id,"%d", worker_num);
		if (!strcmp(card_id, " ")) {
			EzUI_SetNowActiveGui(ADD_FAIL_WARNING_INDEX);
		} else if (fileopt_reapply_card_by_work_id(worker_id, card_id)) {
			EzUI_SetNowActiveGui(OPT_FAIL_WARNING_INDEX);
		} else {
			EzUI_SetNowActiveGui(OPT_OK_WARNING_INDEX);
		}
	}

	EzUI_StringEdit_SetString(CARD_ADD_CONTROL_WORKER_NAME," ");
	EzUI_StringEdit_SetString(CARD_ADD_CONTROL_CARD_ID," ");
    EzUI_NumberEdit_SetVarInt(CARD_ADD_CONTROL_WORKER_NUM,lcdopt_mgr_str.cur_worker_num);
	UART_DMA_Tx_Enable();
}

void lcdopt_click_add_card_button_return(void)
{
    EzUI_StringEdit_SetString(CARD_ADD_CONTROL_WORKER_NAME," ");
	EzUI_StringEdit_SetString(CARD_ADD_CONTROL_CARD_ID," ");
	UART_DMA_Tx_Enable();
	lcdopt_mgr_str.cur_icon_index = ADMIN_BOOT_UI_INDEX;
}

void lcdopt_click_add_card_control_worker_num(void)
{
	int worker_num = 0;
	char worker_id[20] = {0};
	char worker_name[20] = {0};
	char card_id[20] = {0};

	EzUI_NumberEdit_ReadVarInt(CARD_ADD_CONTROL_WORKER_NUM,&worker_num);
	sprintf(worker_id, "%d", worker_num);
	fileopt_find_worker_msg_by_workid(worker_id, worker_name, 20, card_id, 20);

	if (worker_name[0] != 0) {
		EzUI_StringEdit_SetString(CARD_ADD_CONTROL_WORKER_NAME,worker_name);
	} else {
		EzUI_NumberEdit_SetVarInt(CARD_ADD_CONTROL_WORKER_NUM,lcdopt_mgr_str.cur_worker_num);
		EzUI_SetNowActiveGui(OPT_FAIL_WARNING_INDEX);
	}

	UART_DMA_Tx_Enable();
}

/*del card ui opt callback func*/
void lcdopt_click_del_card_control_worker_num(void)
{
	int worker_num = 0;
    char worker_name[20] = {0};
	char card_id[20] = {0};
    char worker_id[20] = {0};

	EzUI_NumberEdit_ReadVarInt(CARD_DEL_CONTROL_WORKER_NUM,&worker_num);
	sprintf(worker_id, "%d", worker_num);
	fileopt_find_worker_msg_by_workid(worker_id, worker_name, 20, card_id, 20);
	if (worker_name[0] != 0 && card_id[0] != 0) {
    	EzUI_StringEdit_SetString(CARD_DEL_CONTROL_WORKER_NAME, worker_name);
		EzUI_StringEdit_SetString(CARD_DEL_CONTROL_CARD_ID, card_id);
	} else {
		EzUI_StringEdit_SetString(CARD_DEL_CONTROL_WORKER_NAME," ");
		EzUI_NumberEdit_SetVarInt(CARD_DEL_CONTROL_WORKER_NUM,0);
		EzUI_StringEdit_SetString(CARD_DEL_CONTROL_CARD_ID," ");
		EzUI_SetNowActiveGui(DEL_FAIL_WARNING_INDEX);
	}
	UART_DMA_Tx_Enable();
}

void lcdopt_click_del_card_button_del(void)
{
    unsigned char id[20] = {0};
	unsigned char len = 0;

    EzUI_StringEdit_ReadString(CARD_DEL_CONTROL_CARD_ID, id, &len);

	if (fileopt_del_worker_msg(id)) {
		EzUI_SetNowActiveGui(DEL_FAIL_WARNING_INDEX);
	} else {
	    EzUI_StringEdit_SetString(CARD_DEL_CONTROL_WORKER_NAME," ");
		EzUI_NumberEdit_SetVarInt(CARD_DEL_CONTROL_WORKER_NUM,0);
		EzUI_StringEdit_SetString(CARD_DEL_CONTROL_CARD_ID," ");
		EzUI_SetNowActiveGui(DEL_OK_WARNING_INDEX);
	}
	
	UART_DMA_Tx_Enable();
}

void lcdopt_click_del_card_button_return(void)
{
    EzUI_StringEdit_SetString(CARD_DEL_CONTROL_WORKER_NAME," ");
	EzUI_NumberEdit_SetVarInt(CARD_DEL_CONTROL_WORKER_NUM,0);
	EzUI_StringEdit_SetString(CARD_DEL_CONTROL_CARD_ID," ");
	UART_DMA_Tx_Enable();
	lcdopt_mgr_str.cur_icon_index = ADMIN_BOOT_UI_INDEX;
}

/*add goods ui opt callback func*/
void lcdopt_click_goods_button_sure(void)
{
    float unit_price = 0.0;
    char goods_name[10] = {0};
	unsigned char len = 0;
	
    EzUI_StringEdit_ReadString(GOODS_ADD_CONTROL_TYPE, goods_name, &len);
    EzUI_NumberEdit_ReadVarFloat(GOODS_ADD_CONTROL_UNIT_PRICE, &unit_price);

    if (fileopt_add_goods(goods_name, &unit_price))
   	{
		EzUI_SetNowActiveGui(OPT_FAIL_WARNING_INDEX);
	}
	else
	{
		EzUI_SetNowActiveGui(OPT_OK_WARNING_INDEX);
	}

	EzUI_NumberEdit_SetVarFloat(GOODS_ADD_CONTROL_UNIT_PRICE,0.00);
	EzUI_StringEdit_SetString(GOODS_ADD_CONTROL_TYPE," ");

    UART_DMA_Tx_Enable();
}

void lcdopt_click_goods_button_cancle(void)
{
    EzUI_NumberEdit_SetVarFloat(GOODS_ADD_CONTROL_UNIT_PRICE,0.00);
	EzUI_StringEdit_SetString(GOODS_ADD_CONTROL_TYPE," ");
	UART_DMA_Tx_Enable();
}

void lcdopt_click_goods_button_find(void)
{
    EzUI_NumberEdit_SetVarFloat(GOODS_ADD_CONTROL_UNIT_PRICE,0.00);
	EzUI_StringEdit_SetString(GOODS_ADD_CONTROL_TYPE," ");
	lcdopt_show_del_goods_list();
	UART_DMA_Tx_Enable();
	lcdopt_mgr_str.cur_icon_index = GOODS_DEL_UI_INDEX;
}

void lcdopt_click_goods_button_return(void)
{
    EzUI_NumberEdit_SetVarFloat(GOODS_ADD_CONTROL_UNIT_PRICE,0.00);
	EzUI_StringEdit_SetString(GOODS_ADD_CONTROL_TYPE," ");
	UART_DMA_Tx_Enable();
}

/*del goods ui opt callback func*/
void lcdopt_click_goods_del_button_del(void)
{
    int i = 0;
	int j = 0;
	char goods_name[10] = {0};
	unsigned char len = 0;

	lcdopt_del_goods_flag_check();

	if (lcdopt_mgr_str.del_goods_num == 0) {

		EzUI_SetNowActiveGui(OPT_FAIL_WARNING_INDEX);
		goto TAG_OUT;
	}

	while (i < 4)
	{
		j = 0;
		while (j < lcdopt_mgr_str.del_goods_num) {
			if (lcdopt_mgr_str.del_goods_flag[j] == GOODS_DEL_CONTROL_GOODS_DEL_FLAG_1 + i)
			{
				lcdopt_mgr_str.del_goods_flag[j] = 0;
				EzUI_StringEdit_ReadString(GOODS_DEL_CONTROL_GOODS_TYPE_1 +i, goods_name, &len);
				fileopt_del_goods(goods_name);
				if (!strcmp(lcdopt_mgr_str.cur_goods, goods_name)) {
					strcpy(lcdopt_mgr_str.cur_goods, " ");
					lcdopt_mgr_str.cur_unit_price = 0;
				}
				
				break;
			}
			j++;
		}
		i++;
	}
	
	EzUI_SetNowActiveGui(OPT_OK_WARNING_INDEX);
	lcdopt_clear_del_goods_list();
	lcdopt_show_del_goods_list();
	lcdopt_mgr_str.del_goods_num = 0;
	
TAG_OUT:
	UART_DMA_Tx_Enable();
}

void lcdopt_click_goods_del_button_return(void)
{
	int i = 0;
	
	lcdopt_clear_del_goods_list();

	for (i = 0; i < lcdopt_mgr_str.del_goods_num; i++) {	
		lcdopt_mgr_str.del_goods_flag[i] = 0;
	}
	lcdopt_mgr_str.del_goods_num = 0;
	lcdopt_mgr_str.cur_icon_index = GOODS_ADD_UI_INDEX;
}

/*setting ui opt callback func*/
void lcdopt_click_setting_button_sure(void)
{
    int year   = 0;
	int month  = 0;
	int day    = 0;
	int hours  = 0;
	int minute = 0;
	int second = 0;
	unsigned char len = 0;
	char sel_buf = 0;

	EzUI_Combox_ReadSel(SETTING_CONTROL_WEIGHT_UNIT,&sel_buf);

	EzUI_NumberEdit_ReadVarInt(SETTING_CONTROL_TIME_YEAR,&year);
	EzUI_NumberEdit_ReadVarInt(SETTING_CONTROL_TIME_MONTH,&month);
	EzUI_NumberEdit_ReadVarInt(SETTING_CONTROL_TIME_DAY,&day);
	EzUI_NumberEdit_ReadVarInt(SETTING_CONTROL_TIME_HOURS,&hours);
	EzUI_NumberEdit_ReadVarInt(SETTING_CONTROL_TIME_MINUTE,&minute);
	EzUI_NumberEdit_ReadVarInt(SETTING_CONTROL_TIME_SECOND,&second);
    RTC_Set((u16)year,(u8)month,(u8)day,(u8)hours,(u8)minute,(u8)second);    

	if (sel_buf == LCDOPT_GONGJIN) {
		strcpy(lcdopt_mgr_str.weight_unit, "公斤");
	} else if (sel_buf == LCDOPT_JIN) {
		strcpy(lcdopt_mgr_str.weight_unit, "市斤");
	}

	EzUI_SetNowActiveGui(OPT_OK_WARNING_INDEX);
    UART_DMA_Tx_Enable();
}

void lcdopt_click_setting_button_restore(void)
{
	fileopt_copy_flash_to_sd();
    EzUI_SetNowActiveGui(OPT_OK_WARNING_INDEX);
	UART_DMA_Tx_Enable();
}

void lcdopt_click_setting_button_return(void)
{
  /*EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_YEAR,0);
	EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_MONTH,0);
	EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_DAY,0);
	EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_HOURS,0);
	EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_MINUTE,0);
	EzUI_NumberEdit_SetVarInt(SETTING_CONTROL_TIME_SECOND,0);*/
	//UART_DMA_Tx_Enable();
	lcdopt_mgr_str.cur_icon_index = ADMIN_BOOT_UI_INDEX;
}

/*root auth ui opt callback func*/
void lcdopt_click_root_auth_button_sure(void)
{
    unsigned char len = 0;
	unsigned char auth_buf[20] = {0};
	unsigned char passwd[20] = {0};

    EzUI_StringEdit_ReadString(ROOT_AUTH_CONTROL_AUTH, auth_buf, &len);
    fileopt_get_passwd (passwd, 20, ROOT_PASSWD);

	if (!strcmp(auth_buf, passwd))
	{
		float water_rate = 0.0;
		fileopt_get_water_rate(&water_rate);
		EzUI_NumberEdit_SetVarFloat(ROOT_OPT_CONTROL_RATE,water_rate);
	    EzUI_SetNowActiveGui(ROOT_OPT_UI_INDEX);
	    lcdopt_mgr_str.cur_icon_index = ROOT_OPT_UI_INDEX;
	}
	else
	{
        EzUI_SetNowActiveGui(PWD_ERR_WARNING_INDEX);
	}

	EzUI_StringEdit_SetString(ROOT_AUTH_CONTROL_AUTH," ");
	UART_DMA_Tx_Enable();
}
void lcdopt_click_root_auth_button_return(void)
{
    EzUI_StringEdit_SetString(ROOT_AUTH_CONTROL_AUTH," ");
	UART_DMA_Tx_Enable();
	lcdopt_mgr_str.cur_icon_index = ADMIN_BOOT_UI_INDEX;
}

/*root opt ui callback func*/

void lcdopt_click_root_opt_button_sure(void)
{
    float rate;
	
    EzUI_NumberEdit_ReadVarFloat(ROOT_OPT_CONTROL_RATE, &rate);
	fileopt_set_water_rate(&rate);
    UART_DMA_Tx_Enable();
	lcdopt_mgr_str.cur_rate = rate;

}

void lcdopt_click_root_opt_button_list_export(void)
{
	unsigned char *list_name = "/名单.CSV";

	if (!InitUSB()) {
		goto TAG_OUT;
	}

	if (!OpenUSBFile((PUINT8)list_name))
	{
		CloseUSBFile();
		goto TAG_OUT;
	}

	if (fileopt_write_list_to_udev() != 0)
	{
		CloseUSBFile();
		goto TAG_OUT;
	}

	if (!CloseUSBFile()) {
		goto TAG_OUT;
	}
	WaitTakeOutUSB();
	EzUI_SetNowActiveGui(OPT_OK_WARNING_INDEX);
	UART_DMA_Tx_Enable();
	return ;
	

TAG_OUT:
	EzUI_SetNowActiveGui(OPT_FAIL_WARNING_INDEX);
	UART_DMA_Tx_Enable();
}

void lcdopt_click_root_opt_button_bill_export(void)
{
	unsigned char *bill_name = "/账单.CSV";

	if (!InitUSB()) {
		goto TAG_OUT;
	}

	if (!OpenUSBFile((PUINT8)bill_name))
	{
		CloseUSBFile();
		goto TAG_OUT;
	}

	if (fileopt_write_bill_to_udev() != 0)
	{
		CloseUSBFile();
		goto TAG_OUT;
	}

	if (!CloseUSBFile()) {
		goto TAG_OUT;
	}
	WaitTakeOutUSB();
	EzUI_SetNowActiveGui(OPT_OK_WARNING_INDEX);
	UART_DMA_Tx_Enable();
	return ;
	

TAG_OUT:
	EzUI_SetNowActiveGui(OPT_FAIL_WARNING_INDEX);
	UART_DMA_Tx_Enable();
}

void lcdopt_click_root_opt_button_quit(void)
{
    lcdopt_mgr_str.cur_icon_index = BOOT_UI_INDEX;
}

void lcdopt_click_pwd_manage_button_sure(void)
{
    unsigned char raw_pwd_len = 0;
	unsigned char new_pwd_len = 0;
	unsigned char sure_pwd_len = 0;
	unsigned char raw_pwd_buf[20] = {0};
	unsigned char new_pwd_buf[20] = {0};
	unsigned char sure_pwd_buf[20] = {0};
	unsigned char admin_pwd_buf[20] = {0};
	unsigned char root_pwd_buf[20] = {0};
	char sel_buf = 0;

    EzUI_Combox_ReadSel(PWD_MANAGE_CONTROL_ADMIN_LEVEL,&sel_buf);
	EzUI_StringEdit_ReadString(PWD_MANAGE_CONTROL_RAW_PWD, raw_pwd_buf,  &raw_pwd_len);
    EzUI_StringEdit_ReadString(PWD_MANAGE_CONTROL_NEW_PWD, new_pwd_buf,  &new_pwd_len);
	EzUI_StringEdit_ReadString(PWD_MANAGE_CONTROL_SURE_PWD, sure_pwd_buf, &sure_pwd_len);	

	if (strcmp(new_pwd_buf,sure_pwd_buf))
	{
        EzUI_SetNowActiveGui(PWD_NO_SAME_WARNING_INDEX);
        return ;
	}

    
	fileopt_get_passwd (admin_pwd_buf, 20, ADMIN_PASSWD);
    fileopt_get_passwd (root_pwd_buf, 20, ROOT_PASSWD);
	
	if (sel_buf == LCDOPT_ADMIN)
	{
		if (!strcmp(raw_pwd_buf, admin_pwd_buf) || !strcmp(raw_pwd_buf, root_pwd_buf)) 
		{
            fileopt_set_passwd (new_pwd_buf , new_pwd_len, ADMIN_PASSWD);
			EzUI_SetNowActiveGui(OPT_OK_WARNING_INDEX);
		} 
		else 
		{
            EzUI_SetNowActiveGui(OLD_PWD_ERR_WARNING_INDEX);
		}
	}
	else if (sel_buf == LCDOPT_ROOT)
	{
	  	if (!strcmp(raw_pwd_buf, root_pwd_buf)) 
		{
            fileopt_set_passwd (new_pwd_buf , new_pwd_len, ROOT_PASSWD);
			EzUI_SetNowActiveGui(OPT_OK_WARNING_INDEX);
		} 
		else 
		{
            EzUI_SetNowActiveGui(OLD_PWD_ERR_WARNING_INDEX);
		}
	}

	EzUI_StringEdit_SetString(PWD_MANAGE_CONTROL_RAW_PWD,0);
	EzUI_StringEdit_SetString(PWD_MANAGE_CONTROL_NEW_PWD," ");
	EzUI_StringEdit_SetString(PWD_MANAGE_CONTROL_SURE_PWD," ");
}

void lcdopt_click_pwd_manage_button_return(void)
{
    EzUI_StringEdit_SetString(PWD_MANAGE_CONTROL_RAW_PWD,0);
	EzUI_StringEdit_SetString(PWD_MANAGE_CONTROL_NEW_PWD," ");
	EzUI_StringEdit_SetString(PWD_MANAGE_CONTROL_SURE_PWD," ");
    lcdopt_mgr_str.cur_icon_index = ADMIN_BOOT_UI_INDEX;    
	UART_DMA_Tx_Enable();
}

int lcdopt_printf_card_id(void)
{
    char id[20] = {0};

	if (!ReadCardId(id))
	{
	    strcpy(lcdopt_mgr_str.card_id, id);
        switch(lcdopt_mgr_str.cur_icon_index)
        {
            case ADD_CARD_UI_INDEX:
			{
				EzUI_StringEdit_SetString(CARD_ADD_CONTROL_CARD_ID,id);
                UART_DMA_Tx_Enable();
				break;
            }
			case DEL_CARD_UI_INDEX:
			{
				int work_id = 0;
			    char worker_name[20] = {0};
				char worker_id[20] = {0};
				
                EzUI_StringEdit_SetString(CARD_DEL_CONTROL_CARD_ID,id);
				fileopt_find_worker_msg_by_cardid(id, worker_name, 20, worker_id, 20);

				if (worker_name[0] != 0 && worker_id[0] != 0) {
					work_id = atoi(worker_id);
			    	EzUI_StringEdit_SetString(CARD_DEL_CONTROL_WORKER_NAME,worker_name);
					EzUI_NumberEdit_SetVarInt(CARD_DEL_CONTROL_WORKER_NUM,work_id);
				} else {
					EzUI_StringEdit_SetString(CARD_DEL_CONTROL_WORKER_NAME," ");
					EzUI_NumberEdit_SetVarInt(CARD_DEL_CONTROL_WORKER_NUM,0);
					EzUI_StringEdit_SetString(CARD_DEL_CONTROL_CARD_ID," ");
					EzUI_SetNowActiveGui(DEL_FAIL_WARNING_INDEX);
				}
				
				UART_DMA_Tx_Enable();
				break;
			}
			case WORK_UI_INDEX:
			{
				char name[30] = {0};

				//fileopt_find_worker_name_by_cardid(id, name, 30);
                EzUI_StringEdit_SetString(WORK_CONTROL_WORKER_NAME,name);
				break;
			}
			case ADMIN_ENTER_UI_INDEX:
			{
				EzUI_StringEdit_SetString(ADMIN_CONTROL_AUTH,id);
                UART_DMA_Tx_Enable();
				break;
			}
			default:
				break;
	    }
	}

	return 0;
}

int lcdopt_printf_weight(void)
{
    float total_val = 0.0;
    float weight_val = 0.0;
	
    if (lcdopt_mgr_str.cur_icon_index != WORK_UI_INDEX)
    {
        return -1;
    }


	weight_val = Read_Weight_Val(lcdopt_mgr_str.ad_show_pi);
    total_val = weight_val * lcdopt_mgr_str.cur_unit_price;
	
	EzUI_NumberEdit_SetVarFloat(WORK_CONTROL_WEIGHT_VAL,weight_val);
	EzUI_NumberEdit_SetVarFloat(WORK_CONTROL_TOTAL_PRICE,total_val);
	UART_DMA_Tx_Enable();
	
	return 0;
}

void lcdopt_main(void)
{
    u8 index = 0;
    u16 id = 0;
	u8 class = 0;
	unsigned char Msg_Vall = 0;
	
	id = UART_GetWord();	     //获取发生事件的控件ID号
	class = UART_GetByte();		 //获取发生事件的控件的类型(可参考EzUI_Ctrl.h文件中的定义)
	Msg_Vall = UART_GetByte();   //获取数据包的参数1

	while (index < CONTROL_NUM)
	{
        if (opt_str[index].control_id == id)
        {
            opt_str[index].CONTROL_CALLBACK_FUNC();
			break;
		}
		index ++;
	}	
}

void lcdopt_init(void)
{
   unsigned char *weight_unit = "公斤";

   lcdopt_mgr_str.cur_worker_num = fileopt_get_work_id();
   lcdopt_mgr_str.cur_icon_index = BOOT_UI_INDEX;
   lcdopt_mgr_str.ad_show_pi = Weight_QuPi();
   lcdopt_mgr_str.cur_unit_price = 0;
   lcdopt_mgr_str.user = LCDOPT_NONE;
   fileopt_get_water_rate(&(lcdopt_mgr_str.cur_rate));
   strcpy(lcdopt_mgr_str.weight_unit, weight_unit);
}
