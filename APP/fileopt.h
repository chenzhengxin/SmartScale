#ifndef _FILEOPT_H_
#define _FILEOPT_H_

#include <stdio.h>
#include "malloc.h"
#include "ff.h"

typedef struct total_msg {
    char lastest_total_price[10];
    char lastest_total_weight[10];
	char total_price[10];
    char total_weight[10];
}total_msg;

typedef struct goods_parm_t {
	char goods_name[32];
	float price;
}goods_parm_t;

typedef struct goods_list_t {
	goods_parm_t goods[4];
	s32 total_num;
	int init_flag;
} goods_list_t;

enum {
	ROOT_PASSWD = 0,
	ADMIN_PASSWD = 1,
	CARD_PASSWD = 2,
};

enum {
	JIN = 0,
	KILO,
};
enum {
	FILEOPT_PARM_ERR = -10,
	FILEOPT_ONFILE_ERR = -9,
	FILEOPT_MALLOC_ERR = -8,
	FILEOPT_NOT_FOUND = -7,
	FILEOPT_UPDATE_ERR = -6,
	FILEOPT_EXIST	   = -5,
	FILEOPT_OUTOF_RANGE	 = -4,
	FILEOPT_REINIT = -3,
	FILEOPT_ARRAY_FULL   = 1,
};


s32 fileopt_create_card(char *card_id, char *worker_name);

char fileopt_find_worker(char *card_id);

// char fileopt_find_worker_name_by_cardid(char *card_id, char *get_name_buf, u32 buf_len);

// s32 fileopt_add_worker_product_info(char* weight, char *total_price,char *card_id);

s32 fileopt_price_set(char *comlum, char *price);

// s32 fileopt_update_total_msg(char *card_id, float total_weight);

// s32 fileopt_read_total_msg(char *card_id, total_msg *msg);

char fileopt_price_get(char *comlum);

void set_current_price(float *now_price);

char fileopt_add_worker_msg(char *card_id, float *weight);

int fileopt_del_worker_msg(char *card_id);

s32 fileopt_get_price_comlum(char *comlum);

s32 fileopt_update_account(char *card_id);

int fileopt_set_work_goods(char *goods_name);

int fileopt_get_work_goods(char *goods_name);

int fileopt_del_work_goods(void);

int fileopt_write_bill_to_udev(void);

int fileopt_write_list_to_udev(void);

int fileopt_copy_sd_to_flash(void);

int fileopt_copy_flash_to_sd(void);

#endif
