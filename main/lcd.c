

#include "lcd.h"

void LCD_Command( unsigned char cmd )
{

    unsigned char temp;
    gpio_set_level(GPIO_LCD_RS_OUT,0);
    gpio_set_level(GPIO_LCD_E_OUT,1);
    temp = cmd & 0x10; 
    temp = temp >> 4;
    gpio_set_level(GPIO_LCD_D4_OUT,temp);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    temp = cmd & 0x20; 
    temp = temp >> 5;
    gpio_set_level(GPIO_LCD_D5_OUT,temp);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    temp = cmd & 0x40; 
    temp = temp >> 6;
    gpio_set_level(GPIO_LCD_D6_OUT,temp);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    temp = cmd & 0x80; 
    temp = temp >> 7;
    gpio_set_level(GPIO_LCD_D7_OUT,temp);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_E_OUT,0);
    vTaskDelay(2 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_E_OUT,1);
    vTaskDelay(2 / portTICK_PERIOD_MS);
    temp = cmd & 0x01; 
    temp = temp >> 0;
    gpio_set_level(GPIO_LCD_D4_OUT,temp);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    temp = cmd & 0x02; 
    temp = temp >> 1;
    gpio_set_level(GPIO_LCD_D5_OUT,temp);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    temp = cmd & 0x04; 
    temp = temp >> 2;
    gpio_set_level(GPIO_LCD_D6_OUT,temp);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    temp = cmd & 0x08; 
    temp = temp >> 3;
    gpio_set_level(GPIO_LCD_D7_OUT,temp);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_E_OUT,0);
    vTaskDelay(2 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_E_OUT,1);
    

}

void LCD_Char( unsigned char data )
{
    unsigned char temp;
    gpio_set_level(GPIO_LCD_RS_OUT,1);
    
    gpio_set_level(GPIO_LCD_E_OUT,1);
    
    temp = data & 0x10; 
    temp = temp >> 4;
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_D4_OUT,temp);
    
    temp = data & 0x20; 
    temp = temp >> 5;
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_D5_OUT,temp);
    
    temp = data & 0x40; 
    temp = temp >> 6;
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_D6_OUT,temp);
    
    temp = data & 0x80; 
    temp = temp >> 7;
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_D7_OUT,temp);
    
    gpio_set_level(GPIO_LCD_E_OUT,0);
    
    vTaskDelay(2 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_E_OUT,1);
    

    vTaskDelay(2 / portTICK_PERIOD_MS);
    temp = data & 0x01; 
    temp = temp >> 0;
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_D4_OUT,temp);
    
    temp = data & 0x02; 
    temp = temp >> 1;
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_D5_OUT,temp);
    
    temp = data & 0x04; 
    temp = temp >> 2;
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_D6_OUT,temp);
    
    temp = data & 0x08; 
    temp = temp >> 3;
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_LCD_D7_OUT,temp);
    
    gpio_set_level(GPIO_LCD_E_OUT,0);
    vTaskDelay(2 / portTICK_PERIOD_MS);
    
    gpio_set_level(GPIO_LCD_E_OUT,1);

}
void LCD_Init (void)					
{
    vTaskDelay(15 / portTICK_PERIOD_MS);
    LCD_Command(0x33);
    
	vTaskDelay(3 / portTICK_PERIOD_MS);
    LCD_Command(0x32);		    		/* send for 4 bit initialization of LCD  */
	
    vTaskDelay(3 / portTICK_PERIOD_MS);
    LCD_Command(0x28);              	/* Use 2 line and initialize 5*7 matrix in (4-bit mode)*/
	
    vTaskDelay(3 / portTICK_PERIOD_MS);
    LCD_Command(0x0E);              	/* Display on cursor off*/
	vTaskDelay(3 / portTICK_PERIOD_MS);
    
    LCD_Command(0x06);              	/* Increment cursor (shift cursor to right)*/
	LCD_Command(0x01);              	/* Clear display screen*/
	vTaskDelay(3 / portTICK_PERIOD_MS);
	LCD_Command (0x80);	
}
void LCD_String (char *str)				/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)				/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);		/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);		/* Command of first row and required position<16 */
	LCD_String(str);					/* Call LCD string function */
}

void LCD_Clear()
{
	LCD_Command (0x01);					/* Clear display */
	vTaskDelay(100 / portTICK_PERIOD_MS);
	LCD_Command (0x80);					/* Cursor 1st row 0th position */
}