#ifndef RESPONSE_QUOTE_MESSAGE_H
#define RESPONSE_QUOTE_MESSAGE_H

#include "MemPtr.H"
#include <inttypes.h>

class QuoteMessage
{
	public:
		QuoteMessage();
		void deserialize(const char*,uint16_t);
		void serialize(char*&,uint16_t&);
		int getSize();

		uint16_t getHour();
		uint16_t getMinute();
		uint16_t getSecond();
		uint16_t getMilliSeconds();

		uint8_t getCondition();

		char getBidExchange();
		char getAskExchange();

		double getBidPrice();
		uint8_t getBidPrecision();

		double getAskPrice();
		uint8_t getAskPrecision();

		uint32_t getBidSize();
		uint32_t getAskSize();

		std::string getSymbol();


		void setHour(uint16_t);
		void setMinute(uint16_t);
		void setSecond(uint16_t);
		void setMilliSeconds(uint16_t);

		void setCondition(uint8_t);

		void setBidExchange(char);
		void setAskExchange(char);

		void setBidPrice(double);
		void setBidPrecision(uint8_t);

                void setAskPrice(double);
                void setAskPrecision(uint8_t);

		void setBidSize(uint32_t);
		void setAskSize(uint32_t);

		void setSymbol(std::string);
		
	private:
		MemPtr<2>  _hour;
		MemPtr<2>  _minute;
		MemPtr<2>  _second;
		MemPtr<2>  _milliseconds;

		MemPtr<1>  _condition;

		MemPtr<1>  _bidExchange;
		MemPtr<1>  _askExchange;

		MemPtr<sizeof(double)>  _bidPrice;
		MemPtr<1>  _bidPrecision;

                MemPtr<sizeof(double)>  _askPrice;
                MemPtr<1>  _askPrecision;

                MemPtr<4>  _bidSize;
		MemPtr<4>  _askSize;

		MemPtr<40> _symbol;

		char _buffer[61+sizeof(double)+sizeof(double)];
		int _size;
};

#endif
