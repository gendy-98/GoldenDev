#ifndef _UART0_COMP_H_
#define _UART0_COMP_H_

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"



//void uart1_init();
void uart0_send_data(uint8_t *data_printer );
int uart0_recieve_data(char *buf);

#endif