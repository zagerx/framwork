#ifndef __PROTOCOL__H
#define __PROTOCOL__H


/*--------------------------------------------------
该文件为模块的接口文件
--------------------------------------------------*/
enum{
    CMD_ORIGE = 0x00,
    CMD_RESP,
    CMD_ACK,
};
#if 1
    enum{
        CMD_EVENT_UNINVAIL = -1,
        CMD_EVENT_01,
        CMD_EVENT_02,
        CMD_EVENT_NUMBER
    };//事件1、用来同步协议之间的通信 2、也可以供外部调用以用来同步其他

    typedef enum{
        PRO_FUNC_TESTCMD01 = 1,
        PRO_FUNC_C_M_HEARTPACK ,
        PRO_FUNC_C_S_HEARTPACK,        
        PRO_FUNC_C_PF300= 0x0A,
        PRO_FUNC_C_TEMP,
        CMD_NUMBER
    }E_CMD;
#else
    #define PRO_FUNC_C_M_HEARTPACK	0x00	//主机心跳包
    #define PRO_FUNC_C_S_HEARTPACK	0x01	//从机心跳包
    #define PRO_FUNC_C_TEMP	        0x0B	//温度数据
    #define PRO_FUNC_C_PF300	    0x0A	//温度数据
#endif

/*---------------------------------协议接口------------------------------------------*/
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
