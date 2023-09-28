/** Modified by:			zager
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
#include "string.h"
extern byte_fifo_t uart1_rx_fifo;//fifo���ƿ�

unsigned short fram_len = 0; 
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
	unsigned char fram_buf[PRO_FRAME_MAX_SIZE];
	unsigned char cmd;
    unsigned char len;
    float data_buf[2] = {6.4f,2.3f};
    unsigned char data_len;
	/*��ȡ1֡����*/

	if(get_fram_process(&fram_buf[0]) != 0)
	{
		return;
	}
    // pro_frame_t * p_r_fram;
    // p_r_fram = pro_frame_unpack((unsigned char*)fram_buf,fram_len);

    // USER_DEBUG_RTT("fram->head 0x%x\r\n",__SWP16(p_r_fram->head));
    // USER_DEBUG_RTT("fram->crc  0x%x\r\n",__SWP16(p_r_fram->crc16));
    // USER_DEBUG_RTT("fram->tail 0x%x\r\n",__SWP16(p_r_fram->tail));
    // float *pf1;
    // pf1 = (float *)p_r_fram->pdata;
    // USER_DEBUG_RTT("fram->data %f\r\n",pf1[0]);
    // USER_DEBUG_RTT("fram->data %f\r\n",pf1[1]);
    // free(p_r_fram);
    cmd = fram_buf[2];
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
            puctemp = (unsigned char*)pro_frame_packet_sigle(PRO_FUNC_C_PF300 | CMD_RESP,data_buf,data_len);
            memcpy((unsigned char*)p_msg->pdata,puctemp,sizeof(pro_frame_t) + data_len);
            free(puctemp);
            /*��ӵ���Ϣ��*/
            // mesgqueue_write(p_msg);
            ipc_msgpool_write(p_msg);        
            SET_IPC_EVENT(PROTOCOL_CMD_01);//֪ͨ����
			break;
		case 0x01:
			break;
		case 0x03:
			USER_DEBUG_RTT("recive cmd 03\r\n");
			break;
		case 0x04://������ʪ�����ݸ����������ȴ�������Ӧ
			USER_DEBUG_RTT("recive cmd 04\r\n");
			break;
		case 0x05:
			USER_DEBUG_RTT("recive cmd 05\r\n");
			break;
		case 0x0B:
			USER_DEBUG_RTT("recive cmd 06\r\n");
			break;		
		default:
			break;
	}
}





