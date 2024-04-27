// WiFi library for ESP8266
#include <ESP8266WiFi.h>

// MQTT and Thingsboard libraries
#define THINGSBOARD_ENABLE_PROGMEM 0 // ESP8266 does not support Strings in flash memory
#define THINGSBOARD_ENABLE_DEBUG 0   // debug mode select
#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>

// Configuration file
#include "config.h"

// Initalize the Mqtt client and Thingsboard instance
WiFiClient espClient;
Arduino_MQTT_Client mqttClient(espClient);
ThingsBoard tb(mqttClient, TB_MAX_MESSAGE_SIZE);

void setup() {
  // Connect to WiFi and Thingsboard
  connect();
  
  // Send system info to TB (attributes)
  sendSystemInfo();

  // Init relays in OFF state
  initRelays();

  // Subscribe to RPCs
  initRPCs();
}

void loop() {
  delay(500);

  // Reestablish connection with TB, if lost
  connect();

  // MQTT send / receive routines
  tb.loop();
}
