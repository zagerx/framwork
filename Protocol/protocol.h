#ifndef __PROTOCOL__H
#define __PROTOCOL__H


/*--------------------------------------------------
���ļ�Ϊģ��Ľӿ��ļ�
--------------------------------------------------*/
enum{
    CMD_ORIGE = 0x00,
    // CDM_NO_RESP = 0x01,
    CMD_RESP,
    CMD_ACK,
};
typedef enum{
    PRO_FUNC_C_M_HEARTPACK ,
    PRO_FUNC_C_S_HEARTPACK,
    PRO_FUNC_C_PF300= 0x0A,
    PRO_FUNC_C_TEMP,
    CMD_NUMBER
}E_CMD;
// #define PRO_FUNC_C_M_HEARTPACK	0x00	//����������
// #define PRO_FUNC_C_S_HEARTPACK	0x01	//�ӻ�������
// #define PRO_FUNC_C_TEMP	        0x0B	//�¶�����
// #define PRO_FUNC_C_PF300	    0x0A	//�¶�����


/*---------------------------------Э��ӿ�------------------------------------------*/
extern void protocol_init(void);
extern void protocol_process(void);
extern void _bsp_protransmit(unsigned char* pdata,unsigned short len);
extern unsigned char protocol_reciverdata_tofifo(unsigned char *pdata,unsigned short len);
extern char protocol_nowtransmit(unsigned char cmd_type,\
                        unsigned char cmd,void *pdata,unsigned short data_len);
extern void protocol_transmit(unsigned char cmd_type,unsigned char cmd,\
                            void *pdata,unsigned short data_len);      
void protocol_transmitprocess(void);
/*--------------------------------------------------------------------------------------*/
#endif
