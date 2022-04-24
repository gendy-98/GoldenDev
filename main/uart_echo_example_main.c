
/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "rtc_i2c.h"
#include "Ready_spiffs.h"
#include "uart0_computer.h"
#include "uart1_scanner.h"
#include "uart2_printer.h"
#include "my_gpio.h"
#include "lcd.h"

#include "enc_init.h"
#include "funcs.h"


#include "bt_init.h"

/*
#include "esp_heap_trace.h"
#include "esp_heap_caps.h"

#define NUM_RECORDS 100
static heap_trace_record_t trace_record[NUM_RECORDS]; // This buffer must be in internal RAM
size_t heapUsedAtStart;

int ii = 0;
*/
#define PORT 99
int sock;
bool flag_sock = true;
//TaskHandle_t xEthHandle = NULL;
//TaskHandle_t xTaskHandle = NULL;
#define TCP_TASK_DEBUGsd
static void tcp_server_task(void *pvParameters)
{
    
    char addr_str[128];
    
    int ip_protocol = 0;
    struct sockaddr_in6 dest_addr;
    #ifdef CONFIG_EXAMPLE_IPV4
        int addr_family = AF_INET;
        //printf("########################version 4 ######################\n");
    #else
        //printf("########################version 6 ######################3");
        int addr_family = AF_INET6;
    #endif

    char buffer[6];
    memset(buffer, 0, 6);
    if (read_file("C", buffer, 5))
    {
        count = strtol(buffer, NULL, 10);
    }
    else
    {
        count = 0;
        sprintf(buffer, "%05d", count);
        write_file("C", buffer);
    }
    //caloc(buffer);


    if (addr_family == AF_INET) {
        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
        dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family = AF_INET;
        dest_addr_ip4->sin_port = htons(PORT);
        ip_protocol = IPPROTO_IP;
    } else if (addr_family == AF_INET6) {
        bzero(&dest_addr.sin6_addr.un, sizeof(dest_addr.sin6_addr.un));
        dest_addr.sin6_family = AF_INET6;
        dest_addr.sin6_port = htons(PORT);
        ip_protocol = IPPROTO_IPV6;
    }


    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);

    if (listen_sock < 0)
    {
        //ESP_LOGE(TAG, "UM77 Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    #if defined(CONFIG_EXAMPLE_IPV4) && defined(CONFIG_EXAMPLE_IPV6)
    // Note that by default IPV6 binds to both protocols, it is must be disabled
    // if both protocols used at the same time (used in CI)
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(listen_sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
    #endif
    //ESP_LOGI(TAG, "UM81 Socket created");

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0)
    {
        //ESP_LOGE(TAG, "UM97 Socket unable to bind: errno %d", errno);
        //ESP_LOGE(TAG, "UM98 IPPROTO: %d", addr_family);
        goto CLEAN_UP;
    }
    //ESP_LOGI(TAG, "UM89 Socket bound, port %d", PORT);

    err = listen(listen_sock, 1);
    if (err != 0)
    {
        //ESP_LOGE(TAG, "UM94 Error occurred during listen: errno %d", errno);
        goto CLEAN_UP;
    }
    /*
    //printf("\nbefore the delay 117\n");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    //printf("\nwe will dump 119\n");
    ESP_ERROR_CHECK( heap_trace_stop() );
    heap_trace_dump();
    //printf("\ndumped 119\n");
    */
    while (1)
    {
        

        
        struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
        uint addr_len = sizeof(source_addr);
        sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0)
        {
            //ESP_LOGE(TAG, "UM108 Unable to accept connection: errno %d", errno);
            break;
        }
        /*
        #ifdef TCP_TASK_DEBUG
        ESP_ERROR_CHECK(heap_trace_start(HEAP_TRACE_LEAKS));
        #endif
        */
        // Convert ip address to string
        if (source_addr.sin6_family == PF_INET)
        {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
        }
        else if (source_addr.sin6_family == PF_INET6)
        {
            inet6_ntoa_r(source_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
        }
        //ESP_LOGI(TAG, "UM122 Socket accepted ip address: %s", addr_str);
/*
        #ifdef TCP_TASK_DEBUG
        //printf("\nbefore the delay 117\n");
        //vTaskDelay(1000 / portTICK_PERIOD_MS);
        //printf("\nwe will dump 119\n");
        ESP_ERROR_CHECK(heap_trace_stop());
        heap_trace_dump();
        //printf("\ndumped 119\n");
        //vTaskDelay(1000 / portTICK_PERIOD_MS);
        //printf("\nstart again 119\n");
        #endif
  */      //socket accepted
        socket_transmission(sock);

        shutdown(sock, 0);
        close(sock);
        
        //gpio_set_level(GPIO_YELLOW_OUT, 0);
    }
CLEAN_UP:
    //printf("M147 clean up\n");
    close(listen_sock);
    vTaskDelete(NULL);
}

void task2()
{

    vTaskDelay(500 / portTICK_PERIOD_MS);
    init_init(); //init all the HAL level
    LCD_Clear();
    LCD_String("Initializing...");
    char data[DATA_SIZE]; //data from tcp
    char time[7];
    char serial[SERIAL_NUMBER_SIZE + SERIAL_CONSTRAINS];
    char serial_number_from_scanner[SERIAL_NUMBER_SIZE];
    char file_name_buffer[FILE_NAME_SIZE];
    char logs_data[LOGS_DATA_SIZE];
    char just_serial[SERIAL_NUMBER_SIZE];
    char barcode[13];
    //varialbe for each message
    char data1[1024];
    char mw[LOGS_DATA_SIZE];
    char m2[LOGS_DATA_SIZE];
    char m3[LOGS_DATA_SIZE];
    char m4[LOGS_DATA_SIZE];
    memset(data1,0,1024);
    read_file("D1", data1, 1024);
    char m5[LOGS_DATA_SIZE];
    char m6[LOGS_DATA_SIZE];
    char m7[LOGS_DATA_SIZE];
    char switch_state[8];
    char panel_state[8];
  

 

    uint32_t iterator;
    uint32_t card_counter;

    bool flag_card = false;
    bool flag_time = false;
    bool flag_counter = false;
    bool flag_ticket = false;
    
    bool flag_serial = false;

    bool flag_switch = true;
    bool flag_pause = false;
    int cmp;

    memset(logs_data, 0, LOGS_DATA_SIZE);
    memset(file_name_buffer, 0, FILE_NAME_SIZE);
    memset(data, 0, DATA_SIZE);
    memset(serial, 0, SERIAL_NUMBER_SIZE + SERIAL_CONSTRAINS);
    memset(serial_number_from_scanner, 0, SERIAL_NUMBER_SIZE);
    memset(just_serial, 0, SERIAL_NUMBER_SIZE);
    memset(mw, 0, LOGS_DATA_SIZE);
    memset(m2, 0, LOGS_DATA_SIZE);
    memset(m3, 0, LOGS_DATA_SIZE);
    memset(m4, 0, LOGS_DATA_SIZE);
    memset(m5, 0, LOGS_DATA_SIZE);
    memset(m6, 0, LOGS_DATA_SIZE);
    memset(m7, 0, LOGS_DATA_SIZE);
    memset(switch_state, 0, 8);
    memset(panel_state, 0, 8);

    if (!read_file("Z", panel_state, 8))
        flag_pause = false;

    else if (!strcmp(panel_state, "ON"))
        flag_pause = false;

    else if (!strcmp(panel_state, "OFF"))
        flag_pause = true;

    ////printf("panel state value : (%s)\nflage pause vlaue : (%d)\n", panel_state, flag_pause);
    if (!read_file("X", switch_state, 8))
        flag_switch = true;

    else if (!strcmp(switch_state, "ON"))
        flag_switch = true;

    else if (!strcmp(switch_state, "OFF"))
        flag_switch = false;

    ////printf("switch state value : (%s)\nflage switch vlaue : (%d)\n", switch_state, flag_switch);
    ////printf("\nwelcome 231\n");
    if (!read_file("Mw", mw, LOGS_DATA_SIZE))
        sprintf(mw, "Welcome                                 DEFAULT");
    if (!read_file("M2", m2, LOGS_DATA_SIZE))
        sprintf(m2, "Printing                                DEFAULT");
    if (!read_file("M3", m3, LOGS_DATA_SIZE))
        sprintf(m3, "Printed                                 DEFAULT");
    if (!read_file("M4", m4, LOGS_DATA_SIZE))
        sprintf(m4, "Right ID                                DEFAULT");
    if (!read_file("M5", m5, LOGS_DATA_SIZE))
        sprintf(m5, "Wrong ID                                DEFAULT");
    if (!read_file("M6", m6, LOGS_DATA_SIZE))
        sprintf(m6, "Expired ID                              DEFAULT");
    ////printf("\nwelcome 244\n");    
    LCD_Clear();
    LCD_String("Done");
    if (!read_file("M7", m7, LOGS_DATA_SIZE))
        sprintf(m7, "Used ID                                 DEFAULT");
    get_time(time);    

    ////printf("\nwelcome 252\n");
    while (1)
    {
        /*
        ii++;
        heapUsedAtStart = heap_caps_get_free_size(MALLOC_CAP_8BIT);
        //printf("Heap free at loop %d is %d.\n", ii, heapUsedAtStart);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        */

        flag_ticket = true;


        if (!flag_pause)
        {
            //////printf("flage switch vlaue : (%d)", flag_switch);
            if (!gpio_get_level(GPIO_DRY_CONTACT_IN) || !flag_switch)
            {
                vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS);
                if (!gpio_get_level(GPIO_DRY_CONTACT_IN) || !flag_switch)
                {
                    vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS);
                    //gpio_set_level(GPIO_GREEN_OUT, 1);
                    if(flag_switch){
                        //printf("\nwelcome\n");
                        //printf("\nkokoko318\n");
                        LCD_Clear();
                        LCD_String(mw);
                    }
                    

                    while (!gpio_get_level(GPIO_DRY_CONTACT_IN) || !flag_switch)
                    {
                        vTaskDelay(1 / portTICK_PERIOD_MS);
                        if (gpio_get_level(GPIO_PUSH_BUTTON_IN))
                        {
                            vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS);
                            if (gpio_get_level(GPIO_PUSH_BUTTON_IN) && flag_ticket)
                            {
                                flag_ticket = false;
                                ////printf("\nprinting ticket\n");
                                LCD_Clear();

                                LCD_String(m2);
                                   /*
                                if (sock > 0)
                                {
                                    m2[strlen(m2)] = '\n';
                                    socket_tx(strlen(m2), sock, m2);
                                    m2[strlen(m2) - 1] = 0;
                                }
                                    */
                                vTaskDelay(10 / portTICK_PERIOD_MS);
                                get_time(time);
                                //vTaskSuspend(xEthHandle);
                                vTaskDelay(1 / portTICK_PERIOD_MS);
                                barcode[0] = time[0] % 10 + 48;
                                barcode[1] = time[0] / 10 + 48;
                                barcode[2] = time[1] % 10 + 48;
                                barcode[3] = time[1] / 10 + 48;
                                barcode[4] = time[2] % 10 + 48;
                                barcode[5] = time[2] / 10 + 48;
                                barcode[6] = time[4] % 10 + 48;
                                barcode[7] = time[4] / 10 + 48;
                                barcode[8] = time[5] % 10 + 48;
                                barcode[9] = time[5] / 10 + 48;
                                barcode[10] = time[6] % 10 + 48;
                                barcode[11] = time[6] / 10 + 48;

                                barcode[12] = 0;
                                uart2_send_data(time, data1, barcode);
                                //////////////////////////////////////////////////////////////////
                                sprintf(file_name_buffer, "L");

                                sprintf(logs_data, "%s,C,%02d/%02d,%02d:%02d\n", barcode, time[4], time[5], time[2], time[1]);
                                append_file(file_name_buffer, logs_data);
                                write_file("N", logs_data);
                                    /*
                                if (sock > 0)
                                    socket_tx(strlen(logs_data), sock, logs_data);
                                    */
                                memset(logs_data, 0, LOGS_DATA_SIZE);
                                //////////////////////////////////////
                                vTaskDelay(1 / portTICK_PERIOD_MS);
                                //vTaskResume(xEthHandle);
                               
                                iterator = 0;
                                vTaskDelay(100 / portTICK_PERIOD_MS);
                                ////printf("\nplease take the ticket....\n");
                                LCD_Clear();

                                LCD_String(m3);
                                    /*
                                if (sock > 0)
                                {
                                    m3[strlen(m3)] = '\n';
                                    socket_tx(strlen(m3), sock, m3);
                                    m3[strlen(m3) - 1] = 0;
                                }
                                        */
                                //gpio_set_level(GPIO_GREEN_OUT, 0);
                                while (!(/*Ticket removed -- ALWAYS NOT REMOVED ==0*/ 0) && iterator < MAXIMUM_WAIT_TIME)
                                {
                                    vTaskDelay(200 / portTICK_PERIOD_MS);
                                    iterator++;
                                }
                                if (iterator < MAXIMUM_WAIT_TIME)
                                {
                                    iterator = 0;
                                    gpio_set_level(GPIO_RELAY_OUT, 1);
                                    while ((!gpio_get_level(GPIO_DRY_CONTACT_IN) || !flag_switch) && iterator < MAXIMUM_DOOR_TIME)
                                    {
                                        vTaskDelay(200 / portTICK_PERIOD_MS);
                                        iterator++;
                                    }
                                    LCD_Clear();
                                    gpio_set_level(GPIO_RELAY_OUT, 0);
                                    iterator = 0;
                                }
                                else
                                {
                                    //ticket is not removed
                                    iterator = 0;
                                    gpio_set_level(GPIO_RELAY_OUT, 1);
                                    while ((!gpio_get_level(GPIO_DRY_CONTACT_IN) || !flag_switch) && iterator < MAXIMUM_DOOR_TIME)
                                    {
                                        vTaskDelay(200 / portTICK_PERIOD_MS);
                                        iterator++;
                                    }
                                    gpio_set_level(GPIO_RELAY_OUT, 0);
                                    iterator = 0;
                                    LCD_Clear();
                                }
                            }
                        }
                        else if ((uart1_serial_read(serial_number_from_scanner) > 0) && flag_ticket) //serial number reading
                        {
                            LCD_Clear();
                            LCD_String("Reading...");
                            uart_flush_input(UART_NUM_1);
                            iterator = 0;
                            flag_serial = true;
                            while (serial_number_from_scanner[iterator])
                            {
                                if ((serial_number_from_scanner[iterator] >= '0' && serial_number_from_scanner[iterator] <= '9') || (serial_number_from_scanner[iterator] >= 'A' && serial_number_from_scanner[iterator] <= 'Z') || (serial_number_from_scanner[iterator] >= 'a' && serial_number_from_scanner[iterator] <= 'z'))
                                {
                                    ////printf("char ok\n");
                                }
                                else
                                {
                                    flag_serial = false;
                                    ////printf("char not ok\n");
                                    break;
                                }
                                iterator++;
                            }
                            iterator = 0;
                            ////printf("pure Serial %s\n",serial_number_from_scanner);
                            if (flag_serial)
                            {
                                ////printf("flag serial true\n");
                                while (strlen(serial_number_from_scanner) < SERIAL_NUMBER_SIZE - 1)
                                {
                                    serial_number_from_scanner[strlen(serial_number_from_scanner)] = '0';
                                }
                                sprintf(logs_data, "Serial => %s\n", serial_number_from_scanner);
                                    /*
                                if (sock > 0)
                                    socket_tx(strlen(logs_data), sock, logs_data);
                                    */
                                memset(logs_data, 0, LOGS_DATA_SIZE);
                                //779B5600 to reset IP
                                sprintf(just_serial, "779B5600");
                                cmp = strcmp(just_serial, serial_number_from_scanner);
                                if (cmp == 0)
                                {
                                    remove("/spiffs/IP");
                                    remove("/spiffs/Mw");
                                }
                                else
                                {

                                    if (read_file(serial_number_from_scanner, serial, SERIAL_NUMBER_SIZE + SERIAL_CONSTRAINS - 1))
                                    {
                                        flag_card = true;
                                        get_time(time);
                                        if (time[6] == strtol(serial + 15, NULL, 10))
                                        {
                                            if (time[5] == strtol(serial + 12, NULL, 10))
                                            {
                                                if (time[4] == strtol(serial + 9, NULL, 10))
                                                {
                                                    if (time[2] == strtol(serial + 18, NULL, 10))
                                                    {
                                                        if (time[1] <= strtol(serial + 21, NULL, 10))
                                                        {
                                                            flag_time = true;
                                                        }
                                                    }
                                                    else if (time[2] < strtol(serial + 18, NULL, 10))
                                                    {
                                                        flag_time = true;
                                                    }
                                                }
                                                else if (time[4] < strtol(serial + 9, NULL, 10))
                                                {
                                                    flag_time = true;
                                                }
                                            }
                                            else if (time[5] < strtol(serial + 12, NULL, 10))
                                            {
                                                flag_time = true;
                                            }
                                        }
                                        else if (time[6] < strtol(serial + 15, NULL, 10))
                                        {
                                            flag_time = true;
                                        }
                                        if (flag_time)
                                        {
                                            card_counter = strtol(serial + 24, NULL, 10);
                                            if (card_counter == 0)
                                            {
                                                flag_counter = true;
                                            }
                                            if (card_counter == 1)
                                            {
                                                flag_counter = false;
                                            }
                                            if (card_counter > 1)
                                            {
                                                card_counter--;
                                                serial[24] = 0;
                                                serial[25] = 0;
                                                memset(data, 0, DATA_SIZE);
                                                sprintf(data, "%s%05d", serial, card_counter);
                                                write_file(serial_number_from_scanner, data);
                                                flag_counter = true;
                                            }
                                        }
                                        if (flag_card && flag_time && flag_counter)
                                        {
                                            flag_ticket = false;
                                            ////printf("\nplease enter\n");
                                            LCD_Clear();
                                            LCD_String(m4);
                                                /*
                                            if (sock > 0)
                                            {
                                                m4[strlen(m4)] = '\n';
                                                flag_sock = socket_tx(strlen(m4), sock, m4);
                                                m4[strlen(m4) - 1] = 0;
                                            }
                                                */
                                            gpio_set_level(GPIO_RELAY_OUT, 1);
                                            get_time(time);
                                            sprintf(file_name_buffer, "L");
                                            sprintf(logs_data, "%s,T,%02d/%02d,%02d:%02d\n", serial_number_from_scanner, time[4], time[5], time[2], time[1]);
                                            append_file(file_name_buffer, logs_data);
                                            write_file("N", logs_data);
                                                /*
                                            if (sock > 0)
                                                socket_tx(strlen(logs_data), sock, logs_data);
                                                    */
                                            memset(logs_data, 0, LOGS_DATA_SIZE);
                                            iterator = 0;
                                            while ((!gpio_get_level(GPIO_DRY_CONTACT_IN) || !flag_switch) && iterator < MAXIMUM_DOOR_TIME)
                                            {
                                                vTaskDelay(200 / portTICK_PERIOD_MS);
                                                iterator++;
                                            }
                                            gpio_set_level(GPIO_RELAY_OUT, 0);
                                            iterator = 0;
                                            memset(serial, 0, SERIAL_NUMBER_SIZE + SERIAL_CONSTRAINS);
                                            //memset(serial_number_from_scanner, 0, SERIAL_NUMBER_SIZE);
                                            LCD_Clear();
                                        }
                                    }
                                    if (!flag_card)
                                    {
                                        flag_counter = false;
                                        flag_time = false;
                                        flag_card = false;
                                        ////printf("\nserial not found\n");
                                        LCD_Clear();
                                        LCD_String(m5);
                                                        /*
                                        if (sock > 0)
                                        {
                                            m5[strlen(m5)] = '\n';
                                            flag_sock = socket_tx(strlen(m5), sock, m5);
                                            m5[strlen(m5) - 1] = 0;
                                        }
                                                */
                                        get_time(time);
                                        sprintf(file_name_buffer, "L");
                                        sprintf(logs_data, "%s,F,%02d/%02d,%02d:%02d\n", serial_number_from_scanner, time[4], time[5], time[2], time[1]);
                                        append_file(file_name_buffer, logs_data);
                                        write_file("N", logs_data);
                                                    /*
                                        if (sock > 0)
                                            socket_tx(strlen(logs_data), sock, logs_data);
                                                        */
                                        memset(logs_data, 0, LOGS_DATA_SIZE);
                                    }
                                    else if (!flag_time)
                                    {
                                        flag_counter = false;
                                        flag_time = false;
                                        flag_card = false;
                                        ////printf("\nserial expired\n");
                                        LCD_Clear();
                                        LCD_String(m6);
                                        get_time(time);
                                        sprintf(file_name_buffer, "L");
                                        sprintf(logs_data, "%s,E,%02d/%02d,%02d:%02d\n", serial_number_from_scanner, time[4], time[5], time[2], time[1]);
                                        append_file(file_name_buffer, logs_data);
                                        write_file("N", logs_data);
                                            /*
                                        if (sock > 0)
                                            socket_tx(strlen(logs_data), sock, logs_data);
                                            */
                                        memset(logs_data, 0, LOGS_DATA_SIZE);
                                    }
                                    else if (!flag_counter)
                                    {
                                        flag_counter = false;
                                        flag_time = false;
                                        flag_card = false;
                                        ////printf("\nserial not valid any more\n");
                                        LCD_Clear();
                                        LCD_String(m7);

                                        get_time(time);
                                        sprintf(file_name_buffer, "L");
                                        sprintf(logs_data, "%s,U,%02d/%02d,%02d:%02d\n", serial_number_from_scanner, time[4], time[5], time[2], time[1]);
                                        append_file(file_name_buffer, logs_data);
                                        write_file("N", logs_data);
                                        memset(logs_data, 0, LOGS_DATA_SIZE);
                                    }
                                    flag_counter = false;
                                    flag_time = false;
                                    flag_card = false;
                                }
                            }
                            memset(data, 0, DATA_SIZE);
                            memset(serial, 0, SERIAL_NUMBER_SIZE + SERIAL_CONSTRAINS);
                            memset(serial_number_from_scanner, 0, SERIAL_NUMBER_SIZE);
                            uart_flush_input(UART_NUM_1);
                        }
                        if(!flag_switch)
                            break;
                    }

                }
            }
            else
            {
                LCD_Clear();
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
            memset(file_name_buffer, 0, FILE_NAME_SIZE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        else
        {

            LCD_Clear();
            LCD_String("M");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            LCD_String("a");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            LCD_String("i");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            LCD_String("n");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            LCD_String("t");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            LCD_String("e");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            LCD_String("n");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            LCD_String("a");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            LCD_String("n");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            LCD_String("c");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            LCD_String("e");
            vTaskDelay(100 / portTICK_PERIOD_MS);

            vTaskDelay(500 / portTICK_PERIOD_MS);
            LCD_String(".");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            LCD_String(".");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            LCD_String(".");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            LCD_String(".");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            LCD_String(".");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            LCD_String(".");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            LCD_String(".");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            LCD_String(".");
            



        }
        
    }
}

extern "C" void app_main(void)
{
    /*

    ESP_ERROR_CHECK( heap_trace_init_standalone(trace_record, NUM_RECORDS) );
    heap_trace_start(HEAP_TRACE_ALL);
    */

    ESP_ERROR_CHECK(nvs_flash_init());
    
   bt_init_fn();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    /*
    heapUsedAtStart = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    //printf("Heap free at start %d.\n", heapUsedAtStart);

    */

    i2c_init();
    spiffs_init();

    enc_init();

    
    xTaskCreatePinnedToCore(task2,              "task2",        16384,  NULL,   3,  NULL,   0);
    xTaskCreatePinnedToCore(tcp_server_task,    "tcp_server",   16384,  NULL,   5,  NULL,   1);
    /*
    if (xEthHandle == NULL)
    {
        ////printf("\nkoko el da3ef\n");
    }
    else
    {
        ////printf("\nkoko el qawe\n");
    }
    */
}
