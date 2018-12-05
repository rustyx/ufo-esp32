#pragma once
#include "DynamicRequestHandler.h"
#include <mqtt_client.h>

class MQTTIntegration {
public:
    MQTTIntegration(Ufo& ufo);
    virtual ~MQTTIntegration();
    void Init();
    void HandleMessage(String data);
private:
    static void task_function_mqtt(void *pvParameter);
    void TaskInit();
    static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);
    esp_err_t HandleEvent(esp_mqtt_event_handle_t event);
    void SendStatus();

    Ufo& mUfo;
	esp_mqtt_client_config_t mClientConfig;
    esp_mqtt_client_handle_t mClient;
    bool mActive;
    bool mbConnected = false;
};
