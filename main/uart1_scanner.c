


#include "uart1_scanner.h"


void uart1_init()
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TX1, RX1, RTS, CTS);

}



void uart1_send_data(uint8_t *data_printer )
{
    
    uart_write_bytes(UART_NUM_1, (const char *) data_printer, strlen((const char *)data_printer));

}

int uart1_serial_read(char * my_buf)
{
    //printf("\ninside uart1_serial_read\n");   
    vTaskDelay(1 / portTICK_PERIOD_MS);
    return uart_read_bytes(UART_NUM_1, (uint8_t * ) my_buf, 9, 100/portTICK_RATE_MS); 
    vTaskDelay(1 / portTICK_PERIOD_MS);
    //printf("the shit from uart %s\n", my_buf);
}