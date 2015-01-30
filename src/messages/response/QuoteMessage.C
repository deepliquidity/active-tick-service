#include "QuoteMessage.H"
#include <cstring>
#include <cstdio>

QuoteMessage::QuoteMessage():_hour(_buffer),
			     _minute(_buffer + 2),
			     _second(_buffer + 4),
			     _milliseconds(_buffer + 6),
			     _condition(_buffer + 8),
			     _bidExchange(_buffer + 9),
			     _askExchange(_buffer + 10),
			     _bidPrice(_buffer + 11),
			     _bidPrecision(_buffer + 11+sizeof(double)),
			     _askPrice(_buffer + 11 + sizeof(double) + 1),
			     _askPrecision(_buffer + 11 + sizeof(double) + 1 + sizeof(double)),
			     _bidSize(_buffer + 11 + sizeof(double) + 1 + sizeof(double) + 1),
			     _askSize(_buffer + 11 + sizeof(double) + 1 + sizeof(double) + 1 + 4),
			     _symbol(_buffer + 11 + sizeof(double) + 1 + sizeof(double) + 1 + 4 + 4)
{
	memset(_buffer,'\0',61+sizeof(double)+sizeof(double));
	_size = 61+sizeof(double)+sizeof(double);
}

int QuoteMessage::getSize()
{
	return _size;
}

uint16_t QuoteMessage::getHour()
{
	return _hour.getValue<uint16_t>();
}

uint16_t QuoteMessage::getMinute()
{
        return _minute.getValue<uint16_t>();
}

uint16_t QuoteMessage::getSecond()
{
        return _second.getValue<uint16_t>();
}

uint16_t QuoteMessage::getMilliSeconds()
{
        return _milliseconds.getValue<uint16_t>();
}

uint8_t QuoteMessage::getCondition()
{
	return _condition.getValue<char>();
}

char QuoteMessage::getBidExchange()
{
	return _bidExchange.getValue<char>();
}

char QuoteMessage::getAskExchange()
{
        return _askExchange.getValue<char>();
}

double QuoteMessage::getBidPrice()
{
	return _bidPrice.getValue<double>();
}

uint8_t QuoteMessage::getBidPrecision()
{
        return _bidPrecision.getValue<uint8_t>();
}

double QuoteMessage::getAskPrice()
{
        return _askPrice.getValue<double>();
}

uint8_t QuoteMessage::getAskPrecision()
{
        return _askPrecision.getValue<uint8_t>();
}

uint32_t QuoteMessage::getBidSize()
{
	return _bidSize.getValue<uint32_t>();
}

uint32_t QuoteMessage::getAskSize()
{
        return _askSize.getValue<uint32_t>();
}

std::string QuoteMessage::getSymbol()
{
	return _symbol.getValue<std::string>();
}


void QuoteMessage::setHour(uint16_t hour)
{
	_hour.setValue(&hour,2);
}

void QuoteMessage::setMinute(uint16_t minute)
{
	_minute.setValue(&minute,2);
}

void QuoteMessage::setSecond(uint16_t second)
{
	_second.setValue(&second,2);
}

void QuoteMessage::setMilliSeconds(uint16_t milliSeconds)
{
	_milliseconds.setValue(&milliSeconds,2);
}

void QuoteMessage::setCondition(uint8_t condition)
{
	_condition.setValue(&condition,1);
}

void QuoteMessage::setBidExchange(char exchange)
{
	_bidExchange.setValue(&exchange,1);
}

void QuoteMessage::setAskExchange(char exchange)
{
	_askExchange.setValue(&exchange,1);
}

void QuoteMessage::setBidPrice(double price)
{
	_bidPrice.setValue(&price,sizeof(double));
}

void QuoteMessage::setBidPrecision(uint8_t precision)
{
	_bidPrecision.setValue(&precision,1);
}

void QuoteMessage::setAskPrice(double price)
{
	_askPrice.setValue(&price,sizeof(double));
}

void QuoteMessage::setAskPrecision(uint8_t precision)
{
	_askPrecision.setValue(&precision,1);
}

void QuoteMessage::setBidSize(uint32_t size)
{
	_bidSize.setValue(&size,4);
}

void QuoteMessage::setAskSize(uint32_t size)
{
	_askSize.setValue(&size,4);
}

void QuoteMessage::setSymbol(std::string symbol_)
{
	printf("SSSSS %s %d\n",symbol_.data(),symbol_.length());
	_symbol.setValue(const_cast<char*>(symbol_.data()),symbol_.length());
}

void QuoteMessage::deserialize(const char *ptr_,uint16_t size_)
{
	memcpy(_buffer,ptr_,size_);
	_size = size_;
}

void QuoteMessage::serialize(char *&ptr_,uint16_t &size_)
{
	ptr_ = _buffer;
	size_ = _size;
}
