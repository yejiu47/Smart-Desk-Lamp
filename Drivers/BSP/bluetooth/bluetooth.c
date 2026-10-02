#include "sys.h"
#include "bluetooth.h"
#include "string.h"
#include "stdarg.h"

UART_HandleTypeDef uart2_handle;                                            /* uart2句柄 */

uint8_t command = 0;

uint8_t uart2_rx_buf[UART2_RX_BUF_SIZE];                                    /* uart2接收缓冲区 */
uint16_t uart2_rx_len = 0;                                                  /* uart2接收字符长度 */

/**
 * @brief       串口1初始化函数
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @retval      无
 */
void bt_init(uint32_t baudrate)
{
    /*uart2 初始化设置*/
    uart2_handle.Instance = USART2;                                         /* USART1 */
    uart2_handle.Init.BaudRate = baudrate;                                  /* 波特率 */
    uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    uart2_handle.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    uart2_handle.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* 无硬件流控 */
    uart2_handle.Init.Mode = UART_MODE_TX_RX;                               /* 收发模式 */
    HAL_UART_Init(&uart2_handle);                                           /* HAL_UART_Init()会使能uart2 */
}

/**
 * @brief       uart2接收缓冲区清除
 * @param       无
 * @retval      无
 */
void uart2_rx_clear(void)
{
    memset(uart2_rx_buf, 0, sizeof(uart2_rx_buf));                          /* 清空接收缓冲区 */
    uart2_rx_len = 0;                                                       /* 接收计数器清零 */
}

/**
 * @brief       串口2中断服务函数
 * @note        在此使用接收中断及空闲中断，实现不定长数据收发
 * @param       无
 * @retval      无
 */
void USART2_IRQHandler(void)
{
    uint8_t receive_data = 0;   
    if(__HAL_UART_GET_FLAG(&uart2_handle, UART_FLAG_RXNE) != RESET){        /* 获取接收RXNE标志位是否被置位 */
        if(uart2_rx_len >= sizeof(uart2_rx_buf))                            /* 如果接收的字符数大于接收缓冲区大小， */
            uart2_rx_len = 0;                                               /* 则将接收计数器清零 */
        HAL_UART_Receive(&uart2_handle, &receive_data, 1, 1000);            /* 接收一个字符 */
        uart2_rx_buf[uart2_rx_len++] = receive_data;                        /* 将接收到的字符保存在接收缓冲区 */
    }

    if (__HAL_UART_GET_FLAG(&uart2_handle, UART_FLAG_IDLE) != RESET)        /* 获取接收空闲中断标志位是否被置位 */
    {
        printf("bt recv: %s\r\n", uart2_rx_buf);                               /* 将接收到的数据打印出来 */
        command = uart2_rx_buf[0];
        uart2_rx_clear();
        __HAL_UART_CLEAR_IDLEFLAG(&uart2_handle);                           /* 清除UART总线空闲中断 */
    }
}

//void bt_send(char *send_buf, uint8_t size)
//{
//    HAL_UART_Transmit(&uart2_handle, (uint8_t *)send_buf, size, 100);
//}

void bt_send(char * format, ...)
{
    uint8_t send_buf[128] ={0};
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)send_buf, format, arg);
    va_end(arg);
    HAL_UART_Transmit(&uart2_handle, send_buf, sizeof(send_buf), 100);
}

uint8_t bt_rx_get(void)
{
    uint8_t temp = command;
    command = '9';
    return temp;
}
