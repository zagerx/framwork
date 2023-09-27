#include "fifo.h"
#include "fsm.h"



/*协议要用到的组件
1、fifo.c/.h
2、fsm.h
*/
unsigned char fifo_receive_buff[256];//fifo数据缓存区
byte_fifo_t uart1_rx_fifo;//fifo控制块




/*********************************************************************************************************
** Function name(函数名称):				protocol_reciver_datafram()
**
** Descriptions（描述）:				接收一帧数据
**
** input parameters（输入参数）:		len:一帧数据长度(包含帧头、帧尾)
** Returned value（返回值）:				0:成功
**         
** Used global variables（全局变量）:	None
** Calling modules（调用模块）:			fifo.c/h crc.c/.h
**
** Created by（创建人）:				zager
** Created Date（创建日期）:			2023-08-14
**
********************************************************************************************************/
signed char protocol_reciver_datafram(unsigned char *pdata,unsigned short len)
{
	bytefifo_writemulitebyge(&uart1_rx_fifo,pdata,len);
	return 0;

}








void protocol_init(void)
{
    bytefifo_init(&uart1_rx_fifo,fifo_receive_buff,sizeof(fifo_receive_buff));
    _protocol_cmd_init();
}

void protocol_process(void)
{
    protocol_parse();
    protocl_cmd_process();
}

