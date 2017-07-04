#include "Ufo.h"
#include "AWSIntegration.h"
#include "DynatraceMonitoring.h"
#include "DynatraceAction.h"
#include "Config.h"
#include "String.h"
#include "esp_system.h"
#include <esp_log.h>
#include <cJSON.h>


static const char* LOGTAG = "DTMon";
static const char* AGENT = "Dynatrace Open Kit";
static const char* ENDPOINT = "Dynatrace UFO";
static const char* VERSION = "2.0";


void task_function_dynatrace_monitoring(void *pvParameter)
{
	((DynatraceMonitoring*)pvParameter)->Connect();
	vTaskDelete(NULL);
}


DynatraceMonitoring::DynatraceMonitoring() {
	ESP_LOGI(LOGTAG, "Start");
    mActive = true;
}

DynatraceMonitoring::~DynatraceMonitoring() {

}

bool DynatraceMonitoring::Init(Ufo* pUfo, AWSIntegration* pAws) {
	ESP_LOGI(LOGTAG, "Init");
    mpUfo = pUfo;
    mpAws = pAws;      
    mStartTimestamp = getTimestamp();
    mDevice.id = mpUfo->GetId();
    mDevice.name = mpUfo->GetId();
    mDevice.cpu = "ESP32"; 
    mDevice.os = "ESP32";
    mDevice.totalmem = 532480;
    mDevice.manufacturer = "Dynatrace";
    mDevice.modelId = "UFO2.0";
    mDevice.appVersion = "2.0";
    mDevice.appBuild = "1000";
    mBatterylevel = 100;
    mInitialized = true;
	xTaskCreate(&task_function_dynatrace_monitoring, "Task_DynatraceMonitoring", 8192, this, 5, NULL);
    return mInitialized;
}

bool DynatraceMonitoring::Connect() {
	ESP_LOGI(LOGTAG, "Connecting");

    while (!mConnected) {
        if (mpAws->mActive) {
            mConnected = true;
            mDevice.clientIp = mpUfo->GetWifi().GetLocalAddress();
        }
		vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    return Run();
}

bool DynatraceMonitoring::Run() {
	ESP_LOGI(LOGTAG, "Run");
    mActive = true;
    while (mpAws->mActive) {
        if (mActive) {
    		vTaskDelay(20000 / portTICK_PERIOD_MS);
            mActive = Process();
        }
		vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
    Shutdown();
    return mActive;
}

void DynatraceMonitoring::Stop() {
    ESP_LOGI(LOGTAG, "Stopping Dynatrace Monitoring");
    mActive = false;
}

bool DynatraceMonitoring::Process() {

    ESP_LOGI(LOGTAG, "Processing monitoring payload (%i actions)", mActionCount);

//    taskENTER_CRITICAL(&myMutex);
    __uint8_t actionCount = mActionCount;
    DynatraceAction* actionBuffer[100];
    for (__uint8_t i=0; i<mActionCount; i++) {
        actionBuffer[i] = mAction[i];
    }
    mActionCount = 0;
//    taskEXIT_CRITICAL(&myMutex);

    String payload = getPayload(actionBuffer, actionCount);
    Send(&payload);

    for (uint i=0; i<actionCount; i++) {
        delete actionBuffer[i];
    }

    return true;
}

String DynatraceMonitoring::getPayload(DynatraceAction* pActions[], __uint8_t pCount) {
    String sPayload = "{";
    sPayload.printf("\"timestamp\":\"%i\",", getTimestamp());
    sPayload.printf("\"sessionStart\":\"%i\",", mStartTimestamp);
    sPayload.printf("\"endpoint\":\"%s\",", ENDPOINT);
    sPayload.printf("\"agent\":\"%s\",", AGENT);
    sPayload.printf("\"version\":\"%s\",", VERSION);

    sPayload.printf("\"device\":{");
    sPayload.printf("\"id\":\"%s\",", mDevice.id.c_str());
    sPayload.printf("\"name\":\"%s\",", mDevice.name.c_str());
    sPayload.printf("\"clientIP\":\"%s\",", mDevice.clientIp.c_str());
    sPayload.printf("\"cpu\":\"%s\",", mDevice.cpu.c_str());
    sPayload.printf("\"os\":\"%s\",", mDevice.os.c_str());
    sPayload.printf("\"freemem\":\"%i\",", esp_get_free_heap_size());
    sPayload.printf("\"totalmem\":\"%i\",", mDevice.totalmem);
    sPayload.printf("\"batteryLevel\":\"%i\",", mBatterylevel);
    sPayload.printf("\"manufacturer\":\"%s\",", mDevice.manufacturer.c_str());
    sPayload.printf("\"modelId\":\"%s\",", mDevice.modelId.c_str());
    sPayload.printf("\"appVersion\":\"%s\",", mDevice.appVersion.c_str());
    sPayload.printf("\"appBuild\":\"%s\"", mDevice.appBuild.c_str());
    sPayload.printf("},");
    sPayload.printf("\"session\":{");
    sPayload.printf("\"id\":\"%i\"",1);
    sPayload.printf("},");
    sPayload.printf("\"actions\":[");
    for (uint i=0; i<pCount; i++) {
        sPayload.printf("%s", pActions[i]->getPayload().c_str());
        if (i<pCount-1) sPayload.printf(",");
    }
    sPayload.printf("]}");

    return sPayload;
}

void DynatraceMonitoring::Send(String* json) {
    ESP_LOGI(LOGTAG, "%s", json->c_str());
    String topic;
    topic.printf("/dynatraceufo/monitoring/%s", mpUfo->GetId().c_str());
    mpAws->Publish(topic.c_str(), topic.length(), json);
}

void DynatraceMonitoring::Shutdown() {
	ESP_LOGI(LOGTAG, "Shutdown");
    mActive = false;
}

DynatraceAction* DynatraceMonitoring::enterAction(String pName) {
    return this->enterAction(pName, ACTION_MANUAL, NULL);    
};

DynatraceAction* DynatraceMonitoring::enterAction(String pName, int pType) {
    return this->enterAction(pName, pType, NULL);    
};

DynatraceAction* DynatraceMonitoring::enterAction(String pName, DynatraceAction* pParent) {
    return this->enterAction(pName, ACTION_MANUAL, pParent);
};

DynatraceAction* DynatraceMonitoring::enterAction(String pName, int pType, DynatraceAction* pParent) {
    if (!mActive) return NULL;
    __uint32_t id = 0;
    __uint32_t parentId = 0;
    if (pParent) {
        parentId = pParent->getId();
    }
    DynatraceAction* action = new DynatraceAction(this);
    id = action->enter(pName, pType, parentId);
    ESP_LOGD(LOGTAG, "Action %i created: %s", id, pName.c_str());
    return action;
};

void DynatraceMonitoring::leaveAction(DynatraceAction* action) {
    if (action != NULL) {
        action->leave();
    }
}

void DynatraceMonitoring::leaveAction(DynatraceAction* action, String* pUrl, ushort pResponseCode, uint pResponseSize) {
    if (action != NULL) {
        action->leave(pUrl, pResponseCode, pResponseSize);
    }    
}

void DynatraceMonitoring::addAction(DynatraceAction* action) {
//	ESP_LOGI(LOGTAG, "addAction");
//    taskENTER_CRITICAL(&myMutex);
    if (mActionCount < 90) {
        ESP_LOGI(LOGTAG, "Action %i added to stack: %s", mActionCount, action->getName().c_str());
        mAction[mActionCount++] = action;
    } else {
        ESP_LOGW(LOGTAG, "Action buffer full, action %s skipped", action->getName().c_str());
        delete action;
    }
//    taskEXIT_CRITICAL(&myMutex);
}

__uint32_t DynatraceMonitoring::getSequence0() {
    return seq0++;
};

__uint32_t DynatraceMonitoring::getSequence1() {
    return seq1++;
};

__uint32_t DynatraceMonitoring::getTimestamp() {
    __uint32_t ms = esp_log_timestamp();
    ESP_LOGI(LOGTAG, "current timestamp: %i", ms);
    return ms;
};

