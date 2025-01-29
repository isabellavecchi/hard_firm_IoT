#ifndef __PROJECT_CONFIG_LIB__
#define __PROJECT_CONFIG_LIB__

#define TRUE    1
#define FALSE   0

// I2S
#define I2S_BCLK    2
#define I2S_DATA    4
#define BUF_SIZE    4096 ///> tamanho [em bytes] dos buffers de entrada/transmissao
#define I2S_FREQ    22050
#define I2S_SAMPLE  I2S_DATA_BIT_WIDTH_24BIT

// RGB LED GPIOs
#define RGB_LED_RED_GPIO	7
#define RGB_LED_GREEN_GPIO	5
#define RGB_LED_BLUE_GPIO	6
    // RGB LED color mix channels
#define RGB_LED_CHANNEL_NUM	3 ///> qty of led rgb pins

// WI-FI
#define WIFI_AP_SSID			"WAKE_N_DANCE"		// Access Point SSID
#define WIFI_AP_PASSWORD		"12345678"			// AP password
#define WIFI_AP_CHANNEL			1					// AP channel
#define WIFI_AP_SSID_HIDDEN		0					// AP visible = 0
#define WIFI_AP_MAX_CONNECTIONS	5					// how many people can connect to AP
#define WIFI_AP_BEACON_INTERVAL	100					// MAYBE IT SHOULD BE BIGGER FOR NOT GETTING IN THE WAY OF SONG
#define WIFI_AP_IP				"192.168.0.1"		// AP default IP
#define WIFI_AP_GATEWAY			"192.168.0.1"		// AP default gateway
#define WIFI_AP_NETMASK			"255.255.255.0"     // AP netmask
#define WIFI_AP_BANDWIDTH		WIFI_BW_HT20		// AP bandwidth = 20 MHz, the 40 will be for station
#define WIFI_STA_POWER_SAVE		WIFI_PS_NONE		// Power save is not being used
#define WIFI_SSID_LENGTH		32					// IEEE standard maximum
#define WIFI_PASSWORD_LENGTH	64					// IEEE standard maximum
#define MAX_CONNECTION_RETRIES	5					// retry numbers on disconnect

// BUTTONS
#define MENU_BUTTON_PIN				0 ///> Menu Button is the BOOT button on the chip
#define LEFT_BUTTON_PIN				15
#define RIGHT_BUTTON_PIN			16

// DISPLAY OLED
#define DISPLAY_OLED_SDA_PIN    17
#define DISPLAY_OLED_SCL_PIN    18

#endif //__PROJECT_CONFIG_LIB__