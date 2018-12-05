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

private:
	Ufo* mpUfo;
	bool mbRestart;

	Ota mOta;

};

#endif 
