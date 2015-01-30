#include "Message.H"
#include <cstring>

RequestMessage::RequestMessage():_msgType(_buffer + 2),
				 _subscription(_buffer + 3),
				 _command(_buffer + 4),
				 _symbol(_buffer + 44),
				 _exchange(_buffer + 54)
{
	memset(_buffer,'\0',102);
	_size = 102;
}

int RequestMessage::getSize()
{
	return _size;
}

/*uint16_t RequestMessage::getLength()
{
	return _length.getValue<uint16_t>();
}

void RequestMessage::setLength(uint16_t length)
{
	_length.setValue(&length,2);
}*/

char RequestMessage::getMessageType()
{
	return _msgType.getValue<char>();
}

void RequestMessage::setMessageType(char msgType)
{
	_msgType.setValue(&msgType,1);
}

char RequestMessage::getSubscriptionType()
{
	return _subscription.getValue<char>();
}

void RequestMessage::setSubscriptionType(char subscription)
{
	_subscription.setValue(&subscription,1);
}

std::string RequestMessage::getCommand()
{
	return _command.getValue<std::string>();
}

void RequestMessage::setCommand(std::string command_)
{
	_command.setValue(const_cast<char*>(command_.data()),command_.length());
}

std::string RequestMessage::getSymbol()
{
	return _symbol.getValue<std::string>();
}

void RequestMessage::setSymbol(std::string symbol_)
{
	_symbol.setValue(const_cast<char*>(symbol_.data()),symbol_.length());
}

std::string RequestMessage::getExchange()
{
	return _exchange.getValue<std::string>();
}

void RequestMessage::setExchange(std::string exchange_)
{
	_exchange.setValue(const_cast<char*>(exchange_.data()),exchange_.length());
}

void RequestMessage::deserialize(char *ptr_,uint16_t size_)
{
	memcpy(_buffer,ptr_,size_);
	_size = size_;
}

void RequestMessage::serialize(char *&ptr_,uint16_t &size_)
{
	ptr_ = _buffer;
	size_ = _size;
}
