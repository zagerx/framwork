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
    };
    static unsigned int t0 = 0;
    switch (this.chState)
    {
    case START:
    case FIRST:
        if(GET_IPC_EVENT(PROTOCOL_CMD_01))
        {
            CLEAR_IPC_EVENT(PROTOCOL_CMD_01);
            TRAN_STATE(&cmd_cb,protocl_cmdtype_idle_state);
        }
        /*��ȡҪ���͵�����֡*/
        printf("cmd_type01 runing\r\n");
        break;
    case EXIT:
        break;
    default:
        break;
    }
    return fsm_rt_on_going;
}


