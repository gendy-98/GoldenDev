

#include "uart2_printer.h"

void uart2_init()
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, TX2, RX2, RTS, CTS);
}

void uart2_send_data(char *time, char *data, char *barcode)
{

    char time_buffer[16];
    char line[64];
    memset(line,0,64);
    char command[8];
    time_buffer[8] = 0;
    char date_buffer[16];
    date_buffer[10] = 0;
    sprintf(time_buffer, "%02d:%02d:%02d", time[2], time[1], time[0]);
    sprintf(date_buffer, "%02d/%02d/20%02d", time[4], time[5], time[6]);
    int i = 0;
    int j = 0;
    int k = 0;
    //printf("\nin uart printer\n");
    data[strlen(data)] = 0;
    if (strlen(data) != 0)
    {
        //printf("\n data  is %s  strlen = %d\n", data,strlen(data));

        while (data[i] != 0)
        {
            
            j = 0;
            memset(line,0,64);
            while (data[i] != '\n' && data[i] != '\0' && j < 64)
            {
                line[j] = data[i];
                j++;
                i++;
            }

            //printf("\nline from data %s\n",line);
            if (line[0] == '<')
            {
                if (line[1] == 'D' && line[2] == 'A' && line[3] == 'T' && line[4] == 'E' && line[5] == '>')
                {
                    //printf("\ncommand DATE %s\n",date_buffer);
                    uart_write_bytes(UART_NUM_2, (const char *)date_buffer, strlen((const char *)date_buffer) + 1);
                    memset(line,0,64);
                }
                else if (line[1] == 'T' && line[2] == 'I' && line[3] == 'M' && line[4] == 'E' && line[5] == '>')
                {
                    //printf("\ncommand TIME %s\n",time_buffer);
                    uart_write_bytes(UART_NUM_2, (const char *)time_buffer, strlen((const char *)time_buffer) + 1);
                    memset(line,0,64);
                }
                else if (line[1] == 'C' && line[2] == 'O' && line[3] == 'D' && line[4] == 'E' && line[5] == '>')
                {
                    //printf("\ncommand CODE %s\n",barcode);

                    uart_write_bytes(UART_NUM_2, (const char *)barcode, strlen((const char *)barcode) + 1);
                    memset(line,0,64);
                }
                else
                {
                    k = 1;
                    j = 0;
                    while (line[k] != '>')
                    {
                        if (line[k] <= '9' && line[k] >= '0')
                        {
                            command[j] = (line[k] - '0') * 16;
                        }
                        else if (line[k] <= 'F' && line[k] >= 'A')
                        {
                            command[j] = (line[k] - 'A' + 10) * 16;
                        }
                        if (line[k + 1] <= '9' && line[k + 1] >= '0')
                        {
                            command[j] += line[k + 1] - '0';
                        }
                        else if (line[k + 1] <= 'F' && line[k + 1] >= 'A')
                        {
                            command[j] += line[k + 1] - 'A' + 10;
                        }
                        if (line[k + 2] == ',')
                        {
                            k += 3;
                            j++;
                        }
                        else
                        {
                            k += 3;
                            j++;
                            break;
                        }
                    }
                    //printf("\ncommand COMMAND %s\n",command);
                    uart_write_bytes(UART_NUM_2, (const char *)command, j + 1);
                    memset(line,0,64);
                    j = 0;
                }
            }
            else
            {
                line[j] = 10;
                line[j + 1] = 0;
                //printf("\ncommand NOT COMMAND %s\n",line);
                uart_write_bytes(UART_NUM_2, (const char *)line, strlen((const char *)line) + 1);
            }
            i++;
        }
    }

    /*    
    char data6[256];
    char cut_command[] = {29, 86, 66, 20, 0};
    char lf_2_command[] = {10, 10, 0};
    //char lf_1_command[] = {10, 0};
    //char bold_command[] = {27, 33, 100, 0};
    //char normal_command[] = {27, 33, 1, 0};
    char middle_command[] = {27, 97, 49, 0};
    char left_command[] = {27, 97, 48, 0};
    //char print_empthasize[] = {27, 33, 8, 0};
    char barcode_command[] = {29, 104, 180, 0x1d, 107, 65, 11, 0};


*/
    /*
    char data[256];
    char data4[256];
    char data3[256];
    char data2[256];
    
    //memset(data4, 0, 256);
    read_file("D4", data4, 256);
    //memset(data3, 0, 256);
    read_file("D3", data3, 256);
    //memset(data, 0, 256);
    read_file("D2", data2, 256);
    //memset(data, 0, 256);
    read_file("D1", data, 256);
*/
    /*
    //uart_write_bytes(UART_NUM_2, (const char *)left_command, strlen((const char *)left_command) + 1);
	uart_write_bytes(UART_NUM_2, (const char *)lf_2_command, strlen((const char *)lf_2_command) + 1);
	vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)lf_2_command, strlen((const char *)lf_2_command) + 1);
	
	vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)data, strlen((const char *)data) + 1);
	
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)lf_2_command, strlen((const char *)lf_2_command) + 1);
	
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)time_buffer, strlen((const char *)time_buffer) + 1);
	
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)lf_2_command, strlen((const char *)lf_2_command) + 1);
	
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)data2, strlen((const char *)data2) + 1);
	
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)lf_2_command, strlen((const char *)lf_2_command) + 1);
	
    vTaskDelay(1 / portTICK_PERIOD_MS);    
    uart_write_bytes(UART_NUM_2, (const char *)date_buffer, strlen((const char *)date_buffer) + 1);
	   
	vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)lf_2_command, strlen((const char *)lf_2_command) + 1);
	
    vTaskDelay(1 / portTICK_PERIOD_MS);    
    uart_write_bytes(UART_NUM_2, (const char *)data3, strlen((const char *)data3) + 1);
	//printf("data 3 :%s\n",data3);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)lf_2_command, strlen((const char *)lf_2_command) + 1);
    
    memset(data6, 0, 256);
    sprintf(data6, "%s%s%s", middle_command,
            barcode_command,
            barcode);
    
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)data6, strlen((const char *)data6) + 1);
	
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)lf_2_command, strlen((const char *)lf_2_command) + 1);
	
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)left_command, strlen((const char *)left_command) + 1);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)data4, strlen((const char *)data4) + 1);
	
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)lf_2_command, strlen((const char *)lf_2_command) + 1);
    uart_write_bytes(UART_NUM_2, (const char *)lf_2_command, strlen((const char *)lf_2_command) + 1);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_2, (const char *)cut_command, strlen((const char *)cut_command) + 1);
*/
}

bool uart2_recieve_data(char *my_buf)
{

    uart_read_bytes(UART_NUM_2, (uint8_t *)my_buf, 64, 100 / portTICK_PERIOD_MS);
    return true;
}
