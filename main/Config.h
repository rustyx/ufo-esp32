#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_

#include "nvs.h"
#include "String.h"

class Config {
public:
	Config();
	virtual ~Config();

	bool Read();
	bool Write();

	void ToggleAPMode() { mbAPMode = !mbAPMode; };

private:
	bool ReadString(nvs_handle h, const char* sKey, String& rsValue);
	bool ReadBigString(nvs_handle h, const char* sKey, String& rsValue);
	bool ReadBool(nvs_handle h, const char* sKey, bool& rbValue);
	bool ReadInt(nvs_handle h, const char* sKey, uint32_t& rbValue);
	bool ReadShort(nvs_handle h, const char* sKey, uint16_t& rbValue);
	bool WriteString(nvs_handle h, const char* sKey, String& rsValue);
	bool WriteBigString(nvs_handle h, const char* sKey, String& rsValue);
	bool WriteBool(nvs_handle h, const char* sKey, bool bValue);
	bool WriteInt(nvs_handle h, const char* sKey, uint32_t bValue);
	bool WriteShort(nvs_handle h, const char* sKey, uint16_t bValue);

public:
	String msAPSsid;
	String msAPPass;
	String msSTASsid;
	String msSTAPass;
	String msSTAENTUser;
	String msSTAENTCA;
	String msHostname;
	String msUfoId;
	String msUfoName;
	String msOrganization;
	String msDepartment;
	String msLocation;
	String msAdminPw;
	String msMqttTopic;
	String msMqttStatusTopic;
	uint16_t muMqttStatusPeriodSeconds = 120;
	uint16_t muMqttStatusQos = 0;
	uint16_t muMqttQos = 1;
	uint16_t muMqttKeepalive = 30;
	String msMqttUri;
	String msMqttPw;
	String msMqttServerCert;
	String msMqttClientKey;
	String msMqttClientCert;
	String msMqttErrorState;
	
    String msDTEnvIdOrUrl;
    String msDTApiToken;
    uint32_t miDTInterval;

	uint32_t muLastSTAIpAddress;	
	String msWebServerCert;
	uint16_t muWebServerPort;
	bool mbWebServerUseSsl;
	bool mbDTEnabled;
	bool mbDTMonitoring;
	bool mbAPMode;

};

#endif /* MAIN_CONFIG_H_ */
