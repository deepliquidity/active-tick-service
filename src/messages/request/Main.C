#include "Message.H"
#include <iostream>
#include <string>

int main()
{
	RequestMessage msg1;
	RequestMessage msg2;
	char *ptr;
	int size;

	//msg1.setLength(102);
	msg1.setMessageType('I');
	msg1.setSubscriptionType('S');
	msg1.setCommand("subscribeMarketMovers");
	msg1.setSymbol("MSFT");
	msg1.setExchange("ExchangeInternationalSecuritiesExchange");
	msg1.serialize(ptr,size);	

	msg2.deserialize(ptr,size);
	//std::cout << msg2.getLength() << std::endl;
	std::cout << msg2.getSize() << std::endl;
	std::cout << msg2.getMessageType() << std::endl;
	std::cout << msg2.getSubscriptionType() << std::endl;
	std::cout << msg2.getCommand() << " " << msg2.getCommand().length() << std::endl;
	std::cout << msg2.getSymbol() << " " << msg2.getSymbol().length() << std::endl;
	std::cout << msg2.getExchange() << std::endl;
	return 0;
}
