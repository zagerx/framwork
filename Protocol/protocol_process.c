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
#include "usart.h"
#include "global_data.h"


#include "fsm.h"
static unsigned char fram_buf[32];
static unsigned short fram_len = 0;
#undef NULL
#define NULL 0                  
#undef this
#define this (*ptThis)
static fsm_cb_t get_fram_cb;
static fsm_rt_t get_fram_process(fsm_cb_t *ptThis);
#define GETFRAM_RESET()               \
        do { this.chState = START; } while(0)

static fsm_cb_t* get_fram_init(void)
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
			USER_DEBUG("recive cmd 01\r\n");
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
//			unsigned char buf[4] = {0};
//			pro_frame_t test_msg={
//				.head = PRO_FRAME_HEAD,
//				.func_c = PRO_FUNC_C_TEMP,
//				.len = 4,
//				.pdata = buf,
//				.tail = PRO_FRAME_TAIL
//			};
//			protocol_sendfram(&test_msg);
			break;		
		default:
			break;
	}
}

/*Э��Ҫ�õ������
1��fifo.c/.h
2��fsm.h
*/

uint8_t fifo_receive_buff[256];//fifo���ݻ�����
byte_fifo_t uart1_rx_fifo;//fifo���ƿ�
void protocol_init(void)
{
    bytefifo_init(&uart1_rx_fifo,fifo_receive_buff,sizeof(fifo_receive_buff));
    get_fram_init();
}



