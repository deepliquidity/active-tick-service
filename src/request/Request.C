#include "Request.H"
#include <cstdio>
#include <ActiveTickServerAPI/ActiveTickServerAPI.h>
#include <util/Util.H>
#include <messages/request/Message.H>
#include <util/ExchangeConstant.H>
#include <iostream>


void Request::initExchangeMap()
{
	//_exchangeMap[ExchangeConstant::ExchangeAMEX] = ExchangeAMEX;
}

void Request::setStreamer(Streamer *streamer_)
{
	_streamer = streamer_;
}

void Request::inputRawData(const char *ptr_,uint16_t size_)
{
	RequestMessage msg;
	std::map<std::string,std::string> params;

	msg.deserialize(ptr_,size_);
	params["exchange"] = msg.getExchange();
	params["symbol"] = msg.getSymbol();
	params["cmd"] = msg.getCommand();
	params["type"] = msg.getSubscriptionType();
	
	handle(params);
}

void Request::handle(std::map<std::string,std::string> params)
{
	std::string key = params["cmd"];
	std::string type = params["type"];
	if(key == "MarketMovers")
	{
		uint8_t exchange = Util::FromString(params["exchange"]);
		std::string symbol = params["symbol"];

		ATSYMBOL atSymbol = Util::StringToSymbol(symbol);
		atSymbol.symbolType = SymbolTopMarketMovers;
		atSymbol.exchangeType = exchange;

		ATStreamRequestType requestType = (type == "S" ? StreamRequestSubscribe : StreamRequestUnsubscribe);
		uint64_t request = _streamer->SendATMarketMoversStreamRequest(atSymbol, requestType, DEFAULT_REQUEST_TIMEOUT);

		printf("SEND (%llu): Market movers stream request [%s]\n", request, symbol.c_str());
	}
	else if(key == "QuoteStream")
	{
		std::string s = params["symbol"];
		std::vector<ATSYMBOL> symbols = Util::StringToSymbols(s);

		printf("SUBS %s\n",type.data());
		ATStreamRequestType requestType = (type == "S" ? StreamRequestSubscribe : StreamRequestUnsubscribe);
		uint64_t request = _streamer->SendATQuoteStreamRequest(symbols.data(), (uint16_t)symbols.size(), requestType, DEFAULT_REQUEST_TIMEOUT);

		printf("SEND (%llu): Quote stream request [%s]\n", request, s.c_str());
	}
	else if(key == "TradesOnlyQuoteStream")
	{
		std::string s = params["symbol"];
		std::vector<ATSYMBOL> symbols = Util::StringToSymbols(s);

		ATStreamRequestType requestType = (type == "S" ? StreamRequestSubscribeTradesOnly : StreamRequestUnsubscribeTradesOnly);
		uint64_t request = _streamer->SendATQuoteStreamRequest(symbols.data(), (uint16_t)symbols.size(), requestType, DEFAULT_REQUEST_TIMEOUT);

		printf("SEND (%llu): Quote stream (trades only) request [%s]\n", request, s.c_str());
	}
	else if(key == "QuotesOnlyQuoteStream")
	{
		std::string s = params["symbol"];
		std::vector<ATSYMBOL> symbols = Util::StringToSymbols(s);

		ATStreamRequestType requestType = (type == "S" ? StreamRequestSubscribeQuotesOnly : StreamRequestUnsubscribeQuotesOnly);
		uint64_t request = _streamer->SendATQuoteStreamRequest(symbols.data(), (uint16_t)symbols.size(), requestType, DEFAULT_REQUEST_TIMEOUT);

		printf("SEND (%llu): Quote stream (quotes only) request [%s]\n", request, s.c_str());
	}
}
