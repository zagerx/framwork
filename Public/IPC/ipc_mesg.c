#include "ipc.h"
#include "usart.h"
#include "stdlib.h"
#include "malloc.h"

#undef NULL
#define NULL 0


/*IPC消息封包*/
msg_t* ipc_mesg_packet(unsigned short id,unsigned short len)
{
    unsigned char* pbuf;
    pbuf = (unsigned char*)malloc(len);
    /*开始封包*/
    msg_t *pMsg;
    pMsg = (msg_t *)&pbuf[0];
    pMsg->id = 0x01;
    pMsg->len = len-sizeof(msg_t);
    pMsg->pdata = &pbuf[sizeof(msg_t)];
    return pMsg;
}


_list_t *_01_head;


_node_t *creat_node(msg_t *pmsg)
{
    _node_t *p = NULL;
    p = malloc(sizeof(_node_t));
    p->next = 0;
    p->msg = pmsg;
    return p;
}

_list_t* list_creat(void)
{
    _list_t *phead;
    phead = malloc(sizeof(_list_t));
    phead->head = 0;
    phead->tail = 0;
    phead->node_numb = 0;
    return phead;
}

void list_insert_node(_list_t *pthis, _node_t *node)
{
    _node_t *tail_node;
    /*当前链表是否为空*/
    if (pthis->head == 0)
    {
        /* code */
        pthis->head = node;
        pthis->tail = node;
    }else{
        tail_node = pthis->tail;
        tail_node->next = node;
        pthis->tail = tail_node->next;//更新尾节点
    }
    pthis->node_numb++;
}
void list_delete_node(_list_t *pthis,_node_t *node)
{
    _node_t *cur_node,*pre_node = NULL;
    cur_node = pthis->head;
    /*1、找到该节点*/
    while (0 != cur_node)//遍历整个链表
    {
        /* code */
        if (cur_node->msg != node->msg)//
        {
            /* code */
            pre_node = cur_node;
            cur_node = cur_node->next;
        }else{//找到
            /*保存上一个节点  保存下一个节点*/
            pre_node->next = cur_node->next;
            break;
        }
    }
    free(cur_node);
    pthis->node_numb--;
}
_node_t* list_read_node(_list_t *pthis)
{
    _node_t *p_rnode;
    if (pthis->node_numb == 0)
    {
        return (_node_t *)NULL;
    }
    p_rnode = pthis->head;
    return (_node_t *)p_rnode;
}
void list_free(_list_t *pthis, _node_t *node)
{
    _node_t *cur_node,*next_node;
    cur_node = pthis->head;

    while (cur_node !=0 )
    {
        /* code */
        /*记录下一个节点*/
        next_node = cur_node->next;
        free(cur_node);
        cur_node = next_node;
    }
    free(pthis);
}


_list_t msgpool_cb;
void ipc_msgpool_init(void)
{
    _01_head = list_creat();
}

void ipc_msgpool_write(msg_t *msg)
{
    _node_t *pnew_node;
    pnew_node = creat_node(msg);
    list_insert_node(_01_head, pnew_node);
}

msg_t* ipc_msgpool_read(void)
{
    static _node_t *pnode;
    pnode = list_read_node(_01_head);
    if (pnode != NULL)
    {
        /* code */
        return (pnode)->msg;
    }
    
    return (msg_t*)NULL;

}

#include "usart.h"
void ipc_msgpool_del(msg_t *msg)
{
    _node_t *pdel_node;
    _node_t temp_node;
    pdel_node = &temp_node;
    pdel_node->msg = msg;
    list_delete_node(_01_head,pdel_node);

}


