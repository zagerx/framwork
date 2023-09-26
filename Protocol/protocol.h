#ifndef __PROTOCOL__H
#define __PROTOCOL__H


#include "ipc.h"


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

enum{
    CMD_NO_RESP = 8,
    CMD_RESP,
    CMD_ACK,
};


#include "fifo.h"
extern byte_fifo_t uart1_rx_fifo;   //仅protoc_recive.c文件调用


#pragma pack(push,1)
typedef struct pro_frame{
	unsigned short head;
	unsigned short func_c;
	unsigned char len;
	unsigned char *pdata;
	unsigned short crc16;
	unsigned short tail;
}pro_frame_t;
#pragma pack(pop)

extern signed char protocol_reciver_datafram(unsigned char *pdata,unsigned short len);
extern void protocol_parse(void);
extern char protocol_sendfram(pro_frame_t *msg);

extern void protocol_init(void);
extern void protocol_process(void);
extern void _protocol_cmd_init(void);
extern void protocl_cmd_process(void);
#endif
