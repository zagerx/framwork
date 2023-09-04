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


/*¡Á????¨²?????¨¦*/
struct fsm_cb{
    unsigned char chState;//????????
    unsigned char sig;
    fsm_t *fsm;//¡Á????¨²????
};








#undef this
#define this (*ptThis)
#define TRAN_FSM(_state)               \
        do { this.chState = _state; } while(0)

#endif
