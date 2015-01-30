#ifndef Request_H
#define Request_H

#include <string>
#include <map>
#include <streamer/Streamer.H>
//#include <ATServerAPIDefines.h>

class Request
{
	public:
		void setStreamer(Streamer *streamer_);
		void handle(std::map<std::string,std::string>);
		void inputRawData(const char *ptr_,uint16_t size_);
	private:
		void initExchangeMap();
		
//		std::map<std::string,ATExchangeType> _exchangeMap;
		Streamer *_streamer;
};

#endif
