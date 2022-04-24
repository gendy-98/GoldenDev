#ifndef _MY_GPIO_H_
#define _MY_GPIO_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"


#define GPIO_RELAY_OUT 2

#define GPIO_DRY_CONTACT_IN 35  
#define GPIO_PUSH_BUTTON_IN 34  


#define GPIO_LCD_D4_OUT 15   
#define GPIO_LCD_D5_OUT 13  
#define GPIO_LCD_D6_OUT 33
#define GPIO_LCD_D7_OUT 32      
#define GPIO_LCD_RS_OUT 16
#define GPIO_LCD_E_OUT 4

#define GPIO_OUTPUT_PIN_SEL    ((1ULL<<GPIO_RELAY_OUT) | (1ULL<<GPIO_LCD_D4_OUT) | (1ULL<<GPIO_LCD_D5_OUT) | (1ULL<<GPIO_LCD_D6_OUT) | (1ULL<<GPIO_LCD_D7_OUT) | (1ULL<<GPIO_LCD_RS_OUT) | (1ULL<<GPIO_LCD_E_OUT))
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_PUSH_BUTTON_IN) | (1ULL<<GPIO_DRY_CONTACT_IN))

void GPIO_init();

#endif