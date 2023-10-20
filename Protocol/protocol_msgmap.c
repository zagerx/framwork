
#include "macro_defined.h"
#include "protocol_cfg.h"
#include "protocol_comment.h"
/*
���ļ�������
*/

#undef NULL
#define NULL 0

bool press_data_updata(msg_item_t *p,void *pdata,uint_fast16_t hwSize)
{
}

const msg_item_t c_tMSGTable[] = {
    [0] = {
        .chID = 8,
        .chAccess = 9,
        .fnHandler = press_data_updata,
    },
};

static volatile uint8_t s_chCurrentAccessPermission =0x07;

/*! \brief ������Ϣ��ͼ����ִ�ж�Ӧ�Ĵ������
 *! \retval false  ��Ϣ�����ڻ�����Ϣ����������������Ч
 *! \retval true   ��Ϣ�õ�����ȷ�Ĵ���
 */
char search_msgmap(uint_fast8_t chID,
                   void *pData,
                   uint_fast16_t hwSize)
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
