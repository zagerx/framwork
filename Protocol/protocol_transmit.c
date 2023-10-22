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
    /*协议帧封包*/
    p_fram = _packet_proframe(cmd | (cmd_type<<8),pdata,data_len);
    /*准备数据*/
    _send_proframe(p_fram,sizeof(pro_frame_t)+data_len);
    free(p_fram);
    return NULL;
}

/*--------------------------------协议重发机制------------------------------------------------

要求:
随机发送n条指令，每条指令都必须被响应(如:响应为指令取反)
没有正确响应的应彻底删除该次发送

1、发送接口:    protocol_transmit(frame，timeout，cnt)
2、1、创建一个重发的句柄
        a、可以被随时删除(发送完成被删除)
        b、每个frame对应一个句柄
        c、非阻塞式发送


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
发送状态机  ptThis里面的消息为待发送的消息
需要完善协议的重发机制
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
    static msg_t *p_wait_send_msg;//待发送的消息
    static pro_frame_t *p_wait_send_fram;  
    static unsigned char wait_send_cmd;
    static unsigned char wait_send_cmdtype;

    switch (this.chState)
    {
    case START:
        {
            t0 = 0;
            cnt = 0;
            /*记录状态机所携带数据*/
            p_wait_send_msg = (msg_t *)ptThis->pdata;//获取状态机机的数据域
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
        /*直接发送*/
        _bsp_sendfram((pro_frame_t *)p_wait_send_msg->pdata,p_wait_send_msg->len);
        /*释放状态机携带的数据区域*/
        free(p_wait_send_fram);
        free(p_wait_send_msg);
        /*更新状态机数据区域*/
        this.pdata = NULL;
        this.len = 0;
        this.sig = 0x02;//通知
        this.chState = EXIT;  
        break;
    case SECOND://重传发送
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
            /*获取当前命令接收到的消息*/
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
            if ((unsigned char)(p_reciv_fram->func_c) == CMD_ACK)//成功接收到命令的响应
            {
                /*释放状态机携带的旧的数据区域   */
                free(p_wait_send_fram);
                free(p_wait_send_msg);

                /*更新状态机数据区域   更新新的数据到状态机的数据域*/
                this.pdata = p_reciv_msg;
                this.len = p_reciv_msg->len;

                this.sig = 0x02;//通知线程可以关闭此次通信
                this.chState = EXIT;
            }
        }
        break;
    case ERR:
        /*释放状态机携带的数据区域*/
        free(p_wait_send_fram);
        free(p_wait_send_msg);
        /*更新状态机数据区域*/
        this.pdata = NULL;
        this.len = 0;    
        this.sig = 0x03;//通知线程可以关闭此次通信  
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
协议重发机制
需完善
*/
msg_t* pro_send_cmd_data(unsigned short id_fsm,unsigned char cmd_type,unsigned char cmd,\
                        void *pdata,unsigned short data_len)
{
    pro_frame_t* p_fram;
    /*协议帧封包*/
    p_fram = _packet_proframe(cmd | (cmd_type<<8),pdata,data_len);
    /*准备数据*/
    msg_t msg;
    msg.id = cmd;
    msg.len = sizeof(pro_frame_t)+data_len;
    /*消息封包*/
    msg_t *p_msg01;
    p_msg01 = ipc_mesg_packet_02(msg.id,msg.len,p_fram);
    /*创建一个命令发送状态机*/
    fsm_cb_t *p_cmd_fsm;
    p_cmd_fsm = fsm_creat((fsm_t *)protocol_send,sizeof(msg_t),p_msg01);
    /*将cmd_fsm添加到状态机链表中*/
    msg_t *p_msg02;
    p_msg02 = ipc_mesg_packet_02(id_fsm,sizeof(fsm_cb_t),p_cmd_fsm);

    ipc_msgpool_write(p_msg02);
    return p_msg02;
}
#endif