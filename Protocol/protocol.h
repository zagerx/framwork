#ifndef __PROTOCOL__H
#define __PROTOCOL__H


#include "ipc.h"


enum{
    CMD_NO_RESP = 8,
    CMD_RESP,
    CMD_ACK,
};


/*
**从机心跳包
			head			func_c		len				crc_16						tail
**0x5A 0xA5 		0x01 			0x00 		 0xFC 0x0D		   	0xEF 0xEF
*/


#define	PRO_FRAME_MAX_SIZE	32
#define PRO_FRAME_MIN_SIZE  8
#define PRO_FRAME_HEAD	0x5AA5
#define PRO_FRAME_TAIL	0xFEFE

#define PRO_FUNC_C_M_HEARTPACK	0x00	//主机心跳包
#define PRO_FUNC_C_S_HEARTPACK	0x01	//从机心跳包
#define PRO_FUNC_C_TEMP	0x0B	//温度数据
#define PRO_FUNC_C_PF300	0x0A	//温度数据

#pragma pack(push,4)//必须四字节对齐，1字节对齐，给pdata赋值，程序hardfalut(原因未知)
typedef struct pro_frame{
    unsigned short _x_x;//无意义，解决结构体4字节对齐
	unsigned short head;
	unsigned short func_c;
	unsigned short len;
    void *pdata;
	unsigned short crc16;
	unsigned short tail;
}pro_frame_t;//size  16
#pragma pack(pop)

#define FRAM_HEAD_OFFSET     2
#define FRAM_FUNC_OFFSET    FRAM_HEAD_OFFSET + sizeof(unsigned short)
#define FRAM_LEN_OFFSET     FRAM_FUNC_OFFSET + sizeof(unsigned short)
#define FRAM_PDATA_OFFSET   FRAM_LEN_OFFSET + sizeof(unsigned short) + 0
#define FRAM_CRC_OFFSET     FRAM_PDATA_OFFSET + sizeof(void *)
#define FRAM_TAIL_OFFSET    FRAM_CRC_OFFSET + sizeof(unsigned short)
#define FRAM_BUF_OFFSET     FRAM_TAIL_OFFSET + sizeof(unsigned short)

extern signed char protocol_reciver_datafram(unsigned char *pdata,unsigned short len);
extern void protocol_parse(void);
extern char protocol_sendfram(pro_frame_t *msg,unsigned short len);

extern void protocol_init(void);
extern void protocol_process(void);
extern void _protocol_cmd_init(void);
extern void protocl_cmd_process(void);
#endif
