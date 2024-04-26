#include <ESP8266WiFi.h>
#define THINGSBOARD_ENABLE_PROGMEM 0

#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>

constexpr char WIFI_SSID[] = "";
constexpr char WIFI_PASSWORD[] = "";

constexpr char TOKEN[] = "";

constexpr char THINGSBOARD_SERVER[] = "";
constexpr uint16_t THINGSBOARD_PORT = 1883U;

constexpr uint16_t MAX_MESSAGE_SIZE = 256U;

constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;


constexpr const char RPC_SET_VALUE_METHOD[] = "setValue";
constexpr const char RPC_GET_VALUE_METHOD[] = "getValue";
constexpr const char RPC_VALUE_KEY[] = "value";
constexpr const char RPC_RESPONSE_KEY[] = "response";


// Initialize ThingsBoard instance with the maximum needed buffer size
WiFiClient espClient;
Arduino_MQTT_Client mqttClient(espClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

// Statuses for subscribing to rpc
bool subscribed = false;

int led_status = 1;

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {
  Serial.println("Connecting to AP ...");
  
  // Attempting to establish a connection to the given WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    // Delay 500ms until a connection has been successfully established
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

/// @brief Reconnects the WiFi uses InitWiFi if the connection has been removed
/// @return Returns true as soon as a connection has been established again
bool reconnect() {
  // Check to ensure we aren't connected yet
  const wl_status_t status = WiFi.status();
  if (status == WL_CONNECTED) {
    return true;
  }

  // If we aren't establish a new connection to the given WiFi network
  InitWiFi();
  return true;
}

StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;

RPC_Response processGetValue(const RPC_Data &data) {
  Serial.println("Received getValue call");

  doc[RPC_RESPONSE_KEY] = led_status;
  return RPC_Response(doc);
}

RPC_Response processSetValue(const RPC_Data &data) {
  Serial.println("Received setValue call");

  led_status = data[RPC_VALUE_KEY];
  
  digitalWrite(BUILTIN_LED, led_status);

  doc[RPC_RESPONSE_KEY] = led_status;
  return RPC_Response(doc);
}

void setup() {
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, led_status);

  delay(1000);
  InitWiFi();
}

void loop() {
  delay(200);

  if (!reconnect()) {
    return;
  }

  if (!tb.connected()) {
    // Reconnect to the ThingsBoard server,
    // if a connection was disrupted or has not yet been established
    Serial.printf("Connecting to: (%s) with token (%s)\n", THINGSBOARD_SERVER, TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      Serial.println("Failed to connect");
    }
  }

  if (!subscribed) {
    Serial.println("Subscribing for RPC...");

    const std::array<RPC_Callback, 2U> callbacks = {
      RPC_Callback{ RPC_SET_VALUE_METHOD, processSetValue },
      RPC_Callback{ RPC_GET_VALUE_METHOD, processGetValue }
    };

    // Perform a subscription. All consequent data processing will happen in
    // processTemperatureChange() and processSwitchChange() functions,
    // as denoted by callbacks array.
    if (!tb.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
      Serial.println("Failed to subscribe for RPC");
      return;
    }

    Serial.println("Subscribe done");
    subscribed = true;
  }

  tb.loop();
}
