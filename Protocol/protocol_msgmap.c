
#include "protocol_cfg.h"
#include "protocol_comment.h"
/*
���ļ�������
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
        if (!(ptItem->chAccess & s_chCurrentAccessPermission)) {
            continue;  //!< ��ǰ�ķ�������û��һ������Ҫ��
        }
        if (hwSize < ptItem->hwValidDataSize) {
            continue;  //!< ����̫С��
        }
        if (NULL == ptItem->fnHandler) {
            continue;  //!< ��Ч��ָ�����Ӧ�÷�����
        }
        //! ������Ϣ������
        return ptItem->fnHandler(ptItem, pData, hwSize);
    }    
    return false;   //!< û�ҵ���Ӧ����Ϣ
}
