#include "ia_sensor.h"
#include "sys.h"
#include "delay.h"
#include "led.h"

uint8_t ia_flag = FALSE;

void ia_init(void)
{
    GPIO_InitTypeDef gpio_initstruct;
    //打开时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();                           // 使能GPIOA时钟
    
    //调用GPIO初始化函数
    gpio_initstruct.Pin = GPIO_PIN_12;                       // 按键对应的引脚
    gpio_initstruct.Mode = GPIO_MODE_INPUT;            
    gpio_initstruct.Pull = GPIO_PULLUP;                     // 上拉
    HAL_GPIO_Init(GPIOA, &gpio_initstruct);
    
}

uint8_t ia_flag_get(void)
{
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET)
        return TRUE;
    else
        return FALSE;
}
