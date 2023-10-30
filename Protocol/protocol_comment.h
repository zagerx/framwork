#ifndef __PROTOCOL_COMMENT__H
#define __PROTOCOL_COMMENT__H
/*------------------------------------------
���ļ�����ģ���ڲ�ʹ��
����ʹ�����·���������ļ�
--------------------------------------------*/

#include "protocol_cfg.h"
#include "protocol.h"
extern _list_t *gtransmit_list;
extern unsigned short g_protocol_event;

// typedef enum{
//     CMD_ERR = -1,
//     CMD_M_HEARTPACK,
//     CMD_S_HEARTPACK,
//     CMD_TEMP,
//     CMD_PF300,
    
// }E_CMD_COVMAP;

typedef struct cmd_keymap
{
    E_CMD key;
    unsigned char event;
}cmd_keymap_t;


#define PRO_FRAME_HEAD	    0x5AA5
#define PRO_FRAME_TAIL	    0xFEFE
#define FRAM_HEAD_OFFSET    sizeof(unsigned short)
#define FRAM_FUNC_OFFSET    FRAM_HEAD_OFFSET  + sizeof(unsigned short)
#define FRAM_LEN_OFFSET     FRAM_FUNC_OFFSET  + sizeof(unsigned short)
#define FRAM_PDATA_OFFSET   FRAM_LEN_OFFSET   + sizeof(unsigned short)
#define FRAM_CRC_OFFSET     FRAM_PDATA_OFFSET + sizeof(void *)
#define FRAM_TAIL_OFFSET    FRAM_CRC_OFFSET   + sizeof(unsigned short)
#define FRAM_BUF_OFFSET     FRAM_TAIL_OFFSET  + sizeof(unsigned short)

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

#pragma pack(push,4)
typedef struct pro_pack
{
    fsm_cb_t statemach_cb;
    /* data */
    unsigned int timeout;//��ʱʱ��
    unsigned short recnt;//�ط�����
    unsigned short t0;//   
    pro_frame_t *frame; 
}pro_pack_t;
#pragma pack(pop)


/*-----------------------------------��Ϣ��ͼ���-----------------------------------------*/
typedef struct msg_item_t msg_item_t;
struct msg_item_t {
    unsigned char chID;                 //!< ָ��
    unsigned char chAccess;             //!< ����Ȩ�޼��
    unsigned short hwValidDataSize;     //!< ���ݳ���Ҫ��
    char (*fnHandler)(msg_item_t *ptMSG,   
                      void *pData, 
                      unsigned short  hwSize);
};
char search_msgmap(unsigned char chID,
                   void *pData,
                   unsigned short  hwSize);
/*--------------------------------------------------------------------------------------*/

short forch_keymap_enevt(E_CMD key);

fsm_rt_t _trancemit_statemach(fsm_cb_t *ptThis);
pro_pack_t* _packet_propack(pro_frame_t *frame,unsigned int timeout,unsigned char recnt);
extern pro_frame_t* _packet_proframe(unsigned short cmd,void *pdata,unsigned short len);
extern pro_frame_t* _unpack_proframe(unsigned char *pdata,unsigned short len);
extern void protocol_parse(void);
extern char _get_framedata_fromfifo(unsigned char *pbuf,unsigned short *pframe_len);
#endif
