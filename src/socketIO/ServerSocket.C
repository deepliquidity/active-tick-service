#include "ServerSocket.H"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <thread>
#include <sys/syscall.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <util/Util.H>

void ActiveTickService::open_listener(std::string host,std::string port)
{
	int err;
	socklen_t len;
	struct addrinfo *aptr, hints;
	struct sockaddr_in output;

	/* get structure for local interface listener is to use */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if  ((err = getaddrinfo(host.data(), port.data(), &hints, &aptr)))
	{
		fprintf(stderr, "%s: %s port %s\n", host.data(), port.data(), gai_strerror(err));
		return;
	}

	/* open an internet tcp socket for listener to listen on */
	if ((server_socket = socket(aptr->ai_family, aptr->ai_socktype, 0)) < 0)
	{
		perror("openlistener socket");
		freeaddrinfo(aptr);
		return;
	}
	/* bind this socket to the listener's Internet address */
	if (bind(server_socket, aptr->ai_addr, aptr->ai_addrlen) < 0)
	{
		perror("openlistener bind");
		close(server_socket);
		freeaddrinfo(aptr);
		return;
	}

	/* set up listening backlog for connect requests from clients */
	if (listen(server_socket, LISTENING_DEPTH) < 0) \
	{
		perror("openlistener listen");
		close(server_socket);
		freeaddrinfo(aptr);
		return;
	}
}

void ActiveTickService::startConnection()
{
	struct sockaddr_in cli_addr;
	int newsockfd;
	int clilen = sizeof(cli_addr);
	char client_host[INET_ADDRSTRLEN];
	int client_port;
	pthread_t *iter;

	while (1)
	{
		newsockfd = accept(server_socket, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen);
		if (newsockfd < 0)
			perror("ERROR on accept");
		else
		{
			inet_ntop(AF_INET, &(cli_addr.sin_addr), client_host, INET_ADDRSTRLEN);
			client_port = ntohs(cli_addr.sin_port);
			printf("connected to client. Fd: %d\nClient address: %s:%d\n",newsockfd,client_host,client_port);
			client_socket = newsockfd;
			_loginCB();
		}
	}
	close(server_socket);
}

void ActiveTickService::handleConnection()
{
	std::thread serverThread(&ActiveTickService::threadFunction,this,client_socket);

	serverThread.join();
}

void ActiveTickService::threadFunction(int fd)
{
	uint16_t length;

	while(1)
	{
		char *rawmessage = Util::readRawData(fd,2);
		if(rawmessage == NULL)
		{
			printf("Client %d has stopped the communication\n",fd);
			_closeCB();
			return;
		}
		memcpy(&length,rawmessage,2);
		rawmessage = Util::readRawData(fd,length);
		_inputCB(rawmessage,length);
	}
}

void ActiveTickService::setInputCB(InputCB inputCB_)
{
	_inputCB = inputCB_;
}

void ActiveTickService::sendMessage(const char *ptr_,uint16_t size_)
{
	write(client_socket,ptr_,size_);
}

void ActiveTickService::setLoginCB(LoginCB loginCB_)
{
	_loginCB = loginCB_;
}

void ActiveTickService::setCloseCB(CloseCB closeCB_)
{
        _closeCB = closeCB_;
}
