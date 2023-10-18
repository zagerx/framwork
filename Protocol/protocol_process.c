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
#include "macro_defined.h"

extern byte_fifo_t uart1_rx_fifo;//fifo控制块

unsigned char fram_buf[PRO_FRAME_MAX_SIZE];
static unsigned short fram_len = 0; 
static char _get_data_fram(unsigned char *pbuf)
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
            data = 0;
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
                if(index>=sizeof(fram_buf))
                {
                    /* code */
                    fram_len = 0;
                    index = 0;
                    chState = START;//获取到完整的数据帧  复位状态机
                    return 1;
                }
                bytefifo_readmulintebyte(&uart1_rx_fifo,&data,1);
                pbuf[index++] = data;
                if(data == (unsigned char)(PRO_FRAME_TAIL>>8))
                {
                    data = 0;
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
    unsigned char len;
    float data_buf[2] = {6.4f,2.3f};
    unsigned char data_len;
    unsigned char *puctemp;
    msg_t *p_msg;
    unsigned short msg_id = 0 ;
	/*读取1帧数据*/
	if(_get_data_fram(&fram_buf[0]) != 0)
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
    float *pf1;
    pf1 = (float *)p_r_fram->pdata;
    unsigned short cmd = __SWP16(p_r_fram->func_c);
    unsigned char cmd_fun = (unsigned char)cmd;
    unsigned char cmd_type = (unsigned char)(cmd>>8);
    free(p_r_fram);

    search_msgmap(cmd_fun,pf1,data_len);
    return;

    // USER_DEBUG_RTT("cmd  0x%x\r\n",cmd);
    // USER_DEBUG_RTT("cmd_fun  0x%x\r\n",cmd_fun);
    // USER_DEBUG_RTT("cmd_type  0x%x\r\n",cmd_type);
	switch (cmd_fun)
	{                 
		case 0x06:
            /*添加数据到链表并打印*/
            ipc_msg_printf();
			break; 
 		case 0x0A:
            {
                
            }
			break;   	
		default:
			break;
	}

}





