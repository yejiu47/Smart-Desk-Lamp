#ifndef __ADC_H__
#define __ADC_H__

#include "sys.h"

void adc_init(void);
uint32_t adc_get_result(uint32_t ch);
void ls_init(void);
uint8_t ls_get_value(void);

#endif
