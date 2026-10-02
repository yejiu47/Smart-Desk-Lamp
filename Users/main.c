#include "sys.h"
#include "delay.h"
#include "led.h"
#include "uart1.h"
#include "led.h"
#include "beep.h"
#include "bluetooth.h"
#include "hcsr04.h"
#include "ia_sensor.h"
#include "key.h"
#include "light_sensor.h"
#include "oled.h"
#include "timer.h"

enum lamp_mode
{
    AUTO_MODE = 0,
    MANUAL_MODE,
    REMOTE_MODE
};

int main(void)
{
    HAL_Init();                       
    stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
            
    uart1_init(115200);
    led_init();
    beep_init();
    bt_init(9600);
    hcsr04_init();
    ia_init();
    key_init();
    ls_init();
    oled_init();
    timer_init(10000 - 1, 7200 - 1);
    printf("hello world!\r\n");
    
    oled_show_init();

    uint8_t person_flag = 0, first_loop = 1, dis = 0, dis_str[3] = {0}, sit_time_str[3] = {0},
        key_num = 0, mode = AUTO_MODE, light_value = 0, led_level = 0;
    while(1)
    { 
        person_flag = ia_flag_get();
        if(person_flag == TRUE)         //有人
        {
            oled_show_chinese(10, 4, 9);
            if(first_loop)
            {
                first_loop = 0;
                timer_start();
            }
            
            dis = hcsr04_get_length();
            if(dis < 10)
                beep_on();
            else
                beep_off();
            
            if(sit_time_get() >= 5)
                beep_on();
        }
        else                            //无人
        {
            oled_show_chinese(10, 4, 10);
            timer_stop();
            sit_time_set(0);
            first_loop = 1;
            beep_off();
            dis = 0;
        }
        
        sprintf((char *)dis_str, "%3d", dis);
        oled_show_string(60, 4, (char *)dis_str, 16);
        
        sprintf((char *)sit_time_str, "%3d", sit_time_get());
        oled_show_string(60, 6, (char *)sit_time_str, 16);
        
        key_num = key_scan();
        if(key_num == 1)
        {
            if(mode++ > 1)
                mode = AUTO_MODE;
        }
        else if(key_num == 3)
        {
            timer_toggle();
        }
        else if(key_num == 4)
        {
            timer_stop();
            sit_time_set(0);
        }
        
        switch(mode)
        {
            case AUTO_MODE:
            {
                oled_show_chinese(60, 0, 3);        //智
                oled_show_chinese(75, 0, 4);        //能
                
                if(person_flag == TRUE)
                {
                    light_value = ls_get_value();
                    if(light_value < 40)
                        led_high();
                    else if(light_value >= 40 && light_value <= 70)
                        led_medium();
                    else if(light_value > 70)
                        led_low();
                }
                else
                    led_off();
                break;
            }
            
            case MANUAL_MODE:
            {
                oled_show_chinese(60, 0, 5);        //按
                oled_show_chinese(75, 0, 6);        //键
                
                if(key_num == 2)
                {
                    led_level = led_leve_get();
                    if(led_level++ > 2)
                        led_level = 0;
                    switch(led_level)
                    {
                        case 0:
                            led_off();
                            break;
                        case 1:
                            led_low();
                            break;
                        case 2:
                            led_medium();
                            break;
                        case 3:
                            led_high();
                            break;
                        default:
                            break;
                    }
                }
                break;
            }
            
            case REMOTE_MODE:
            {
                oled_show_chinese(60, 0, 7);        //远
                oled_show_chinese(75, 0, 8);        //程
                
                switch(bt_rx_get() - '0')
                {
                    case 0:
                        led_off();
                        break;
                    case 1:
                        led_low();
                        break;
                    case 2:
                        led_medium();
                        break;
                    case 3:
                        led_high();
                        break;
                    case 4:
                        timer_start();
                        break;
                    case 5:
                        timer_stop();
                        break;
                    case 6:
                        timer_stop();
                        sit_time_set(0);
                        break;
                    default:
                        break;
                }
                break;
            }
            
            default:
                break;
        }
        
        delay_ms(20);
    }
}

