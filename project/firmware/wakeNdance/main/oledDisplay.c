/*
 * oledDisplay.c
 *
 *  Created on: 20 de nov. de 2024
 *      Author: IVecchiFerreira
 */


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "driver/i2c.h"
#include "esp_lcd_panel_ssd1306.h"
#include "esp_err.h"
#include "esp_log.h"


static const char *TAG = "example";

#define I2C_HOST  0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DISPLAY_OLED_LCD_PIXEL_CLOCK_HZ    (400 * 1000)
#define DISPLAY_OLED_PIN_SDA				17
#define DISPLAY_OLED_PIN_SCL           		18
#define DISPLAY_OLED_PIN_RST           		-1
#define DISPLAY_OLED_I2C_HW_ADDR           	0x3C

// The pixel number in horizontal and vertical
#define DISPLAY_OLED_LCD_H_RES              128
#define DISPLAY_OLED_LCD_V_RES              CONFIG_DISPLAY_OLED_SSD1306_HEIGHT

// Bit number used to represent command and parameter
#define DISPLAY_OLED_LCD_CMD_BITS           8
#define DISPLAY_OLED_LCD_PARAM_BITS         8

void app_main(void)
{
    ESP_LOGI(TAG, "Initialize I2C bus");
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = DISPLAY_OLED_PIN_SDA,
        .scl_io_num = DISPLAY_OLED_PIN_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = DISPLAY_OLED_LCD_PIXEL_CLOCK_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_HOST, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_HOST, I2C_MODE_MASTER, 0, 0, 0));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = DISPLAY_OLED_I2C_HW_ADDR,
        .control_phase_bytes = 1,               // According to SSD1306 datasheet
        .lcd_cmd_bits = DISPLAY_OLED_LCD_CMD_BITS,   // According to SSD1306 datasheet
        .lcd_param_bits = DISPLAY_OLED_LCD_CMD_BITS, // According to SSD1306 datasheet
        .dc_bit_offset = 6,                     // According to SSD1306 datasheet
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(I2C_HOST, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install SSD1306 panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = DISPLAY_OLED_PIN_RST,
    };
    // This only work with esp-idf master:
     esp_lcd_panel_ssd1306_config_t ssd1306_config = {
         .height = DISPLAY_OLED_LCD_V_RES,
     };
    // panel_config.vendor_config = &ssd1306_config;
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &ssd1306_config, &panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
}