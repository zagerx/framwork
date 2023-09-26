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
    unsigned short len;
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
			USER_DEBUG("recive cmd 02\r\n");
			break;
		case 0x01:
            // SET_IPC_EVENT(PROTOCOL_CMD_01);
			USER_DEBUG("recive cmd 01\r\n");
            /*������*/
            /*����֡��С*/
            unsigned char len;
            len = sizeof(pro_frame_t) + sizeof(float);

            /*������Ϣ��С      �ݶ�Ϊ8byte*/
            // void* pmsg;
            // pmsg = malloc(len+8);
            // if (!pmsg)
            // {
            //     printf(" malloc fail\r\n");
            // }
            // /*��ʼ���*/
            
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
		case 0x04://������ʪ�����ݸ����������ȴ�������Ӧ
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





