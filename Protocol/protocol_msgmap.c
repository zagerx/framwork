#include "protocol_cfg.h"
#include "protocol_comment.h"
/*
���ļ�������
*/
static bool pro_testfunc(cmdmsg_t *p,void *pdata,unsigned short datalen);
static bool read_sensordata(cmdmsg_t *p,void *pdata,unsigned short datalen);

static const cmdmsg_t sgMsgTable[] = {
    [0] = {
        .cmd = PRO_FUNC_TESTCMD01,        
        .flagbit = CMD_EVENT_UNINVAIL,
        .fnHandler = pro_testfunc,        
    }, 
    [1] = {
        .cmd = PRO_FUNC_C_PF300,        
        .flagbit = CMD_EVENT_01,
        .fnHandler = read_sensordata,
    },
    [2] = {
        .cmd = PRO_FUNC_C_TEMP,        
        .flagbit = CMD_EVENT_02,
        .fnHandler = read_sensordata,        
    },   
};

bool pro_testfunc(cmdmsg_t *p,void *pdata,unsigned short datalen)
{
    float buf[4] = {6.2f,0.0f,0.0f,3.1f};
    int flagbit;
    flagbit = p->flagbit;
    if (flagbit != CMD_EVENT_UNINVAIL) /*��Ӧ�����������Ӧ���¼�  �����¼���λ ͬ�������߳�*/
    {
        IPC_SET_EVENT(g_protocol_event,flagbit);
    }
    protocol_transmit(CMD_RESP,PRO_FUNC_C_PF300,buf,16);//������
    protocol_transmit(CMD_RESP,PRO_FUNC_C_TEMP,buf,16);//������
}

bool read_sensordata(cmdmsg_t *p,void *pdata,unsigned short datalen)
{
    int flagbit;
    flagbit = p->flagbit;
    if (flagbit != CMD_EVENT_UNINVAIL)
    {
        IPC_SET_EVENT(g_protocol_event,flagbit);
    }        
}

char search_msgmap(unsigned char cmd,
                   void *pData,
                   unsigned short hwSize)
{
    for (int n = 0; n < dimof(sgMsgTable); n++) {
        cmdmsg_t *ptItem = &sgMsgTable[n];
        if (cmd != ptItem->cmd) {
            continue;
        }
        if (NULL == ptItem->fnHandler) {
            continue; 
        }        
        return ptItem->fnHandler(ptItem, pData, hwSize);
    }    
    return false;   //!< û�ҵ���Ӧ����Ϣ
}

short search_msgmap_event(unsigned char cmd)
{
    for (int n = 0; n < dimof(sgMsgTable); n++) {
        cmdmsg_t *ptItem = &sgMsgTable[n];
        if (cmd != ptItem->cmd) {
            continue;
        }
        return ptItem->flagbit;
    }
}