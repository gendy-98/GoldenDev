
#include "uart0_computer.h"

void uart0_init()
{
    
    // uart_config_t uart_config = {
    //     .baud_rate = 115200,
    //     .data_bits = UART_DATA_8_BITS,
    //     .parity    = UART_PARITY_DISABLE,
    //     .stop_bits = UART_STOP_BITS_1,
    //     .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    // };
    // uart_param_config(UART_NUM_0, &uart_config);
    // uart_set_pin(UART_NUM_1, TX1, RX1, RTS1, CTS1);
    // uart_driver_install(UART_NUM_1, BUF1_SIZE * 2, 0, 0, NULL, 0);
    
}


void uart0_send_data(uint8_t *data_printer )
{
    
    printf("%s\n",data_printer);

}

int uart0_recieve_data(char buf[])
{   
    uint32_t i=0;
    char c;
    do
    {
        c=fgetc(stdin);
        if (c!=0xff)
        {
           buf[i]=c;
           i++;
        }
    vTaskDelay(2);
        
    } while (c!=0xff);
    
    return i;
    

    
}

