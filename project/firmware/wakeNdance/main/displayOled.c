/**
 * Author: Isabella Vecchi Ferreira
 * Based on:
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */


/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries
#include <string.h>

// Personal libraries
#include "displayOled.h"
#include "projectConfig.h"


/**************************
**		DECLARATIONS	 **
**************************/

	/* Variables */

static const char TAG[] = "display-oled";
static lv_disp_t * disp = NULL;
static lv_obj_t *scr = NULL;
static lv_obj_t *labelDate  = NULL;
static lv_obj_t *labelTime = NULL;
static lv_obj_t *label_header = NULL;
static lv_obj_t *label_body = NULL;



/**************************
**		APP FUNCTIONS	 **
**************************/

void displayOled_setup(void)
{
    displayOled_init();
    scr = lv_disp_get_scr_act(disp);
    labelDate = lv_label_create(scr);
    labelTime = lv_label_create(scr);
    label_header = lv_label_create(scr);
    label_body = lv_label_create(scr);

    lv_label_set_text(labelDate, "");
    lv_label_set_text(labelTime, "");
    lv_label_set_text(label_header, "");
    lv_label_set_text(label_body, "");
}

void displayOled_printDateTime(const char * date, const char * hora)
{
	// Lock the mutex due to the LVGL APIs are not thread-safe
	if (lvgl_port_lock(0))
    {
        lv_label_set_text(labelDate, date);
        /* Size of the screen (if you use rotation 90 or 270, please set disp->driver->ver_res) */
        lv_obj_set_width(labelDate, disp->driver->hor_res);
        lv_obj_align(labelDate, LV_ALIGN_TOP_LEFT, 0, 0);
        
        lv_label_set_text(labelTime, hora);
        /* Size of the screen (if you use rotation 90 or 270, please set disp->driver->ver_res) */
        lv_obj_set_width(labelTime, disp->driver->hor_res);
        lv_obj_align(labelTime, LV_ALIGN_TOP_LEFT, 85, 0);
		// Release the mutex
        lvgl_port_unlock();
    }
}

void displayOled_printHeaderNBody(const char * header, const char * body)
{
	// Lock the mutex due to the LVGL APIs are not thread-safe
	if (lvgl_port_lock(0))
    {
        lv_label_set_text(label_header, header);
        /* Size of the screen (if you use rotation 90 or 270, please set disp->driver->ver_res) */
        lv_obj_set_width(label_header, disp->driver->hor_res);
        lv_obj_align(label_header, LV_ALIGN_CENTER, 0, 0);
        
        lv_label_set_text(label_body, body);
        /* Size of the screen (if you use rotation 90 or 270, please set disp->driver->ver_res) */
        lv_obj_set_width(label_body, disp->driver->hor_res);
        lv_obj_align(label_body, LV_ALIGN_BOTTOM_MID, 0, 0);
		// Release the mutex
        lvgl_port_unlock();
    }
}

void displayOled_printAccessPoint(void)
{
    char _ssid[32] = "ssid: ";
    char _passwd[32] = "passwd: ";
    strcat(_ssid, WIFI_AP_SSID);
    strcat(_passwd, WIFI_AP_PASSWORD);
    displayOled_printHeaderNBody(_ssid, _passwd);
}

void displayOled_printWebPageAccess(void)
{
    char webPage[32] = "www.";
    strcat(webPage, WIFI_AP_IP);
    strcat(webPage, "/index.html");
    displayOled_printDateTime("go to:", "");
    displayOled_printHeaderNBody(webPage, "");
}

void displayOled_printMusicName(void)
{
	// Lock the mutex due to the LVGL APIs are not thread-safe
	if (lvgl_port_lock(0))
    {
		// Release the mutex
        lvgl_port_unlock();
    }
}

void displayOled_printMenuList(void)
{
	// Lock the mutex due to the LVGL APIs are not thread-safe
	if (lvgl_port_lock(0))
    {
		// Release the mutex
        lvgl_port_unlock();
    }
}



/**************************
**		HAL FUNCTION	 **
**************************/


void displayOled_init(void)
{
    ESP_LOGI(TAG, "Initialize I2C bus");
    i2c_master_bus_handle_t i2c_bus = NULL;
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .i2c_port = I2C_BUS_PORT,
        .sda_io_num = EXAMPLE_PIN_NUM_SDA,
        .scl_io_num = EXAMPLE_PIN_NUM_SCL,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = EXAMPLE_I2C_HW_ADDR,
        .scl_speed_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .control_phase_bytes = 1,               // According to SSD1306 datasheet
        .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,   // According to SSD1306 datasheet
        .lcd_param_bits = EXAMPLE_LCD_CMD_BITS, // According to SSD1306 datasheet
#if CONFIG_EXAMPLE_LCD_CONTROLLER_SSD1306
        .dc_bit_offset = 6,                     // According to SSD1306 datasheet
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
        .dc_bit_offset = 0,                     // According to SH1107 datasheet
        .flags =
        {
            .disable_control_phase = 1,
        }
#endif
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install SSD1306 panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = EXAMPLE_PIN_NUM_RST,
    };
#if CONFIG_EXAMPLE_LCD_CONTROLLER_SSD1306
    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = EXAMPLE_LCD_V_RES,
    };
    panel_config.vendor_config = &ssd1306_config;
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
    ESP_ERROR_CHECK(esp_lcd_new_panel_sh1107(io_handle, &panel_config, &panel_handle));
#endif

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

#if CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
#endif

    ESP_LOGI(TAG, "Initialize LVGL");
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_port_init(&lvgl_cfg);

    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES,
        .double_buffer = true,
        .hres = EXAMPLE_LCD_H_RES,
        .vres = EXAMPLE_LCD_V_RES,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }
    };
    disp = lvgl_port_add_disp(&disp_cfg);

    /* Rotation of the screen */
    lv_disp_set_rotation(disp, LV_DISP_ROT_180);
}