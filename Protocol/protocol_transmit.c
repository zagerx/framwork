// #include "protocol.h"
#include "protocol_comment.h"
#include "protocol_cfg.h"

#undef  NULL
#define NULL 0   

/*
**  协议底层发送接口，应根据硬件接口由外部实现
*/
__attribute__((weak)) void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
}

/*
**  len:
*/
static char _send_proframe(pro_frame_t *msg,unsigned short len)
{
    /*整理帧数据*/
    unsigned char *pucmsg;
    unsigned char *pdata;
    unsigned short data_len;
    data_len = len-sizeof(pro_frame_t);
    unsigned char buf[2] = {0};
    pucmsg = pvPortMalloc(len);
    memcpy(pucmsg,msg,len);
    memcpy(buf,(unsigned char*)&pucmsg[FRAM_TAIL_OFFSET],2);
    pdata = pvPortMalloc(data_len);
    memcpy(pdata,(unsigned char*)&pucmsg[sizeof(pro_frame_t)],data_len);
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET],pdata,data_len);    
    vPortFree(pdata);
    unsigned short crc_16;
    crc_16 = CRC16_Subsection((unsigned char*)&pucmsg[2],0xFFFF,FRAM_PDATA_OFFSET+data_len-2);
    crc_16 = __SWP16(crc_16);
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len],(unsigned char*)&crc_16,sizeof(crc_16));   
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len+2],buf,2); 
    _bsp_protransmit(pucmsg,sizeof(pro_frame_t)-sizeof(void *)+data_len);
    vPortFree(pucmsg);
	return 0;
}

char protocol_nowtransmit(unsigned char cmd_type,unsigned char cmd,\
                            void *pdata,unsigned short data_len)
{
    pro_frame_t* p_fram;
    /*协议帧封包*/
    p_fram = _packet_proframe(cmd | (cmd_type<<8),pdata,data_len);
    /*准备数据*/
    _send_proframe(p_fram,sizeof(pro_frame_t)+data_len);
    vPortFree(p_fram);
    return NULL;
}
/*---------------------------------------协议重发机制--------------------------------------------------*/
fsm_rt_t _trancemit_statemach(fsm_cb_t *ptThis)
{
    unsigned int delta_t;
    float data_buf[2] = {6.4f,2.3f};
    
    pro_pack_t *pmsg = (pro_pack_t*)ptThis;
    
    switch (ptThis->chState)
    {
    case START:
        if (pmsg->recnt++>7)
        {
            return fsm_rt_cpl;
        }        
    case EXIT:
        break;
    default:
        break;
    }
    return fsm_rt_on_going;   
}
void protocol_transmit(unsigned char cmd_type,unsigned char cmd,\
                            void *pdata,unsigned short data_len)
{
    pro_frame_t* p_fram;
    /*协议帧封包*/
    p_fram = _packet_proframe(cmd | (cmd_type<<8),pdata,data_len);

    /*消息队列封包*/
    pro_pack_t *pmsg;
    pmsg = _packet_propack(p_fram,1000,5);
/*-------------------------------list node 封包--------------------*/
    /*节点item*/
    _lsit_item_t *pitem = list_creat_item(pmsg,sizeof(_lsit_item_t));
    /*list节点封包*/
    _node_t *pnode;
    pnode = list_creatnode(pitem);
/*-----------------------------------------------------------------*/    
    /*添加到消息列表即可*/
    list_insert_node(g_transmit_handle,pnode);
}

void protocol_transmitprocess(void)
{
    /*获取链表 查看是否有待发送的消息*/
    _node_t *cur_node;
    _lsit_item_t *cur_item;
    pro_pack_t *pack;
    pro_frame_t* p_fram;     
    cur_node = g_transmit_handle->head;
    while (cur_node != 0)//遍历整个链表
    {
        cur_item = cur_node->pitem;
        pack = (pro_pack_t *)cur_item->pdata;
        p_fram = (pro_frame_t *)pack->frame;
        if(DISPATCH_FSM(((fsm_cb_t *)pack)) == fsm_rt_cpl)
        {
            /*当前状态机执行结束  可以删除*/
            vPortFree(p_fram);
            vPortFree(pack);
            list_delete_node(g_transmit_handle,cur_node);
            USER_DEBUG("free \r\n");
        }
        cur_node = cur_node->next; 
    }
}
