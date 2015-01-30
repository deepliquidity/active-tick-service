#pragma once
#include <ActiveTickServerAPI/ActiveTickStreamListener.h>
#include <ActiveTickServerAPI/ActiveTickServerRequestor.h>
#include <session/Session.H>
#include <socketIO/ServerSocket.H>

class Streamer : public ActiveTickStreamListener, public ActiveTickServerRequestor
{
	public:
		Streamer(const APISession& session);
		virtual ~Streamer(void);
		void setSocket(ActiveTickService*);

	private:
		virtual void	OnATStreamTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pUpdate);
		virtual void	OnATStreamQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pUpdate);
		virtual void	OnATStreamTopMarketMoversUpdate(LPATMARKET_MOVERS_STREAM_UPDATE pUpdate);


		//virtual void            OnATBarHistoryDbStreamer(uint64_t origRequest, ATBarHistoryStreamerType responseType, LPATBARHISTORY_RESPONSE pStreamer);
		//virtual void            OnATTickHistoryDbStreamer(uint64_t origRequest, ATTickHistoryStreamerType responseType, LPATTICKHISTORY_RESPONSE pStreamer);

		//virtual void            OnATMarketMoversDbStreamer(uint64_t origRequest, ATMarketMoversDbStreamerType responseType, LPATMARKET_MOVERSDB_RESPONSE pStreamer);
		//virtual void            OnATQuoteDbStreamer(uint64_t origRequest, ATQuoteDbStreamerType responseType, LPATQUOTEDB_RESPONSE pStreamer, uint32_t responseCount);

		virtual void              OnATMarketMoversStreamResponse(uint64_t origRequest, ATStreamResponseType responseType, LPATMARKET_MOVERS_STREAM_RESPONSE pResponse);

		virtual void 		  OnATQuoteStreamResponse(uint64_t origRequest, ATStreamResponseType responseType, LPATQUOTESTREAM_RESPONSE pResponse, uint32_t responseCount);
		//virtual void            OnATQuoteStreamStreamer(uint64_t origRequest, ATStreamStreamerType responseType, LPATQUOTESTREAM_RESPONSE pStreamer, uint32_t responseCount);

		//virtual void            OnATSectorListStreamer(uint64_t origRequest, LPATSECTORLIST_RECORD pRecords, uint32_t recordsCount);
		//virtual void            OnATConstituentListStreamer(uint64_t origRequest, LPATSYMBOL pSymbols, uint32_t symbolsCount);

		//virtual void            OnATRequestTimeout(uint64_t origRequest);

	private:
		const APISession&	m_session;
		ActiveTickService	*_socket;
};
