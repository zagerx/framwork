// #include "protocol.h"
#include "protocol_comment.h"
#include "protocol_cfg.h"

#undef  NULL
#define NULL 0   

/*
**  Э��ײ㷢�ͽӿڣ�Ӧ����Ӳ���ӿ����ⲿʵ��
*/
__attribute__((weak)) void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
}

/*
**  len:
*/
static char _send_proframe(pro_frame_t *msg,unsigned short len)
{
    /*����֡����*/
    unsigned char *pucmsg;
    unsigned char *pdata;
    unsigned short data_len;
    data_len = len-sizeof(pro_frame_t);
    unsigned char buf[2] = {0};
    pucmsg = malloc(len);
    memcpy(pucmsg,msg,len);
    memcpy(buf,(unsigned char*)&pucmsg[FRAM_TAIL_OFFSET],2);
    pdata = malloc(data_len);
    memcpy(pdata,(unsigned char*)&pucmsg[sizeof(pro_frame_t)],data_len);
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET],pdata,data_len);    
    free(pdata);
    unsigned short crc_16;
    crc_16 = CRC16_Subsection((unsigned char*)&pucmsg[2],0xFFFF,FRAM_PDATA_OFFSET+data_len-2);
    crc_16 = __SWP16(crc_16);
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len],(unsigned char*)&crc_16,sizeof(crc_16));   
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len+2],buf,2); 
    _bsp_protransmit(pucmsg,sizeof(pro_frame_t)-sizeof(void *)+data_len);
    free(pucmsg);
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
    free(p_fram);
    return NULL;
}

/*--------------------------------Э���ط�����------------------------------------------------

Ҫ��:
�������n��ָ�ÿ��ָ����뱻��Ӧ(��:��ӦΪָ��ȡ��)
û����ȷ��Ӧ��Ӧ����ɾ���ôη���

1�����ͽӿ�:    protocol_transmit(frame��timeout��cnt)
2��1������һ���ط��ľ��
        a�����Ա���ʱɾ��(������ɱ�ɾ��)
        b��ÿ��frame��Ӧһ�����
        c��������ʽ����


--------------------------------------------------------------------------------*/
#if 0
#include "fsm.h"
#undef  NULL
#define NULL 0                  
#undef  this
#define this (*ptThis)


void protocol_trans_process(void)
{
    msg_t *p_readMsg,temp;
    fsm_cb_t *pfsm;
    temp.id = 0xFFFE;
    
    p_readMsg = &temp;
    p_readMsg = (msg_t *)ipc_msgpool_read_val(p_readMsg);

    if (p_readMsg == NULL)
    {
        return;
    }
    pfsm = (fsm_cb_t *)p_readMsg->pdata;
    if (pfsm)
    {
        DISPATCH_FSM(pfsm);
    }
}


/*
����״̬��  ptThis�������ϢΪ�����͵���Ϣ
��Ҫ����Э����ط�����
*/
static fsm_rt_t protocol_send(fsm_cb_t *ptThis)
{
    enum {
        IDLE = USER,
        SEND_NOW,
        FIRST,
        SECOND,
        THIRD,
        FOURTH,
        ERR,
    };
    unsigned int delta_t;
    float data_buf[2] = {6.4f,2.3f};
    
    static unsigned int t0;
    static unsigned char cnt;
    static msg_t *p_wait_send_msg;//�����͵���Ϣ
    static pro_frame_t *p_wait_send_fram;  
    static unsigned char wait_send_cmd;
    static unsigned char wait_send_cmdtype;

    switch (this.chState)
    {
    case START:
        {
            t0 = 0;
            cnt = 0;
            /*��¼״̬����Я������*/
            p_wait_send_msg = (msg_t *)ptThis->pdata;//��ȡ״̬������������
            p_wait_send_fram = (pro_frame_t*)p_wait_send_msg->pdata;
            if(p_wait_send_msg == NULL || p_wait_send_fram==NULL)
            {
                return fsm_rt_cpl;
            }
            wait_send_cmd = (unsigned char)(p_wait_send_fram->func_c>>8);
            wait_send_cmdtype = (unsigned char)(p_wait_send_fram->func_c);
            this.chState = FIRST;
        }
    case FIRST:
            if(wait_send_cmdtype == CMD_RESP)
            {
                this.chState = SECOND;
            }else{
                this.chState = SEND_NOW;
            }
        break;        
    case SEND_NOW:
        /*ֱ�ӷ���*/
        _bsp_sendfram((pro_frame_t *)p_wait_send_msg->pdata,p_wait_send_msg->len);
        /*�ͷ�״̬��Я������������*/
        free(p_wait_send_fram);
        free(p_wait_send_msg);
        /*����״̬����������*/
        this.pdata = NULL;
        this.len = 0;
        this.sig = 0x02;//֪ͨ
        this.chState = EXIT;  
        break;
    case SECOND://�ش�����
        this.chState = THIRD;
    case THIRD:
        {
            _bsp_sendfram((pro_frame_t *)p_wait_send_msg->pdata,p_wait_send_msg->len);
            t0 = HAL_GetTick();
            if (cnt++>5)
            {
                cnt = 0;
                this.chState = ERR;
                break;
            }else{
                this.chState = FOURTH;
            }
        }
    case FOURTH:
        {
            msg_t *p_reciv_msg;
            pro_frame_t *p_reciv_fram;
            /*��ȡ��ǰ������յ�����Ϣ*/
            msg_t p_msg;
            p_msg.id = wait_send_cmd;
            p_reciv_msg = (msg_t *)ipc_msgpool_read_val(&p_msg);
            if (p_reciv_msg == NULL)
            {
                delta_t = HAL_GetTick()-t0;
                if(delta_t > 1000)
                {
                    this.chState = THIRD;
                }
                break;
            }
            p_reciv_fram = (pro_frame_t *)p_reciv_msg->pdata;
            if ((unsigned char)(p_reciv_fram->func_c) == CMD_ACK)//�ɹ����յ��������Ӧ
            {
                /*�ͷ�״̬��Я���ľɵ���������   */
                free(p_wait_send_fram);
                free(p_wait_send_msg);

                /*����״̬����������   �����µ����ݵ�״̬����������*/
                this.pdata = p_reciv_msg;
                this.len = p_reciv_msg->len;

                this.sig = 0x02;//֪ͨ�߳̿��Թرմ˴�ͨ��
                this.chState = EXIT;
            }
        }
        break;
    case ERR:
        /*�ͷ�״̬��Я������������*/
        free(p_wait_send_fram);
        free(p_wait_send_msg);
        /*����״̬����������*/
        this.pdata = NULL;
        this.len = 0;    
        this.sig = 0x03;//֪ͨ�߳̿��Թرմ˴�ͨ��  
        this.chState = EXIT;
        break;
    case EXIT:
        break;
    default:
        break;
    }
    return fsm_rt_on_going;   
}

/*
Э���ط�����
������
*/
msg_t* pro_send_cmd_data(unsigned short id_fsm,unsigned char cmd_type,unsigned char cmd,\
                        void *pdata,unsigned short data_len)
{
    pro_frame_t* p_fram;
    /*Э��֡���*/
    p_fram = _packet_proframe(cmd | (cmd_type<<8),pdata,data_len);
    /*׼������*/
    msg_t msg;
    msg.id = cmd;
    msg.len = sizeof(pro_frame_t)+data_len;
    /*��Ϣ���*/
    msg_t *p_msg01;
    p_msg01 = ipc_mesg_packet_02(msg.id,msg.len,p_fram);
    /*����һ�������״̬��*/
    fsm_cb_t *p_cmd_fsm;
    p_cmd_fsm = fsm_creat((fsm_t *)protocol_send,sizeof(msg_t),p_msg01);
    /*��cmd_fsm��ӵ�״̬��������*/
    msg_t *p_msg02;
    p_msg02 = ipc_mesg_packet_02(id_fsm,sizeof(fsm_cb_t),p_cmd_fsm);

    ipc_msgpool_write(p_msg02);
    return p_msg02;
}
#endif