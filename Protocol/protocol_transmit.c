#include "protocol.h"
#include "crc.h"
#include "stdio.h"

char protocol_sendfram(pro_frame_t *msg)
{
	unsigned char len;
	unsigned char buf[PRO_FRAME_MAX_SIZE] = {0};
	unsigned char i;
	unsigned char *pdata = NULL;
	pdata = msg->pdata;
	len = msg->len;
	
	buf[0] = msg->head >> 8;
	buf[1] = (unsigned char)msg->head;
	buf[2] = msg->func_c;
	buf[3] = len;
	
	/*4  帧头+功能字+数据长度*/
	for(i = 4;i < 4+len;i++)
	{
		buf[i] = *(pdata++);
	}
	msg->crc16 = CRC16_Subsection(buf,0xFFFF,4+len);
	buf[i] = msg->crc16 >> 8;
	buf[i+1] = (unsigned char)msg->crc16;
	
	buf[i+2] = msg->tail>>8;
	buf[i+2+1] = (unsigned char)msg->tail;

	/*串口采用轮询发送*/
	for(unsigned char j = 0;j<PRO_FRAME_MIN_SIZE+len;j++)
	{
		/*
		USART_SendData(USART1,(unsigned char)buf[j]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);*/
	}
	return 0;
}
