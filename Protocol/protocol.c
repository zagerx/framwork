#include "fifo.h"
#include "fsm.h"

/*Э��Ҫ�õ������
1��fifo.c/.h
2��fsm.h
*/
unsigned char fifo_receive_buff[256];//fifo���ݻ�����
byte_fifo_t uart1_rx_fifo;//fifo���ƿ�
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

