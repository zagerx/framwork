/**------------------------------------------------------------------------------------------------------
** Created by:			
** Created date:		2023-08-17
** Version:				1.0
** Descriptions:		
**
**------------------------------------------------------------------------------------------------------
** Modified by:			zager
** Modified date:		2023-08-17
** Version:
** Descriptions:	Э�����
**
********************************************************************************************************/
#include "./protocol.h"
#include "fifo.h"
#include "fsm.h"
#include "usart.h"

#include "malloc.h"

extern byte_fifo_t uart1_rx_fifo;//fifo���ƿ�


static char get_fram_process(unsigned char *pbuf)
{
    enum {
        START,
        FIRST ,
        SECOND,
        IDLE ,			
    };
    unsigned char data;    
    static unsigned short index = 0;		    
    unsigned char chState;
    unsigned short fram_len = 0;        
    chState = START;
    switch (chState)
    {
    case START:
        chState = FIRST;
    case FIRST:
        bytefifo_readmulintebyte(&uart1_rx_fifo,&data,1);
        if(data == (unsigned char)(PRO_FRAME_HEAD>>8))
        {
            pbuf[index++] = data;
            chState = SECOND;
        }else{
            index = 0;
            chState = START;
            break;
        }
    case SECOND:
        bytefifo_readmulintebyte(&uart1_rx_fifo,&data,1);
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
                bytefifo_readmulintebyte(&uart1_rx_fifo,&data,1);
                pbuf[index++] = data;
                if(data == (unsigned char)(PRO_FRAME_TAIL>>8))
                {
                    bytefifo_readmulintebyte(&uart1_rx_fifo,&data,1);
                    pbuf[index++] = data;
                    if (data == (unsigned char)(PRO_FRAME_TAIL>>8))
                    {
                        /* code */
                        fram_len = index;
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


#define __SWP16(A)   (( ((unsigned short)(A) & 0xff00) >> 8)    | \
                                        (( (unsigned short)(A) & 0x00ff) << 8))  
 
#define __SWP32(A)   ((( (unsigned int)(A) & 0xff000000) >> 24) | \
                                        (( (unsigned int)(A) & 0x00ff0000) >> 8)   | \
                                        (( (unsigned int)(A) & 0x0000ff00) << 8)   | \
                                        (( (unsigned int)(A) & 0x000000ff) << 24))

/*IPC��Ϣ���*/
mesg_t* ipc_mesg_packet(unsigned short id,unsigned short len)
{
    unsigned char* pbuf;
    pbuf = (unsigned char*)malloc(len);
    /*��ʼ���*/
    mesg_t *pMsg;
    pMsg = (mesg_t *)&pbuf[0];
    pMsg->id = 0x01;
    pMsg->len = len-sizeof(mesg_t);
    pMsg->pdata = &pbuf[sizeof(mesg_t)];
    return pMsg;
}

/*�����ת��*/
unsigned char* pro_frame_packet(unsigned short cmd,void *pdata,unsigned char len)
{
    unsigned char *puctemp;
    pro_frame_t *pfram;
    puctemp = malloc(sizeof(pro_frame_t) + len);
    pfram = (pro_frame_t *)puctemp;
    pfram->head = __SWP16(PRO_FRAME_HEAD);
//    pfram->tail = __SWP16(PRO_FRAME_TAIL);
    pfram->func_c = __SWP16(cmd);
    pfram->len = __SWP16(len);
    if(pdata != 0)
    {
        memcpy((unsigned char*)&puctemp[FRAM_PDATA_OFFSET],(unsigned char*)pdata,len);
    }


    
    unsigned short crc_16;
    crc_16 = CRC16_Subsection(&puctemp[2],0xFFFF,FRAM_PDATA_OFFSET+len-2);
    crc_16 = __SWP16(crc_16);
    memcpy((unsigned char*)&puctemp[FRAM_PDATA_OFFSET+len],(unsigned char*)&crc_16,sizeof(crc_16));
    unsigned short tail_ = __SWP16(PRO_FRAME_TAIL);
    memcpy((unsigned char*)&puctemp[FRAM_PDATA_OFFSET+len+2],(unsigned char*)&tail_,sizeof(tail_));

    return puctemp;
}    






/*********************************************************************************************************
** Function name(��������):						protocol_parse()
**
** Descriptions��������:							Э�����
**
** input parameters�����������:			None
** Returned value������ֵ��:					None
**         
** Used global variables��ȫ�ֱ�����:	None
** Calling modules������ģ�飩:				fifo.c/h
**
** Created by�������ˣ�:							zager
** Created Date���������ڣ�:					2023-08-17
**
********************************************************************************************************/
void protocol_parse(void)
{
	unsigned char buf[PRO_FRAME_MAX_SIZE];
	unsigned char cmd;
    unsigned char len;
    float data_buf[2] = {6.4f,2.3f};
    unsigned char data_len;
	/*��ȡ1֡����*/
	// if(get_fram_process(&get_fram_cb) != fsm_rt_cpl)
	// {
	// 	return;
	// }
	if(get_fram_process(&buf[0]) != 0)
	{
		return;
	}    
	/*��ʼУ��*/
	
	/*��ʼ����*/
    cmd = buf[2];
	switch (cmd)
	{
		case 0x02:
            /*����Ҫ���͵����ݳ���*/        
            data_len = sizeof(data_buf);
            /*������Ϣ�������С*/
            len =sizeof(mesg_t) + sizeof(pro_frame_t) + data_len;
            /*������Ϣ��С*/
            unsigned char *puctemp;
            mesg_t *p_msg;

            p_msg = ipc_mesg_packet(0x01,len);
        
            puctemp = pro_frame_packet(PRO_FUNC_C_PF300 | CMD_RESP,data_buf,data_len);           
            memcpy((unsigned char*)p_msg->pdata,puctemp,sizeof(pro_frame_t) + data_len);
            free(puctemp);
            /*��ӵ���Ϣ����*/
            mesgqueue_write(p_msg);
            SET_IPC_EVENT(PROTOCOL_CMD_01);//֪ͨ����
        
            // HAL_UART_Transmit(&huart1,(unsigned char*)p_msg->pdata,sizeof(pro_frame_t)+2,0XFFFF);
            // free(p_msg);        
			break;
		case 0x01:
			break;
		case 0x03:
			USER_DEBUG("recive cmd 03\r\n");
			break;
		case 0x04://������ʪ�����ݸ����������ȴ�������Ӧ
			USER_DEBUG("recive cmd 04\r\n");
			break;
		case 0x05:
			USER_DEBUG("recive cmd 05\r\n");
			break;
		case 0x0B:
			USER_DEBUG("recive cmd 06\r\n");
			break;		
		default:
			break;
	}
}





