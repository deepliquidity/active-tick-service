#ifndef REQUEST_MESSAGE_H
#define REQUEST_MESSAGE_H

#include <util/MemPtr.H>
#include <inttypes.h>

class RequestMessage
{
	public:
		RequestMessage();
		void deserialize(const char*,uint16_t);
		void serialize(char*&,uint16_t&);
		int getSize();

		//uint16_t getLength();
		char getMessageType();
		char getSubscriptionType();
		std::string getCommand();
		std::string getSymbol();
		std::string getExchange();

		//void setLength(uint16_t);
		void setMessageType(char);
		void setSubscriptionType(char);
		void setCommand(std::string);
		void setSymbol(std::string);
		void setExchange(std::string);
		
	private:
		//MemPtr<2>  _length;
		MemPtr<1>  _msgType;
		MemPtr<1>  _subscription;
		MemPtr<40> _command;
		MemPtr<10> _symbol;
		MemPtr<50> _exchange;

		char _buffer[102];
		int _size;
};

#endif
