#include <ActiveTickServerAPI/ActiveTickServerAPI.h>
#include <session/Session.H>
#include <string>
#include <iostream>
#include <util/Util.H>
#include <util/CConfig.H>
#include <request/Request.H>
#include <functional>
#include <streamer/Streamer.H>
#include <socketIO/ServerSocket.H>

int main(int argc,char *argv[])
{
	ATInitAPI();
	APISession session;
	Streamer streamer(session);
	Request request;
	ActiveTickService service;

	if(argc > 1 && CConfig::Config_Init(argv[1]))
	{
		std::cout << "Successfully initialize configarator." << std::endl;
	}
	else
	{
		std::cerr << "Failed to initialize configarator." << std::endl;
		return -1;
	}
	request.setStreamer(&streamer);
	streamer.setSocket(&service);

	service.setInputCB(std::bind(&Request::inputRawData,&request,std::placeholders::_1,std::placeholders::_2));
	service.setLoginCB(std::bind(&APISession::startLogin,&session));
	service.setCloseCB(std::bind(&APISession::close,&session));
	session.setCommunicationCB(std::bind(&ActiveTickService::handleConnection,&service));

	service.open_listener(CConfig::Config_Get("hostname"),CConfig::Config_Get("port"));
	service.startConnection();


	while(1);
	return 0;
}
