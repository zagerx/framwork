#include "aht20_fsm.h"
#include "stdio.h"
#include "aht20.h"
#include "systick.h"


#undef NULL
#define NULL 0                  
#undef this
#define this (*ptThis)

#define AHT20_RESET_FSM()               \
        do { this.chState = START; } while(0)
				
aht20_get_t aht20_fsm_ctrlblock;

int aht20_get_init(aht20_get_t *ptThis)
{
    if(ptThis == NULL) {
        return -1; 
    }
    this.sig = 0;
    this.chState = 0;
    return 0;
}
fsm_rt_t aht20_get_process(aht20_get_t *ptThis)
{
	float buf[6] = {0};
	static unsigned int t0 = 0;
	unsigned char sta_byte = 0;
	static unsigned char busy_cnt = 0;
    enum {
    START = 0,
		INIT,
		MEASURE_START,
		WAIT_MEASURE,
		MEASURE_END,
		GET_END_WAIT,
    };
    switch (this.chState) {
     case START:
			aht20_init();
			if(aht20_get_statusbyte()&0x08)
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
			sta_byte = aht20_get_temp_humi(buf);
			if(sta_byte & 0x80)
			{
				/*传感器数据已更新*/
				printf("%f %f\r\n",buf[0],buf[1]);
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







