// #include "protocol.h"
#include "protocol_comment.h"
#include "protocol_cfg.h"

#undef  NULL
#define NULL 0   

#define RESEND_CNT  5
#define TIMEOUT     1000



/*
**  Э��ײ㷢�ͽӿڣ�Ӧ����Ӳ���ӿ����ⲿʵ��
*/
__attribute__((weak)) void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
}

/*
**  len: ֡���ܳ�
*/
static char _send_proframe(pro_frame_t *msg,unsigned short len)
{
    /*����֡����*/
    unsigned char *pucmsg;
    unsigned char *pdata;
    unsigned short data_len;
    data_len = len-sizeof(pro_frame_t);
    unsigned char buf[2] = {0};
    pucmsg = heap_malloc(len);
    memcpy(pucmsg,msg,len);
    memcpy(buf,(unsigned char*)&pucmsg[FRAM_TAIL_OFFSET],2);
    pdata = heap_malloc(data_len);
    memcpy(pdata,(unsigned char*)&pucmsg[sizeof(pro_frame_t)],data_len);
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET],pdata,data_len);    
    heap_free(pdata);
    unsigned short crc_16;
    crc_16 = CRC16_Subsection((unsigned char*)&pucmsg[2],0xFFFF,FRAM_PDATA_OFFSET+data_len-2);
    crc_16 = __SWP16(crc_16);
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len],(unsigned char*)&crc_16,sizeof(crc_16));   
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len+2],buf,2); 
    _bsp_protransmit(pucmsg,sizeof(pro_frame_t)-sizeof(void *)+data_len);
    heap_free(pucmsg);
	return 0;
}

char protocol_nowtransmit(unsigned char cmd_type,unsigned char cmd,\
                            void *pdata,unsigned short data_len)
{
    pro_frame_t* p_fram;
    /*Э��֡���*/
    p_fram = _packet_proframe(cmd | (cmd_type<<8),pdata,data_len);
    /*׼������*/
    _send_proframe(p_fram,sizeof(pro_frame_t)+data_len);
    heap_free(p_fram);
    return NULL;
}



/*---------------------------------------Э���ط�����--------------------------------------------------*/
fsm_rt_t _trancemit_statemach(fsm_cb_t *ptThis)
{
    enum{
        SEND_CMD = USER,
        WAIT_ACK,
        RECIVE_ACK,
        IDLE,
    };
    pro_pack_t *pmsg = (pro_pack_t*)ptThis;
    pro_frame_t *pframe;
    unsigned short cmd ;    
    unsigned char cmd_fun;  
    unsigned char cmd_type;
    unsigned short data_len;
    unsigned char data[16] = {0};
    unsigned short fsm_cycle = 0;
    fsm_cycle = pmsg->statemach_cb.cycle;
    pframe = pmsg->frame;
    cmd  = __SWP16(pframe->func_c);
    cmd_fun  = (unsigned char)cmd;
    cmd_type  = (unsigned char)(cmd>>8); 
    data_len = __SWP16(pframe->len);

    unsigned char event = 0;
    float buf[4] = {0.0f};
    switch (ptThis->chState)
    {
    case START:
        ptThis->chState = SEND_CMD;
    case SEND_CMD:
        // _send_proframe(pframe,data_len+sizeof(pro_frame_t));
        USER_DEBUG("send one cmd\r\n");
        ptThis->chState = WAIT_ACK;        
    case WAIT_ACK:
        if (pmsg->recnt >= RESEND_CNT)
        {
            pmsg->recnt = 0;
            /* û�н��յ����������Ӧ */
            USER_DEBUG("cmd no ack\r\n");
            ptThis->chState = EXIT;  
            return fsm_rt_cpl;
        }
        event = forch_keymap_enevt(cmd_fun);
        if (!IPC_GET_EVENT(g_protocol_event,event))//û�н��յ���Ӧ
        {
            /* code */
            if (pmsg->timeout++ >= TIMEOUT/fsm_cycle)//��ʱ
            {
                /* code */
                USER_DEBUG("cmd timeout\r\n");
                pmsg->timeout = 0;
                pmsg->recnt++;
                ptThis->chState = SEND_CMD;
            }
            break;
        }
        ptThis->chState = RECIVE_ACK;
    case RECIVE_ACK:
        USER_DEBUG("recive cmd ack\r\n");
        /*���յ������Ӧ����Ӧ �������ݴ���*/
        ptThis->chState = EXIT;
        return fsm_rt_cpl;
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
    /*Э��֡���*/
    p_fram = _packet_proframe(cmd | (cmd_type<<8),pdata,data_len);

    /*��Ϣ���з��*/
    pro_pack_t *pmsg;
    pmsg = _packet_propack(p_fram,1000,5);
/*-------------------------------list node ���--------------------*/
    /*�ڵ�item*/
    _lsit_item_t *pitem = list_creat_item(pmsg,sizeof(_lsit_item_t));
    /*list�ڵ���*/
    _node_t *pnode;
    pnode = list_creatnode(pitem);
/*-----------------------------------------------------------------*/    
    /*��ӵ���Ϣ�б���*/
    list_insert_node(g_transmit_handle,pnode);
}

void protocol_transmitprocess(void)
{
    /*��ȡ���� �鿴�Ƿ��д����͵���Ϣ*/
    _node_t *cur_node;
    _lsit_item_t *cur_item;
    pro_pack_t *pack;
    pro_frame_t* p_fram;     
    cur_node = g_transmit_handle->head;
    while (cur_node != 0)//������������
    {
        cur_item = cur_node->pitem;
        pack = (pro_pack_t *)cur_item->pdata;
        p_fram = (pro_frame_t *)pack->frame;
        if(DISPATCH_FSM(((fsm_cb_t *)pack)) == fsm_rt_cpl)
        {
            /*��ǰ״̬��ִ�н���  ����ɾ��*/
            heap_free(p_fram);
            heap_free(pack);
            list_delete_node(g_transmit_handle,cur_node);
            USER_DEBUG("free \r\n");
        }
        cur_node = cur_node->next; 
    }
}
