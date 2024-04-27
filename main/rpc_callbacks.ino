// Response object for RPCs
static StaticJsonDocument<JSON_OBJECT_SIZE(N_DEVICES + 2)> responseDoc; // 2 for setState and N_DEVICES for getState

/**
 * @brief Initializes RPC callbacks and subscribes to them.
 * 
 * This function sets up the callbacks for RPC methods like setting and getting the state.
 * It attempts to subscribe to the RPCs and sends a status message based on the subscription outcome.
 * In case of subscription failure, the system will restart.
 */
void initRPCs() {
  // Setup callbacks for RPCs
  const std::array<RPC_Callback, 2U> callbacks = {
    RPC_Callback{ RPC_SET_STATE_METHOD, processSetState },
    RPC_Callback{ RPC_GET_STATE_METHOD, processGetState }
  };

  // Perform subscriptions and report status
  if (!tb.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
    sendStatus("RPC_SUB", "ERROR", true);
    ESP.restart();
  }
  sendStatus("RPC_SUB", "OK", false);
}

/**
 * @brief Processes the RPC_SET_STATE_METHOD RPC request.
 *
 * This function interprets the RPC data to change the state of a specific device.
 * It validates the device number and the requested state before applying any changes.
 * If any part of the data is invalid (e.g., non-existent device or invalid state),
 * an error message is prepared in the response.
 * 
 * @param data The RPC data containing the device number and the new state to set.
 * @return An RPC_Response object that encapsulates the response data.
 */
RPC_Response processSetState(const RPC_Data &data) {
  responseDoc.clear();

  int deviceNumber = data[DEVICE_NUMBER_KEY];

  if(!data.containsKey(DEVICE_NUMBER_KEY) || deviceNumber < 0 || deviceNumber >= N_DEVICES) {
    responseDoc[STATUS_KEY] = ERROR_STATUS;
    responseDoc[MESSAGE_KEY] = INVALID_DEVICE_NUMBER_MESSAGE;
    return RPC_Response(responseDoc);
  }

  String requestedState = data[STATE_KEY];
  int requestedStateI = -1;

  if(requestedState == ON_STATE_VALUE) {
    requestedStateI = 1;
  }
  else if(requestedState == OFF_STATE_VALUE) {
    requestedStateI = 0;
  }

  if(!data.containsKey(STATE_KEY) || requestedStateI == -1) {
    responseDoc[STATUS_KEY] = ERROR_STATUS;
    responseDoc[MESSAGE_KEY] = INVALID_STATE_MESSAGE;
    return RPC_Response(responseDoc);
  }

  controlSetState(deviceNumber, requestedStateI == 1);

  responseDoc[STATUS_KEY] = SUCCESS_STATUS;
  return RPC_Response(responseDoc);
}

/**
 * @brief Processes the RPC_GET_STATE_METHOD RPC request.
 *
 * This function collects the current state of all devices and constructs a response.
 * Each device's state is fetched and stored in a JSON document, which is then returned.
 *
 * @param data The RPC data, not specifically used in this function as the state of all devices is returned.
 * @return An RPC_Response object containing the states of all devices.
 */
RPC_Response processGetState(const RPC_Data &data) {
  responseDoc.clear();

  char aux[10];

  for(int i = 0; i < N_DEVICES; i++) {
    snprintf(aux, sizeof(aux), "device_%d", i);
    responseDoc[aux] = controlGetState(i) ? ON_STATE_VALUE : OFF_STATE_VALUE;
  }

  return RPC_Response(responseDoc);
}
