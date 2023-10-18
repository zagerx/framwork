
#include "protocol.h"
#include "stdint.h"
#include "usart.h"
#include "macro_defined.h"
/*
该文件需完善
*/

#undef NULL
#define NULL 0

bool press_data_updata(msg_item_t *p,void *pdata,uint_fast16_t hwSize)
{
    #define POINT_OFFSET 1
    char buf[] = "%RM3#324\r\n";
    unsigned char startpos = ((unsigned int)strchr(buf,'#')-(unsigned int)buf);
    unsigned char endpos = ((unsigned int)strchr(buf,'\r')-(unsigned int)buf);
    unsigned char len = endpos-startpos;
    unsigned char int_len = len-POINT_OFFSET-1;
    USER_DEBUG_RTT("%d %d\r\n",startpos,endpos);
    char *pbuf = malloc(endpos - startpos);
    strncpy(pbuf,&buf[startpos+1],int_len);
    USER_DEBUG_RTT("%s\r\n",pbuf);
    strcpy(pbuf[int_len],".");
    USER_DEBUG_RTT("%s\r\n",pbuf);
}

const msg_item_t c_tMSGTable[] = {
    [0] = {
        .chID = PRO_FUNC_C_PF300,
        .chAccess = CMD_ACK,
        .fnHandler = press_data_updata,
    },
};


static volatile uint8_t s_chCurrentAccessPermission =0x07;

/*! \brief 搜索消息地图，并执行对应的处理程序
 *! \retval false  消息不存在或者消息处理函数觉得内容无效
 *! \retval true   消息得到了正确的处理
 */
bool search_msgmap(uint_fast8_t chID,
                   void *pData,
                   uint_fast16_t hwSize)
{
    for (int n = 0; n < dimof(c_tMSGTable); n++) {
        msg_item_t *ptItem = &c_tMSGTable[n];
        if (chID != ptItem->chID) {
            continue;
        }
        if (!(ptItem->chAccess & s_chCurrentAccessPermission)) {
            continue;  //!< 当前的访问属性没有一个符合要求
        }
        if (hwSize < ptItem->hwValidDataSize) {
            continue;  //!< 数据太小了
        }
        if (NULL == ptItem->fnHandler) {
            continue;  //!< 无效的指令？（不应该发生）
        }
        //! 调用消息处理函数
        return ptItem->fnHandler(ptItem, pData, hwSize);
    }    
    return false;   //!< 没找到对应的消息
}
