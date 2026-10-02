#ifndef __PWM_H__
#define __PWM_H__

#include "sys.h"

void pwm_init(uint16_t arr, uint16_t psc);
void pwm_compare_set(uint16_t val);
void led_init(void);
void led_off(void);
void led_low(void);
void led_medium(void);
void led_high(void);
uint8_t led_leve_get(void);

#endif
