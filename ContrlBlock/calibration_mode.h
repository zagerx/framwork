#ifndef __CALIBRATION_MODE__H
#define __CALIBRATION_MODE__H

#include "fsm.h"

#define CALIB_MODE_RESET_FSM()               \
        do { this.chState = START; } while(0)

//fsm_rt_t calib_mode_process(fsm_cb_t *ptThis);
void _calib_mode_init(void);
void _calib_mode_proess(void);        
void _calibmode_tran_stopstate(void);    
#endif


