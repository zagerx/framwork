#ifndef __PROTOCOL_COMMENT__H
#define __PROTOCOL_COMMENT__H
/*------------------------------------------

���ļ�����ģ���ڲ�ʹ��
����ʹ�����·���������ļ�
--------------------------------------------*/

#include "protocol_cfg.h"

#include "stdint.h"

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

#pragma pack(push,4)//�������ֽڶ��룬1�ֽڶ��룬��pdata��ֵ������hardfalut(ԭ��δ֪)
typedef struct pro_frame{
    unsigned short _x_x;//�����壬����ṹ��4�ֽڶ���
	unsigned short head;
	unsigned short func_c;
	unsigned short len;
    void *pdata;
	unsigned short crc16;
	unsigned short tail;
}pro_frame_t;//size  16
#pragma pack(pop)


/*-----------------------------------��Ϣ��ͼ���-----------------------------------------*/
typedef struct msg_item_t msg_item_t;
struct msg_item_t {
    uint8_t chID;                 //!< ָ��
    uint8_t chAccess;             //!< ����Ȩ�޼��
    uint16_t hwValidDataSize;     //!< ���ݳ���Ҫ��
    char (*fnHandler)(msg_item_t *ptMSG,   
                      void *pData, 
                      uint_fast16_t hwSize);
};
char search_msgmap(uint_fast8_t chID,
                   void *pData,
                   uint_fast16_t hwSize);
/*--------------------------------------------------------------------------------------*/

extern byte_fifo_t protocol_fifo_handle;//fifo���ƿ�

extern pro_frame_t* pro_frame_packet(unsigned short cmd,void *pdata,unsigned short len);
extern pro_frame_t* pro_frame_unpack(unsigned char *pdata,unsigned short len);
extern void protocol_parse(void);

#endif
