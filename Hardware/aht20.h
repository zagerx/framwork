#ifndef _AHT10_H__
#define _AHT10_H__
/*****************函数声明，对外接口******************/
	#if 0
		#include "stdint.h"
		void AHT10Init(void);
		uint8_t AHT10Check(void);
		void AHT10Reset(void);
		uint8_t AHT10ReadData(uint16_t *temperature,uint16_t *humidity);
	#else
		void aht20_init(void);
		unsigned char aht20_get_temp_humi(float *data);
		unsigned char aht20_get_statusbyte(void);
		void aht20_start_ceasure(void);
		void aht20_test(void);
	#endif
#endif
