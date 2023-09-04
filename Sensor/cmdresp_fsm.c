#include "./cmdresp_fsm.h"
#include "stdio.h"
#include "protocol.h"
#undef NULL
#define NULL 0                  
#undef this
#define this (*ptThis)
#define CMDRESP_RESET_FSM()               \
        do { this.chState = START; } while(0)

cmd_resp_t cmd_resp_task1;
				
int cmd_resp_init(cmd_resp_t *ptThis)
{
    if(ptThis == NULL) {
        return -1; 
    }
    this.sig = NO_CMD_SEND_SIG;
    this.chState = 0;
    //在这个例子中，this.pchStr 更适合在运行时刻由用户指定。
    return 0;
}
fsm_rt_t cmd_resp(cmd_resp_t *ptThis)
{
		unsigned char buf[24] = {0X55,0X44};
		pro_frame_t test_msg={
			.head = PRO_FRAME_HEAD,
			.func_c = PRO_FUNC_C_S_HEARTPACK,
			.len = sizeof(buf),
			.pdata = buf,
			.tail = PRO_FRAME_TAIL
		};
    static unsigned char timeout = 0;
    enum {
        START = 0,
				SEND_IDLE,
        SEND_CMD,
        SEND_TIMEOUT
    };
    switch (this.chState) {
        case START:
            timeout = 0;
            this.sig = NO_CMD_SEND_SIG;
            this.chState = SEND_IDLE;
        case SEND_IDLE:
            if((this.sig) != CMD_SEND_SIG) {
                break;
            }else{
                this.chState = SEND_CMD;
            }
        case SEND_CMD:
            if (protocol_sendfram(&test_msg)){
                this.chState = SEND_TIMEOUT;
                break;
            }
            CMDRESP_RESET_FSM();
            return fsm_rt_cpl;
        case SEND_TIMEOUT:
            if(timeout++ > 3)
            {
                timeout = 0;
                CMDRESP_RESET_FSM();
                return fsm_rt_cpl;               
            }
            this.chState = SEND_CMD;
    }
    return fsm_rt_on_going;
}


