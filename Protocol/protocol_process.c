/** File name:			main.c
** Version:				The original version
** Descriptions��	main C entry file for EN_FN80_Driver project
** Copyright@2015-2025: Enmind, All Right Reserved
**
**------------------------------------------------------------------------------------------------------
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
#include "stdio.h"
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
//	if(fifo_read(buf,PRO_FRAME_MAX_SIZE))
//	{
//		return;
//	}

	if(bytefifo_readmulintebyte(buf,4))
	{
		return;
	}
	unsigned short head,tail;
	unsigned char len;
	head = buf[0]<<8 | (unsigned char)buf[1];
	cmd = buf[2];
	len = buf[3];
	bytefifo_readmulintebyte(&buf[4],len+4);
	tail = buf[4+len+2]<<8|buf[4+len+2+1];
	if((tail) != 0xFEFE)
	{
		printf("get data fail  %x\r\n",tail);
		return;
	}
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
		case 0x04:
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






