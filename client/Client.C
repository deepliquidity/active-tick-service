#include "Client.H"
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/syscall.h>


char* Client::readRawData(int sockfd, int len)
{
	char *data = (char*)malloc(len * sizeof(char));
	char *ptr = data;
	int n = 0;

	while(n < len)
	{
		n = read(sockfd, data, len);
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


bool Client::open_connection(char *host,char *port)
{
	int n = 0;
	struct sockaddr_in serv_addr;

	if((_sockFd = socket(AF_INET, SOCK_STREAM, 0))< 0)
	{
		fprintf(stderr,"Error : Could not create socket\n");
		return false;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(port));
	serv_addr.sin_addr.s_addr = inet_addr(host);

	//printf("%s %s\n",host,port);
	if(connect(_sockFd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
	{
		fprintf(stderr,"\n Error : Connect Failed \n");
		return false;
	}
	return true;
}

void Client::sendMessages(const char *data_,int size_)
{
	write(_sockFd,data_,size_);
}

void Client::recvMessage(char *&ptr_,uint16_t &size_)
{
	uint16_t length;

	char *rawmessage = readRawData(_sockFd,2);
	memcpy(&length,rawmessage,2);
	printf("Length %d\n",length);
	rawmessage = readRawData(_sockFd,length);
	ptr_ = rawmessage;
	size_ = length;
}
