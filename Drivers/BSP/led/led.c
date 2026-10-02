#include "led.h"
#include "oled.h"

uint8_t led_level = 0;

TIM_HandleTypeDef pwm_handle = {0};
// init函数
void pwm_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef pwm_config = {0};
    
    pwm_handle.Instance = TIM3;
    pwm_handle.Init.Prescaler = psc;
    pwm_handle.Init.Period = arr;
    pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_PWM_Init(&pwm_handle);
    
    pwm_config.OCMode = TIM_OCMODE_PWM1;
    pwm_config.Pulse = 0;
    pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&pwm_handle, &pwm_config, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&pwm_handle, TIM_CHANNEL_3);
}


//msp函数
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3)
    {
        GPIO_InitTypeDef gpio_initstruct;
        //打开时钟
        __HAL_RCC_GPIOB_CLK_ENABLE();                           // 使能GPIOB时钟
        __HAL_RCC_TIM3_CLK_ENABLE();
        
        //调用GPIO初始化函数
        gpio_initstruct.Pin = GPIO_PIN_0;                    // 两个LED对应的引脚
        gpio_initstruct.Mode = GPIO_MODE_AF_PP;             // 推挽输出
        gpio_initstruct.Pull = GPIO_PULLUP;                     // 上拉
        gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;           // 高速
        HAL_GPIO_Init(GPIOB, &gpio_initstruct);
    }
}


//修改CCR值的函数
void pwm_compare_set(uint16_t val)
{
    __HAL_TIM_SET_COMPARE(&pwm_handle, TIM_CHANNEL_3, val);
}

void led_init(void)
{
    pwm_init(500 - 1, 72 - 1);
}

void led_off(void)
{
    pwm_compare_set(0);
    led_level = 0;
    oled_show_chinese(70, 2, 17);
}

void led_low(void)
{
    pwm_compare_set(150);
    led_level = 1;
    oled_show_chinese(70, 2, 14);
}

void led_medium(void)
{
    pwm_compare_set(300);
    led_level = 2;
    oled_show_chinese(70, 2, 15);
}

void led_high(void)
{
    pwm_compare_set(450);
    led_level = 3;
    oled_show_chinese(70, 2, 16);
}

uint8_t led_leve_get(void)
{
    return led_level;
}

