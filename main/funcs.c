
#include "funcs.h"

void init_init(void)
{
    vTaskDelay(10 / portTICK_PERIOD_MS);
    GPIO_init();
    vTaskDelay(10 / portTICK_PERIOD_MS);
    uart2_init();
    vTaskDelay(10 / portTICK_PERIOD_MS);
    LCD_Init();
    vTaskDelay(10 / portTICK_PERIOD_MS);
    uart1_init();
    vTaskDelay(10 / portTICK_PERIOD_MS);
}
void socket_transmission(const int sock)
{
    int len;
    int i;
    esp_err_t ret;
    uint32_t serial_first, serial_last;
    char rx_buffer[DATA_SIZE];
    char *data = rx_buffer + 1;
    uint8_t time[7];
    uint32_t iterator = 0;
    char buffer[32];
    char data_buffer[DATA_SIZE + DATA_EXTENTION];
    char serial_buff[SERIAL_NUMBER_SIZE + SERIAL_CONSTRAINS];
    char serial_comp[SERIAL_NUMBER_SIZE];
    size_t total = 0 , used = 0;
    FILE *fl;
    FILE *f;

    memset(time, 0, 7);
    memset(buffer, 0, 32);
    memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
    memset(serial_comp, 0, SERIAL_NUMBER_SIZE);
    do
    {
        //vTaskDelay(10 / portTICK_PERIOD_MS);
        len = recv(sock, rx_buffer, sizeof(rx_buffer) + 1, 0);
        if (len < 0)
        {
            //ESP_LOGE(TAG, "Error occurred during receiving FN42: errno %d", errno);
        }
        else if (len == 0)
        {
            //ESP_LOGW(TAG, "Connection closed FN46");
        }
        else
        {
            char state = *(data - 1);
            rx_buffer[len] = 0; // Null-terminate whatever is received and treat it like a string
            //rx_buffer[len - 2] = 0;
            //ESP_LOGI(TAG, "FN53 Received %d bytes: %s", len, rx_buffer);
            switch (state)
            {
            case '@':
                data = rx_buffer;
                for (iterator = 0; iterator < 7; iterator++)
                {
                    time[iterator] = strtol(data + 1, &data, 10); //time format ss:mm:hh/ww/dd:mm:yy no spaces and no more than single separator
                }                                                 //ex @01:37:04/03/25:02:20 @01:08:04/01/03:02:20
                data = rx_buffer + 1;
                sprintf(data_buffer, "%02d:%02d:%02d/%02d/%02d:%02d:20%02d\n", time[0], time[1], time[2], time[3], time[4], time[5], time[6]);
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                set_time(time);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
            case 't':
            case 'T':
                memset(time, 0, 7);
                get_time((char *)time);
                sprintf(data_buffer, "Time is => %02d:%02d:%02d/%02d/%02d:%02d:20%02d\n", time[0], time[1], time[2], time[3], time[4], time[5], time[6]);
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
                //Define Pre time data
            case '1':
                //memcpy(data_buffer, data, DATA_SIZE);
                write_file("D1", data);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
            case '2':
                //memcpy(data_buffer, data, DATA_SIZE);
                write_file("D2", data);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
                //Define End data
            case '3':
                //memcpy(data_buffer, data, DATA_SIZE);
                write_file("D3", data);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
            case '4':
                //memcpy(data_buffer, data, DATA_SIZE);
                write_file("D4", data);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
            case '5':
                //memcpy(data_buffer, data, DATA_SIZE);
                write_file("Mw", data);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
            case '6':
                //memcpy(data_buffer, data, DATA_SIZE);
                write_file("M2", data);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
            case '7':
                //memcpy(data_buffer, data, DATA_SIZE);
                write_file("M3", data);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
            case '8':
                //memcpy(data_buffer, data, DATA_SIZE);
                write_file("M4", data);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
            case '9':
                //memcpy(data_buffer, data, DATA_SIZE);
                write_file("M5", data);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
            case '0':
                //memcpy(data_buffer, data, DATA_SIZE);
                write_file("M6", data);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
            case 'o':
            case 'O':
                //memcpy(data_buffer, data, DATA_SIZE);
                write_file("M7", data);
                sprintf(data_buffer, "Done \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
                // Append NEW Serial Number
            case 'f':
            case 'F':
                sprintf(data_buffer, "Restarting... \n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                esp_restart();
                break;
                //relay on
            case 'R':
            case 'r':
                gpio_set_level(GPIO_RELAY_OUT, 1);
                sprintf(buffer, "Relay on \n");
                //printf("%s", buffer);
                socket_tx(strlen(buffer), sock, buffer);
                memset(buffer, 0, BUFFER_SIZE);
                vTaskDelay(3000 / portTICK_PERIOD_MS);
                gpio_set_level(GPIO_RELAY_OUT, 0);
                sprintf(buffer, "Relay off \n");
                //printf("%s", buffer);
                socket_tx(strlen(buffer), sock, buffer);
                memset(buffer, 0, BUFFER_SIZE);
                break;
            case 'g':
            case 'G':
                sprintf(buffer, "%d\n", !gpio_get_level(GPIO_DRY_CONTACT_IN));
                //printf("%s", buffer);
                socket_tx(strlen(buffer), sock, buffer);
                memset(buffer, 0, 32);
                break;
            case 'I':
            case 'i':
                //I192.168.002.125,255.255.255.000,192.168.001.001
                if (strlen(data) == IP_GW_NM)
                {
                    write_file("IP", data);
                    sprintf(buffer, "Done \n");
                    //printf("%s", buffer);
                    socket_tx(strlen(buffer), sock, buffer);
                    memset(buffer, 0, BUFFER_SIZE);
                }
                else
                {
                    sprintf(data_buffer, "ERROR : Wrong format EX:\"I192.168.002.125,255.255.255.000,192.168.001.001\"\n");
                    //printf("%s", data_buffer);
                    socket_tx(strlen(data_buffer), sock, data_buffer);
                    memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                }

                break;
            case 's':
            case 'S':
                sprintf(buffer, "Formating file system... \n");
                //printf("%s", buffer);
                socket_tx(strlen(buffer), sock, buffer);
                if (read_file("IP", data, IP_GW_NM))
                {
                    sprintf(buffer, "IP stored. \n");
                    //printf("%s", buffer);
                    socket_tx(strlen(buffer), sock, buffer);
                }
                ret = esp_spiffs_format(NULL);

                if (ret == ESP_OK)
                {
                    sprintf(buffer, "Done \n");
                    //printf("%s", buffer);
                    socket_tx(strlen(buffer), sock, buffer);
                    
                }
                else
                {
                    sprintf(buffer, "Error while formating \n");
                    //printf("%s", buffer);
                    socket_tx(strlen(buffer), sock, buffer);
                }
                if (strlen(data) == IP_GW_NM)
                {
                    write_file("IP", data);
                    sprintf(buffer, "IP retrived. \n");
                    //printf("%s", buffer);
                    socket_tx(strlen(buffer), sock, buffer);
                    memset(buffer, 0, BUFFER_SIZE);
                }
                break;
            case 'p':
            case 'P':
                uart_write_bytes(UART_NUM_2, (const char *)data, strlen((const char *)data) + 1);
                sprintf(buffer, "Done \n");
                //printf("%s", buffer);
                socket_tx(strlen(buffer), sock, buffer);
                break;

            case 'A':
            case 'a':
                
                esp_spiffs_info(NULL, &total, &used);
                
                sprintf(data_buffer, "Partition size: total: %f MB, used: %f MB\n", (((float)total/1024)/1024), (((float)used/1024)/1024));
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;
            case 'Q':
            case 'q':
                LCD_Clear();
                LCD_String(data);
                sprintf(buffer, "Done \n");
                //printf("%s", buffer);
                socket_tx(strlen(buffer), sock, buffer);
                break;
            case 'w':
            case 'W':
                LCD_Clear();
                sprintf(buffer, "Done \n");
                //printf("%s", buffer);
                socket_tx(strlen(buffer), sock, buffer);
                break;
            case 'x'://disable switch
            case 'X':
            
                for(i = 0; i < strlen(data); i++){
                    if(!((data[i] <= 'z' && data[i] >= 'a') || (data[i] <= 'Z' && data[i] >= 'A'))) 
                    data[i] = 0;
                }                  
                printf("Xdaaataa is (%s)\n",data);
                if(!strcmp(data, "ON")){
                    printf("writing (%s) in x \n",data);    
                    write_file("X", data);
                    sprintf(data_buffer, "The switch is enabled \n");
                }
                else if (!strcmp(data, "OFF"))
                {
                    printf("writing (%s) in x \n",data);   
                    write_file("X", data);
                    sprintf(data_buffer, "The switch is disabled \n");
                }
                else{
                   sprintf(data_buffer, "Invaled input \n"); 
                }
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break; 
            case 'Z'://disable everything
            case 'z':  
                for(i = 0; i < strlen(data); i++){
                    if(!((data[i] <= 'z' && data[i] >= 'a') || (data[i] <= 'Z' && data[i] >= 'A'))) 
                    data[i] = 0;
                }                  
                printf("Zdaaataa is (%s)",data);
                if(!strcmp(data, "ON")){
                    //printf("writing (%s) in Z \n",data);   
                    write_file("Z", data);
                    sprintf(data_buffer, "The panel is enabled \n");
                }
                else if (!strcmp(data, "OFF"))
                {
                    //printf("writing (%s) in Z\n",data);   
                    write_file("Z", data);
                    sprintf(data_buffer, "The panel is disabled \n");
                }
                else{
                   sprintf(data_buffer, "Invaled input \n"); 
                }
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;  
            case 'e'://disable everything
            case 'E':
                if (read_file("Z", buffer, 8)){
                    sprintf(data_buffer, "The panel state is %s \n", buffer);
                    socket_tx(strlen(data_buffer), sock, data_buffer);
                }
                if (read_file("X", buffer, 8)){
                    sprintf(data_buffer, "The switch state is %s \n", buffer);
                    socket_tx(strlen(data_buffer), sock, data_buffer);
                }
                    
                //printf("%s", data_buffer);

                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                break;  
            case '+':
                if (count >= 99999)
                {
                    memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                    sprintf(data_buffer, "Error : maximum number of saved Serials \" 99999\"\n");
                    //printf("%s", data_buffer);
                    socket_tx(strlen(data_buffer), sock, data_buffer);
                }
                else
                {

                    //+123456789,dd:MM:yy:HH:mm,00654
                    if (strlen(data) == (SERIAL_NUMBER_SIZE + SERIAL_CONSTRAINS - 1))
                    {
                        count++;
                        memset(buffer, 0, 32);
                        buffer[0] = data[0];
                        buffer[1] = data[1];
                        buffer[2] = data[2];
                        buffer[3] = data[3];
                        buffer[4] = data[4];
                        buffer[5] = data[5];
                        buffer[6] = data[6];
                        buffer[7] = data[7];
                        write_file(buffer, data);
                        sprintf(buffer, "Done \n");
                        //printf("%s", buffer);
                        socket_tx(strlen(buffer), sock, buffer);
                        memset(buffer, 0, 32);
                    }
                    else
                    {
                        sprintf(data_buffer, "ERROR : Wrong format \"+CN(#8),DD:MM:YY:HH:mm,COUNTER(#5)\"\n");
                        //printf("%s,/%d\n", data_buffer, strlen(data));
                        socket_tx(strlen(data_buffer), sock, data_buffer);
                        memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                    }
                }
                break;

            case 'l':
            case 'L':
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                sprintf(data_buffer, "displaying logs\n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                
                f = fopen("/spiffs/L", "r");
                if (f == NULL)
                {
                    memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                    sprintf(data_buffer, "Failed to open L for reading \n");
                    //printf("%s", data_buffer);
                    socket_tx(strlen(data_buffer), sock, data_buffer);
                }
                else
                {
                    fseek(f, 0, SEEK_SET);
                    while (!feof(f))
                    {
                        fread(data_buffer, DATA_SIZE + DATA_EXTENTION, 1, f);
                        socket_tx(strlen(data_buffer), sock, data_buffer);
                        memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                    }
                    fclose(f);
                }
                break;
            case 'N':
            case 'n':
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                sprintf(data_buffer, "displaying logs\n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                
                fl = fopen("/spiffs/N", "r");
                if (fl == NULL)
                {
                    memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                    sprintf(data_buffer, "Failed to open L for reading \n");
                    //printf("%s", data_buffer);
                    socket_tx(strlen(data_buffer), sock, data_buffer);
                }
                else
                {
                    fseek(fl, 0, SEEK_SET);
                    while (!feof(fl))
                    {
                        fread(data_buffer, DATA_SIZE + DATA_EXTENTION, 1, fl);
                        socket_tx(strlen(data_buffer), sock, data_buffer);
                        memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                    }
                    fclose(fl);
                }
                break;
            case 'd':
            case 'D':
                if (remove("/spiffs/L") == 0)
                {
                    memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                    sprintf(data_buffer, "log file deleted successfully\n");
                    //printf("%s", data_buffer);
                    socket_tx(strlen(data_buffer), sock, data_buffer);
                }
                else
                {
                    memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                    sprintf(data_buffer, "unable to delete the log file\n");
                    //printf("%s", data_buffer);
                    socket_tx(strlen(data_buffer), sock, data_buffer);
                }
                break;
            // show files from to//////////////////////////////////////////
            case 'u':
            case 'U':
                //data[strlen(data) - 1] = 0;
                if (strlen(data) == (SERIAL_NUMBER_SIZE + SERIAL_NUMBER_SIZE - 1))
                {
                    serial_first = strtoll(data, NULL, 10);
                    serial_last = strtoll(data + SERIAL_NUMBER_SIZE, NULL, 10);
                    for (iterator = serial_first; iterator <= serial_last; iterator++)
                    {
                        sprintf(buffer, "%d", iterator);
                        if (read_file(buffer, serial_buff, SERIAL_NUMBER_SIZE + SERIAL_CONSTRAINS - 1))
                        {
                            serial_buff[strlen(serial_buff)] = '\n';
                            ////printf("%s", serial_buff);
                            socket_tx(strlen(serial_buff), sock, serial_buff);
                            memset(serial_buff, 0, SERIAL_NUMBER_SIZE + SERIAL_CONSTRAINS);
                        }
                    }
                }
                else
                {
                    sprintf(data_buffer, "ERROR : Wrong format \"uCN_first(#8),CN_last(#8)\"\n");
                    ////printf("%s", data_buffer);
                    socket_tx(strlen(data_buffer), sock, data_buffer);
                    memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                }
                break;
                //show file
            case 'K':
            case 'k':
                //data[strlen(data) - 1] = 0;
                if (strlen(data) == (SERIAL_NUMBER_SIZE - 1))
                {

                    if (read_file(data, serial_buff, SERIAL_NUMBER_SIZE + SERIAL_CONSTRAINS - 1))
                    {
                        serial_buff[strlen(serial_buff)] = '\n';
                        ////printf("%s", serial_buff);
                        socket_tx(strlen(serial_buff), sock, serial_buff);
                        memset(serial_buff, 0, SERIAL_NUMBER_SIZE + SERIAL_CONSTRAINS);
                    }
                    else
                    {
                        sprintf(data_buffer, "%s is not registered\n", data);
                        //printf("%s", data_buffer);
                        socket_tx(strlen(data_buffer), sock, data_buffer);
                        memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                    }
                }
                else
                {
                    sprintf(data_buffer, "ERROR : Wrong format \"uCN_first(#8),CN_last(#8)\"\n");
                    //printf("%s", data_buffer);
                    socket_tx(strlen(data_buffer), sock, data_buffer);
                    memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                }
                break;
            case 'c':
            case 'C':
                //c250ASC00,
                //data[strlen(data)-1] = 0;
                if (strlen(data) == (SERIAL_NUMBER_SIZE - 1))
                {
                    sprintf(buffer, "/spiffs/%s", data);

                    if (remove(buffer) == 0)
                    {
                        memset(buffer, 0, 32);
                        sprintf(buffer, "Delete successful\n");
                        //printf("%s", buffer);
                        socket_tx(strlen(buffer), sock, buffer);
                    }
                    else
                    {

                        memset(buffer, 0, 32);
                        sprintf(buffer, "The ID is not in the list\n");
                        //printf("%s", buffer);
                        socket_tx(strlen(buffer), sock, buffer);
                    }
                }
                else
                {
                    sprintf(data_buffer, "ERROR : Wrong format \"cCNOLD(#8),CNNEW(#8),DD:MM:YY:HH:mm,COUNTER(#5)\"\n");
                    //printf("%s", data_buffer);
                    socket_tx(strlen(data_buffer), sock, data_buffer);
                    memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                }
                break;
            case 'h':
            case 'H':
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                sprintf(data_buffer, "Change time \t\t\t\"@ss:mm:hh/ww/dd:mm:yy\"\nTicket message pre-time \t\"1MESSAGE\"\nTicket message post-time \t\"2MESSAGE\"\nTicket message pre-bracode \t\"3MESSAGE\"\nTicket message post-barcode \t\"4MESSAGE\"\nLCD welcome message  \t\t\"5MESSAGE\"\nLCD printing ticket message  \t\"6MESSAGE\"\nLCD ticket printed  \t\t\"7MESSAGE\"\nLCD correct ID message  \t\"8MESSAGE\"\nLCD wrong ID message  \t\t\"9MESSAGE\"\nLCD wrong Time message  \t\"0MESSAGE\"\nLCD Pass limit message  \t\"oMESSAGE\"\nDisplay current time \t\t\"T\"\nTurn relay on for 3 sec\t\t\"R");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                sprintf(data_buffer, "\"\nDelete all serial numbers\t\"K\"\nDisplay Logs \t\t\t\"L\"\nRestart the board\t\t\"F\"\nGet dry contact state \t\t\"G\"\nDelete Logs \t\t\t\"D\"\nDisplay this message \t\t\"H\"\nAdd new serial \t\t\t\"+SERIAL,DD:MM:YY:HH:mm,CCCCC\" \nChange serial  \t\t\t\"cOLDSERIAL,NEWSERIAL,DD:MM:YY:HH:mm,CCCCC\"\n");
                socket_tx(strlen(data_buffer), sock, data_buffer);
                break;
            default:
                memset(data_buffer, 0, DATA_SIZE + DATA_EXTENTION);
                sprintf(data_buffer, "\n");
                //printf("%s", data_buffer);
                socket_tx(strlen(data_buffer), sock, data_buffer);
                break;
            }
        }
    } while (len > 0);
}

bool socket_tx(int len, const int sock, const char *tx_buffer)
{
    bool flag = true;
    int to_write;
    int written;
    to_write = len;
    while (to_write > 0)
    {
        written = send(sock, tx_buffer + (len - to_write), to_write, 0);
        if (written < 0)
        {
            ////ESP_LOGE(TAG, "Error occurred during sending FN507: errno %d", errno);
            flag = false;
            break;
        }
        to_write -= written;
    }
    return flag;
}
