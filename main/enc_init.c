#include "enc_init.h"


void enc_init()
{
    printf("(EI)init 6\n");
    //DISABLE DHCP CLIENT
    unsigned short int IP_data[12];
    char IP_file[255];

    memset(IP_file, 0, 255);


    if (read_file("IP", IP_file, IP_GW_NM))
    {
        IP_data[0] = strtol(IP_file, NULL, 10);
        IP_data[1] = strtol(IP_file + 4, NULL, 10);
        IP_data[2] = strtol(IP_file + 8, NULL, 10);
        IP_data[3] = strtol(IP_file + 12, NULL, 10);
        IP_data[4] = strtol(IP_file + 16, NULL, 10);
        IP_data[5] = strtol(IP_file + 20, NULL, 10);
        IP_data[6] = strtol(IP_file + 24, NULL, 10);
        IP_data[7] = strtol(IP_file + 28, NULL, 10);
        IP_data[8] = strtol(IP_file + 32, NULL, 10);
        IP_data[9] = strtol(IP_file + 36, NULL, 10);
        IP_data[10] = strtol(IP_file + 40, NULL, 10);
        IP_data[11] = strtol(IP_file + 44, NULL, 10);
    }
    else
    {
        IP_data[0] = 192;
        IP_data[1] = 168;
        IP_data[2] = 2;
        IP_data[3] = 50;
        IP_data[4] = 225;
        IP_data[5] = 225;
        IP_data[6] = 255;
        IP_data[7] = 0;
        IP_data[8] = 192;
        IP_data[9] = 168;
        IP_data[10] = 2;
        IP_data[11] = 1;
    }

    memset(IP_file, 0, 50);

    sprintf(IP_file, "IP %d:%d:%d:%d\nnetmask %d:%d:%d:%d\nGW %d:%d:%d:%d\n", IP_data[0], IP_data[1], IP_data[2], IP_data[3], IP_data[4], IP_data[5], IP_data[6], IP_data[7], IP_data[8], IP_data[9], IP_data[10], IP_data[11]);
    printf("(EI)%s\n",IP_file);
    ESP_ERROR_CHECK(gpio_install_isr_service(0)); //

    esp_netif_config_t netif_cfg = ESP_NETIF_DEFAULT_ETH(); //

    esp_netif_t *eth_netif = esp_netif_new(&netif_cfg);     //
    ESP_ERROR_CHECK(esp_netif_dhcpc_stop(eth_netif));
    //SET STATIC IP
    
    const esp_netif_ip_info_t STATIC_ip_info ={
        .ip ={ .addr = ESP_IP4TOADDR((const int)IP_data[0], (const int)IP_data[1], (const int)IP_data[2], (const int)IP_data[3]) },
        .gw ={ .addr = ESP_IP4TOADDR((const int)IP_data[8], (const int)IP_data[9], (const int)IP_data[10], (const int)IP_data[11]) },
        .netmask ={ .addr = ESP_IP4TOADDR((const int)IP_data[4], (const int)IP_data[5], (const int)IP_data[6], (const int)IP_data[7]) },
    };
    

    ESP_ERROR_CHECK(esp_netif_set_ip_info(eth_netif, &STATIC_ip_info));


    // Set default handlers to process TCP/IP stuffs
    ESP_ERROR_CHECK(esp_eth_set_default_handlers(eth_netif)); //
    // Register user defined event handers
    ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL)); //

    spi_bus_config_t buscfg ={
        .miso_io_num = CONFIG_EXAMPLE_ENC28J60_MISO_GPIO,
        .mosi_io_num = CONFIG_EXAMPLE_ENC28J60_MOSI_GPIO,
        .sclk_io_num = CONFIG_EXAMPLE_ENC28J60_SCLK_GPIO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };                                                                                 //
    ESP_ERROR_CHECK(spi_bus_initialize(CONFIG_EXAMPLE_ENC28J60_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO)); //
    /* ENC28J60 ethernet driver is based on spi driver */
    spi_device_interface_config_t devcfg ={
        .command_bits = 3,
        .address_bits = 5,
        .mode = 0,
        .cs_ena_posttrans = 2,
        .clock_speed_hz = SPI_MASTER_FREQ_8M,
        .spics_io_num = CONFIG_EXAMPLE_ENC28J60_CS_GPIO,
        .queue_size = 20 };                                                                       //
    spi_device_handle_t spi_handle = NULL;                                                       //
    ESP_ERROR_CHECK(spi_bus_add_device(CONFIG_EXAMPLE_ENC28J60_SPI_HOST, &devcfg, &spi_handle)); //

    eth_enc28j60_config_t enc28j60_config = ETH_ENC28J60_DEFAULT_CONFIG(spi_handle); //
    enc28j60_config.int_gpio_num = CONFIG_EXAMPLE_ENC28J60_INT_GPIO;                 //

    eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG(); //
    mac_config.smi_mdc_gpio_num = -1;                       // ENC28J60 doesn't have SMI interface
    mac_config.smi_mdio_gpio_num = -1;
    esp_eth_mac_t *mac = esp_eth_mac_new_enc28j60(&enc28j60_config, &mac_config); //

    eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();     //
    phy_config.autonego_timeout_ms = 0;                         // ENC28J60 doesn't support auto-negotiation
    phy_config.reset_gpio_num = -1;                             //// ENC28J60 doesn't have a pin to reset internal PHY//
    esp_eth_phy_t *phy = esp_eth_phy_new_enc28j60(&phy_config); //

    esp_eth_config_t eth_config = ETH_DEFAULT_CONFIG(mac, phy);
    esp_eth_handle_t eth_handle = NULL;
    ESP_ERROR_CHECK(esp_eth_driver_install(&eth_config, &eth_handle));
    
    /* ENC28J60 doesn't burn any factory MAC address, we need to set it manually.
       get wifi mac and init with it
    */

    uint8_t macaddr[6];
    //wifi_init_config_t ifx = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_efuse_mac_get_default(macaddr));
    //ESP_ERROR_CHECK(esp_wifi_get_mac(ifx, macaddr));
    //ESP_ERROR_CHECK( esp_wifi_deinit(void) );
    macaddr[5]+=4; // ethernet local address = base address + 4

    esp_err_t ret;
    ret = mac->set_addr(mac, macaddr);
    if(ret == ESP_OK)
    {
        ESP_LOGI(TAG, "set mac addr OK");
    }
    else if(ret == ESP_ERR_INVALID_ARG)
    {
        ESP_LOGI(TAG, "set mac addr INVALID_ARG");
    }
    //uncomment to force full duplex as well as line 776 in esp_eth_mac_enc28j60.c 
    //and 104 in esp_eth_phy_enc28j60.c
    /*
    ret = mac->set_duplex(mac, ETH_DUPLEX_FULL);
    if(ret == ESP_OK)
    {
        ESP_LOGW(TAG, "set_duplex OK");
    }
    else if(ret == ESP_FAIL)
    {
        ESP_LOGW(TAG, "set_duplex Fail");
    }
    */

    /* attach Ethernet driver to TCP/IP stack */
    ESP_ERROR_CHECK(esp_netif_attach(eth_netif, esp_eth_new_netif_glue(eth_handle)));
    /* start Ethernet driver state machine */
    ESP_ERROR_CHECK(esp_eth_start(eth_handle));
    //s_connection_name = "Ethernet";

   /* bool eth_set_promiscuous = true;
    ret = esp_eth_ioctl(eth_handle, ETH_CMD_S_PROMISCUOUS, &eth_set_promiscuous);
    if(ret == ESP_OK){
        ESP_LOGI(TAG, "ETH_CMD_S_PROMISCUOUS OK");
    } else {
        ESP_LOGI(TAG, "ETH_CMD_S_PROMISCUOUS not OK");
    }*/
}

/** Event handler for Ethernet events */
void eth_event_handler(void *arg, esp_event_base_t event_base,
    int32_t event_id, void *event_data)
{
    
    uint8_t mac_addr[6] ={ 0 };
    /* we can get the ethernet driver handle from event data */
    esp_eth_handle_t eth_handle = *(esp_eth_handle_t *)event_data;

    switch (event_id)
    {
    case ETHERNET_EVENT_CONNECTED: 
        //esp_eth_ioctl(eth_handle, 4, mac_addr);
        esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr);
        ESP_LOGI(TAG, "EI139 Ethernet Link Up");
        ESP_LOGI(TAG, "EI140 Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x",
            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
        mac_addr[0]=1;

        esp_eth_ioctl(eth_handle, ETH_CMD_G_SPEED, mac_addr);
        if (mac_addr[0] == ETH_SPEED_10M)
            ESP_LOGI(TAG, "EI139 Ethernet Link Speed 10Mbps ");
        else if(mac_addr[0] == ETH_SPEED_100M)
            ESP_LOGI(TAG, "EI139 Ethernet Link Speed 10Mbps ");

        esp_eth_ioctl(eth_handle, ETH_CMD_G_DUPLEX_MODE, mac_addr);
        if (mac_addr[0] == ETH_DUPLEX_HALF)
            ESP_LOGI(TAG, "EI139 Ethernet Link half duplex ");
        else if(mac_addr[0] == ETH_DUPLEX_FULL)
            ESP_LOGI(TAG, "EI139 Ethernet Link full duplex ");
        break;
    case ETHERNET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "EI144 Ethernet Link Down");
        break;
    case ETHERNET_EVENT_START:
        ESP_LOGI(TAG, "EI147 Ethernet Started");
        break;
    case ETHERNET_EVENT_STOP:
        ESP_LOGI(TAG, "EI150 Ethernet Stopped");
        break;
    default:
        break;
    }

}

/** Event handler for IP_EVENT_ETH_GOT_IP */
void got_ip_event_handler(void *arg, esp_event_base_t event_base,
    int32_t event_id, void *event_data)
{
    printf("(EI)init 161\n");
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    const esp_netif_ip_info_t *ip_info = &event->ip_info;

    ESP_LOGI(TAG, "EI164 Ethernet Got IP Address");
    ESP_LOGI(TAG, "~~~~~~~~~~~");
    ESP_LOGI(TAG, "EI166 ETHIP:" IPSTR, IP2STR(&ip_info->ip));
    /*
    char lcd_ip[32];
    sprintf(lcd_ip, "IP"IPSTR, IP2STR(&ip_info->ip));
    printf("(EI)%s\n", lcd_ip);
    LCD_Clear();
    LCD_String(lcd_ip);
    */
    ESP_LOGI(TAG, "EI174 ETHMASK:" IPSTR, IP2STR(&ip_info->netmask));
    ESP_LOGI(TAG, "EI175 ETHGW:" IPSTR, IP2STR(&ip_info->gw));
    ESP_LOGI(TAG, "~~~~~~~~~~~");
}

