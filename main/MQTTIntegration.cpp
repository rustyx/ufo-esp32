#include "MQTTIntegration.h"
#include "Ufo.h"
#include <mqtt_client.h>
#include <esp_log.h>
#include <cJSON.h>

static const char* LOGTAG = "MQTT";

MQTTIntegration::MQTTIntegration(Ufo& ufo) : mUfo(ufo) {
    mActive = true;
}

MQTTIntegration::~MQTTIntegration() {
    mActive = false;
}

void MQTTIntegration::task_function_mqtt(void *pvParameter) {
    ((MQTTIntegration*)pvParameter)->TaskInit();
    vTaskDelete(NULL);
}

void MQTTIntegration::Init() {
    if (mUfo.GetConfig().msMqttUri.empty()) {
        ESP_LOGI(LOGTAG, "Not starting MQTT - no uri configured");
        return;
    }
    if (mUfo.GetConfig().msMqttTopic.empty()) {
        ESP_LOGI(LOGTAG, "Not starting MQTT - no topic configured");
        return;
    }
	xTaskCreate(MQTTIntegration::task_function_mqtt, "Task_MQTT_init", 4096, this, 5, NULL);
}

esp_err_t MQTTIntegration::mqtt_event_handler(esp_mqtt_event_handle_t event) {
    return ((MQTTIntegration*)event->user_context)->HandleEvent(event);
}

void MQTTIntegration::TaskInit() {
    ESP_LOGI(LOGTAG, "Waiting for connection");
    for(int i = 0; i < 10 && !mUfo.GetWifi().IsConnected(); ++i) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    mClientConfig.event_handle = MQTTIntegration::mqtt_event_handler;
    mClientConfig.user_context = this;
    mClientConfig.client_id = mUfo.GetId().c_str();
    mClientConfig.keepalive = mUfo.GetConfig().muMqttKeepalive;
    mClientConfig.uri = mUfo.GetConfig().msMqttUri.c_str();
    if (!mUfo.GetConfig().msMqttPw.empty())
        mClientConfig.password = mUfo.GetConfig().msMqttPw.c_str();
    if (!mUfo.GetConfig().msMqttServerCert.empty())
        mClientConfig.cert_pem = mUfo.GetConfig().msMqttServerCert.c_str();
    if (!mUfo.GetConfig().msMqttClientKey.empty())
        mClientConfig.client_key_pem = mUfo.GetConfig().msMqttClientKey.c_str();
    if (!mUfo.GetConfig().msMqttClientCert.empty())
        mClientConfig.client_cert_pem = mUfo.GetConfig().msMqttClientCert.c_str();
    const char* uri = strchr(mClientConfig.uri, '@');
    ESP_LOGI(LOGTAG, "Connecting to %s", (uri ? uri + 1 : mClientConfig.uri));
    mClient = esp_mqtt_client_init(&mClientConfig);
    esp_mqtt_client_start(mClient);
    ESP_LOGI(LOGTAG, "Started");
    if (!mUfo.GetConfig().msMqttStatusTopic.empty() && mUfo.GetConfig().muMqttStatusPeriodSeconds > 0) {
        const uint32_t delay = std::max<int>(2, mUfo.GetConfig().muMqttStatusPeriodSeconds) * 1000 / portTICK_PERIOD_MS;
        while (mActive) {
            if (mbConnected) {
                SendStatus();
                vTaskDelay(delay);
            } else {
                vTaskDelay(2000 / portTICK_PERIOD_MS);
            }
        }
    }
}

void MQTTIntegration::SendStatus() {
    String sPayload;
    sPayload.printf("{\"timestamp\":\"%u\",", esp_log_timestamp());
    sPayload.printf("\"device\":{\"id\":\"%s\",", mUfo.GetId().c_str());
    sPayload.printf("\"clientIP\":\"%s\",", mUfo.GetWifi().GetLocalAddress().c_str());
    sPayload.printf("\"freemem\":\"%u\"}}", esp_get_free_heap_size());
    int msg_id = esp_mqtt_client_publish(mClient, mUfo.GetConfig().msMqttStatusTopic.c_str(), sPayload.c_str(), sPayload.length(),
        mUfo.GetConfig().muMqttStatusQos, 0);
    ESP_LOGI(LOGTAG, "status publish successful, msg_id=%d", msg_id);
}

esp_err_t MQTTIntegration::HandleEvent(esp_mqtt_event_handle_t event) {
    //esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            mbConnected = true;
            msg_id = esp_mqtt_client_subscribe(mClient, mUfo.GetConfig().msMqttTopic.c_str(), 0);
            ESP_LOGI(LOGTAG, "Connected, subscribing to %s, msg_id=%d", mUfo.GetConfig().msMqttTopic.c_str(), msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            mbConnected = false;
            ESP_LOGI(LOGTAG, "Disconnected");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(LOGTAG, "Subscribed, msg_id=%d", event->msg_id);
            //msg_id = esp_mqtt_client_publish(client, mUfo.GetConfig().msMqttTopic, "ping", 0, 0, 0);
            //ESP_LOGI(LOGTAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(LOGTAG, "Message received: '%.*s'", event->data_len, event->data);
            HandleMessage(String(event->data, event->data_len));
            //printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            //printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGE(LOGTAG, "MQTT_EVENT_ERROR");
            break;
    }
    return ESP_OK;
}

static inline unsigned toHex(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'F' + 10;
    return 16;
}

static inline void parseParams(const String& data, std::list<TParam>& params) {
    TParam *param = nullptr;
    bool eop = true, inName = true;
    unsigned hex1 = 0, hex2 = 0;
    for (const char *p = data.c_str(); *p; ++p) {
        if (eop) {
            params.emplace_back();
            param = &params.back();
            eop = false;
        }
        char c = *p;
        switch (c) {
            case '%':
                if ((hex1 = toHex(p[1])) < 16 && (hex2 = toHex(p[2])) < 16) {
                    p += 2;
                    c = (hex1 << 4) | hex2;
                }
                break;
            case '&':
                if (!param->paramName.empty() || !param->paramValue.empty())
                    eop = inName = true;
                continue;
            case '=':
                if (inName) {
                    inName = false;
                    continue;
                }
                break;
        }
        (inName ? param->paramName : param->paramValue) += c;
    }
}

void MQTTIntegration::HandleMessage(String data) {
    DynamicRequestHandler handler{ mUfo.CreateRequestHandler() };
    const char* end = nullptr;
    cJSON *json = cJSON_ParseWithOpts(data.c_str(), &end, 0);
    if (!json) {
        ESP_LOGE(LOGTAG, "json parse error at position %d", (end ? end - data.c_str() : 0));
        return;
    }
    auto *command = cJSON_GetObjectItemCaseSensitive(json, "command");
    if (cJSON_IsString(command) && command->valuestring) {
        std::list<TParam> params;
        parseParams(command->valuestring, params);
        /*
        String temp;
        for (auto &it : params) {
            if (!temp.empty())
                temp += ", ";
            temp += it.paramName;
            temp += '=';
            temp += it.paramValue;
        }
        ESP_LOGI(LOGTAG, "Handling api request: %s", temp.c_str());
        //*/
        handler.HandleApiRequest(params);
    }
    cJSON_Delete(json);
}
