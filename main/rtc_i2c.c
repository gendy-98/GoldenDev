
#include "rtc_i2c.h"

char decToBcd(char val)
{
	return ((val / 10 * 16) + (val % 10));
}
// Convert binary coded decimal to normal decimal numbers
char bcdToDec(char val)
{
	return ((val / 16 * 10) + (val % 16));
}

void i2c_init()
{
	i2c_config_t conf = {0};
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = PIN_SDA;
	conf.scl_io_num = PIN_SCL;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = 100000;
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_1, &conf));
	vTaskDelay(20 / portTICK_PERIOD_MS);
	
	i2c_driver_install(I2C_NUM_1, I2C_MODE_MASTER, 0, 0, 0);
	//ESP_ERROR_CHECK(i2c_driver_delete(I2C_NUM_1));
	//ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_1, I2C_MODE_MASTER, 0, 0, 0));
	vTaskDelay(20 / portTICK_PERIOD_MS);

}

void get_time(char *date)
{
	i2c_cmd_handle_t cmd;

	cmd = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(cmd));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (RTC_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
	i2c_master_write_byte(cmd, 0, 1);
	ESP_ERROR_CHECK(i2c_master_stop(cmd));
	i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);

	cmd = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(cmd));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (RTC_I2C_ADDRESS << 1) | I2C_MASTER_READ, 1));
	ESP_ERROR_CHECK(i2c_master_read_byte(cmd, (uint8_t *)date, 0));
	ESP_ERROR_CHECK(i2c_master_read_byte(cmd, (uint8_t *)date + 1, 0));
	ESP_ERROR_CHECK(i2c_master_read_byte(cmd, (uint8_t *)date + 2, 0));
	ESP_ERROR_CHECK(i2c_master_read_byte(cmd, (uint8_t *)date + 3, 0));
	ESP_ERROR_CHECK(i2c_master_read_byte(cmd, (uint8_t *)date + 4, 0));
	ESP_ERROR_CHECK(i2c_master_read_byte(cmd, (uint8_t *)date + 5, 0));
	ESP_ERROR_CHECK(i2c_master_read_byte(cmd, (uint8_t *)date + 6, 1));
	ESP_ERROR_CHECK(i2c_master_stop(cmd));
	i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
	date[6] = bcdToDec(date[6]);
	date[5] = bcdToDec(date[5]);
	date[4] = bcdToDec(date[4]);
	date[3] = bcdToDec(date[3]);
	date[2] = bcdToDec(date[2] & 0x3f);
	date[1] = bcdToDec(date[1]);
	date[0] = bcdToDec(date[0] & 0x7f);
}
void set_time(uint8_t *date)
{
	i2c_cmd_handle_t cmd;

	cmd = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(cmd));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (RTC_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
	i2c_master_write_byte(cmd, 0, 1);
	i2c_master_write_byte(cmd, decToBcd(date[0]), 1);
	i2c_master_write_byte(cmd, decToBcd(date[1]), 1);
	i2c_master_write_byte(cmd, decToBcd(date[2]), 1);
	i2c_master_write_byte(cmd, decToBcd(date[3]), 1);
	i2c_master_write_byte(cmd, decToBcd(date[4]), 1);
	i2c_master_write_byte(cmd, decToBcd(date[5]), 1);
	i2c_master_write_byte(cmd, decToBcd(date[6]), 1);
	ESP_ERROR_CHECK(i2c_master_stop(cmd));
	i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
}
