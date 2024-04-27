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


// Define custom MAC address, if needed. Comment out to use original MAC
// #define CUSTOM_MAC_ADDRESS
// uint8_t customMacAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// CONTROL Types definitions
#define CONTROL_TYPE_SERIAL 2
#define CONTROL_TYPE_GPIO 1

// Devices controlled configuration
#define N_DEVICES 2
#define CONTROL_TYPE CONTROL_TYPE_SERIAL // CONTROL_TYPE_GPIO or CONTROL_TYPE_SERIAL

// Type 1: by gpio PIN state pin
#if CONTROL_TYPE == CONTROL_TYPE_GPIO
  uint8_t devicePins[N_DEVICES] = {0};

  #define ON_STATE_GPIO HIGH
  #define OFF_STATE_GPIO LOW
#endif
  
// Type 2: by UART
#if CONTROL_TYPE == CONTROL_TYPE_SERIAL
  #define SERIAL_BAUD_RATE 115200
#endif


// RPC configurations
constexpr char RPC_SET_STATE_METHOD[] = "setDeviceState";
constexpr char RPC_GET_STATE_METHOD[] = "getDeviceState";

constexpr char DEVICE_NUMBER_KEY[] = "deviceNumber";
constexpr char STATE_KEY[]         = "state";

constexpr char ON_STATE_VALUE[]  = "on";
constexpr char OFF_STATE_VALUE[] = "off";

constexpr char STATUS_KEY [] = "status";
constexpr char MESSAGE_KEY[] = "message";

constexpr char SUCCESS_STATUS[] = "OK";
constexpr char ERROR_STATUS[]   = "ERROR";

constexpr char INVALID_DEVICE_NUMBER_MESSAGE[] = "Invalid device number, must be 0 ... N_DEVICES-1";
constexpr char INVALID_STATE_MESSAGE[]         = "Invalid requested state, accept on/off";
