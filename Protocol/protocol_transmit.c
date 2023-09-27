#include "protocol.h"
#include "crc.h"
#include "stdio.h"
#include "usart.h"
char protocol_sendfram(pro_frame_t *msg,unsigned short len)
{
    HAL_UART_Transmit(&huart1,(unsigned char*)msg,sizeof(pro_frame_t)+4,0XFFFF);
	return 0;
}
