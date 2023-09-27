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
    static unsigned int t0 = 0;
    switch (this.chState)
    {
    case START:
    case FIRST:
        if(GET_IPC_EVENT(PROTOCOL_CMD_01))
        {
            CLEAR_IPC_EVENT(PROTOCOL_CMD_01);
            TRAN_STATE(&cmd_cb,protocl_cmdtype_01_state);
        }
        break;
    case EXIT:
        break;
    default:
        break;
    }
    return fsm_rt_on_going;
}

static fsm_rt_t protocl_cmdtype_01_state(fsm_cb_t *ptThis)
{
    enum {
        FIRST = USER,
        WAIT_ACK,
    };
    mesg_t *p_readMsg;
    pro_frame_t *p_readfram;
    unsigned char buf[32] = {0};
    static unsigned int t0 = 0;
    switch (this.chState)
    {
    case START:
    case FIRST:
        /*��ȡҪ���͵�����֡*/

        /*�ȴ���Ϣ�����ڵ���Ϣ*/

        /*��ʼ���*/        
        p_readMsg = mesgqueue_read();
        protocol_sendfram((pro_frame_t *)p_readMsg->pdata,p_readMsg->len);      
        free(p_readMsg);
        TRAN_STATE(&cmd_cb,protocl_cmdtype_idle_state);
        break;
    case WAIT_ACK:
        break;        
    case EXIT:
        break;
    default:
        break;
    }
    return fsm_rt_on_going;
}


