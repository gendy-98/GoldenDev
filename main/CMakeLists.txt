set(srcs "esp_eth_mac_enc28j60.c"
         "esp_eth_phy_enc28j60.c"
         "enc_init.c"
         "funcs.c"
         "lcd.c"
		 "bt_init.c"
		 "spp_task.c"
         "uart_echo_example_main.c"
         "my_gpio.c"
         "Ready_spiffs.c"
         "rtc_i2c.c"
         "serviceFn.c"
         "uart0_computer.c"
         "uart1_scanner.c"
         "uart2_printer.c")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS ".")
