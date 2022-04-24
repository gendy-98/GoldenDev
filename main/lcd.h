#ifndef _LCD_H_
#define _LCD_H_


#include "my_gpio.h"


void LCD_Clear();
void LCD_String_xy (char row, char pos, char *str);
void LCD_String (char *str)	;
void LCD_Init (void);
void LCD_Char( unsigned char data );
void LCD_Command( unsigned char cmd );


#endif
