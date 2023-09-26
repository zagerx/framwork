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

#undef NULL
#define NULL 0                  
#undef this
#define this (*ptThis)
static fsm_cb_t get_fram_cb;
static fsm_rt_t get_fram_process(fsm_cb_t *ptThis);
#define GETFRAM_RESET()               \
        do { this.chState = START; } while(0)

static unsigned char fram_buf[32];
static unsigned short fram_len = 0;
fsm_cb_t* get_fram_init(void)
{
  get_fram_cb.fsm = (fsm_t*)get_fram_process;
  get_fram_cb.chState = START;
  get_fram_cb.sig = 0;
  return &get_fram_cb;
}
static fsm_rt_t get_fram_process(fsm_cb_t *ptThis)
{
		enum {
			FIRST = USER,
			SECOND,
			IDLE ,			
			THREE
		};
		static unsigned short index = 0;		
		unsigned char data;
		static unsigned int t0 = 0;
		switch (this.chState)
		{
		case START:
			this.chState = FIRST;
		case FIRST:
			bytefifo_readmulintebyte(&uart1_rx_fifo,&data,1);
			if(data == (unsigned char)(PRO_FRAME_HEAD>>8))
			{
				fram_buf[index++] = data;
				this.chState = SECOND;
			}else{
				index = 0;
				GETFRAM_RESET();
				break;
			}
		case SECOND:
			bytefifo_readmulintebyte(&uart1_rx_fifo,&data,1);
			if(data == (unsigned char)(PRO_FRAME_HEAD))
			{
				this.chState = IDLE;
				fram_buf[index++] = data;
			}else{
				GETFRAM_RESET();
				break;
			}
			case IDLE:
			bytefifo_readmulintebyte(&uart1_rx_fifo,&data,1);
			fram_buf[index++] = data;
			if(data == (unsigned char)(PRO_FRAME_TAIL>>8))
			{
				this.chState = THREE;
			}else{
				break;
			}
		case THREE:
			bytefifo_readmulintebyte(&uart1_rx_fifo,&data,1);
			if(data == (unsigned char)PRO_FRAME_TAIL)
			{
				fram_buf[index++] = data;
				fram_len = index;
				index = 0;
				GETFRAM_RESET();//��ȡ������������֡  ��λ״̬��
				return fsm_rt_cpl;				
			}else{
				this.chState = IDLE;
				break;
			}
		case EXIT:
			break;
		default:
			break;
		}
		return fsm_rt_on_going;
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
	/*��ȡ1֡����*/
	if(get_fram_process(&get_fram_cb) != fsm_rt_cpl)
	{
		return;
	}
	/*��ʼУ��*/
	
	/*��ʼ����*/
    cmd = fram_buf[2];
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





