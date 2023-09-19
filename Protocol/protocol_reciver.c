/****************************************Copyright (c)**************************************************
**                               SHENZHEN  ELECTRONICS Co.,LTD.
**                                      
**                                 
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			main.c
** Version:				The original version
** Descriptions��	main C entry file for EN_FN80_Driver project
** Copyright@2015-2025: , All Right Reserved
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
#include "crc.h"

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
#include "stdio.h"
#include "protocol.h"
signed char protocol_reciver_datafram(unsigned char *pdata,unsigned short len)
{
	bytefifo_writemulitebyge(&uart1_rx_fifo,pdata,len);
	return 0;

}
