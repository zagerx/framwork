#ifndef __PROTOCOL__H
#define __PROTOCOL__H

#include "stdint.h"
#include "ipc.h"

enum{
    CMD_ORIGE = 0x00,
    CDM_NO_RESP = 0x01,
    CMD_RESP = 0x02,
    CMD_ACK,
};

#define	PRO_FRAME_MAX_SIZE	512
#define PRO_FRAME_MIN_SIZE  8
#define PRO_FIFO_SIZE PRO_FRAME_MAX_SIZE * 5

#define PRO_FRAME_HEAD	0x5AA5
#define PRO_FRAME_TAIL	0xFEFE

#define FRAM_HEAD_OFFSET     2
#define FRAM_FUNC_OFFSET    FRAM_HEAD_OFFSET + sizeof(unsigned short)
#define FRAM_LEN_OFFSET     FRAM_FUNC_OFFSET + sizeof(unsigned short)
#define FRAM_PDATA_OFFSET   FRAM_LEN_OFFSET + sizeof(unsigned short) + 0
#define FRAM_CRC_OFFSET     FRAM_PDATA_OFFSET + sizeof(void *)
#define FRAM_TAIL_OFFSET    FRAM_CRC_OFFSET + sizeof(unsigned short)
#define FRAM_BUF_OFFSET     FRAM_TAIL_OFFSET + sizeof(unsigned short)

#define PRO_FUNC_C_M_HEARTPACK	0x00	//主机心跳包
#define PRO_FUNC_C_S_HEARTPACK	0x01	//从机心跳包
#define PRO_FUNC_C_TEMP	        0x0B	//温度数据
#define PRO_FUNC_C_PF300	    0x0A	//温度数据

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


/*-----------------------------------消息地图相关-----------------------------------------*/
#include "stdbool.h"
typedef struct msg_item_t msg_item_t;
struct msg_item_t {
    uint8_t chID;                 //!< 指令
    uint8_t chAccess;             //!< 访问权限检测
    uint16_t hwValidDataSize;     //!< 数据长度要求
    bool (*fnHandler)(msg_item_t *ptMSG,   
                      void *pData, 
                      uint_fast16_t hwSize);
};
bool search_msgmap(uint_fast8_t chID,
                   void *pData,
                   uint_fast16_t hwSize);
/*--------------------------------------------------------------------------------------*/






/*---------------------------------协议接口------------------------------------------*/
extern void protocol_init(void);
extern void protocol_process(void);
extern void protocol_parse(void);
extern pro_frame_t* pro_frame_packet(unsigned short cmd,void *pdata,unsigned short len);
extern pro_frame_t* pro_frame_unpack(unsigned char *pdata,unsigned short len);
extern signed char protocol_reciver_datafram(unsigned char *pdata,unsigned short len);
extern msg_t* pro_send_cmd_data(unsigned short id_fsm,unsigned char cmd_type,\
                                unsigned char cmd,void *pdata,unsigned short data_len);
extern msg_t* pro_nowsend_cmd_data(unsigned short id_fsm,unsigned char cmd_type,\
                        unsigned char cmd,void *pdata,unsigned short data_len);
/*--------------------------------------------------------------------------------------*/
#endif
