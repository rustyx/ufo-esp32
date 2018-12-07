#include <freertos/FreeRTOS.h>
#include "Config.h"
#include "nvs_flash.h"
#include <esp_log.h>

static const char LOGTAG[] = "Config";

Config::Config() {
	mbAPMode = true;
	msAPSsid = "UFO";
	msHostname = "UFO";

	msSTASsid = "";
	msSTAPass = "";

	mbWebServerUseSsl = false;
	muWebServerPort = 0;

	mbDTEnabled = false;
	mbDTMonitoring = false;

	muLastSTAIpAddress = 0;
}

Config::~Config() {
}

bool Config::Read(){
	nvs_handle h;

	if (nvs_flash_init() != ESP_OK)
		return false;
	if (nvs_open("Ufo Config", NVS_READONLY, &h) != ESP_OK)
		return false;
	/*
	 * max key length = 15 characters!
	 */
	ReadBool(h, "APMode", mbAPMode);
	ReadString(h, "APSsid", msAPSsid);
	ReadString(h, "APPass", msAPPass);
	ReadInt(h, "STAIpAddress", muLastSTAIpAddress);
	ReadString(h, "STASsid", msSTASsid);
	ReadString(h, "STAPass", msSTAPass);
	ReadString(h, "STAENTUser", msSTAENTUser);
	ReadBigString(h, "STAENTCA", msSTAENTCA);
	ReadString(h, "hostname", msHostname);
	ReadBool(h, "DTEnabled", mbDTEnabled);
	ReadString(h, "DTEnvId", msDTEnvIdOrUrl);
	ReadString(h, "DTApiToken", msDTApiToken);
	ReadInt(h, "DTInterval", miDTInterval);
	ReadBool(h, "DTMonitoring", mbDTMonitoring);
	ReadBool(h, "SrvSSLEnabled", mbWebServerUseSsl);
	ReadShort(h, "SrvListenPort", muWebServerPort);
	ReadString(h, "SrvCert", msWebServerCert);
	ReadString(h, "UfoId", msUfoId);
	ReadString(h, "UfoName", msUfoName);
	ReadString(h, "Organization", msOrganization);
	ReadString(h, "Department", msDepartment);
	ReadString(h, "Location", msLocation);
	ReadString(h, "AdminPw", msAdminPw);
	ReadString(h, "MqttTopic", msMqttTopic);
	ReadString(h, "MqttStatusTopic", msMqttStatusTopic);
	ReadShort(h, "MqttStPeriod", muMqttStatusPeriodSeconds);
	ReadShort(h, "MqttStatusQos", muMqttStatusQos);
	ReadShort(h, "MqttQos", muMqttQos);
	ReadShort(h, "MqttKeepalive", muMqttKeepalive);
	ReadString(h, "MqttUri", msMqttUri);
	ReadString(h, "MqttPw", msMqttPw);
	ReadBigString(h, "MqttServerCert", msMqttServerCert);
	ReadBigString(h, "MqttClientKey", msMqttClientKey);
	ReadBigString(h, "MqttClientCert", msMqttClientCert);
	ReadString(h, "MqttErrorState", msMqttErrorState);

	nvs_close(h);
	return true;
}


bool Config::Write()
{
	nvs_handle h;

	if (nvs_flash_init() != ESP_OK)
		return false;
	if (nvs_open("Ufo Config", NVS_READWRITE, &h) != ESP_OK)
		return false;
	nvs_erase_all(h); //otherwise I need double the space

	if (!WriteBool(h, "APMode", mbAPMode))
		return nvs_close(h), false;
	if (!WriteString(h, "APSsid", msAPSsid))
		return nvs_close(h), false;
	if (!WriteString(h, "APPass", msAPPass))
		return nvs_close(h), false;
	if (!WriteString(h, "STASsid", msSTASsid))
		return nvs_close(h), false;
	if (!WriteString(h, "STAPass", msSTAPass))
		return nvs_close(h), false;
	if (!WriteString(h, "hostname", msHostname))
		return nvs_close(h), false;
	if (!WriteString(h, "STAENTUser", msSTAENTUser))
		return nvs_close(h), false;
	if (!WriteBigString(h, "STAENTCA", msSTAENTCA))
		return nvs_close(h), false;
	if (!WriteInt(h, "STAIpAddress", muLastSTAIpAddress))
		return nvs_close(h), false;

	if (!WriteBool(h, "DTEnabled", mbDTEnabled))
		return nvs_close(h), false;
	if (!WriteString(h, "DTEnvId", msDTEnvIdOrUrl))
		return nvs_close(h), false;
	if (!WriteString(h, "DTApiToken", msDTApiToken))
		return nvs_close(h), false;
	if (!WriteInt(h, "DTInterval", miDTInterval))
		return nvs_close(h), false;

	if (!WriteBool(h, "DTMonitoring", mbDTMonitoring))
		return nvs_close(h), false;

	if (!WriteBool(h, "SrvSSLEnabled", mbWebServerUseSsl))	
		return nvs_close(h), false;
	if (!WriteShort(h, "SrvListenPort", muWebServerPort))
		return nvs_close(h), false;
	if (!WriteString(h, "SrvCert", msWebServerCert))
		return nvs_close(h), false;

	if (!WriteString(h, "UfoId", msUfoId))
		return nvs_close(h), false;
	if (!WriteString(h, "UfoName", msUfoName))
		return nvs_close(h), false;
	if (!WriteString(h, "Organization", msOrganization))
		return nvs_close(h), false;
	if (!WriteString(h, "Department", msDepartment))
		return nvs_close(h), false;
	if (!WriteString(h, "Location", msLocation))
		return nvs_close(h), false;
	if (!WriteString(h, "AdminPw", msAdminPw))
		return nvs_close(h), false;

	WriteString(h, "MqttTopic", msMqttTopic);
	WriteString(h, "MqttStatusTopic", msMqttStatusTopic);
	WriteShort(h, "MqttStPeriod", muMqttStatusPeriodSeconds);
	WriteShort(h, "MqttStatusQos", muMqttStatusQos);
	WriteShort(h, "MqttQos", muMqttQos);
	WriteShort(h, "MqttKeepalive", muMqttKeepalive);
	WriteString(h, "MqttUri", msMqttUri);
	WriteString(h, "MqttPw", msMqttPw);
	WriteBigString(h, "MqttServerCert", msMqttServerCert);
	WriteBigString(h, "MqttClientKey", msMqttClientKey);
	WriteBigString(h, "MqttClientCert", msMqttClientCert);
	WriteString(h, "MqttErrorState", msMqttErrorState);

	nvs_commit(h);
	nvs_close(h);
	return true;
}

//------------------------------------------------------------------------------------

bool Config::ReadString(nvs_handle h, const char* sKey, String& rsValue){
	char* sBuf = NULL;
	__uint32_t u = 0;

	nvs_get_str(h, sKey, NULL, &u);
	if (!u)
		return false;
	sBuf = (char*)malloc(u+1);
	if (nvs_get_str(h, sKey, sBuf, &u) != ESP_OK)
		return free(sBuf), false;
	sBuf[u] = 0x00;
	rsValue = sBuf;
	free(sBuf);
	return true;
}

bool Config::ReadBigString(nvs_handle h, const char* sKey, String& rsValue){
	String sHelp;
	if (!ReadString(h, sKey, sHelp))
		return false;
	rsValue = sHelp;
	int i = 1;
	while ((i <= 5) && sHelp.length() == 1900){
		String sKeyHelp = sKey;
		sKeyHelp += i;
		i++;
		if (!ReadString(h, sKeyHelp.c_str(), sHelp))
			break;
		rsValue += sHelp;
	}
	return true;
}

bool Config::ReadBool(nvs_handle h, const char* sKey, bool& rbValue){
	uint8_t u = 0;
	esp_err_t rc = nvs_get_u8(h, sKey, &u);
	if (rc != ESP_OK) {
		ESP_LOGW(LOGTAG, "nvs_get_u8(%s) failed: %s", sKey, esp_err_to_name(rc));
		return false;
	}
	rbValue = u;
	return true;
}

bool Config::ReadInt(nvs_handle h, const char* sKey, uint32_t& riValue){
	esp_err_t rc = nvs_get_u32(h, sKey, &riValue);
	if (rc != ESP_OK) {
		ESP_LOGW(LOGTAG, "nvs_get_u32(%s) failed: %s", sKey, esp_err_to_name(rc));
		return false;
	}
	return true;
}

bool Config::ReadShort(nvs_handle h, const char* sKey, uint16_t& riValue){
	esp_err_t rc = nvs_get_u16(h, sKey, &riValue);
	if (rc != ESP_OK) {
		ESP_LOGW(LOGTAG, "nvs_get_u16(%s) failed: %s", sKey, esp_err_to_name(rc));
		return false;
	}
	return true;
}

bool Config::WriteString(nvs_handle h, const char* sKey, String& rsValue){
	esp_err_t rc = nvs_set_str(h, sKey, rsValue.c_str());
	if (rc != ESP_OK) {
		ESP_LOGE(LOGTAG, "nvs_set_str(%s) failed: %s", sKey, esp_err_to_name(rc));
		return false;
	}
	return true;
}

bool Config::WriteBigString(nvs_handle h, const char* sKey, String& rsValue){
	if (rsValue.length() <= 1900){
		return WriteString(h, sKey, rsValue);
	}
	int i = 0;
	int iWritten = 0;
	while (iWritten < rsValue.length()){
		String sKeyHelp = sKey;
		if (i)
			sKeyHelp += i;
		String sSub = rsValue.substring(iWritten, iWritten+1900);
		if (!WriteString(h, sKeyHelp.c_str(), sSub))
			return false;
		i++;
		iWritten += 1900;
	}
	return true;
}


bool Config:: WriteBool(nvs_handle h, const char* sKey, bool bValue){
	esp_err_t rc = nvs_set_u8(h, sKey, bValue ? 1 : 0);
	if (rc != ESP_OK) {
		ESP_LOGE(LOGTAG, "nvs_set_u8(%s) failed: %s", sKey, esp_err_to_name(rc));
		return false;
	}
	return true;
}

bool Config:: WriteInt(nvs_handle h, const char* sKey, uint32_t iValue){
	esp_err_t rc = nvs_set_u32(h, sKey, iValue);
	if (rc != ESP_OK) {
		ESP_LOGE(LOGTAG, "nvs_set_u32(%s) failed: %s", sKey, esp_err_to_name(rc));
		return false;
	}
	return true;
}

bool Config:: WriteShort(nvs_handle h, const char* sKey, uint16_t iValue){
	esp_err_t rc = nvs_set_u16(h, sKey, iValue);
	if (rc != ESP_OK) {
		ESP_LOGE(LOGTAG, "nvs_set_u16(%s) failed: %s", sKey, esp_err_to_name(rc));
		return false;
	}
	return true;
}
