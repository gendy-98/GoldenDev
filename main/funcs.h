
#ifndef _FUNCS_H_
#define _FUNCS_H_

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

#define SERIAL_NUMBER_SIZE      9 
#define SERIAL_CONSTRAINS       21
#define FILE_NAME_SIZE          16
#define LOGS_DATA_SIZE          96
#define BUFFER_SIZE             32
#define IP_GW_NM                47
#define DATA_SIZE               512   //max data from uart/ethernet
#define DATA_EXTENTION          0
#define MAXIMUM_WAIT_TIME       20 //in 200ms
#define MAXIMUM_DOOR_TIME       20 //in 200ms
#define DEBOUNCE_DELAY          10    //in ms
//static const char *TAG = "TCP/IP Gate";
uint32_t count;
SemaphoreHandle_t man_task_semaphore;

void socket_transmission(const int sock);
void init_init(void);
bool socket_tx(int len, const int sock ,const char *tx_buffer);


#endif
