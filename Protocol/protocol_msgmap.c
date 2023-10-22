
#include "protocol_cfg.h"
#include "protocol_comment.h"
/*
该文件需完善
*/





bool press_data_updata(msg_item_t *p,void *pdata,unsigned short hwSize)
{
}

const msg_item_t c_tMSGTable[] = {
    [0] = {
        .chID = 8,
        .chAccess = 9,
        .fnHandler = press_data_updata,
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
