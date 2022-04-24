#ifndef _RTC_I2C_H_
#define _RTC_I2C_H_


#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "driver/i2c.h"
#include "rtc_i2c.h"


#define PIN_SDA 17
#define PIN_SCL 5
#define RTC_I2C_ADDRESS 0x68 

char decToBcd(char val);
char bcdToDec(char val);
void i2c_init();
void set_time(uint8_t *date);
void get_time(char *date);



#endif