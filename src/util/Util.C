#include <Util.H>
#include <sstream> 
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

ATTIME Util::StringToATTime(const std::string& datetime)
{
	ATTIME st = {0};

	if(datetime.length() == 14)
	{
		st.year = (uint16_t)atol(datetime.substr(0, 4).c_str());
		st.month = (uint16_t)atol(datetime.substr(4, 2).c_str());
		st.day = (uint16_t)atol(datetime.substr(6, 2).c_str());
		st.hour = (uint16_t)atol(datetime.substr(8, 2).c_str());
		st.minute = (uint16_t)atol(datetime.substr(10, 2).c_str());
		st.second = (uint16_t)atol(datetime.substr(12, 2).c_str());
	}

	return st;
}

std::vector<ATSYMBOL> Util::StringToSymbols(const std::string& symbols)
{
	std::vector<ATSYMBOL> out;
	std::size_t pos = 0;
	std::size_t prevpos = 0;

	while((pos = symbols.find(",", pos)) != std::string::npos)
	{
		std::string symbol = symbols.substr(prevpos, pos - prevpos);
		out.push_back(StringToSymbol(symbol));		
		
		++pos;
		prevpos = pos;				
	}	

	out.push_back(StringToSymbol(symbols.substr(prevpos)));

	return out;
}

ATSYMBOL Util::StringToSymbol(const std::string& symbol)
{
	ATSYMBOL atSymbol;
	memset(&atSymbol, 0, sizeof(atSymbol));

	if(symbol.empty() == true)
		return atSymbol;

	//SYMBOL.EXCHANGE@COUNTRY -> CSCO.Q@U, CSCO.Q, CSCO, CSCO@U; .ABCDE.Q@U, .ABCDE.Q, .ABCDE;

	if(symbol.empty() == false)
	{
		switch(symbol.at(0))
		{
		case '.':	atSymbol.symbolType = SymbolStockOption; break;
		case '$':	atSymbol.symbolType = SymbolIndex; break;
		case '^':	atSymbol.symbolType = SymbolMutualFund; break;
		case '#':	atSymbol.symbolType = SymbolCurrency; break;
		case '&':	atSymbol.symbolType = SymbolBond; break;
		default:	atSymbol.symbolType = SymbolStock; break;
		}
		
		if(atSymbol.symbolType == SymbolCurrency)
		{
			atSymbol.countryType = CountryInternational;
			atSymbol.exchangeType = ExchangeForex;
		}
		else
		{
			atSymbol.countryType = CountryUnitedStates;
			atSymbol.exchangeType = ExchangeComposite;
		}
	}

	//find .EXCHANGE
	std::size_t exchangeIndex = symbol.find('.', 1);
	if(exchangeIndex != std::string::npos && symbol.length() > exchangeIndex + 1)
		atSymbol.exchangeType = (uint8_t)symbol.at(exchangeIndex + 1);

	//find @COUNTRY
	std::size_t countryIndex = symbol.find('@', 1);
	if(countryIndex != std::string::npos && symbol.length() > countryIndex + 1)
		atSymbol.countryType = (uint8_t)symbol.at(countryIndex + 1);

	std::size_t symbolBodyLen = 0;
	
	if(exchangeIndex != std::string::npos)
		symbolBodyLen = exchangeIndex;
	else if(countryIndex != std::string::npos)
		symbolBodyLen = countryIndex;
	else
		symbolBodyLen = symbol.length();

	if(symbolBodyLen > 0)
	{
		if(atSymbol.symbolType == SymbolStock)
			Util::ConvertString(symbol.c_str(), atSymbol.symbol, symbolBodyLen);
		else
			Util::ConvertString(symbol.c_str() + 1, atSymbol.symbol, symbolBodyLen - 1);
	}

	return atSymbol;
}

ATGUID Util::StringToATGuid(const std::string& guid)
{
	ATGUID atGuid = {0};

	if(guid.length() == 32)
	{
		for(uint32_t i = 0; i < 16; i+=2)
		{
			uint32_t n = strtoul(guid.substr(i, 2).c_str(), NULL, 16);

			if(i < 8)
				((uint8_t*)&atGuid)[3 - (i / 2)] = n;
			else if(i < 12)
				((uint8_t*)&atGuid)[9 - (i / 2)] = n;
			else if(i < 16)
				((uint8_t*)&atGuid)[13 - (i / 2)] = n;
		}

		for(uint32_t i = 16; i < 32; i+=2)
		{
			uint32_t n = strtoul(guid.substr(i, 2).c_str(), NULL, 16);
			((uint8_t*)&atGuid)[i / 2] = n;
		}
	}

	return atGuid;
}

bool Util::ConvertString(const char* src, wchar16_t* dest, uint32_t destcount)
{
	bool rc = true;
	uint32_t i = 0;

	while(src[i])
	{
		if(i >= destcount)
		{
			rc = false;
			break;
		}

		dest[i] = (wchar16_t)src[i];
		++i;
	}

	if(rc == true && i < destcount)
		dest[i] = 0;
	else
		rc = false;

	return rc;
}

std::vector<wchar16_t> Util::ConvertString(const std::string& src)
{
	std::vector<wchar16_t> dest;

	for(size_t i = 0; i < src.length(); i++)
		dest.insert(dest.end(), (wchar16_t)src.at(i));

	dest.push_back(0);
	return dest;
}

std::string Util::ConvertString(const wchar16_t* src, uint32_t count)
{
	std::string dest;
	dest.reserve(count + 1);
	
	for(uint32_t i = 0; i < count; i++)
		dest.insert(dest.end(), (char)(src[i]));
	dest.push_back(0);

	return dest;
}

void Util::CopyString(wchar16_t* dest, const wchar16_t* src)
{
	const wchar16_t* s = src;
	wchar16_t* d = dest;

	while(*s)
	{
		*d = *s;
		d++;
		s++;
	}
	
	*d = 0;
}

void Util::CopyString(wchar16_t* dest, const wchar16_t* src, uint32_t count)
{
	const wchar16_t* s = src;
	wchar16_t* d = dest;
	uint32_t c = 0;

	while(*s && c < count)
	{
		*d = *s;
		d++;
		s++;
		c++;
	}

	if(c < count)
		*d = 0;
}

uint32_t Util::StringLength(const wchar16_t* src)
{
	uint32_t count = 0;

	const wchar16_t* p = src;
	while(*p++)
		++count;

	return count;
}

uint32_t Util::FromString(const std::string& str)
{
	std::istringstream ss(str);
	uint32_t ret;
	ss >> ret;
	return ret;
}

char* Util::readRawData(int sockfd, int len)
{
        char *data = (char*)malloc(len * sizeof(char));
        char *ptr = data;
        int n = 0;

        while(n < len)
        {
                n = read(sockfd, data, len);
		if(n == 0)
			return NULL;
                if(n == len)
                {
                        return ptr;
                }
                len -= n;
                data += n;
                n = 0;
        }
        return ptr;
}

