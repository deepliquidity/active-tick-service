#ifndef _CCONFIG_H_
#define _CCONFIG_H_
#include <map>
#include <iostream>
#include <string>

class CConfig 
{
	public:
		static bool Config_Init(std::string fileName);
		static std::string Config_Get(std::string key);

	private:
		static std::map<std::string,std::string> Configrations;
};
#endif
