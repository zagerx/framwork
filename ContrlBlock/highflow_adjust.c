#include "highflow_adjust.h"
#include "fsm.h"
#include "stdio.h"
#undef NULL
#define NULL 0                  
#undef this
#define this (*ptThis)
#define hf_adjust_RESET_FSM()               \
        do { this.chState = START; } while(0)

fsm_cb_t hf_adjust_cb;
fsm_rt_t hf_adjust_process(fsm_cb_t *ptThis);
fsm_cb_t* hf_adjust_init(void)
{
  printf("hf_adj init\r\n");
  hf_adjust_cb.fsm = (fsm_t*)NULL;
  hf_adjust_cb.chState = START;
  hf_adjust_cb.sig = START;
  return &hf_adjust_cb;
}

#define ABS(x)         ((x)>0?(x):-(x))
//  float pid(float tar,float cur){}
//  float get_startflow(void){}
 float get_curflow(void){
  return 3.2f;
 }
 float sensor_getdata(unsigned char index)
 {
    return index+0.1f;
 }
//  int get_climbtime(void){}
//  unsigned int HAL_GetTick(void){}
//  void set_pwm(int output){}
//  int get_targeflow(void){}
fsm_rt_t hf_adjust_process(fsm_cb_t *ptThis)
{
    enum {
    CLIMB_INIT = USER,
    CLIMB_READY,
    CLIMB_RUN,
    CLIMB_ACC,
    CLIMB_FINISH
    };
    static unsigned int t0 = 0;
    static float climbTime = 0;
    static float start_flow,cur_flow,targe_flow,climb_targeflow,kb;
		static unsigned int climb_t;
		int output = 0;
    switch (this.chState)
    {
    case START:
    printf("hf_adj start\r\n");
    this.chState = CLIMB_INIT;
    case CLIMB_INIT:
			t0 = HAL_GetTick();
			// climbTime = get_climbtime();//爬坡时间			
      if(climbTime != 0)
      {
				climb_t = climbTime*60*1000;//将分钟换算成ms
        this.chState = CLIMB_READY;
      }else{//不进行爬坡
        this.chState = CLIMB_FINISH;
      }
			break;
    case CLIMB_READY:
      // start_flow = get_startflow();
      // cur_flow = get_curflow();
			// targe_flow = get_targeflow();
      if (cur_flow >= start_flow)
      {
        start_flow = cur_flow;
        {
          float diff = ABS(targe_flow-start_flow);
          kb = diff/climb_t; 
        }
        this.chState = CLIMB_RUN;
      }else{
        this.chState = CLIMB_ACC;
      }
    case CLIMB_ACC:
      // cur_flow = get_curflow();
			// targe_flow = get_targeflow();
      // output = pid(start_flow,cur_flow);
      if(cur_flow >= start_flow*0.98f)//*0.98接近就可以迁移状态
      {
        start_flow = cur_flow;
        float diff = ABS(targe_flow-start_flow);
        kb = diff/climb_t;
        this.chState = CLIMB_RUN;
      }
      break;
    case CLIMB_RUN:
      {
        unsigned int t = HAL_GetTick()-t0;//已经治疗的时长
        climb_targeflow = start_flow + kb*t;
      }
      // cur_flow = get_curflow();
			// targe_flow = get_targeflow();
      // output = pid(climb_targeflow,cur_flow);
      if (climb_targeflow >= targe_flow*0.98f)
      {
        this.chState = CLIMB_FINISH;
      }
      break;    
    case CLIMB_FINISH:
			// targe_flow = get_targeflow();
      // cur_flow = get_curflow();      
      // output = pid(targe_flow,cur_flow);
      break;               
    case EXIT:
      printf("hf_adj exit\r\n");
			output = 0;
      break;
    default:
      break;
    }
		// set_pwm(output);
    return fsm_rt_cpl;
}


