#include <CConfig.H>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>

std::map<std::string,std::string> CConfig::Configrations;

bool CConfig::Config_Init(std::string fileName)
{
	 
	FILE *pFile;
	pFile = fopen (fileName.c_str(), "r");
	if(pFile == NULL)
		return false;

	char line[200],*key,*value;
	while ( ! feof (pFile) )
	{
		if ( fgets (line , 200 , pFile) == NULL ) break;

		if(strlen(line) <= 1 || line[0] == '#' || (line[0] == '/' && line[1] == '/'))
			continue;

		key = strtok(line," =");
		value = strtok(NULL," =\n");
		if(value == NULL) continue;
		CConfig::Configrations[std::string(key)] = std::string(value);
		//printf("%s = %s\n", key, CConfig::Config_Get(key).c_str());
 	}
	return true;
}

std::string CConfig::Config_Get(std::string key)
{
	std::map<std::string,std::string> :: const_iterator value;
	value = CConfig::Configrations.find(key);
	if(value == CConfig::Configrations.end())
	{
		return NULL;
	}
	return value->second;
}
