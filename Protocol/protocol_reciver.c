/** Created by:			
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
signed char protocol_reciver_datafram(unsigned char *pdata,unsigned short len)
{
	/**/
//	unsigned short crc_16 = 0;
//	crc_16 = ((unsigned short)pdata[len-4]) << 8;
//	crc_16 |= (unsigned short)pdata[len-3];
//	if(len > PRO_FRAME_MAX_SIZE || len < PRO_FRAME_MIN_SIZE)
//	{/*������Χ*/
//		goto out;
//	}
//	/*4: crc+֡β*/
//	if(CRC16_Subsection(pdata,0xFFFF,len - 4) != crc_16)
//	{/*У��ʧ��*/
//		goto out;
//	}
	/*���*/
//	fifo_write(pdata,len);
	bytefifo_writemulitebyge(pdata,len);
	return 0;
//out:
//	return -1;
}
