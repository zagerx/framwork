#include "fsm.h"
#include "usart.h"
#include "protocol.h"
#include "stdlib.h"
#include "string.h"
#undef NULL
#define NULL 0                  
#undef this
#define this (*ptThis)

fsm_cb_t cmd_cb;

static fsm_rt_t protocl_cmdtype_idle_state(fsm_cb_t *ptThis);
static char protocol_cmd_init(fsm_cb_t *ptThis)
{
    if(ptThis == NULL) {
        return -1; 
    }
    this.fsm = (fsm_t *)protocl_cmdtype_idle_state;
    this.sig = 0;
    this.chState = START;
    return 0;
}
void _protocol_cmd_init(void)
{
    protocol_cmd_init(&cmd_cb);
}
void protocl_cmd_process(void)
{
    // DISPATCH_FSM(&cmd_cb);

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


static fsm_rt_t protocl_cmdtype_idle_state(fsm_cb_t *ptThis)
{
    enum {
        IDLE = USER,
        SEND_NOW,
        SECOND,
        THIRD,
        FOURTH,
        ERR,
    };
    static msg_t *p_readMsg;
    pro_frame_t *p_readfram;
    static unsigned char *p_ucfram;
    static unsigned short fram_len;
    static unsigned int t0;
    static unsigned short cmd_ack;
    unsigned int delta_t;
    static unsigned char cnt;
    unsigned short cmd_tmp;

    unsigned char len;
    float data_buf[2] = {6.4f,2.3f};
    unsigned char data_len;
    unsigned char *puctemp;
    msg_t *p_msg; 
    switch (this.chState)
    {
    case START:
        p_readMsg = NULL;
        p_ucfram = NULL;
        fram_len = 0;
        cmd_ack = 0;
        cnt = 0;
        cmd_tmp = 0;
        break;
        /*获取消息池里面的消息*/
        p_readMsg = (msg_t *)ipc_msgpool_read();
        if (p_readMsg == NULL)
        {
            break;
        }
        /*对消息池里面的消息解析*/
        p_readfram = (pro_frame_t *)p_readMsg->pdata;
        cmd_tmp = __SWP16(p_readfram->func_c);
        cmd_ack = (unsigned char)cmd_tmp | (unsigned short)(CMD_ACK<<8); //获取对应命令的ACK
        if(cmd_tmp>>8 != CMD_RESP)
        {//该命令不需要响应
            this.chState = SEND_NOW;
        }else{
            this.chState = SECOND;
        }
        break;
    case SEND_NOW:
        protocol_sendfram((pro_frame_t *)p_readMsg->pdata,p_readMsg->len); 
        ipc_msgpool_del(p_readMsg);
        free(p_readMsg);
        this.chState = START;
        break;
    case SECOND:
        fram_len = p_readMsg->len;
        p_ucfram = malloc(fram_len);
        memcpy((unsigned char *)p_ucfram,p_readMsg->pdata,fram_len);
        ipc_msgpool_del(p_readMsg);
        free(p_readMsg);
        this.chState = THIRD;
    case THIRD:
        protocol_sendfram((pro_frame_t *)p_ucfram,fram_len); //先发送命令给从机
        t0 = HAL_GetTick();
        if (cnt++>5)
        {
            free(p_ucfram);
            this.chState = ERR;
            break;
        }else{
            this.chState = FOURTH;
        }
    case FOURTH:

        data_len = 0;
        /*计算消息的整体大小*/
        len =sizeof(msg_t) + sizeof(pro_frame_t) + data_len;
        /*计算消息大小*/
        p_msg = ipc_mesg_packet(0x01,len);
        puctemp = (unsigned char*)pro_frame_packet_sigle(PRO_FUNC_C_PF300 | (CMD_RESP<<8),data_buf,data_len);
        memcpy((unsigned char*)p_msg->pdata,puctemp,sizeof(pro_frame_t) + data_len);
        free(puctemp);



        p_readMsg = (msg_t *)ipc_msgpool_read_val(p_msg);
        if (p_readMsg == NULL)
        {
            delta_t = HAL_GetTick()-t0;
            if(delta_t > 1000)
            {           
                this.chState = THIRD;
            }
            break;
        }
        p_readfram = (pro_frame_t *)p_readMsg->pdata;
        if (p_readfram->func_c == cmd_ack)
        {//成功接收到命令的响应
            ipc_msgpool_del(p_readMsg);
            free(p_readMsg);
            free(p_ucfram);
            this.chState = START;//复位状态机
        }
        break;
    case ERR:
        USER_DEBUG_RTT("no respond\r\n");
        this.chState = START;//复位状态机
        break;    
    case EXIT:
        break;
    default:
        break;
    }
    return fsm_rt_on_going;
}
















// static fsm_rt_t protocl_cmdtype_idle_state(fsm_cb_t *ptThis)
// {
//     enum {
//         FIRST = USER,
//     };
//     msg_t *p_readMsg;
//     pro_frame_t *p_readfram;
//     unsigned char *p_ucfram;
//     static unsigned short fram_len;
//     switch (this.chState)
//     {
//     case START:
//         if(!GET_IPC_EVENT(PROTOCOL_CMD_01))
//         {
//             break;
//         }
//         CLEAR_IPC_EVENT(PROTOCOL_CMD_01);
//         // p_readMsg = (msg_t *)ipc_msgpool_read();
//         // protocol_sendfram((pro_frame_t *)p_readMsg->pdata,p_readMsg->len); 
//         // ipc_msgpool_del(p_readMsg);
//         // free(p_readMsg);
//         /*获取消息*/
//         p_readMsg = (msg_t *)ipc_msgpool_read();
//         /*查看命令*/
//         p_readfram = (pro_frame_t *)p_readMsg->pdata;
//         if(p_readfram->func_c == 0x0B00)
//         {//该命令需要响应
//             fram_len = p_readMsg->len;
//             p_ucfram = malloc(p_readMsg->len);
//             memcpy((unsigned char *)p_ucfram,p_readMsg->pdata,p_readMsg->len);
//             free(p_readMsg);
//             this.chState = FIRST;
//         }else{//无需响应
//             protocol_sendfram((pro_frame_t *)p_readMsg->pdata,p_readMsg->len); 
//             ipc_msgpool_del(p_readMsg);
//             free(p_readMsg);
//             this.chState = START;//状态机复位
//             break;
//         }
//         break;
//     case FIRST:
//         p_readfram = (pro_frame_t *)p_ucfram;
//         protocol_sendfram(p_readfram,fram_len);
//         free(p_readfram);
//         this.chState = START;//状态机复位
//     case EXIT:
//         break;
//     default:
//         break;
//     }
//     return fsm_rt_on_going;
// }
