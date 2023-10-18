
#include "protocol.h"
#include "stdint.h"
#include "usart.h"
#include "macro_defined.h"
/*
���ļ�������
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

/*! \brief ������Ϣ��ͼ����ִ�ж�Ӧ�Ĵ������
 *! \retval false  ��Ϣ�����ڻ�����Ϣ����������������Ч
 *! \retval true   ��Ϣ�õ�����ȷ�Ĵ���
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
