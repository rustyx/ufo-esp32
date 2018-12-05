#ifndef MAIN_UFOWEBSERVER_H_
#define MAIN_UFOWEBSERVER_H_

#include "Ota.h"
#include "freertos/FreeRTOS.h"
#include "openssl/ssl.h"
#include "WebServer.h"

class Ufo;
class DisplayCharter;

class UfoWebServer : public WebServer{
public:
	UfoWebServer();
	virtual ~UfoWebServer();

	bool StartUfoServer();

	void SetUfo(Ufo* pUfo) { mpUfo = pUfo; };

	virtual bool HandleRequest(HttpRequestParser& httpParser, HttpResponse& httpResponse);
	bool checkAuthAdmin(HttpRequestParser& httpParser);

private:
	Ufo* mpUfo;
	String mAuthHeader;
	String mAdminAuth;
	Ota mOta;
	bool mbRestart;

};

#endif 
