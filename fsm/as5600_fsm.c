#include "as5600_fsm.h"
#include "as5600.h"
#include "stdio.h"
#include "fsm.h"

#undef NULL
#define NULL 0                  
#undef this
#define this (*ptThis)

#define as5600_RESET_FSM()               \
        do { this.chState = START; } while(0)
				
fsm_cb_t as5600_fsm_ctrlblock;

int as5600_fsm_init(fsm_cb_t *ptThis)
{
    if(ptThis == NULL) {
        return -1; 
    }
    this.sig = 0;
    this.chState = START;
    return 0;
}
static float a_angle;
fsm_rt_t as5600_fsm_process(fsm_cb_t *ptThis)
{
    enum {
    START ,
		MEASURE_START = USER,
    };
    if(ptThis == NULL) {
        return fsm_rt_err; 
    }    
    switch (this.chState) {
      case START:
        as5600_init();
        this.chState = MEASURE_START;
        break;
      case MEASURE_START:
        a_angle = as5600_getangle();
        printf("a_angle %f\r\n",a_angle);
    }
    return fsm_rt_on_going;
}







