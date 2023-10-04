#include "ipc.h"
#include "usart.h"
#include "stdlib.h"
#include "malloc.h"

#undef NULL
#define NULL 0

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
    pre_node = pthis->head;
    if (cur_node == NULL)
    {
        /* code */
        return;
    }
    while (cur_node->msg != node->msg)
    {
        /* code */
        pre_node = cur_node;
        cur_node = cur_node->next;
        if (cur_node == NULL)
        {
            /* code */
            break;
        }
    }
    pthis->head = cur_node->next;

    free(cur_node);
    pthis->node_numb--;
}

_node_t* list_read_node_vale(_list_t *pthis,_node_t *node)
{
    _node_t *p_rnode;
    _node_t *p_curnode;
    if (pthis->node_numb == 0)
    {
        return (_node_t *)NULL;
    }
    p_curnode = pthis->head;

    unsigned char *p1,p2;
    unsigned short len;
    len = node->msg->len;
    p1 = node->msg->pdata;
    p2 = p_curnode->msg->pdata;
    while (p_curnode->msg != node->msg)
    {
        p_curnode = p_curnode->next;
    }
    return p_curnode;
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
        /*记录下一个节点*/
        next_node = cur_node->next;
        free(cur_node);
        cur_node = next_node;
    }
    free(pthis);
}


void _list_printf(_list_t *pthis)
{
    unsigned short len;
    unsigned char* pdata;    
    if (pthis->node_numb==0)
    {
        USER_DEBUG_RTT("list no node\r\n");
        return;
    }
    _node_t *cur_node;
    cur_node = pthis->head;
    while (cur_node != NULL)
    {
        len = cur_node->msg->len;
        pdata = cur_node->msg->pdata;
        HAL_UART_Transmit(&huart1,pdata,len,0xFFFF);        
        cur_node = cur_node->next;
    }

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
msg_t* ipc_msgpool_read_val(msg_t* msg)
{
    static _node_t *pnode;
    pnode = list_read_node_vale(_01_head,msg);
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


void ipc_msg_printf(void)
{
    _list_printf(_01_head);
}

/*IPC消息封包*/
msg_t* ipc_mesg_packet(unsigned short id,unsigned short len)
{
    unsigned char* pbuf;
    pbuf = (unsigned char*)malloc(len);
    /*开始封包*/
    msg_t *pMsg;
    pMsg = (msg_t *)&pbuf[0];
    pMsg->id = id;
    pMsg->len = len-sizeof(msg_t);
    pMsg->pdata = &pbuf[sizeof(msg_t)];
    return pMsg;
}


