#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "stdio.h"
#include "sys.h"

/* UART收发缓冲大小 */
#define UART2_RX_BUF_SIZE            128
#define UART2_TX_BUF_SIZE            64


void bt_init(uint32_t bound);            /* 串口初始化函数 */
//void bt_send(char *send_buf, uint8_t size);
void bt_send(char * format, ...);
uint8_t bt_rx_get(void);

#endif


