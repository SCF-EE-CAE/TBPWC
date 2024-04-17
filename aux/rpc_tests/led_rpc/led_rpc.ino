// WiFi library for ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h> // for OTA discovery in Arduino

// MQTT libraries
#include <WiFiClient.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Logging library
#include <ArduinoLog.h>

// Configuration file
#include "config.h"

// Define MQTT client to send data
WiFiClient wifiClient;
Adafruit_MQTT_Client mqttClient(&wifiClient,
                                MQTT_SERVER_ADDRESS, MQTT_SERVER_PORT,
                                MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);

// Topics to publish data
Adafruit_MQTT_Subscribe rpcRequestTopic = Adafruit_MQTT_Subscribe(&mqttClient, MQTT_RPC_REQUEST_TOPIC);
Adafruit_MQTT_Publish attributeTopic = Adafruit_MQTT_Publish(&mqttClient, MQTT_ATTRIBUTE_TOPIC, MQTT_QOS_1);

// Buffer for MQTT message
char msg[MSG_BUFFER_SIZE];

void WifiSetup() {
  Log.noticeln(NL NL "Connecting to " WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int8_t ret;
  if ((ret = WiFi.waitForConnectResult()) != WL_CONNECTED) {
    Log.errorln("Connection Error, status_code = %d", ret);
    Log.errorln("Resetting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  Log.noticeln(NL "WiFi connected. IP address: ");
  Log.noticeln(WiFi.localIP());
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqttClient.connected()) return;

  Log.noticeln("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqttClient.connect()) != 0) { // connect will return 0 for connected
    Log.errorln(mqttClient.connectErrorString(ret));
    Log.errorln("Retrying MQTT connection in 5 seconds...");
    mqttClient.disconnect();

    delay(5000);

    retries--;
    if (retries == 0) ESP.restart();
  }

  Log.noticeln("MQTT Connected!");
}

void setup() {
  // Iniatilize serial interface for logging
  Serial.begin(115200);
  while(!Serial && !Serial.available());
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  // Show MAC
  Log.noticeln(NL NL "MAC address: %s", WiFi.macAddress().c_str());

  // Initialize WiFi connection
  WifiSetup();

  // Setup RPC request topic
  mqttClient.subscribe(&rpcRequestTopic);

  // Connect with MQTT Broker
  MQTT_connect();

  Log.noticeln("enviando teste");
  if(attributeTopic.publish("{'teste': 'teste'}"))
    Log.noticeln("sucesso");
}

void loop() {
  MQTT_connect();

  Log.noticeln("main loop");
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqttClient.readSubscription(5000))) {
    if (subscription == &rpcRequestTopic) {
      char *rpc_request = (char *)rpcRequestTopic.lastread;
      Log.noticeln("Request received: %s", rpc_request);
    }

    Log.noticeln("while loop");
  }
  Log.noticeln("%x", subscription);

  delay(200);
}
