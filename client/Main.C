#include "Client.H"
#include <iostream>
#include "Message.H"
#include <cstdio>
#include "QuoteMessage.H"

int main(int argc, char *argv[])
{
	Client client;
	RequestMessage msg;
	char length[2];
	char *ptr;
	uint16_t size;

	msg.setMessageType('I');
	msg.setSubscriptionType('S');
	msg.setCommand("QuoteStream");
	msg.setSymbol("MSFT");
	msg.setExchange("ExchangeInternationalSecuritiesExchange");
	msg.serialize(ptr,size);
	memcpy(length,&size,2);

	/*QuoteMessage quoteMsg;

        quoteMsg.setHour(12);
        quoteMsg.setMinute(12);
        quoteMsg.setSecond(12);
        quoteMsg.setMilliSeconds(100);
        quoteMsg.setCondition(1);
        quoteMsg.setBidExchange('A');
        quoteMsg.setAskExchange('S');
        quoteMsg.setBidPrice(44.145);
        quoteMsg.setBidPrecision(2);
        quoteMsg.setAskPrice(188.234);
        quoteMsg.setAskPrecision(2);
        quoteMsg.setBidSize(20);
        quoteMsg.setAskSize(24);
        quoteMsg.setSymbol("MSFT");


	quoteMsg.serialize(ptr,size);*/

	if(client.open_connection(argv[1],argv[2]))
	{
		client.sendMessages(length,2);
		client.sendMessages(ptr,size);
	}
	while(1)
	{
		QuoteMessage msg;

		client.recvMessage(ptr,size);
		msg.deserialize(ptr,size);

		printf("RECV: [%0.2d:%0.2d:%0.2d:%0.2d] QUOTE [condition:%d symbol:%s bid:%0.*f ask:%0.*f bidSize:%d askSize:%d bidExchange:%c askExchange:%c]\n",
		msg.getHour(),msg.getMinute(),msg.getSecond(),msg.getMilliSeconds(),msg.getCondition(),msg.getSymbol().c_str(),msg.getBidPrecision(),msg.getBidPrice(),
		msg.getAskPrecision(),msg.getAskPrice(), msg.getBidSize(),msg.getAskSize(),msg.getBidExchange(),msg.getAskExchange());
	}
	return 0;
}
