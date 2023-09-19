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
#include "usart.h"
#include "fsm.h"
#include "global_data.h"

static unsigned char fram_buf[32];
static unsigned short fram_len = 0;

#undef NULL
#define NULL 0                  
#undef this
#define this (*ptThis)
fsm_cb_t get_fram_cb;
fsm_rt_t get_fram_process(fsm_cb_t *ptThis);
#define GETFRAM_RESET()               \
        do { this.chState = START; } while(0)

fsm_cb_t* get_fram_init(void)
{
  get_fram_cb.fsm = (fsm_t*)get_fram_process;
  get_fram_cb.chState = START;
  get_fram_cb.sig = 0;
  return &get_fram_cb;
}

fsm_rt_t get_fram_process(fsm_cb_t *ptThis)
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
            printf("data %d\r\n",data);
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
				printf("fram process ok\r\n");
				fram_buf[index++] = data;
				fram_len = index;
				index = 0;
				GETFRAM_RESET();//获取到完整的数据帧  复位状态机
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
	if(get_fram_process(&get_fram_cb) != fsm_rt_cpl)
	{
		return;
	}
	/*获取到完整的数据帧*/
	for(unsigned char i = 0;i < fram_len;i++)
	{
		printf("0x%2x ",fram_buf[i]);
	}
	printf("\r\n");
	/*开始校验*/
	
	/*开始解析*/
	cmd = fram_buf[3];
	switch (cmd)
	{
		case 0x02:
			printf("recive cmd 02\r\n");
			break;
		case 0x01:
			printf("recive cmd 01\r\n");
			break;
		case 0x03:
			printf("recive cmd 03\r\n");
			break;
		case 0x04://发送温湿度数据给主机，并等待主机响应
			printf("recive cmd 04\r\n");
			break;
		case 0x05:
			printf("recive cmd 05\r\n");
			break;
		case 0x0B:
			printf("recive cmd 06\r\n");
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






