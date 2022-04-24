

#ifndef _ENC_INIT_H_
#define _ENC_INIT_H_

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

#include "funcs.h"

static const char *TAG = "TCP/IP enc init";

void enc_init();

void eth_event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data);

void got_ip_event_handler(void *arg, esp_event_base_t event_base,
                                 int32_t event_id, void *event_data);





#endif
