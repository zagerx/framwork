/**------------------------------------------------------------------------------------------------------
** Created by:			
** Created date:		2023-08-17
** Version:				1.0
** Descriptions:		
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		2023-08-17
** Version:
** Descriptions:	先进先出队列
**
********************************************************************************************************/
#include "fifo.h"

#if 0
#define FIFO_ROW 5
#define FIFO_COL 32
static unsigned char Fifo_buf[FIFO_ROW][FIFO_COL];
static unsigned char tail;
static unsigned char head;
static volatile signed char idle_numb;
void fifo_init(void)
{
	tail = 0;
	head = 0;
	idle_numb = FIFO_ROW;
}

static signed char fifo_isempty(void)
{
	if(!idle_numb)
	{/*队列已满*/
		return -1;
	}
	return 0;
}

signed char bytefifo_writemulitebyge(unsigned char *pdata,unsigned short len)
{
	unsigned char i = 0;
	 
	if(len>FIFO_COL || fifo_isempty())
	{
		goto out;
	}
	while(len--)
	{
		Fifo_buf[head][i] = *pdata;
		i++;
		pdata++;
	}
	head++;
	if(head>=FIFO_ROW)
	{
		head = 0;
	}
	idle_numb--;
	return 0;
	out:
//		printf("fifo_write err\r\n");
		return -1;
}
signed char bytefifo_readmulintebyte(unsigned char *pdata,unsigned short len)
{
	unsigned char i = 0;
	if(idle_numb>=5)
	{/*没有数据可读*/
		return -1;
	}
	while(len--)
	{
		*pdata = Fifo_buf[tail][i];
		i++;
		pdata++;
	}
	tail++;
	if(tail >= FIFO_ROW)
	{
		tail = 0;
	}
	if(idle_numb++>5)
	{
		idle_numb = 5;
	}
	return 0;
}
#else
#include "stdio.h"
#define BYTEFIFO_BUF_SIZE 256
static unsigned short head;
static unsigned short tail;
static byte_t bytefifo_buf[BYTEFIFO_BUF_SIZE];
static volatile unsigned short fifo_remain_byte;//可用字节

static char bytefifo_isfull(void)
{
	if((head+1)%BYTEFIFO_BUF_SIZE == tail)
	{
		return 0;//已满
	}
	return 1;
}
/*FIFO空，不能读数据*/
static char bytefifo_isempty(void)
{
	if(head == tail)
	{
		return 0;//空
	}
	return 1;
}
/*FIFO满不能写数据*/
static char bytefifo_writebyte(byte_t data)
{
    if(bytefifo_isfull() == 0)
    {
        return 1;//满
    }
    bytefifo_buf[head] = data;
    head = (head + 1) % BYTEFIFO_BUF_SIZE;
		fifo_remain_byte--;
    return 0;//写入成功
}
static char bytefifo_readbyte(byte_t *pdata)
{/*FIFO为空 不能读*/
    if (bytefifo_isempty() == 0)
    {
        return 1;//无法读取
    }
		*pdata = bytefifo_buf[tail];
    tail = (tail + 1)%BYTEFIFO_BUF_SIZE;
		fifo_remain_byte++;
		return 0;
}

/*获取FIFO已使用的字节数*/
static unsigned short bytefifo_usendbyte(void)
{
	if(tail <= head)
	{
//		cur_counter = ;
		return (head-tail);		
	}else{
		return (BYTEFIFO_BUF_SIZE + head - tail);
	}
}
#if 0 /*打印测试函数*/
	/* 打印当前FIFO内的数据和读写指针的位置 */
	int queue_print(void)
	{
		
		int i = 0;
		int j = 0;

		for(i = 0; i < tail; i++)
			printf("     ");

		printf("rd=%d", tail);
		printf("\n");

		for(i = 0; i < BYTEFIFO_BUF_SIZE; i++)
		{
			if(head > tail)
			{
				if(i >= tail && i < head)
					printf("[%02x] ", bytefifo_buf[i]);
				else
					printf("[  ] ");
			}
			else//addr_rd > addr_wr
			{
				if(i < head || i >= tail)
					printf("[%02x] ", bytefifo_buf[i]);
				else
					printf("[  ] ");
			}
		}
		printf("------count = %d\n", bytefifo_usendbyte());

		for(i = 0; i < head; i++)
			printf("     ");

		printf("wr=%d", head);
		printf("\n");
		
		return 0;
	}
#endif
void bytefifo_init(void)
{
    head = 0;
    tail = 0;
		fifo_remain_byte = BYTEFIFO_BUF_SIZE;
}
char bytefifo_writemulitebyge(byte_t *buf,unsigned short len)
{
	byte_t *pw;
	pw = buf;
	if(len > (BYTEFIFO_BUF_SIZE - bytefifo_usendbyte()-1))//-1，因为有一个字节始终没有用到
	{
		printf("mulitebyte write 	fail\r\n");
		return 1;//写入失败
	}
	while(len--)
	{
		bytefifo_writebyte(*pw);
		pw++;
	}
	return 0;
}
char bytefifo_readmulintebyte(byte_t *buf,unsigned short len)
{
	byte_t temp = 0;
	char statues = 1;
	if(len > bytefifo_usendbyte())//
	{
//		printf("mulitebyte read fail   usedbyte = %d\r\n",bytefifo_usendbyte());
		return 1;//读取失败
	}
	while(len--)
	{
		statues = bytefifo_readbyte(&temp);
		*buf++ = temp;
	}
	return statues;
}
#endif


/*
** 消息队列
*/
#if 0
	#define MESG_BUF_SIZE 10
	#define MESG_SIZE 		8
	typedef struct _mesg{
		unsigned char ID;
		unsigned char buf[MESG_SIZE];
		unsigned short len;
	}mesg_t;
	mesg_t mesgbuf[MESG_BUF_SIZE];

	unsigned char mesgfifo_head;
	unsigned char mesgfifo_tail;

	void mesgfifo_init(void)
	{
		mesgfifo_head = 0;
		mesgfifo_tail = 0;
	}

	static char mesgfifo_isfull(void)
	{
		if((mesgfifo_head+1)%BYTEFIFO_BUF_SIZE == mesgfifo_tail)
		{
			return 0;//已满
		}
		return 1;
	}
	/*FIFO空，不能读数据*/
	static char mesgfifo_isempty(void)
	{
		if(mesgfifo_head == mesgfifo_tail)
		{
			return 0;//空
		}
		return 1;
	}


	char mesgfifo_readmes(mesg_t* mesg)
	{
		if (mesgfifo_isempty() == 0)
		{
			return 1;//无法读取
		}
			mesg = &mesgbuf[mesgfifo_tail];
			mesgfifo_tail = (mesgfifo_tail + 1)%BYTEFIFO_BUF_SIZE;
			return 0;
	}

	char mesgfifo_writemes(mesg_t* mesg)
	{
		if(mesgfifo_isfull() == 0)
		{
			return 1;//满
		}
			for(unsigned char i = 0;i<mesg->len;i++)
			{
				mesgbuf[mesgfifo_head].buf[i] = mesg->buf[i];
			}
		
			mesgbuf[mesgfifo_head].ID = mesg->ID;
			mesgbuf[mesgfifo_head].len = mesg->len;
			
		mesgfifo_head = (mesgfifo_head + 1) % BYTEFIFO_BUF_SIZE;
		return 0;//写入成功	
	}
#endif
