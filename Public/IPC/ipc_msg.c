#include "ipc.h"
#include "usart.h"
#include "stdlib.h"
#include "malloc.h"



static _list_t *_01_head;

_node_t *creat_node(_lsit_item_t *pmsg)
{
    _node_t *p = NULL;
    p = pvPortMalloc(sizeof(_node_t));
    p->next = 0;
    p->pitem = pmsg;
    return p;
}

// _list_t msgpool_cb;
void ipc_msgpool_init(void)
{
    _01_head = list_creat();
}

void ipc_msgpool_write(_lsit_item_t *msg)
{
    _node_t *pnew_node;
    pnew_node = creat_node(msg);
    list_insert_node(_01_head, pnew_node);
}

_lsit_item_t* ipc_msgpool_read(void)
{
    static _node_t *pnode;
    pnode = list_read_node(_01_head);
    if (pnode != NULL)
    {
        /* code */
        return (pnode)->pitem;
    }
    
    return (_lsit_item_t*)NULL;
}
_lsit_item_t* ipc_msgpool_read_val(_lsit_item_t* msg)
{
    static _node_t *pnode,node;
    node.pitem = msg;
    pnode = list_read_node_vale(_01_head,&node);
    if (pnode != NULL)
    {
        /* code */
        return (pnode)->pitem;
    }
    return (_lsit_item_t*)NULL;
}
#include "usart.h"
void ipc_msgpool_del(_lsit_item_t *msg)
{
    _node_t *pdel_node;
    _node_t temp_node;
    if (msg== NULL)
    {
        /* code */
        USER_DEBUG("no use data\r\n");
        return ;
    }
    
    pdel_node = &temp_node;
    pdel_node->pitem = msg;
    list_delete_node(_01_head,pdel_node);
}


void ipc_msg_printf(void)
{
    _list_printf(_01_head);
}

void ipc_msg_printf_number(void)
{
    _list_printf_number(_01_head);
}



/*IPC消息封包*/
_lsit_item_t* ipc_mesg_packet(unsigned short id,unsigned short len)
{
    unsigned char* pbuf;
    pbuf = (unsigned char*)pvPortMalloc(len);
    /*开始封包*/
    _lsit_item_t *pMsg;
    pMsg = (_lsit_item_t *)&pbuf[0];
    pMsg->len = len-sizeof(_lsit_item_t);
    pMsg->pdata = &pbuf[sizeof(_lsit_item_t)];
    return pMsg;
}

_lsit_item_t* ipc_mesg_packet_02(unsigned short id,unsigned short len,void *pbuf)
{
    _lsit_item_t *pMsg = (_lsit_item_t *)pvPortMalloc(sizeof(_lsit_item_t));
    pMsg->len = len;
    pMsg->pdata = pbuf;
    return pMsg;
}



/*********************/











