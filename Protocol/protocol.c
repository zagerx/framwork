#include "fifo.h"
#include "fsm.h"
#include "protocol.h"
#include "fifo.h"
#include "stdlib.h"
#include "crc.h"
#include "string.h"
#include "ipc.h"
#include "usart.h"
#include "macro_defined.h"

extern void protocol_trans_process(void);

/*Э��Ҫ�õ������
1��fifo.c/.h
2��fsm.h
*/
unsigned char fifo_receive_buff[PRO_FRAME_MAX_SIZE];//fifo���ݻ�����
byte_fifo_t uart1_rx_fifo;//fifo���ƿ�

/*********************************************************************************************************
** Function name(��������):				protocol_reciver_datafram()
**
** Descriptions��������:				����һ֡����
**
** input parameters�����������:		len:һ֡���ݳ���(����֡ͷ��֡β)
** Returned value������ֵ��:				0:�ɹ�
**         
** Used global variables��ȫ�ֱ�����:	None
** Calling modules������ģ�飩:			fifo.c/h crc.c/.h
**
** Created by�������ˣ�:				zager
** Created Date���������ڣ�:			2023-08-14
**
********************************************************************************************************/
signed char protocol_reciver_datafram(unsigned char *pdata,unsigned short len)
{
	bytefifo_writemulitebyge(&uart1_rx_fifo,pdata,len);
	return 0;

}

/*�������*/
pro_frame_t* pro_frame_packet(unsigned short cmd,void *pdata,unsigned short len)
{
    unsigned char *puctemp;
    pro_frame_t *pfram;
    puctemp = malloc(sizeof(pro_frame_t) + len);
    pfram = (pro_frame_t *)puctemp;
    pfram->_x_x = 0x00;
    pfram->head = __SWP16(PRO_FRAME_HEAD);
    pfram->tail = __SWP16(PRO_FRAME_TAIL);
    pfram->func_c = __SWP16(cmd);
    pfram->len = __SWP16(len);
    pfram->pdata = &puctemp[sizeof(pro_frame_t)];
    if(pdata != 0)
    {
        memcpy((unsigned char*)&puctemp[sizeof(pro_frame_t)],(unsigned char*)pdata,len);
    }    
    return pfram;
}

pro_frame_t* pro_frame_unpack(unsigned char *pdata,unsigned short len)
{
    /*�����ڴ�*/
    unsigned char *pr_buf_1;
    unsigned short r_fram_len;
    unsigned short r_data_len;
    unsigned short r_crc_16;
    r_fram_len = len;
    pr_buf_1 = malloc(r_fram_len);
    memcpy(pr_buf_1,pdata,r_fram_len);
    r_crc_16 = CRC16_Subsection((unsigned char *)pr_buf_1,0xFFFF,r_fram_len-4);
    if(r_crc_16 != (pr_buf_1[r_fram_len-4]<<8 | pr_buf_1[r_fram_len-3]))
    {
        free(pr_buf_1);
        return NULL;
    }
    /*У��ɹ�*/
    r_data_len =  pr_buf_1[4]<<8 | pr_buf_1[5];   //��ȡ���ݳ���
    unsigned char *pr_buf_2;
    pr_buf_2 = 0;
    pr_buf_2 = malloc(sizeof(pro_frame_t)+r_data_len);//��������֡�ڴ�
    memcpy(&pr_buf_2[2],pr_buf_1,r_fram_len);
    free(pr_buf_1);

    unsigned char *pr_buf_data;
    pr_buf_data = malloc(r_data_len);
    if (pr_buf_data == NULL)
    {
        free(pr_buf_data);
        return NULL;
    }
    memcpy(pr_buf_data,&pr_buf_2[FRAM_PDATA_OFFSET],r_data_len);   
    memcpy(&pr_buf_2[FRAM_CRC_OFFSET],&pr_buf_2[FRAM_PDATA_OFFSET+r_data_len],4);
    memcpy(&pr_buf_2[FRAM_BUF_OFFSET],pr_buf_data,r_data_len);    
    free(pr_buf_data);

    pro_frame_t *p_r_fram;
    p_r_fram = (pro_frame_t *)pr_buf_2;
    p_r_fram->pdata = &pr_buf_2[FRAM_BUF_OFFSET];

    return p_r_fram;
}

void protocol_init(void)
{
    bytefifo_init(&uart1_rx_fifo,fifo_receive_buff,sizeof(fifo_receive_buff));
    ipc_msgpool_init();
}

void protocol_process(void)
{
    protocol_parse();
    protocol_trans_process();
}

