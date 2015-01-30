#include "Streamer.H"
#include <util/Util.H>
#include <Streamer.H>
#include <string>
#include <stdio.h>
#include <messages/response/QuoteMessage.H>
#include <ActiveTickServerAPI/ATQuoteStreamResponseParser.h>

using namespace std;

Streamer::Streamer(const APISession& session)
: ActiveTickStreamListener(session.GetSessionHandle())
, ActiveTickServerRequestor(session.GetSessionHandle())
, m_session(session)
{
}

Streamer::~Streamer(void)
{
}

void Streamer::setSocket(ActiveTickService *socket_)
{
	_socket = socket_;
}

void Streamer::OnATStreamTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pUpdate)
{	
	printf("RECV: [%0.2d:%0.2d:%0.2d] STREAMTRADE [symbol:%s last:%0.*f lastSize:%d exchange:%d]\n",
		pUpdate->lastDateTime.hour, pUpdate->lastDateTime.minute, pUpdate->lastDateTime.second,
		Util::ConvertString(pUpdate->symbol.symbol, _countof(pUpdate->symbol.symbol)).c_str(),
		pUpdate->lastPrice.precision, pUpdate->lastPrice.price,
		pUpdate->lastSize,pUpdate->symbol.exchangeType);
}

void Streamer::OnATStreamQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pUpdate)
{
	printf("RECV: [%0.2d:%0.2d:%0.2d] STREAMQUOTE [condition:%d symbol:%s bid:%0.*f ask:%0.*f bidSize:%d askSize:%d exchange:%d]\n",
		pUpdate->quoteDateTime.hour, pUpdate->quoteDateTime.minute, pUpdate->quoteDateTime.second,pUpdate->condition,
		Util::ConvertString(pUpdate->symbol.symbol, _countof(pUpdate->symbol.symbol)).c_str(),
		pUpdate->bidPrice.precision, pUpdate->bidPrice.price,
		pUpdate->askPrice.precision, pUpdate->askPrice.price,
		pUpdate->bidSize,
		pUpdate->askSize,pUpdate->symbol.exchangeType);

	QuoteMessage quoteMsg;
	char *ptr;
	uint16_t size;
	char length[2];
	std::string symbolValue = Util::ConvertString(pUpdate->symbol.symbol, _countof(pUpdate->symbol.symbol));

	printf("SYM %s %d\n",symbolValue.data(),symbolValue.length());
	quoteMsg.setHour(pUpdate->quoteDateTime.hour);
	quoteMsg.setMinute(pUpdate->quoteDateTime.minute);
	quoteMsg.setSecond(pUpdate->quoteDateTime.second);
	quoteMsg.setMilliSeconds(pUpdate->quoteDateTime.milliseconds);
	quoteMsg.setCondition(pUpdate->condition);
	quoteMsg.setBidExchange(pUpdate->bidExchange);
	quoteMsg.setAskExchange(pUpdate->askExchange);
	quoteMsg.setBidPrice(pUpdate->bidPrice.price);
	quoteMsg.setBidPrecision(pUpdate->bidPrice.precision);
	quoteMsg.setAskPrice(pUpdate->askPrice.price);
	quoteMsg.setAskPrecision(pUpdate->askPrice.precision);
	quoteMsg.setBidSize(pUpdate->bidSize);
	quoteMsg.setAskSize(pUpdate->askSize);
	quoteMsg.setSymbol(symbolValue);
	quoteMsg.serialize(ptr,size);

	printf("Len %d\n",size);
	memcpy(length,&size,2);
	_socket->sendMessage(length,2);
	_socket->sendMessage(ptr,size);
}

void Streamer::OnATStreamTopMarketMoversUpdate(LPATMARKET_MOVERS_STREAM_UPDATE pUpdate)
{
	printf("RECV: [%0.2d:%0.2d:%0.2d] STREAMMOVERS [symbol:%s\n",
		pUpdate->lastUpdateTime.hour, pUpdate->lastUpdateTime.minute, pUpdate->lastUpdateTime.second,
		Util::ConvertString(pUpdate->marketMovers.symbol.symbol, _countof(pUpdate->marketMovers.symbol.symbol)).c_str());

	for(int32_t i = 0; i < ATMarketMoversMaxRecords; i++)
	{
		printf("\tsybmol:%s last:%0.*f\n",
			Util::ConvertString(pUpdate->marketMovers.items[i].symbol.symbol, _countof(pUpdate->marketMovers.items[i].symbol.symbol)).c_str(),
			pUpdate->marketMovers.items[i].lastPrice.precision, pUpdate->marketMovers.items[i].lastPrice.price);
	}
}

void Streamer::OnATMarketMoversStreamResponse(uint64_t origRequest, ATStreamResponseType responseType, LPATMARKET_MOVERS_STREAM_RESPONSE pResponse)
{
        string strResponseType;
        switch(responseType)
        {
        case StreamResponseSuccess: strResponseType = "StreamResponseSuccess"; break;
        case StreamResponseInvalidRequest: strResponseType = "StreamResponseInvalidRequest"; break;
        case StreamResponseDenied: strResponseType = "StreamResponseDenied"; break;
        default: break;
        }
        printf("RECV (%llu): Market movers stream response [%s]\n--------------------------------------------------------------\n", (uint64_t)origRequest, strResponseType.c_str());
}

void Streamer::OnATQuoteStreamResponse(uint64_t origRequest, ATStreamResponseType responseType, LPATQUOTESTREAM_RESPONSE pResponse, uint32_t responseCount)
{
	string strResponseType;
	switch(responseType)
	{
		case StreamResponseSuccess: strResponseType = "StreamResponseSuccess"; break;
		case StreamResponseInvalidRequest: strResponseType = "StreamResponseInvalidRequest"; break;
		case StreamResponseDenied: strResponseType = "StreamResponseDenied"; break;
		default: break;
	}

	printf("RECV (%llu): Quote stream response [%s]\n--------------------------------------------------------------\n", (uint64_t)origRequest, strResponseType.c_str());

	if(responseType == StreamResponseSuccess)
	{
		ATQuoteStreamResponseParser parser(pResponse);
		parser.MoveToBeginning();

		if(parser.MoveToFirstDataItem() == true)
		{
			while(true)
			{
				string symbolStatus;
				switch(parser.GetSymbolStatus())
				{
					case SymbolStatusSuccess: symbolStatus = "SymbolStatusSuccess"; break;
					case SymbolStatusInvalid: symbolStatus = "SymbolStatusInvalid"; break;
					case SymbolStatusUnavailable: symbolStatus = "SymbolStatusUnavailable"; break;
					case SymbolStatusNoPermission: symbolStatus = "SymbolStatusNoPermission"; break;
					default: break;
				}

				printf("\tsymbol:%s symbolStatus:%s\n", Util::ConvertString(parser.GetSymbol()->symbol, _countof(parser.GetSymbol()->symbol)).c_str(), symbolStatus.c_str());

				if(parser.MoveToNextDataItem() == false)
					break;
			}
		}
	}
}

