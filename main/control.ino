#include <stdint.h>

// Global state array for device states: false = OFF, true = ON
static bool deviceStates[N_DEVICES];

/**
 * Initialize all relays to the OFF state according to the control type.
 */
void initRelays() {
  memset(deviceStates, 0, N_DEVICES * sizeof(bool));  // Initialize all devices to OFF

  #if CONTROL_TYPE == CONTROL_TYPE_GPIO
  for(int i = 0; i < N_DEVICES; i++) {
    pinMode(devicePins[i], OUTPUT);
    digitalWrite(devicePins[i], OFF_STATE_GPIO);
  }
  #elif CONTROL_TYPE == CONTROL_TYPE_SERIAL
  Serial.begin(SERIAL_BAUD_RATE);
  delay(500);  // Wait for Serial to initialize

  for(int i = 0; i < N_DEVICES; i++) {
    sendCommand(i + 1, false);
    delay(100);
  }
  #endif
}

/**
 * Set the state of a specific device.
 * 
 * @param deviceNumber The index of the device to control.
 * @param state The state to set (true for ON, false for OFF).
 */
void controlSetState(uint8_t deviceNumber, bool state) {
  if(deviceNumber < N_DEVICES) {
    deviceStates[deviceNumber] = state;

    #if CONTROL_TYPE == CONTROL_TYPE_GPIO
    digitalWrite(devicePins[deviceNumber], state ? ON_STATE_GPIO : OFF_STATE_GPIO);
    #elif CONTROL_TYPE == CONTROL_TYPE_SERIAL
    sendCommand(deviceNumber + 1, state);
    #endif
  }
}

/**
 * Get the current state of a specific device.
 * 
 * @param deviceNumber The index of the device whose state is to be retrieved.
 * @return The current state of the device (true for ON, false for OFF).
 */
bool controlGetState(int deviceNumber) {
  if(deviceNumber < N_DEVICES) {
    return deviceStates[deviceNumber];
  }
  return false;  // Return false if the device number is out of bounds
}

#if CONTROL_TYPE == CONTROL_TYPE_SERIAL
// Reference https://pt.aliexpress.com/item/4001094436563.html?gatewayAdapt=glo2bra,
//           https://templates.blakadder.com/LC-Relay-ESP01-2R-5V.html
/**
 * Send a control message to the relay board via serial communication.
 * 
 * @param deviceID The ID of the device (1-indexed).
 * @param state The desired state (true for ON, false for OFF).
 */
void sendCommand(uint8_t deviceID, bool state) {
  uint8_t header = 0xA0;
  uint8_t command = state ? 0x01 : 0x00;
  uint8_t checksum = header + deviceID + command;

  uint8_t message[4] = { header, deviceID, command, checksum };
  Serial.write(message, sizeof(message));
}
#endif
