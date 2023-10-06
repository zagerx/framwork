/** Modified by:			zager
** Modified date:		2023-08-17
** Version:
** Descriptions:	协议解析
**
********************************************************************************************************/
#include "./protocol.h"
#include "fifo.h"
#include "fsm.h"
#include "usart.h"

#include "malloc.h"
#include "string.h"
#include "stdlib.h"
extern byte_fifo_t uart1_rx_fifo;//fifo控制块

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
                        chState = START;//获取到完整的数据帧  复位状态机
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
** Function name(函数名称):						protocol_parse()
**
** Descriptions（描述）:							协议解析
**
** input parameters（输入参数）:			None
** Returned value（返回值）:					None
**         
** Used global variables（全局变量）:	None
** Calling modules（调用模块）:				fifo.c/h
**
** Created by（创建人）:							zager
** Created Date（创建日期）:					2023-08-17
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
    unsigned short msg_id = 0 ;
	/*读取1帧数据*/

	if(get_fram_process(&fram_buf[0]) != 0)
	{
		return;
	}
    pro_frame_t * p_r_fram;
    p_r_fram = pro_frame_unpack((unsigned char*)fram_buf,fram_len);
    if (!p_r_fram)
    {
        /* code */
        return;
    }
    
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
		case 0x04://测试list
			// USER_DEBUG_RTT("recive cmd 04\r\n");
            data_len = 0;
            /*计算消息的整体大小*/
            len =sizeof(msg_t) + sizeof(pro_frame_t) + data_len;
            /*计算消息大小*/
            msg_id = rand()%5;
            // msg_id = 2;
            p_msg = ipc_mesg_packet(msg_id,len);
            puctemp = (unsigned char*)pro_frame_packet_sigle(PRO_FUNC_C_PF300 | (CMD_RESP<<8),data_buf,data_len);
            memcpy((unsigned char*)p_msg->pdata,puctemp,sizeof(pro_frame_t) + data_len);
            free(puctemp);
            /*添加到消息池*/
            ipc_msgpool_write(p_msg);
			break;

 		case 0x07:
            {
                // USER_DEBUG_RTT("***********del befor*************\r\n");
                // ipc_msg_printf_number();
                // ipc_msg_printf();
                // USER_DEBUG_RTT("*********************************\r\n");
                msg_t *p_readMsg,temp;
                unsigned char _rands;
                _rands = rand()%5;
                // _rands = 2;
                USER_DEBUG_RTT("del node msg id %d\r\n",_rands);
                temp.id = _rands;
                p_readMsg = &temp;
                p_readMsg = (msg_t *)ipc_msgpool_read_val(p_readMsg);
                static unsigned short cnt;
                USER_DEBUG_RTT("cnt %d\r\n",cnt++);
                if (p_readMsg == NULL)
                {
                    /* code */
                    USER_DEBUG_RTT("del readmsg empty\r\n");
                    free(p_readMsg);
                    break;
                }

                ipc_msgpool_del(p_readMsg);
                free(p_readMsg);
                // USER_DEBUG_RTT("=================del end=============\r\n");
                // ipc_msg_printf_number();
                // ipc_msg_printf();
                // USER_DEBUG_RTT("=======================================\r\n");                
            }
			break;           
		case 0x06:
            /*添加数据到链表并打印*/
            ipc_msg_printf();
			break;		
		default:
			break;
	}

    free(p_r_fram);
}





