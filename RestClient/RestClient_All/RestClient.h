#pragma once
#include "Observer.h"
#include "HttpClient.h"

class RestClient : public Observer
{
	private:
		ISXHttpClient::HttpClient client;

	public:
	
		RestClient(Subject* obj);
	
		void request() override;		
		
};