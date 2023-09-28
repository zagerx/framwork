#include "fsm.h"
#include "usart.h"
#include "protocol.h"

#undef NULL
#define NULL 0                  
#undef this
#define this (*ptThis)

fsm_cb_t cmd_cb;

static fsm_rt_t protocl_cmdtype_01_state(fsm_cb_t *ptThis);
static fsm_rt_t protocl_cmdtype_idle_state(fsm_cb_t *ptThis);
static char protocol_cmd_init(fsm_cb_t *ptThis)
{
    if(ptThis == NULL) {
        return -1; 
    }
    this.fsm = protocl_cmdtype_idle_state;
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
    DISPATCH_FSM(&cmd_cb);
}

static fsm_rt_t protocl_cmdtype_idle_state(fsm_cb_t *ptThis)
{
    enum {
        FIRST = USER,
    };
    mesg_t *p_readMsg;
    mesg_t readMsg;
    pro_frame_t *p_readfram;
    unsigned char *p_ucfram;
    unsigned char buf[32] = {0};
    static unsigned int t0 = 0;
    static unsigned short fram_len;
    switch (this.chState)
    {
    case START:
        if(!GET_IPC_EVENT(PROTOCOL_CMD_01))
        {
            break;
        }
        CLEAR_IPC_EVENT(PROTOCOL_CMD_01);
    //    p_readMsg = mesgqueue_read();//消息队列不为空
        p_readMsg = ipc_msgpool_read();
        protocol_sendfram((pro_frame_t *)p_readMsg->pdata,p_readMsg->len); 
        // ipc_msgpool_del(p_readMsg);
        free(p_readMsg);        
        /*查看命令*/
        // p_readfram = (pro_frame_t *)p_readMsg->pdata;
        // if(p_readfram->func_c == 0x0c00)
        // {//该命令需要响应
        //     fram_len = p_readMsg->len;
        //     p_ucfram = malloc(p_readMsg->len);
        //     memcpy(p_ucfram,p_readMsg->pdata,p_readMsg->len);
        //     free(p_readMsg);
        //     this.chState = FIRST;
        // }else{//无需响应
        //     protocol_sendfram((pro_frame_t *)p_readMsg->pdata,p_readMsg->len); 
        //     ipc_msgpool_del(p_readMsg);
        //     free(p_readMsg);
        //     this.chState = START;//状态机复位
        //     break;
        // }
        break;
    case FIRST:
        p_readfram = (pro_frame_t *)p_ucfram;
        protocol_sendfram(p_readfram,fram_len);    
        free(p_readfram);
        this.chState = START;//状态机复位
    case EXIT:
        break;
    default:
        break;
    }
    return fsm_rt_on_going;
}
