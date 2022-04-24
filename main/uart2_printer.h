#ifndef _UART2_PRINTER_H_
#define _UART2_PRINTER_H_

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_eth.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "driver/gpio.h"
#include "enc28j60.h"
//#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "esp_wifi.h"


#include "rtc_i2c.h"
#include "Ready_spiffs.h"
#include "uart0_computer.h"
#include "uart1_scanner.h"
#include "uart2_printer.h"
#include "my_gpio.h"
#include "lcd.h"


#include "enc_init.h"


#define TX2  26
#define RX2  25
#define RTS  (UART_PIN_NO_CHANGE)
#define CTS  (UART_PIN_NO_CHANGE)


#define BUF_SIZE (2048)



//#define uart2_send_data(data_printer) uart_write_bytes(UART_NUM_2,(const char *) data_printer, strlen((const char *)data_printer))

void uart2_init();
void uart2_send_data(char *time, char *data, char *code);
bool uart2_recieve_data(char * my_buf);


#endif
