
#include "protocol_comment.h"
#include "protocol_cfg.h"

/*-------------------------------FIFO���------------------------------------------*/
static unsigned char fifo_receive_buff[PRO_FRAME_MAX_SIZE];//fifo���ݻ�����
static byte_fifo_t protocol_fifo_handle;//fifo���ƿ�

/*********************************************************************************************************
** Function name(��������):				protocol_reciverdata_tofifo()
**
** Descriptions��������:				����һ֡���ݸ�FIFO���
**
** input parameters�����������:		len:һ֡���ݳ���(����֡ͷ��֡β)
** Returned value������ֵ��:				0:�ɹ�
**
** Calling modules������ģ�飩:			fifo.c/h
********************************************************************************************************/
unsigned char protocol_reciverdata_tofifo(unsigned char *pdata,unsigned short len)
{
	bytefifo_writemulitebyge(&protocol_fifo_handle,pdata,len);
	return 0;
}
char _get_framedata_fromfifo(unsigned char *pbuf,unsigned short *pframe_len)
{
    enum {
        START,
        FIRST ,
        SECOND,
        IDLE ,			
    };
    unsigned short index = 0;	
    unsigned char data;
    unsigned char chState = START;
           
    switch (chState)
    {
    case START:
        chState = FIRST;
    case FIRST:
        bytefifo_readmulintebyte(&protocol_fifo_handle,&data,1);
        if(data == (unsigned char)(PRO_FRAME_HEAD>>8))
        {
            pbuf[index++] = data;
            data = 0;
            chState = SECOND;
        }else{
            index = 0;
            chState = START;
            break;
        }
    case SECOND:
        bytefifo_readmulintebyte(&protocol_fifo_handle,&data,1);
        if(data == (unsigned char)(PRO_FRAME_HEAD))
        {
            chState = IDLE;
            pbuf[index++] = data;
        }else{
            chState = START;
            break;
        }
        case IDLE:
            do
            {
                if(index >= PRO_FRAME_MAX_SIZE)
                {
                    /* code */
                    *pframe_len = 0;
                    index = 0;
                    chState = START;//��ȡ������������֡  ��λ״̬��
                    return 1;
                }
                bytefifo_readmulintebyte(&protocol_fifo_handle,&data,1);
                pbuf[index++] = data;
                if(data == (unsigned char)(PRO_FRAME_TAIL>>8))
                {
                    data = 0;
                    bytefifo_readmulintebyte(&protocol_fifo_handle,&data,1);
                    pbuf[index++] = data;
                    if (data == (unsigned char)(PRO_FRAME_TAIL>>8))
                    {
                        /* code */
                        *pframe_len = index;
                        index = 0;
                        chState = START;//��ȡ������������֡  ��λ״̬��
                        return 0;
                    }
                }
            } while (1);
            break;
    default:
        break;
    }
    return 1;
}

/*Э��������*/
pro_frame_t* _packet_proframe(unsigned short cmd,void *pdata,unsigned short len)
{
    unsigned char *puctemp;
    pro_frame_t *pfram;
    puctemp = malloc(sizeof(pro_frame_t) + len);
    if(!puctemp)
    {
        free(puctemp);
        return NULL;
    }
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
/*Э��������*/
pro_frame_t* _unpack_proframe(unsigned char *pdata,unsigned short len)
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
/*��fifoģ���ȡָ������*/








void protocol_init(void)
{
    bytefifo_init(&protocol_fifo_handle,fifo_receive_buff,sizeof(fifo_receive_buff));
}

/*
process:�߳�
parse:����
*/
void protocol_process(void)
{
    protocol_parse();
    // protocol_trans_process();
}

