#include "timer.h"
#include "led.h"

TIM_HandleTypeDef timer_handle = {0};

uint32_t sit_time = 0;
uint8_t timer_running = 0;

//定时器初始化函数
void timer_init(uint16_t arr, uint16_t psc)
{
    timer_handle.Instance = TIM4;
    timer_handle.Init.Prescaler = psc;
    timer_handle.Init.Period = arr;
    timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&timer_handle);
//    HAL_TIM_Base_Start_IT(&timer_handle);
}


//msp函数
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM4)
    {
        __HAL_RCC_TIM4_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM4_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
    }
}


//中断服务函数
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&timer_handle);
}


//更新中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM4)
    {
        sit_time++;
//        printf("sit_time: %d\r\n", sit_time);
//        led1_toggle();
    }
}

void timer_start(void)
{
    HAL_TIM_Base_Start_IT(&timer_handle);
    timer_running = 1;
}

void timer_stop(void)
{
    HAL_TIM_Base_Stop(&timer_handle);
    timer_running = 0;
}

void timer_toggle(void)
{
    if(timer_running)
        timer_stop();
    else
        timer_start();
}

uint32_t sit_time_get(void)
{
    return sit_time;
}

void sit_time_set(uint32_t value)
{
    sit_time = value;
}
