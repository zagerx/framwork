#include "fifo.h"
#include "fsm.h"



/*Э��Ҫ�õ������
1��fifo.c/.h
2��fsm.h
*/
unsigned char fifo_receive_buff[256];//fifo���ݻ�����
byte_fifo_t uart1_rx_fifo;//fifo���ƿ�




/*********************************************************************************************************
** Function name(��������):				protocol_reciver_datafram()
**
** Descriptions��������:				����һ֡����
**
** input parameters�����������:		len:һ֡���ݳ���(����֡ͷ��֡β)
** Returned value������ֵ��:				0:�ɹ�
**         
** Used global variables��ȫ�ֱ�����:	None
** Calling modules������ģ�飩:			fifo.c/h crc.c/.h
**
** Created by�������ˣ�:				zager
** Created Date���������ڣ�:			2023-08-14
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

