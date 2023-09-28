#include "aht20_fsm.h"
#include "stdio.h"
#include "aht20.h"

#undef NULL
#define NULL 0                  
#undef this
#define this (*ptThis)

#define AHT20_RESET_FSM()               \
        do { this.chState = START; } while(0)
				
fsm_cb_t aht20_fsm_ctrlblock;
float buf[6] = {0};
enum{
	AHT20_MESG,
};
int aht20_fsm_init(fsm_cb_t *ptThis)
{
    if(ptThis == NULL) {
        return -1; 
    }
    this.sig = 0;
    this.chState = 0;
    return 0;
}
fsm_rt_t aht20_fsm_process(fsm_cb_t *ptThis)
{
	static unsigned int t0 = 0;
	unsigned char sta_byte = 0;
	static unsigned char busy_cnt = 0;
    enum {
		INIT = USER,
		MEASURE_START,
		WAIT_MEASURE,
		MEASURE_END,
		GET_END_WAIT,
    };
    switch (this.chState) {
     case START:
			aht20_init();
			if(aht20_fsm_statusbyte()&0x08)
			{
				this.chState = MEASURE_START;
				printf("aht20 init ok\r\n");
			}
		case MEASURE_START:
			aht20_start_ceasure();
			t0 = HAL_GetTick();	
			this.chState = WAIT_MEASURE;
		case WAIT_MEASURE:
			if(HAL_GetTick()-t0 > 80)
			{
				this.chState = MEASURE_END;
			}
		case MEASURE_END:
			sta_byte = fsm_cb_temp_humi(buf);
			if(sta_byte & 0x80)
			{
				/*传感器数据已更新*/
				printf("%f %f\r\n",buf[0],buf[1]);
				/*添加到mesg_list*/

				this.chState = GET_END_WAIT;			
				t0 = HAL_GetTick();			
			}else{
				if(busy_cnt++>10)
				{
					busy_cnt = 0;
					AHT20_RESET_FSM();
					printf("aht20 reset !!!!!!!!!!!!\r\n");
				}else{
					this.chState = GET_END_WAIT;
					printf("aht20 busy\r\n");					
				}
			}
		case GET_END_WAIT:
			if(HAL_GetTick()-t0>500)
			{
				t0 = 0;
				this.chState = MEASURE_START;
			}
			break;
    }
    return fsm_rt_on_going;
}







