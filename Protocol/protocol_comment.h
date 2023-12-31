#ifndef __PROTOCOL_COMMENT__H
#define __PROTOCOL_COMMENT__H
/*------------------------------------------
该文件仅供模块内部使用
必须使用相对路径包含该文件
--------------------------------------------*/

#include "protocol_cfg.h"
extern _list_t *g_transmit_handle;

#define PRO_FRAME_HEAD	    0x5AA5
#define PRO_FRAME_TAIL	    0xFEFE
#define FRAM_HEAD_OFFSET    sizeof(unsigned short)
#define FRAM_FUNC_OFFSET    FRAM_HEAD_OFFSET  + sizeof(unsigned short)
#define FRAM_LEN_OFFSET     FRAM_FUNC_OFFSET  + sizeof(unsigned short)
#define FRAM_PDATA_OFFSET   FRAM_LEN_OFFSET   + sizeof(unsigned short)
#define FRAM_CRC_OFFSET     FRAM_PDATA_OFFSET + sizeof(void *)
#define FRAM_TAIL_OFFSET    FRAM_CRC_OFFSET   + sizeof(unsigned short)
#define FRAM_BUF_OFFSET     FRAM_TAIL_OFFSET  + sizeof(unsigned short)

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

#pragma pack(push,4)
typedef struct pro_pack
{
    fsm_cb_t statemach_cb;
    /* data */
    unsigned int timeout;//超时时间
    unsigned short recnt;//重发次数
    unsigned short t0;//   
    pro_frame_t *frame; 
}pro_pack_t;
#pragma pack(pop)


/*-----------------------------------消息地图相关-----------------------------------------*/
typedef struct msg_item_t msg_item_t;
struct msg_item_t {
    unsigned char chID;                 //!< 指令
    unsigned char chAccess;             //!< 访问权限检测
    unsigned short hwValidDataSize;     //!< 数据长度要求
    char (*fnHandler)(msg_item_t *ptMSG,   
                      void *pData, 
                      unsigned short  hwSize);
};
char search_msgmap(unsigned char chID,
                   void *pData,
                   unsigned short  hwSize);
/*--------------------------------------------------------------------------------------*/
fsm_rt_t _trancemit_statemach(fsm_cb_t *ptThis);
pro_pack_t* _packet_propack(pro_frame_t *frame,unsigned int timeout,unsigned char recnt);
extern pro_frame_t* _packet_proframe(unsigned short cmd,void *pdata,unsigned short len);
extern pro_frame_t* _unpack_proframe(unsigned char *pdata,unsigned short len);
extern void protocol_parse(void);
extern char _get_framedata_fromfifo(unsigned char *pbuf,unsigned short *pframe_len);
#endif
