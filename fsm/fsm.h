#ifndef __FSM__H
#define __FSM__H
typedef enum {
    fsm_rt_err          = -1,    //!< fsm error, error code can be get from other interface
    fsm_rt_cpl          = 0,     //!< fsm complete
    fsm_rt_on_going     = 1,     //!< fsm on-going
    fsm_rt_wait_for_obj = 2,     //!< fsm wait for object
    fsm_rt_asyn         = 3,     //!< fsm asynchronose mode, you can check it later.
} fsm_rt_t;

enum {
    START=0,
    EXIT,      
    USER
};


typedef struct fsm_cb fsm_cb_t;
typedef fsm_rt_t *fsm_t(fsm_cb_t*);


/*状态机控制块*/
struct fsm_cb{
    unsigned char state;//状态机的内部状态
    unsigned char sig;
    fsm_t *fsm;//状态机指针
};








#undef this
#define this (*ptThis)
#define TRAN_FSM(_state)               \
        do { this.chState = _state; } while(0)

#endif
