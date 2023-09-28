#include "ipc.h"
#include "usart.h"


static mesg_t *mesg_buf[5];
void mesgqueue_init(void)
{
//    mesg_buf;
}

void mesgqueue_write(mesg_t *msg)
{
    mesg_buf[0] = msg;
}

mesg_t * mesgqueue_read(void)
{
    return mesg_buf[0];
}

/*IPC消息封包*/
mesg_t* ipc_mesg_packet(unsigned short id,unsigned short len)
{
    unsigned char* pbuf;
    pbuf = (unsigned char*)malloc(len);
    /*开始封包*/
    mesg_t *pMsg;
    pMsg = (mesg_t *)&pbuf[0];
    pMsg->id = 0x01;
    pMsg->len = len-sizeof(mesg_t);
    pMsg->pdata = &pbuf[sizeof(mesg_t)];
    return pMsg;
}




msg_list_t msgpool_cb;
void ipc_msgpool_init(void)
{
    msgpool_cb.head = 0;
    msgpool_cb.next = 0;
    msgpool_cb.pdata = 0;
}

void ipc_msgpool_write(mesg_t *msg)
{
    mesg_t *cur_node;
    if(msgpool_cb.head != 0)
    {
        do{
            cur_node = msgpool_cb.head->next;
            if(cur_node == 0)
            {
                cur_node->next = msg;
                break;
            }else{
                cur_node = cur_node->next;//更新节点
            }
        }while(1);
    }else{
        // USER_DEBUG_RTT("XXXXXX\r\n");
        msgpool_cb.head = msg;
    }
    msgpool_cb.node_numb++;
}

mesg_t* ipc_msgpool_read(void)
{
    if(msgpool_cb.node_numb<=0)
    {
        return 0;
    }
    return msgpool_cb.head;
}

#include "usart.h"
void ipc_msgpool_del(mesg_t *msg)
{
    mesg_t *cur_node;
    mesg_t *pre_node;
    cur_node = msgpool_cb.head;
    pre_node = cur_node;
    while(cur_node != msg)
    {
        cur_node = cur_node->next;
    }
    /*找到要删除的节点*/
    pre_node->next = cur_node->next;
    msgpool_cb.node_numb--;
}


