#ifndef CLIENT_H
#define CLIENT_H
#include <inttypes.h>

class Client
{
	public:
		bool open_connection(char *host,char *port);
		void sendMessages(const char*,int);
		void recvMessage(char *&,uint16_t&);
		char* readRawData(int sockfd, int len);
	private:
		int _sockFd;
};

#endif
