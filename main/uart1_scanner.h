#ifndef _UART1_SCANNER_H_
#define _UART1_SCANNER_H_
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
//#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "rtc_i2c.h"
#include "Ready_spiffs.h"
#include "uart0_computer.h"
#include "uart1_scanner.h"
#include "uart2_printer.h"
#include "my_gpio.h"
#include "lcd.h"

#include "enc_init.h"

#include "funcs.h"

#define TX1  27
#define RX1  14
#define RTS  (UART_PIN_NO_CHANGE)
#define CTS  (UART_PIN_NO_CHANGE)


#define BUF_SIZE (2048)



void uart1_init();
void uart1_send_data(uint8_t *data_printer );
int uart1_serial_read(char * my_buf);


#endif