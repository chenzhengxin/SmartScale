#include "util.h"
#include "rtc.h"
#include "fileopt.h"
#include "CH376_api.h"

#define FILE_OPT_BUF_LEN 11
#define LINE_LEN 21
#define WORKER_LEN 50
#define MAX_BUF_LEN 100
#define FILE_SYN_TIME 300

#define NO 0
#define OK 1
#define IN 
#define OUT

#define f(a,b) a##b
#define g(a)  #a
#define h(a) g(a)

#define WORK_DIR "0:work"
#define PASSWD_FILE "1:passwd"
#define WORKER_FILE h(f(work/, worker.csx))
#define WORKERID_FILE h(f(work/, worker_id.csx))
#define GOODS_LIST_FILE h(f(work/, goods_list.csx))
#define WATER_RATE h(f(work/, water_rate))
#define TOTAL_GOODS 4
float current_price = 0.0;
char current_path[16] = {0};
char old_msg[2][40] = {0, 0};

typedef struct worker_msg {
	char *work_id;
    char *card_id;
    char *work_name;
    char *weight;
    char *total_price;
} worker_msg_t;

typedef struct current_parm_t {
	float cur_price;
	char cur_goods[16];
	char cur_path[16];
	char old_msg[2][40];
	unsigned int work_id;
	goods_list_t goods_list;
	u32 last_count;
} current_parm_t;

static current_parm_t cur_parm = {0};
extern _calendar_obj calendar;


//0:不覆盖原有的文件
//1:覆盖原有的文件
int mf_copy(u8 *psrc, u8 *pdst, u8 fwmode)
{
	int res;
	u16 br=0;
	u16 bw=0;
	FIL *fsrc=0;
	FIL *fdst=0;
	u8 *fbuf=0;
	fsrc=(FIL*)mymalloc(sizeof(FIL));//申请内存
	fdst=(FIL*)mymalloc(sizeof(FIL));
	fbuf=(u8*)mymalloc(512);
	if(fsrc==NULL || fdst==NULL || fbuf==NULL)
	{
		res = FILEOPT_MALLOC_ERR;//前面的值留给fatfs
	}
	else
	{   
		if(fwmode==0)
		{
			fwmode=FA_CREATE_NEW;//不覆盖
		}
		else 
		{
			fwmode=FA_CREATE_ALWAYS;   //覆盖存在的文件
		}

		res=f_open(fsrc,(const TCHAR*)psrc,FA_READ|FA_OPEN_EXISTING); //打开只读文件
		if(res==0)
		{
			res=f_open(fdst,(const TCHAR*)pdst,FA_WRITE|fwmode);  //第一个打开成功,才开始打开第二个
		}
		if(res==0)//两个都打开成功了
		{
			while(res==0)//开始复制
			{
				res=f_read(fsrc,fbuf,512,(UINT*)&br); //源头读出512字节
				if(res||br==0)
				{
					break;
				}
				res=f_write(fdst,fbuf,(UINT)br,(UINT*)&bw); //写入目的文件
				if(res||bw<br)
				{
					break;       
				}
			}
		  f_close(fsrc);
		  f_close(fdst);
		}
	}
	if (fsrc != NULL)
	{
		myfree(fsrc);//释放内存
	}
	if (fdst != NULL)
	{
		myfree(fdst);
	}
	if (fbuf != NULL)
	{
		myfree(fbuf);
	}
	return res;
}

static void __set_current_price(float *now_price)
{
    cur_parm.cur_price = *now_price;
}


static int get_name(char *read_data, char *name, int name_len)
{
	char *p_str_tok = NULL;
	char temp[64] = {0};
	int ret;

	if (read_data == NULL)
	{
		ret = FILEOPT_PARM_ERR;
		goto TAR_OUT;
	}

	mymemcpy(temp, read_data, sizeof(temp));
	p_str_tok = strtok(temp, ":");
	p_str_tok = strtok(NULL, ":");	
	p_str_tok = strtok(NULL, ":");

	if (strlen(p_str_tok) > name_len) 
	{
		mymemcpy(name, p_str_tok, name_len);
	}
	else 
	{
		mymemcpy(name, p_str_tok, strlen(p_str_tok));
	}

	ret = 0;
TAR_OUT:

	return ret;
}

void fileopt_get_current_price(float *price)
{
	*price = cur_parm.cur_price;
}

void fileopt_get_current_goods(char *goods_name, int name_len)
{
	mymemcpy(goods_name, cur_parm.cur_goods, name_len);
}

int fileopt_set_current_price(char *goods_name)
{
	int i;
	int ret;

	if (goods_name == NULL)
	{
		ret = FILEOPT_PARM_ERR;
		goto TAR_OUT;
	}

	for (i=0; i<TOTAL_GOODS; i++)
	{
		if (cur_parm.goods_list.goods[i].goods_name[0] != 0)
		{
			if (strcmp(cur_parm.goods_list.goods[i].goods_name, goods_name) == 0)
			{	
				mymemset(cur_parm.cur_goods, 0, sizeof(cur_parm.cur_goods));
				mymemcpy(cur_parm.cur_goods, goods_name, sizeof(cur_parm.cur_goods));
				__set_current_price(&(cur_parm.goods_list.goods[i].price));
				ret = 0;
				break;
			} 
			else 
			{
				ret = FILEOPT_NOT_FOUND;
			}
		}
	}
	
TAR_OUT:

	return ret;

}

char fileopt_file_init(char *filename)
{
	FIL *f_txt_file = 0;
	int ret = -1;
	
	f_txt_file = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_file == NULL) 
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	
	mymemset(f_txt_file, 0, sizeof(FIL));
	if (!f_open(f_txt_file, filename, FA_READ))
	{
		f_close(f_txt_file);
		ret = 0;
	}
	else
	{
		ret = f_open(f_txt_file, filename, FA_CREATE_ALWAYS);
		if (ret != 0)
		{
			ret = FILEOPT_ONFILE_ERR;
			goto TAR_OUT;
		}
		f_close(f_txt_file);
		ret = 0;
	}
	
TAR_OUT:

	return ret;
}

int fileope_goods_list_init()
{
	FIL *f_txt_goods_list = NULL;
	TCHAR read_buf[64] = {0};
	char filename[64] = {0};
	int ret = -1;
	char temp[64] = {0};
	char *p_str_tok = NULL;
	int total = 0;

	if (cur_parm.goods_list.init_flag == OK)
	{
		ret = FILEOPT_REINIT;
		goto TAR_OUT;
	}
	f_txt_goods_list = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_goods_list == NULL)
	{ 
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	
	mymemset(f_txt_goods_list, 0, sizeof(FIL));
	sprintf(filename, "0:%s", GOODS_LIST_FILE);
	if (!f_open(f_txt_goods_list, filename, FA_OPEN_ALWAYS | FA_READ))
	{
		while (f_gets(read_buf, sizeof(read_buf), f_txt_goods_list))
		{	
			mymemcpy(temp, read_buf, strlen(read_buf));
			p_str_tok = strtok(temp, ":");

			mymemcpy(cur_parm.goods_list.goods[total].goods_name, p_str_tok,
				sizeof(cur_parm.goods_list.goods[total].goods_name));
			p_str_tok = strtok(NULL, ":");
			cur_parm.goods_list.goods[total].price = string_to_float(p_str_tok);
			cur_parm.goods_list.total_num++;
			total++;
		}

		f_close(f_txt_goods_list);
		
		ret = 0;
		cur_parm.goods_list.init_flag = OK;
	}
TAR_OUT:
	
	if (f_txt_goods_list != NULL) {
		myfree(f_txt_goods_list);
	}
	return ret;
}

goods_list_t *fileopt_get_goods_list()
{
	goods_list_t *list = NULL;

	list = &cur_parm.goods_list;

	return list;
}


char fileopt_init(void)
{
	int ret;
    FIL *f_txt_worker_list = 0;
    FIL *f_txt_worker_id = 0;
	FIL *f_txt_passwd = 0;
	FIL *f_txt_goods = 0;
	UINT bw;
	char filename[64] = {0};
	char passwd[34] = "888888**************************\r\n";
	char work_id[5] = "10000";

	mymemset(&cur_parm, 0, sizeof(current_parm_t));

	f_txt_passwd = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_passwd == NULL) 
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	mymemset(f_txt_passwd, 0, sizeof(FIL));
	if (!f_open(f_txt_passwd, "0:passwd", FA_READ))
	{
		f_close(f_txt_passwd);
		ret = 0;
	}
	else
	{
		ret = f_open(f_txt_passwd, "0:passwd", FA_CREATE_NEW | FA_WRITE);
		if (ret != 0)
		{
			ret = FILEOPT_ONFILE_ERR;
			goto TAR_OUT;
		}
		f_write(f_txt_passwd, passwd, strlen(passwd), &bw);
		f_write(f_txt_passwd, passwd, strlen(passwd), &bw);
		f_close(f_txt_passwd);
		ret = 0;
	}

	ret = f_mkdir(WORK_DIR);
	mymemset(filename, 0, sizeof(filename));
	sprintf(filename, "0:%s", WATER_RATE);

	if (!f_open(f_txt_passwd, filename, FA_READ))
	{
		f_close(f_txt_passwd);
		ret = 0;
	}
	else
	{
		ret = f_open(f_txt_passwd, filename, FA_CREATE_NEW | FA_WRITE);
		if (ret != 0)
		{
			ret = FILEOPT_ONFILE_ERR;
			goto TAR_OUT;
		}
		f_write(f_txt_passwd, "1.00", 4, &bw);
		f_close(f_txt_passwd);
		ret = 0;
	}

	sprintf(filename, "0:%s", WORKER_FILE);
	ret = fileopt_file_init(filename);
	mymemset(filename, 0, sizeof(filename));
	sprintf(filename, "0:%s", GOODS_LIST_FILE);
	ret = fileopt_file_init(filename);

	f_txt_worker_id = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_worker_id == NULL) 
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	mymemset(f_txt_worker_id, 0, sizeof(FIL));
	mymemset(filename, 0, sizeof(filename));
	sprintf(filename, "0:%s", WORKERID_FILE);
	if (!f_open(f_txt_worker_id, filename, FA_READ))
	{
		f_close(f_txt_worker_id);
		ret = 0;
	}
	else
	{
		ret = f_open(f_txt_worker_id, filename, FA_CREATE_ALWAYS | FA_WRITE);
		if (ret != 0)
		{
			ret = FILEOPT_ONFILE_ERR;
			goto TAR_OUT;
		}
		f_write(f_txt_worker_id, work_id, strlen(work_id), &bw);
		f_close(f_txt_worker_id);
	}
	fileope_goods_list_init();
	ret = 0;
	
TAR_OUT:

	if (f_txt_worker_list) 
	{
		myfree(f_txt_worker_list);
	}
	if (f_txt_passwd)
	{
		myfree(f_txt_passwd);
	}
	if (f_txt_worker_id)
	{
		myfree(f_txt_worker_id);
	}
	if (f_txt_goods)
	{
		myfree(f_txt_goods);

	}
	return ret;
}

s32 fileopt_get_work_id()
{
	char filename[64] = {0};
	FIL *f_txt_worker_id = 0;
	int ret = 0;
	char work_id_buf[8] = {0};
	
	sprintf(filename, "0:%s", WORKERID_FILE);
	f_txt_worker_id = mymalloc(sizeof(FIL));
	if (f_txt_worker_id == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}

	ret = f_open(f_txt_worker_id, filename, FA_OPEN_ALWAYS | FA_READ);
	if (ret == 0)
	{
		f_gets(work_id_buf, 8, f_txt_worker_id);
		ret = atoi(work_id_buf);
		f_close(f_txt_worker_id);
	}
	else 
	{
		ret = FILEOPT_ONFILE_ERR;
	}

TAR_OUT:

	if (f_txt_worker_id != NULL)
	{
		myfree(f_txt_worker_id);
	}
	return ret;
	
}

char fileopt_find_worker(char *card_id)
{
    FIL *f_txt_worker_info = 0;
    TCHAR sd_card_id_path[50] = {0};
	int ret;
	
	f_txt_worker_info = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_worker_info == NULL) {
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}

	sprintf(sd_card_id_path, "0:work/%s.csx", card_id);

	if (!f_open(f_txt_worker_info, sd_card_id_path, FA_READ))
	{
        f_close(f_txt_worker_info);
		ret = 0;
	}
	else
	{
		ret = FILEOPT_ONFILE_ERR;
	}
TAR_OUT:
	if (f_txt_worker_info != NULL)
	{
		myfree(f_txt_worker_info);
	}
	return ret;
}


s32 fileopt_create_card(char *card_id, char *worker_name)
{
	u32 iBw = 0;
    FIL *f_txt_worker_list = 0;
	FIL *f_txt_worker_info = 0;
	FIL *f_txt_worker_id = 0;
	TCHAR work_id_buf[6] = {0};
	int id;
	char filename[64] ={0};
    char sd_card_id_path[50] = {0};
	char write_buf[50] = {0};
	int ret = -1;

	if (!fileopt_find_worker(card_id))
	{
		ret = FILEOPT_EXIST;
        goto TAR_OUT;;
	}

	f_txt_worker_list = (FIL *)mymalloc(sizeof(FIL));
	f_txt_worker_info = (FIL *)mymalloc(sizeof(FIL));
	f_txt_worker_id = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_worker_list == NULL) 
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	if (f_txt_worker_info == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	if (f_txt_worker_id == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	sprintf(filename, "0:%s", WORKERID_FILE);

	ret = f_open(f_txt_worker_id, filename, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
	if (ret < 0)
	{
		ret = FILEOPT_ONFILE_ERR;
		goto TAR_OUT;
	}
	f_gets(work_id_buf, 6, f_txt_worker_id);
	id = atoi(work_id_buf);
	id++;
	f_close(f_txt_worker_id);

    sprintf(write_buf,"%s:%s:%s:**********:**********\r\n",card_id, work_id_buf, worker_name);
	
	if (!f_open(f_txt_worker_id, filename, FA_OPEN_ALWAYS | FA_WRITE))
	{
		mymemset(work_id_buf, 0, sizeof(work_id_buf));
		sprintf(work_id_buf, "%d", id);
		f_write(f_txt_worker_id, work_id_buf, strlen(work_id_buf), &iBw);
		f_close(f_txt_worker_id);
	}
	
	mymemset(filename, 0, sizeof(filename));
	sprintf(filename, "0:%s", WORKER_FILE);

	if (!f_open(f_txt_worker_list, filename, FA_OPEN_ALWAYS | FA_WRITE | FA_READ))
	{
		f_lseek(f_txt_worker_list, f_txt_worker_list->fsize);
		f_write(f_txt_worker_list, write_buf, strlen(write_buf), &iBw);
		f_close(f_txt_worker_list);
	}
	mymemset(write_buf, 0, sizeof(write_buf));
	sprintf(write_buf, "**********,**********\r\n");
	sprintf(sd_card_id_path, "0:work/%s.csx", card_id);

	if (!f_open(f_txt_worker_info, sd_card_id_path, FA_CREATE_ALWAYS | FA_WRITE))
	{
		f_write(f_txt_worker_info, write_buf, strlen(write_buf), &iBw);
		f_write(f_txt_worker_info, write_buf, strlen(write_buf), &iBw);
		f_close(f_txt_worker_info);
	}
	ret = 0;
	
TAR_OUT:

	if (f_txt_worker_list != NULL) 
	{
		myfree(f_txt_worker_list);
	}
	if (f_txt_worker_info != NULL) 
	{
		myfree(f_txt_worker_info);
	}
	if (f_txt_worker_id != NULL)
	{
		myfree(f_txt_worker_id);
	}
	return ret;
}

s32 fileopt_cnt_worker_pos(char *card_id, char *msg, int msg_len)
{
	FIL *f_txt_worker_list = NULL;
	char read_buf[50] = {0};
	char temp[50] = {0};
	int ret = -1;
	char *p_str_tok = NULL;
	char filename[64] = {0};
	int flag = 0;
	int cnt = 0;

	f_txt_worker_list = (FIL *)mymalloc(sizeof(FIL));
    if (f_txt_worker_list == NULL) 
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
    }
	
	sprintf(filename, "0:%s", WORKER_FILE);
    mymemset(f_txt_worker_list, 0, sizeof(FIL));
    if (!f_open(f_txt_worker_list, filename, FA_OPEN_ALWAYS | FA_READ))
    {
		while (f_gets(read_buf, 50, f_txt_worker_list))
		{
			mymemcpy(temp, read_buf, sizeof(temp));
			p_str_tok = strtok(temp, ":");
			if (strcmp(p_str_tok, card_id) == 0) 
			{
				mymemcpy(msg, read_buf, msg_len);
				flag = 1;
				break;
			}
			else 
			{
				flag = 0;
			}
			cnt++;
		}
		if (flag == 1)
		{
			ret = cnt;
		}
		else
		{
			ret = FILEOPT_NOT_FOUND; 
		}
		f_close(f_txt_worker_list);
	}

TAR_OUT:
	
	if (f_txt_worker_list != NULL) {
		myfree(f_txt_worker_list);
	}
	return ret;
}

int fileopt_update_msg(char *readstr, float *weight, float *price)
{
	char *p_str = NULL;
	char replace_space[22] = {0};
	int ret = -1;
	char cnt = 0;
	char *p_str_mark = NULL;
	int index = 0;
	float new_weight = 0.0, new_price = 0.0;
	char old_weight[12] = {0}, old_price[12] = {0};
	
	
	if (readstr == NULL) 
	{
		ret = FILEOPT_PARM_ERR;
		goto TAR_OUT;
	}

	p_str = readstr;
	
	while (*p_str != '\r' && *p_str != '\n')
	{
		if (*p_str == ':')
		{
			index = 0;
			cnt++;
		}
		p_str++;

		if (cnt == 3)
		{
			if (index == 0 && p_str_mark == NULL)
			{
				p_str_mark = p_str;
			}
			if ((*p_str >= '0' && *p_str <= '9') || *p_str == '.') 
			{
				old_weight[index] = *p_str;
				index++;
			}
		} 
		else if (cnt == 4)
		{
			if ((*p_str >= '0' && *p_str <= '9') || *p_str == '.') 
			{
				old_price[index] = *p_str;
				index++;
			}
		}
	}

	if (p_str_mark != NULL)
	{
		new_weight = string_to_float(old_weight) + (*weight);
		new_price = string_to_float(old_price) + (*price);
		mymemset(replace_space, 0, sizeof(replace_space));
		sprintf(replace_space, "%10.02f:%10.02f", new_weight, new_price);
		mymemcpy(p_str_mark, replace_space, strlen(replace_space));
		ret = 0;
	}
TAR_OUT:
	
	return ret;
}


char fileopt_update_worker_product_msg(char *read_str, float *weight, float *price, char *update_str, int str_len)
{
	char temp_str[LINE_LEN] = {0};
	char weight_buf[9] = {0};
	char price_buf[9] = {0};
	char *p_str = NULL;
	int index = 0;
	int cnt = 0;
	volatile float weight_new = 0.0, price_new = 0.0;

	p_str = read_str;

	while (*p_str != '\r' && *p_str != '\n') 
	{
		if (*p_str == ',')
		{
			cnt++;
			index = 0;
		}
		
		if (cnt == 0)
		{
			if ((*p_str >= '0' && *p_str <= '9') || *p_str == '.')
			{
				if (index < 9)
				{
					weight_buf[index] = *p_str;
					index++;
				}
			}
		} 
		else if (cnt == 1)
		{
			if ((*p_str >= '0' && *p_str <= '9') || *p_str == '.')
			{
				if (index < 9) 
				{
					price_buf[index] = *p_str;
					index++;
				}
			}
		}
		p_str++;
	}

	weight_new = string_to_float(weight_buf) + (*weight);
	price_new = string_to_float(price_buf) + (*price);
	sprintf(temp_str, "%10.02f,%10.02f\r\n", weight_new, price_new);
	if (strlen(temp_str) < str_len)
	{
		mymemcpy(update_str, temp_str, strlen(temp_str));
	} 
	else 
	{
		mymemcpy(update_str, temp_str, str_len);
	}
	
	return 0;
}


int fileopt_get_1st_2nd_msg(char *card_id, char *st_msg, int st_len, char *nd_msg, int nd_len)
{
	FIL *f_txt_worker_info = NULL;
	int ret;
	char cnt = 0;
	char filename[64] = {0};
	char read_buf[50] = {0};
	
	f_txt_worker_info = (FIL *)mymalloc(sizeof(FIL));
    if (f_txt_worker_info == NULL) 
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
    }
	mymemset(filename, 0, sizeof(filename));
	sprintf(filename, "0:work/%s.csx", card_id);

    if (!f_open(f_txt_worker_info, filename, FA_OPEN_ALWAYS | FA_WRITE | FA_READ))
    {
		while (f_gets(read_buf, sizeof(read_buf), f_txt_worker_info))
		{
			if (cnt == 0)
			{
				if (strlen(read_buf) > st_len) 
				{
					mymemcpy(st_msg, read_buf, st_len);
				}
				else 
				{
					mymemcpy(st_msg, read_buf, strlen(read_buf));
				}
			} 
			else if(cnt == 1) 
			{
				if (strlen(read_buf) > st_len) 
				{
					mymemcpy(nd_msg, read_buf, nd_len);
				}
				else 
				{
					mymemcpy(nd_msg, read_buf, strlen(read_buf));
				}
				break;
			}
			cnt++;
			
		}
		f_close(f_txt_worker_info);
	}
	
TAR_OUT:

	if (f_txt_worker_info != NULL)
	{
		myfree(f_txt_worker_info);
	}
	return ret;
}

char fileopt_add_worker_msg(char *card_id, float *weight)
{
	char new_worker[WORKER_LEN+1] = {0};
	char new_msg[WORKER_LEN] = {0};
	char str[2][64] = {0};
	float price = 0.0;
	int ret = -1;
	volatile int weight_len;
	char price_buf[FILE_OPT_BUF_LEN];
	char weight_buf[FILE_OPT_BUF_LEN];
	UINT iBw = 0;
	int line = -1;
	char read_buf[WORKER_LEN+1] = {0};
	FIL *f_txt_worker_list = NULL;
	FIL *f_txt_worker_info = NULL;
	char cnt = 0;
	char filename[64] = {0};
	char ucTime[30] = {0};
				

    if (card_id == NULL)
	{
		ret = FILEOPT_PARM_ERR;
		goto TAR_OUT;
    }
	
    price = cur_parm.cur_price * (*weight);
    mymemset(price_buf, '0', FILE_OPT_BUF_LEN);
    sprintf(price_buf, "%10.02f", price);
    mymemset(weight_buf, '0', FILE_OPT_BUF_LEN);
    sprintf(weight_buf, "%10.02f", (*weight));
	
    weight_len = strlen(weight_buf);

	line = fileopt_cnt_worker_pos(card_id, new_worker, sizeof(new_worker));
	if (line < 0) 
	{
		ret = FILEOPT_NOT_FOUND;
		goto TAR_OUT;
	}
	ret = fileopt_update_msg(new_worker, weight, &price);
	if (ret < 0)
	{
		ret = FILEOPT_UPDATE_ERR;
		goto TAR_OUT;
	}

	f_txt_worker_list = (FIL *)mymalloc(sizeof(FIL));
    if (f_txt_worker_list == NULL) 
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
    }

	f_txt_worker_info = (FIL *)mymalloc(sizeof(FIL));
    if (f_txt_worker_info == NULL) 
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
    }

	mymemset(filename, 0, sizeof(filename));
	sprintf(filename, "0:%s", WORKER_FILE);
    mymemset(f_txt_worker_list, 0, sizeof(FIL)); 
	ret = f_open(f_txt_worker_list, filename, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
    if (ret == 0)
	{
		if (cnt == line) 
		{
			f_write(f_txt_worker_list, new_worker, strlen(new_worker), &iBw);
			f_close(f_txt_worker_list);
			ret = 0;
		}  
		else
		{
			while (f_gets(read_buf, 50, f_txt_worker_list))
			{
				cnt++;
				if (cnt == line)
				{
					f_write(f_txt_worker_list, new_worker, strlen(new_worker), &iBw);
					f_close(f_txt_worker_list);
					ret = 0;
				}
				
			}
	    }
	}
	else
	{
		ret = FILEOPT_ONFILE_ERR;
		goto TAR_OUT;
	}
	sprintf(filename, "0:work/%s.csx", card_id);
    mymemset(f_txt_worker_info, 0, sizeof(FIL)); 
	cnt = 0;
	fileopt_get_1st_2nd_msg(card_id, str[0], sizeof(str[0]), str[1], sizeof(str[1]));
    if (!f_open(f_txt_worker_info, filename, FA_OPEN_ALWAYS | FA_WRITE | FA_READ))
    {		
		fileopt_update_worker_product_msg(str[0], weight, &price, new_msg, sizeof(new_msg));
		f_write(f_txt_worker_info, new_msg, strlen(new_msg), &iBw);
		fileopt_update_worker_product_msg(str[1], weight, &price, new_msg, sizeof(new_msg));
		f_write(f_txt_worker_info, new_msg, strlen(new_msg), &iBw);
		f_lseek(f_txt_worker_info, f_txt_worker_info->fsize);
		mymemset(new_msg, 0, sizeof(new_msg));
		sprintf(ucTime,"%d-%d-%d %d:%d:%d",calendar.w_year,calendar.w_month,
			calendar.w_date,calendar.hour,calendar.min,calendar.sec);
		sprintf(new_msg, "%s,%10.02f,%10.02f\r\n", ucTime, *weight, price);
		f_write(f_txt_worker_info, new_msg, strlen(new_msg), &iBw);
		f_close(f_txt_worker_info);
		ret = 0;
	}
	
TAR_OUT:
	
	if (f_txt_worker_list != NULL) 
	{
		myfree(f_txt_worker_list);
	}
	if (f_txt_worker_info != NULL)
	{
		f_close(f_txt_worker_info);
		myfree(f_txt_worker_info);
	}

	return ret;

}


int fileopt_find_worker_msg_by_workid(IN char *work_id, OUT char *get_name_buf, IN u32 buf_len,
	OUT char *card_id, IN int id_len)
{
    FIL *f_txt_worker_list = 0;
	int ret = -1;
	char read_buf[50];
	char card_id_buf[16] = {0};
	char filename[32] = {0};
	int str_len;
	char *p_str_tok = NULL;

	if (work_id == NULL)
	{
		ret = FILEOPT_PARM_ERR;
		goto TAR_OUT;
	}

	f_txt_worker_list = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_worker_list == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	sprintf(filename, "0:%s", WORKER_FILE);
	if (!f_open(f_txt_worker_list, filename, FA_READ))
	{
		while (f_gets(read_buf, sizeof(read_buf), f_txt_worker_list) != NULL)
		{
			p_str_tok = strtok(read_buf, ":");
			mymemcpy(card_id_buf, p_str_tok, sizeof(card_id_buf));
			p_str_tok = strtok(NULL, ":");

			if (strcmp(p_str_tok, work_id) == 0)
			{
				if (card_id != NULL)
				{
					str_len = strlen(card_id_buf);
					if (str_len < id_len)
					{
						mymemcpy(card_id, card_id_buf, str_len);
					}
					else
					{
						mymemcpy(card_id, card_id_buf, id_len);
					}
				}
				
				p_str_tok = strtok(NULL, ":");
				if (get_name_buf != NULL) 
				{
					str_len = strlen(p_str_tok);
					if (str_len < buf_len)
					{
						mymemcpy(get_name_buf, p_str_tok, str_len);
					}
					else
					{
						mymemcpy(get_name_buf, p_str_tok, buf_len);
					}
				}
				break;
			}

						
 		}
		f_close(f_txt_worker_list);
	}

TAR_OUT:

	return ret;
}

int fileopt_find_worker_msg_by_cardid(IN char *card_id, OUT char *get_name_buf, IN u32 buf_len,
	OUT char *work_id, IN int id_len)
{
    FIL *f_txt_worker_list = 0;
	int ret = -1;
	char read_buf[50];
	char filename[32] = {0};
	int str_len;
	char *p_str_tok = NULL;

	if (card_id == NULL)
	{
		ret = FILEOPT_PARM_ERR;
		goto TAR_OUT;
	}

	f_txt_worker_list = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_worker_list == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
    
	mymemset(f_txt_worker_list, 0, sizeof(FIL));	
	sprintf(filename, "0:%s", WORKER_FILE);
	if (!f_open(f_txt_worker_list, filename, FA_WRITE | FA_READ))
	{
		while (f_gets(read_buf, sizeof(read_buf), f_txt_worker_list) != NULL)
		{
			if (strncmp(card_id, read_buf, strlen(card_id)) == 0)
			{
				p_str_tok = strtok(read_buf, ":");
				p_str_tok = strtok(NULL, ":");
				str_len = strlen(p_str_tok);
				if (work_id != NULL)
				{
					if (str_len < id_len)
					{
						mymemcpy(work_id, p_str_tok, str_len);
					}
					else
					{
						mymemcpy(work_id, p_str_tok, id_len);
					}
				}
				p_str_tok = strtok(NULL, ":");
				str_len = strlen(p_str_tok);
				if (get_name_buf != NULL) 
				{
					str_len = strlen(p_str_tok);
					if (str_len < buf_len)
					{
						mymemcpy(get_name_buf, p_str_tok, str_len);
					} 
					else
					{
						mymemcpy(get_name_buf, p_str_tok, buf_len);
					}
				}

				break;
			}
 		}
		
	    f_close(f_txt_worker_list);	
	}
 	
	
TAR_OUT:

	if (f_txt_worker_list != NULL)
	{
		myfree(f_txt_worker_list);
	}
    return ret;
}

static int search_by_work_id(char *work_id, char *data, int data_len)
{
	int ret = -1;
	int len = 0;
	int flag;
	char read_buf[64] = {0};
	char filename[32] = {0};
	char p_read[64] = {0};
	FIL *f_txt_worker_list = NULL;
	char *p_str_tok = NULL;
	int line_cnt = 0;
	
	f_txt_worker_list = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_worker_list == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}

	mymemset(f_txt_worker_list, 0, sizeof(FIL));	
	sprintf(filename, "0:%s", WORKER_FILE);
	if (!f_open(f_txt_worker_list, filename, FA_WRITE | FA_READ))
	{
		while (f_gets(read_buf, sizeof(read_buf), f_txt_worker_list) != NULL)
		{
			mymemcpy(p_read, read_buf, sizeof(p_read));
			
			if (strstr(p_read, work_id) != NULL)
			{
				p_str_tok = strtok(p_read, ":");
				p_str_tok = strtok(NULL, ":");
				if (strcmp(p_str_tok, work_id) == 0) 
				{
					len = strlen(read_buf);
					if (len > data_len)
					{
						mymemcpy(data, read_buf, data_len);
					}
					else 
					{
						mymemcpy(data, read_buf, len);
					}
					flag = 1;
					break;
				}
				else 
				{
					flag = 0;
				}
			}
			else 
			{
				flag = 0;
			}
			line_cnt++;
		}
		f_close(f_txt_worker_list);
	}
	
	if (flag == 1)
	{
		ret = line_cnt;
	}
TAR_OUT:

	if (f_txt_worker_list != NULL)
	{
		myfree(f_txt_worker_list);
	}
	return ret;
}

int fileopt_reapply_card_by_work_id(char *work_id, char *new_card_id)
{
	char *read_data = 0;
	char read_buf[64] = {0};
	int ret;
	int line_cnt = -1;
	int line = 0;
	UINT iBw;
	char *p_str_tok = NULL;
	FIL *f_txt_worker_list = NULL;
	char old_cardid[32] = {0};
	char filename[64] = {0};
	char filename_new[64] = {0};

	if (!fileopt_find_worker(new_card_id))
	{
		ret = FILEOPT_EXIST;
        goto TAR_OUT;;
	}
	
	read_data = (char *)mymalloc(64);
	if (read_data == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	mymemset(read_data, 0, 64);
	line_cnt = search_by_work_id(work_id, read_data, 64);
	if (line_cnt >= 0)
	{
		mymemcpy(read_buf, read_data, sizeof(read_buf));
		p_str_tok = strtok(read_buf, ":");
		mymemcpy(old_cardid, p_str_tok, strlen(p_str_tok));
		mymemcpy(read_data, new_card_id, strlen(new_card_id));
	}

	f_txt_worker_list = (FIL *)mymalloc(sizeof(FIL));
    if (f_txt_worker_list == NULL) 
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
    }
	
	sprintf(filename, "0:%s", WORKER_FILE);
	ret = f_open(f_txt_worker_list, filename, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
	if (ret == 0)
	{
		if (line_cnt == line) 
		{
			f_write(f_txt_worker_list, read_data, strlen(read_data), &iBw);
			ret = 0;
		}  
		else
		{
			while (f_gets(read_buf, 50, f_txt_worker_list))
			{
				line++;
				if (line_cnt == line)
				{
					f_write(f_txt_worker_list, read_data, strlen(read_data), &iBw);
					ret = 0;
					break;
				}
				
			}
		}
		f_close(f_txt_worker_list);
	}

	
	sprintf(filename, "0:work/%s.csx", old_cardid);
	sprintf(filename_new, "0:work/%s.csx", new_card_id);
	ret = f_rename(filename, filename_new);
		
TAR_OUT:

	if (read_data != NULL)
	{
		myfree(read_data);
	}
	if (f_txt_worker_list != NULL)
	{
		myfree(f_txt_worker_list);
	}
	return ret;
}

int fileopt_del_worker_msg(char *card_id)
{
    FIL *f_txt_worker_list = 0;
    u32 iBw = 0;
	int ret = -1;
    char name_buf[16] = {0};
	int line;
	int cnt = 0;
	char filename[64] = {0};
	char filename_new[64] = {0};
	char read_buf[64] = {0};
	char read_data[64] = {0};
	char *dele_str = "********";

	if (card_id == NULL)
	{
		ret = FILEOPT_PARM_ERR;
		goto TAR_OUT;
    }
    f_txt_worker_list = (FIL *)mymalloc(sizeof(FIL));
    if (f_txt_worker_list == NULL) {
		ret = FILEOPT_MALLOC_ERR;
        goto TAR_OUT;
    }
	line = fileopt_cnt_worker_pos(card_id, read_data, sizeof(read_data));
	if (line < 0) 
	{
		ret = FILEOPT_NOT_FOUND;
		goto TAR_OUT;
	}
	get_name(read_data, name_buf, sizeof(name_buf));
	mymemcpy(read_data, dele_str, strlen(dele_str));


	sprintf(filename, "0:%s", WORKER_FILE);
	
    if (!f_open(f_txt_worker_list, filename, FA_OPEN_ALWAYS | FA_WRITE | FA_READ))
	{
	
		if (cnt == line) 
		{
			f_write(f_txt_worker_list, read_data, strlen(read_data), &iBw);
			ret = 0;
		}  
		else 
		{
			while (f_gets(read_buf, 50, f_txt_worker_list))
			{
				cnt++;
				if (cnt == line)
				{
					f_write(f_txt_worker_list, read_data, strlen(read_data), &iBw);
					ret = 0;
					break;
				}
				
			}
		}
        f_close(f_txt_worker_list);
       
    }
    else
	{
	    ret = FILEOPT_ONFILE_ERR;
	}

	mymemset(filename, 0, sizeof(filename));
	sprintf(filename, "0:work/%s.csx", card_id);
	sprintf(filename_new, "0:work/%s_%s.csx", card_id, name_buf);
	f_rename(filename, filename_new);
TAR_OUT:

	return ret;
}

int fileopt_read_total_msg(char *card_id, total_msg *msg)
{
    FIL *f_txt_worker_info = 0;
    char filename[64] = {0};
	char read_buf[64] = {0};
	char temp_buf[64] = {0};
	char *p_str_tok = NULL;
	int ret = -1;
	int  i = 0;
	int  j = 0;

	if (msg == NULL)
	{
		ret = FILEOPT_PARM_ERR;
		goto TAR_OUT;
	}
	
	f_txt_worker_info = (FIL *)mymalloc(sizeof(FIL));
    if (f_txt_worker_info == NULL)
	{
       ret = FILEOPT_MALLOC_ERR;
	   goto TAR_OUT;
    }

	sprintf(filename, "0:work/%s.csx", card_id);

	if (!f_open(f_txt_worker_info, filename, FA_OPEN_ALWAYS | FA_WRITE | FA_READ))
	{
	    f_gets(read_buf, sizeof(read_buf), f_txt_worker_info);
		while (read_buf[i] != '\r' && read_buf[i] != '\n')
		{
			if ((read_buf[i] >= '0' && read_buf[i] <= '9') || read_buf[i] == '.' || read_buf[i] == ',')
			{
				temp_buf[j] = read_buf[i];
				j++;
			}	
			i++;
		}
		p_str_tok = strtok(temp_buf, ",");
		mymemcpy(msg->lastest_total_weight, p_str_tok, strlen(p_str_tok));
		p_str_tok = strtok(NULL, ",");	
		mymemcpy(msg->lastest_total_price, p_str_tok, strlen(p_str_tok));
		i = 0, j = 0;
		f_gets(read_buf, sizeof(read_buf), f_txt_worker_info);
		while (read_buf[i] != '\r' && read_buf[i] != '\n')
		{
			if ((read_buf[i] >= '0' && read_buf[i] <= '9') || read_buf[i] == '.' || read_buf[i] == ',')
			{
				temp_buf[j] = read_buf[i];
				j++;
			}	
			i++;
		}
		p_str_tok = strtok(temp_buf, ",");
		mymemcpy(msg->total_weight, p_str_tok, strlen(p_str_tok));
		p_str_tok = strtok(NULL, ",");	
		mymemcpy(msg->total_price, p_str_tok, strlen(p_str_tok));

        f_close(f_txt_worker_info);
	}
	else
	{
        ret = FILEOPT_ONFILE_ERR;
	}
TAR_OUT:

	if (f_txt_worker_info != NULL)
	{
		myfree(f_txt_worker_info);
	}
	return ret;

}

int fileopt_clear_lastest_data(char *card_id)
{
	char filename[64] = {0};
    FIL *f_txt_worker_info = 0;
	char *dele_str = "**********,**********";
	UINT iBw;
	int ret;
	total_msg *msg = NULL;

	f_txt_worker_info = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_worker_info == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}

	msg = (total_msg *)mymalloc(sizeof(total_msg));
	if (msg == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}

	sprintf(filename, "0:work/%s.csx", card_id);
	if (!f_open(f_txt_worker_info, filename, FA_WRITE | FA_READ))
	{
		f_write(f_txt_worker_info, dele_str, strlen(dele_str), &iBw);
		f_close(f_txt_worker_info);
	}
	else 
	{
		ret = FILEOPT_NOT_FOUND;
	}
	
TAR_OUT:

	if (msg != NULL)
	{
		myfree(msg);
	}
	if (f_txt_worker_info != NULL)
	{
		myfree(f_txt_worker_info);
	}
	return ret;
}

int fileopt_set_water_rate(float *rate)
{
	char water_rate[12] = {0};
	FIL *f_txt_water_rate = 0;
	int ret;
	UINT iBw;

	f_txt_water_rate = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_water_rate == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	sprintf(water_rate, "%.02f", *rate);
	if (!f_open(f_txt_water_rate, "0:work/water_rate", FA_CREATE_ALWAYS | FA_WRITE))
	{
		f_write(f_txt_water_rate, water_rate, strlen(water_rate), &iBw);
		f_close(f_txt_water_rate);
	}
	else 
	{
		ret = FILEOPT_ONFILE_ERR;
	}

TAR_OUT:
	
	if (f_txt_water_rate != NULL)
	{	
		myfree(f_txt_water_rate);
	}
	return ret;
}

int fileopt_get_water_rate(float *rate)
{
	char water_rate[12] = {0};
	FIL *f_txt_water_rate = 0;
	int ret = -1; 

	f_txt_water_rate = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_water_rate == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	if (!f_open(f_txt_water_rate, "0:work/water_rate", FA_READ))
	{
		if (f_gets(water_rate, sizeof(water_rate), f_txt_water_rate) != NULL)
		{
			*rate = string_to_float(water_rate);
			ret = 0;
		}
		f_close(f_txt_water_rate);
	}
	else
	{
		ret = FILEOPT_ONFILE_ERR;
	}
	
TAR_OUT:
	
	if (f_txt_water_rate != NULL)
	{	
		myfree(f_txt_water_rate);
	}
	return ret;	
}


int fileopt_add_goods(char *goods_name, float *price)
{
	FIL *f_txt_goods = 0;
	char goods_msg[64] = {0};
	char filename[64] = {0};
	int ret;
	UINT iBw;
	int i;
	int total = 0;

	if (goods_name ==  NULL)
	{
		ret = FILEOPT_PARM_ERR;
		goto TAR_OUT;
	}
	
	f_txt_goods = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_goods == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	total = cur_parm.goods_list.total_num;
	if (total >= TOTAL_GOODS)
	{
		ret = FILEOPT_ARRAY_FULL;
		goto TAR_OUT;
	}
	
	for (i=0; i<TOTAL_GOODS; i++) 
	{
		if (strcmp(cur_parm.goods_list.goods[i].goods_name, goods_name) == 0)
		{
			ret = FILEOPT_EXIST;
			goto TAR_OUT;
		}
	}

	for (i=0; i<TOTAL_GOODS; i++) 
	{
		if (cur_parm.goods_list.goods[i].goods_name[0] == 0)
		{
			mymemcpy(cur_parm.goods_list.goods[i].goods_name, goods_name,
				sizeof(cur_parm.goods_list.goods[i].goods_name));
			cur_parm.goods_list.goods[i].price = *price;
			cur_parm.goods_list.total_num++;
			break;
		}
	}

	
	sprintf(filename, "0:%s", GOODS_LIST_FILE);
	ret = f_open(f_txt_goods, filename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	
	if (ret == 0)
	{
		sprintf(goods_msg, "%s:%.02f\r\n", goods_name, *price);
		f_lseek(f_txt_goods, f_txt_goods->fsize);
		f_write(f_txt_goods, goods_msg, strlen(goods_msg), &iBw);
		f_close(f_txt_goods);
		ret = 0;
	}
	else 
	{
		ret = FILEOPT_ONFILE_ERR;
	}

TAR_OUT:
	
	if (f_txt_goods != NULL)
	{	
		myfree(f_txt_goods);
	}
	return ret;
}

int fileopt_del_goods(char *goods_name)
{
	FIL *f_txt_goods = 0;
	char filename[64] = {0};
	int ret, i;
//	int total;
	UINT iBw;
	int flag = 0;
	char goods_msg[64] = {0};

	if (goods_name == NULL)
	{
		ret = FILEOPT_PARM_ERR;
	}

	for (i=0; i<TOTAL_GOODS; i++) {
		if (strcmp(goods_name, cur_parm.goods_list.goods[i].goods_name) == 0) {
			mymemset(cur_parm.goods_list.goods[i].goods_name, 0, 
				sizeof(cur_parm.goods_list.goods[i].goods_name));
			cur_parm.goods_list.goods[i].price = 0.0;
			cur_parm.goods_list.total_num--;
			flag = 1;
			break;
		} 
		
	}

	if (flag == 0)
	{
		ret = FILEOPT_NOT_FOUND;
		goto TAR_OUT;
	}

	f_txt_goods = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_goods == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}

	sprintf(filename, "0:%s", GOODS_LIST_FILE);
	if (!f_open(f_txt_goods, filename, FA_CREATE_ALWAYS | FA_WRITE))
	{
		for (i=0; i<TOTAL_GOODS; i++)
		{	
			if (cur_parm.goods_list.goods[i].goods_name[0] != 0) 
			{
				sprintf(goods_msg, "%s:%.02f\r\n", cur_parm.goods_list.goods[i].goods_name, 
					cur_parm.goods_list.goods[i].price);
				f_write(f_txt_goods, goods_msg, strlen(goods_msg), &iBw);

			}
		}
		f_close(f_txt_goods);
	}

TAR_OUT:

	if (f_txt_goods != NULL)
	{
		myfree(f_txt_goods);
	}
	return ret;
}

int fileopt_get_goods_price(char *goods_name, float *price)
{
	int total, i;
	int ret = FILEOPT_NOT_FOUND;

	if (goods_name ==  NULL)
	{
		ret = FILEOPT_PARM_ERR;
		goto TAR_OUT;
	}
	total = cur_parm.goods_list.total_num;

	for (i=0; i<total; i++)
	{
		if (strcmp(cur_parm.goods_list.goods[i].goods_name, goods_name) == 0)
		{
			*price = cur_parm.goods_list.goods[i].price;
			ret = 0;
			break;
		}
	}
	
TAR_OUT:
	
	return ret;
}

char fileopt_set_passwd(char *passwd, int passwd_len, char type)
{
	FIL *passwd_file = NULL;
	UINT bw;
	int ret = -1; 
	char passwd_buf[34] = {0};
	
	passwd_file= (FIL *)mymalloc(sizeof(FIL));
	if (passwd_file == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	
	if (!f_open(passwd_file, "0:passwd", FA_WRITE))
	{
		f_lseek(passwd_file, 34 * type);
		mymemset(passwd_buf, '*', sizeof(passwd_buf));
		passwd_buf[32] = '\r';
		passwd_buf[33] = '\n';
		mymemcpy(passwd_buf, passwd, passwd_len);
		f_write(passwd_file, passwd_buf, sizeof(passwd_buf), &bw);
		if (bw == 0)
		{
			ret = 0;
		}
		ret = 1;
		f_close(passwd_file);
	}
	
TAR_OUT:
	
	if (passwd_file != NULL)
	{
		myfree(passwd_file);
	}

	return ret;
}


char fileopt_get_passwd(char *root_passwd, int passwd_len, char type)
{
	FIL *passwd_file = NULL;
	//int bw;
	int ret = -1; 
	int i = 0;
	int flag = 0;
	char passwd_buf[34] = {0};
	char *p_char = NULL;

	passwd_file= (FIL *)mymalloc(sizeof(FIL));
	if (passwd_file == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	ret = f_open(passwd_file, "0:passwd", FA_READ);
	if (ret == 0)
	{
		f_lseek(passwd_file, 34 * type);
		p_char = f_gets(passwd_buf, 34, passwd_file);
		while(p_char != NULL && *p_char != '*' &&
			*p_char != '\r' && *p_char != '\n')
		{
			if (passwd_len > i)
			{
				root_passwd[i] = *p_char;
				flag = 1;
				i++;
			}
			p_char++;
		}
		if (flag == 1) 
		{
			ret = 0;
		}
		else 
		{
			ret = FILEOPT_NOT_FOUND;
		}
		
		f_close(passwd_file);
	}
	else 
	{
		ret = FILEOPT_ONFILE_ERR;
		goto TAR_OUT;
	}
	
TAR_OUT:
	
	if (passwd_file != NULL)
	{
		myfree(passwd_file);
	}
	return ret;
	
}


int fileopt_set_weight_unit(int type)
{
	FIL *weight_file = NULL;
	int ret;
	char buf[2];
	UINT bw;

	weight_file= (FIL *)mymalloc(sizeof(FIL));
	if (weight_file == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}	

	sprintf(buf, "%d", type);
	ret = f_open(weight_file, "0:work/weight_unit", FA_CREATE_ALWAYS | FA_WRITE);
	if (ret == 0)
	{
		f_write(weight_file, buf, sizeof(buf), &bw);
		f_close(weight_file);
	}


TAR_OUT:

	if (weight_file != NULL)
	{
		myfree(weight_file);
	}
	return ret;
}


int fileopt_get_weight_unit()
{
	FIL *weight_file = NULL;
	int ret;
	char buf[2];

	weight_file= (FIL *)mymalloc(sizeof(FIL));
	if (weight_file == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}	

	ret = f_open(weight_file, "0:work/weight_unit", FA_READ);
	if (ret == 0)
	{
		f_gets(buf, sizeof(buf), weight_file);
		ret = (int)string_to_float(buf);
		f_close(weight_file);
	} else {
		ret = 0;
	}


TAR_OUT:

	if (weight_file != NULL)
	{
		myfree(weight_file);
	}
	return ret;
}

int fileopt_set_work_goods(char *goods_name)
{
	FIL *work_goods_file = NULL;
	int ret;
	UINT bw;

	work_goods_file = (FIL *)mymalloc(sizeof(FIL));
	if (work_goods_file == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}	

	ret = f_open(work_goods_file, "0:work/work_goods", FA_CREATE_ALWAYS | FA_WRITE);
	if (ret == 0)
	{
		f_write(work_goods_file, goods_name, strlen(goods_name), &bw);
		f_close(work_goods_file);
	}


TAR_OUT:

	if (work_goods_file != NULL)
	{
		myfree(work_goods_file);
	}
	return ret;
}


int fileopt_get_work_goods(char *goods_name)
{
	FIL *work_goods_file = NULL;
	int ret;
	char buf[2];

	work_goods_file = (FIL *)mymalloc(sizeof(FIL));
	if (work_goods_file == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}	

	ret = f_open(work_goods_file, "0:work/work_goods", FA_READ);
	if (ret == 0)
	{
		f_gets(buf, sizeof(buf), work_goods_file);
		strcpy(goods_name, buf);
		f_close(work_goods_file);
	} else {
		ret = FILEOPT_NOT_FOUND;
	}


TAR_OUT:

	if (work_goods_file != NULL)
	{
		myfree(work_goods_file);
	}
	return ret;
}


int fileopt_del_work_goods(void)
{
	FIL *work_goods_file = NULL;
	int ret;

	work_goods_file = (FIL *)mymalloc(sizeof(FIL));
	if (work_goods_file == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}	

	ret = f_open(work_goods_file, "0:work/work_goods", FA_READ);
	if (ret == 0)
	{
		f_close(work_goods_file);
		f_unlink("0:work/work_goods");
	} else {
		ret = FILEOPT_NOT_FOUND;
	}


TAR_OUT:

	if (work_goods_file != NULL)
	{
		myfree(work_goods_file);
	}
	return ret;
}
 


int fileopt_write_bill_to_udev()
{
	FIL *f_txt_worker_list = 0;
	int ret = 0;
	char read_buf[50] = {0};
	char filename[32] = {0};
	char *bill_buf = NULL;
	char *p_str_tok = NULL;
	char *dele_str = "********";
	char first_line[80] = {0};
	char *second_line = "卡号,工号,姓名,总重量,总工资,本期重量,本期工资";

	f_txt_worker_list = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_worker_list == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}
	
	bill_buf = (char *)mymalloc(MAX_BUF_LEN);
	if (bill_buf == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}

	if (RTC_Get())
	{
		ret = FILEOPT_ONFILE_ERR;
		goto TAR_OUT;
	}

	sprintf(first_line, "                    %d-%d-%d %d:%d:%d                     ", 
		calendar.w_year, calendar.w_month, calendar.w_date,
		calendar.hour, calendar.min, calendar.sec);

	if (!WriteDataBufToUSB(first_line)) 
	{
		ret = FILEOPT_ONFILE_ERR;
		goto TAR_OUT;
	}
	
	if (!WriteDataBufToUSB(second_line)) 
	{
		ret = FILEOPT_ONFILE_ERR;
		goto TAR_OUT;
	}
	mymemset(f_txt_worker_list, 0, sizeof(FIL));
	sprintf(filename, "0:%s", WORKER_FILE);
	if (!f_open(f_txt_worker_list, filename, FA_WRITE | FA_READ))
	{
		while (f_gets(read_buf, sizeof(read_buf), f_txt_worker_list) != NULL)
		{
			if (strncmp(dele_str, read_buf, strlen(dele_str)))
			{
				char card_id[20] = {0};
				total_msg msg = {0};
				
				mymemset(bill_buf, 0, MAX_BUF_LEN);
				mymemset(&msg, 0, sizeof(total_msg));
				p_str_tok = strtok(read_buf, ":");
				sprintf(bill_buf, "%s,",p_str_tok);
				strcpy(card_id, p_str_tok);
				p_str_tok = strtok(NULL, ":");
				sprintf(bill_buf, "%s%s,", bill_buf, p_str_tok);
				p_str_tok = strtok(NULL, ":");
				sprintf(bill_buf, "%s%s,", bill_buf, p_str_tok);
				fileopt_read_total_msg(card_id, &msg);
				sprintf(bill_buf, "%s%s,", bill_buf, msg.lastest_total_weight);
				sprintf(bill_buf, "%s%s,", bill_buf, msg.lastest_total_price);
				sprintf(bill_buf, "%s%s,", bill_buf, msg.total_weight);
				sprintf(bill_buf, "%s%s", bill_buf, msg.total_price);
				
				fileopt_clear_lastest_data(card_id);
				if (!WriteDataBufToUSB((char *)bill_buf))
				{
					f_close(f_txt_worker_list);
					ret = FILEOPT_ONFILE_ERR;
					goto TAR_OUT;
				}
			}
		}
		
		f_close(f_txt_worker_list); 
	}
	
	
TAR_OUT:

	if (f_txt_worker_list != NULL)
	{
		myfree(f_txt_worker_list);
	}

	if (bill_buf != NULL)
	{
		myfree(bill_buf);
	}
	return ret;
}

int fileopt_write_list_to_udev()
{
	FIL *f_txt_worker_list = 0;
	int ret = 0;
	char read_buf[50] = {0};
	char filename[32] = {0};
	char list_buf[50] = {0};
	char *p_str_tok = NULL;
	char *dele_str = "********";
	char first_line[50] = {0};
	char *second_line = "卡号,工号,姓名";

	f_txt_worker_list = (FIL *)mymalloc(sizeof(FIL));
	if (f_txt_worker_list == NULL)
	{
		ret = FILEOPT_MALLOC_ERR;
		goto TAR_OUT;
	}

	if (RTC_Get())
	{
		ret = FILEOPT_ONFILE_ERR;
		goto TAR_OUT;
	}

	sprintf(first_line, "   %d-%d-%d %d:%d:%d   ", 
		calendar.w_year, calendar.w_month, calendar.w_date,
		calendar.hour, calendar.min, calendar.sec);

	if (!WriteDataBufToUSB(first_line))
	{
		ret = FILEOPT_ONFILE_ERR;
		goto TAR_OUT;
	}

	if (!WriteDataBufToUSB(second_line))
	{
		ret = FILEOPT_ONFILE_ERR;
		goto TAR_OUT;
	}

	mymemset(f_txt_worker_list, 0, sizeof(FIL));	
	sprintf(filename, "0:%s", WORKER_FILE);
	if (!f_open(f_txt_worker_list, filename, FA_WRITE | FA_READ))
	{
		while (f_gets(read_buf, sizeof(read_buf), f_txt_worker_list) != NULL)
		{
			if (strncmp(dele_str, read_buf, strlen(dele_str)))
			{
				mymemset(list_buf, 0, 50);
				p_str_tok = strtok(read_buf, ":");
				sprintf(list_buf, "%s",p_str_tok);
				p_str_tok = strtok(NULL, ":");
				sprintf(list_buf, "%s,%s", list_buf, p_str_tok);
				p_str_tok = strtok(NULL, ":");
				sprintf(list_buf, "%s,%s", list_buf, p_str_tok);
				if (!WriteDataBufToUSB((char *)list_buf))
				{
					f_close(f_txt_worker_list);
					ret = FILEOPT_ONFILE_ERR;
					goto TAR_OUT;
				}
			}
		}
		
		f_close(f_txt_worker_list); 
	}
	
	
TAR_OUT:

	if (f_txt_worker_list != NULL)
	{
		myfree(f_txt_worker_list);
	}
	return ret;
}

int fileopt_copy_sd_to_flash(void)
{
	int ret = 0;
	u32 cur_count = RTC_GET_COUNT();

	if (cur_count < cur_parm.last_count)
	{
		cur_parm.last_count = cur_count;
	} 
	else if (cur_count - cur_parm.last_count >= FILE_SYN_TIME)
	{
		//同步
	}
	
	return ret;
}

int fileopt_copy_flash_to_sd(void)
{
	
}

