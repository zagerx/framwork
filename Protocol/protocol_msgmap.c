
#include "protocol_cfg.h"
#include "protocol_comment.h"
/*
���ļ�������
*/


bool pro_testfunc(msg_item_t *p,void *pdata,unsigned short hwSize)
{
    float buf[4] = {6.2f,0.0f,0.0f,3.1f};
    protocol_transmit(CMD_RESP,PRO_FUNC_C_PF300,buf,16);//������
    protocol_transmit(CMD_RESP,PRO_FUNC_C_TEMP,buf,16);//������
}


bool read_sensordata(msg_item_t *p,void *pdata,unsigned short hwSize)
{
    short event;
    event = p->event;
    IPC_SET_EVENT(g_protocol_event,event);
}

const msg_item_t c_tMSGTable[] = {
    [0] = {
        .chID = PRO_FUNC_TESTCMD01,
        .chAccess = 9,
        .fnHandler = pro_testfunc,
        .event = CMD_EVENT_UNINVAIL,
    }, 
    [1] = {
        .chID = PRO_FUNC_C_PF300,
        .chAccess = 9,
        .fnHandler = read_sensordata,
        .event = CMD_EVENT_01,
    },
    [2] = {
        .chID = PRO_FUNC_C_TEMP,
        .chAccess = 9,
        .fnHandler = read_sensordata,
        .event = CMD_EVENT_02,
    },   
};

static volatile unsigned char s_chCurrentAccessPermission =0x07;

/*! \brief ������Ϣ��ͼ����ִ�ж�Ӧ�Ĵ������
 *! \retval false  ��Ϣ�����ڻ�����Ϣ����������������Ч
 *! \retval true   ��Ϣ�õ�����ȷ�Ĵ���
 */
char search_msgmap(unsigned char chID,
                   void *pData,
                   unsigned short hwSize)
{
    for (int n = 0; n < dimof(c_tMSGTable); n++) {
        msg_item_t *ptItem = &c_tMSGTable[n];
        if (chID != ptItem->chID) {
            continue;
        }
        if (NULL == ptItem->fnHandler) {
            continue;  //!< ��Ч��ָ�����Ӧ�÷�����
        }
        //! ������Ϣ������
        return ptItem->fnHandler(ptItem, pData, hwSize);
    }    
    return false;   //!< û�ҵ���Ӧ����Ϣ
}

short search_msgmap_event(unsigned char chID)
{
    for (int n = 0; n < dimof(c_tMSGTable); n++) {
        msg_item_t *ptItem = &c_tMSGTable[n];
        if (chID != ptItem->chID) {
            continue;
        }
        return ptItem->event;
    }  

}

