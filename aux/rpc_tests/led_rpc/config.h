// WiFi Credentials
#define WIFI_SSID     ""
#define WIFI_PASSWORD ""

// MQTT Broker address/port
#define MQTT_SERVER_ADDRESS ""
#define MQTT_SERVER_PORT    1883

// MQTT Broker credentials
#define MQTT_CLIENT_ID ""
#define MQTT_USERNAME  ""
#define MQTT_PASSWORD  ""

// LED pin
#define LED_PIN 2

// Size of JSON message buffer, for MQTT
#define MSG_BUFFER_SIZE	100

// MQTT Topics
#define MQTT_ATTRIBUTE_TOPIC    "v1/devices/me/attributes"
#define MQTT_RPC_REQUEST_TOPIC  "v1/devices/me/rpc/request/+"
#define MQTT_RPC_RESPONSE_TOPIC "v1/devices/me/rpc/response/" // + REQUEST_ID
