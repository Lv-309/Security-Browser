#pragma once
#include "Observer.h"
#include "HttpClient.h"

class RestClient : public Observer
{
private:
	ISXHttpClient::HttpClient client;

public:

	RestClient(Subject* obj);

	void Request(void) override;
	void Request(std::string) override;
	void SetConfigFile(std::string fileConfig);

};