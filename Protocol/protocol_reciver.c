/** Created by:			
** Created date:		2023-08-17
** Version:				1.0
** Descriptions:		
**
**------------------------------------------------------------------------------------------------------
** Modified by:			zager
** Modified date:		2023-08-17
** Version:
** Descriptions:	协议接收
**
********************************************************************************************************/

#include "./protocol.h"
#include "fifo.h"
#include "crc.h"

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
#include "stdio.h"
signed char protocol_reciver_datafram(unsigned char *pdata,unsigned short len)
{
	bytefifo_writemulitebyge(pdata,len);
	return 0;
}
