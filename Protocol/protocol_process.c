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
	// unsigned char cmd;
    unsigned char len;
    float data_buf[2] = {6.4f,2.3f};
    unsigned char data_len;
    unsigned char *puctemp;
    msg_t *p_msg;
    static unsigned short msg_id = 3;
	/*��ȡ1֡����*/

	if(get_fram_process(&fram_buf[0]) != 0)
	{
		return;
	}
    pro_frame_t * p_r_fram;
    p_r_fram = pro_frame_unpack((unsigned char*)fram_buf,fram_len);

    // USER_DEBUG_RTT("fram->head 0x%x\r\n",__SWP16(p_r_fram->head));
    // USER_DEBUG_RTT("fram->crc  0x%x\r\n",__SWP16(p_r_fram->crc16));
    // USER_DEBUG_RTT("fram->cmd  0x%x\r\n",__SWP16(p_r_fram->func_c));
    // USER_DEBUG_RTT("fram->tail 0x%x\r\n",__SWP16(p_r_fram->tail));
    // float *pf1;
    // pf1 = (float *)p_r_fram->pdata;
    // USER_DEBUG_RTT("fram->data %f\r\n",pf1[0]);
    // USER_DEBUG_RTT("fram->data %f\r\n",pf1[1]);
    // free(p_r_fram);
    // cmd = fram_buf[2];

    unsigned short cmd = __SWP16(p_r_fram->func_c);
    unsigned char cmd_fun = (unsigned char)cmd;
    unsigned char cmd_type = (unsigned char)(cmd>>8);

    // USER_DEBUG_RTT("cmd  0x%x\r\n",cmd);
    // USER_DEBUG_RTT("cmd_fun  0x%x\r\n",cmd_fun);
    // USER_DEBUG_RTT("cmd_type  0x%x\r\n",cmd_type);
	switch (cmd_fun)
	{
		case 0x02:
            /*����Ҫ���͵����ݳ���*/        
            data_len = sizeof(data_buf);
            /*������Ϣ�������С*/
            len =sizeof(msg_t) + sizeof(pro_frame_t) + data_len;
            /*������Ϣ��С*/
            p_msg = ipc_mesg_packet(0x01,len);
            puctemp = (unsigned char*)pro_frame_packet_sigle(PRO_FUNC_C_PF300 |(CMD_RESP<<8),data_buf,data_len);
            memcpy((unsigned char*)p_msg->pdata,puctemp,sizeof(pro_frame_t) + data_len);
            free(puctemp);
            /*��ӵ���Ϣ��*/
            ipc_msgpool_write(p_msg);        
            SET_IPC_EVENT(PROTOCOL_CMD_01);//֪ͨ����
			break;
		case 0x0A:
            /*�ӻ��Ļظ�ACK ��������*/
            if(cmd_type == 0x00)//����
            {

            }else if (cmd_type == 0x01)//�����Ӧ��
            {
                // USER_DEBUG_RTT("pf300  ack\r\n");
                /*����Ҫ���͵����ݳ���*/        
                data_len = 0;
                /*������Ϣ�������С*/
                len =sizeof(msg_t) + sizeof(pro_frame_t) + data_len;
                /*������Ϣ��С*/
                p_msg = ipc_mesg_packet(0x01,len);
                puctemp = (unsigned char*)pro_frame_packet_sigle(PRO_FUNC_C_PF300 |(CMD_RESP<<8),data_buf,data_len);
                memcpy((unsigned char*)p_msg->pdata,puctemp,sizeof(pro_frame_t) + data_len);
                free(puctemp);
                /*��ӵ���Ϣ��*/
                ipc_msgpool_write(p_msg);
                
            }else if(cmd_type == 0x02)//����Ļظ�
            {

            }else if(cmd_type == 0x03)//�������Ӧ
            {}
			break;
		case 0x03://������ȡPF300ָ��
            // USER_DEBUG_RTT("pf300  ack\r\n");
            /*����Ҫ���͵����ݳ���*/
            data_len = 0;
            /*������Ϣ�������С*/
            len =sizeof(msg_t) + sizeof(pro_frame_t) + data_len;
            /*������Ϣ��С*/
            p_msg = ipc_mesg_packet(0x01,len);
            puctemp = (unsigned char*)pro_frame_packet_sigle(PRO_FUNC_C_PF300 | (CMD_RESP<<8),data_buf,data_len);
            memcpy((unsigned char*)p_msg->pdata,puctemp,sizeof(pro_frame_t) + data_len);
            free(puctemp);
            /*��ӵ���Ϣ��*/
            ipc_msgpool_write(p_msg);
			break;
		case 0x04://����list
			// USER_DEBUG_RTT("recive cmd 04\r\n");
            data_len = 0;
            /*������Ϣ�������С*/
            len =sizeof(msg_t) + sizeof(pro_frame_t) + data_len;
            /*������Ϣ��С*/
            p_msg = ipc_mesg_packet(msg_id++,len);
            puctemp = (unsigned char*)pro_frame_packet_sigle(PRO_FUNC_C_PF300 | (CMD_RESP<<8),data_buf,data_len);
            memcpy((unsigned char*)p_msg->pdata,puctemp,sizeof(pro_frame_t) + data_len);
            free(puctemp);
            /*��ӵ���Ϣ��*/
            ipc_msgpool_write(p_msg);

			break;
		case 0x05:
            {
                msg_t *p_readMsg;
                p_readMsg = (msg_t *)ipc_msgpool_read();
                ipc_msgpool_del(p_readMsg);
            }
			break;
 		case 0x07:
            {
                msg_t *p_readMsg,temp;
                temp.id = 5;
                p_readMsg = &temp;
                p_readMsg = (msg_t *)ipc_msgpool_read_val(p_readMsg);
                ipc_msgpool_del(p_readMsg);
            }
			break;           
		case 0x06:
            /*������ݵ�������ӡ*/
            ipc_msg_printf();
			break;		
		default:
			break;
	}
}





