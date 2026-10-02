#ifndef __TIMER_H__
#define __TIMER_H__

#include "sys.h"

void timer_init(uint16_t arr, uint16_t psc);
void timer_start(void);
void timer_stop(void);
void timer_toggle(void);
uint32_t sit_time_get(void);
void sit_time_set(uint32_t value);

#endif
