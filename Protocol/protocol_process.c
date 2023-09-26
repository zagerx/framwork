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
** Descriptions:	协议解析
**
********************************************************************************************************/
#include "./protocol.h"
#include "fifo.h"
#include "fsm.h"
#include "usart.h"

#include "malloc.h"


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
	unsigned char buf[PRO_FRAME_MAX_SIZE];
	unsigned char cmd;
    unsigned short len;
	/*读取1帧数据*/
	// if(get_fram_process(&get_fram_cb) != fsm_rt_cpl)
	// {
	// 	return;
	// }
	if(get_fram_process(&buf[0]) != 0)
	{
		return;
	}    
	/*开始校验*/
	
	/*开始解析*/
    cmd = buf[2];
	switch (cmd)
	{
		case 0x02:
			USER_DEBUG("recive cmd 02\r\n");
			break;
		case 0x01:
            // SET_IPC_EVENT(PROTOCOL_CMD_01);
			USER_DEBUG("recive cmd 01\r\n");
            /*命令封包*/
            /*计算帧大小*/
            unsigned char len;
            len = sizeof(pro_frame_t) + sizeof(float);

            /*计算消息大小      暂定为8byte*/
            // void* pmsg;
            // pmsg = malloc(len+8);
            // if (!pmsg)
            // {
            //     printf(" malloc fail\r\n");
            // }
            // /*开始封包*/
            
			// pro_frame_t test_cmdtype_01={
			// 	.head = PRO_FRAME_HEAD,
			// 	.func_c = PRO_FUNC_C_TEMP | (1<<CMD_RESP) ,
			// 	.len = 0,
			// 	.pdata = 0,
			// 	.tail = PRO_FRAME_TAIL
			// };
			break;
		case 0x03:
			USER_DEBUG("recive cmd 03\r\n");
			break;
		case 0x04://发送温湿度数据给主机，并等待主机响应
			USER_DEBUG("recive cmd 04\r\n");
			break;
		case 0x05:
			USER_DEBUG("recive cmd 05\r\n");
			break;
		case 0x0B:
			USER_DEBUG("recive cmd 06\r\n");
//			protocol_sendfram(&test_msg);
			break;		
		default:
			break;
	}
}





