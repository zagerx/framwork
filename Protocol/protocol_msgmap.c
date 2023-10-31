
#include "protocol_cfg.h"
#include "protocol_comment.h"
/*
该文件需完善
*/


bool pro_testfunc(msg_item_t *p,void *pdata,unsigned short hwSize)
{
    float buf[4] = {6.2f,0.0f,0.0f,3.1f};
    protocol_transmit(CMD_RESP,PRO_FUNC_C_PF300,buf,16);//测试用
    protocol_transmit(CMD_RESP,PRO_FUNC_C_TEMP,buf,16);//测试用
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

/*! \brief 搜索消息地图，并执行对应的处理程序
 *! \retval false  消息不存在或者消息处理函数觉得内容无效
 *! \retval true   消息得到了正确的处理
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
            continue;  //!< 无效的指令？（不应该发生）
        }
        //! 调用消息处理函数
        return ptItem->fnHandler(ptItem, pData, hwSize);
    }    
    return false;   //!< 没找到对应的消息
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

