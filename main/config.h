// Code version
#define FIRMWARE_VERSION "v1_alpha"

// WiFi Credentials
#define WIFI_SSID     ""
#define WIFI_PASSWORD ""

// Thingsboard address and port
#define TB_SERVER_ADDRESS ""
#define TB_SERVER_PORT    1883U

// Thingsboard Access Token
#define TB_TOKEN ""

// Size of JSON message buffer
#define TB_MAX_MESSAGE_SIZE	256U

// NTP server address and update interval
#define NTP_SERVER "ntp.cti.gov.br"
#define NTP_UPDATE_INTERVAL 21600UL // in seconds (6 hours)

// BAUD rate for Serial port
#define SERIAL_BAUD_RATE 115200

// Define custom MAC address, if needed. Comment out to use original MAC
// #define CUSTOM_MAC_ADDRESS
// uint8_t customMacAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
