#include "protocol.h"
#include "crc.h"
#include "stdio.h"
#include "usart.h"
#include "stdlib.h"
#include "string.h"
char protocol_sendfram(pro_frame_t *msg,unsigned short len)
{
    /*整理帧数据*/
    unsigned char *pucmsg;
    unsigned char *pdata;
    unsigned short data_len;
    data_len = len-sizeof(pro_frame_t);
    unsigned char buf[2] = {0};
    
    pucmsg = (unsigned char*)msg;
    memcpy(buf,(unsigned char*)&pucmsg[FRAM_TAIL_OFFSET],2);
    pdata = malloc(data_len);
    memcpy(pdata,(unsigned char*)&pucmsg[sizeof(pro_frame_t)],data_len);
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET],pdata,data_len);    
    free(pdata);
    unsigned short crc_16;
    crc_16 = CRC16_Subsection((unsigned char*)&pucmsg[2],0xFFFF,FRAM_PDATA_OFFSET+data_len-2);
    crc_16 = __SWP16(crc_16);    
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len],(unsigned char*)&crc_16,sizeof(crc_16));   
    
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len+2],buf,2);    
    HAL_UART_Transmit(&huart1,(unsigned char*)msg,sizeof(pro_frame_t)-sizeof(void *)+data_len,0XFFFF);
	return 0;
}
