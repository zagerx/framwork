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
	/**/
//	unsigned short crc_16 = 0;
//	crc_16 = ((unsigned short)pdata[len-4]) << 8;
//	crc_16 |= (unsigned short)pdata[len-3];
//	if(len > PRO_FRAME_MAX_SIZE || len < PRO_FRAME_MIN_SIZE)
//	{/*超出范围*/
//		goto out;
//	}
//	/*4: crc+帧尾*/
//	if(CRC16_Subsection(pdata,0xFFFF,len - 4) != crc_16)
//	{/*校验失败*/
//		goto out;
//	}
	/*入队*/
//	fifo_write(pdata,len);
	bytefifo_writemulitebyge(pdata,len);
	return 0;
//out:
//	return -1;
}
