#ifndef ServerSocket_H
#define ServerSocket_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <pthread.h>
#include <functional>
#include <session/Session.H>

#define LISTENING_DEPTH 5

class ActiveTickService
{
	public:
		typedef std::function<void(const char*,uint16_t)> InputCB;
		typedef std::function<void()>			  LoginCB;
		typedef std::function<void()>                     CloseCB;

		void open_listener(std::string host,std::string port);
		void startConnection();
		void handleConnection();
		void threadFunction(int);
		void setInputCB(InputCB);
		void setLoginCB(LoginCB);
		void setCloseCB(CloseCB);
		void sendMessage(const char *ptr_,uint16_t size);
		void setSessionPtr(APISession*);
		
	private:
		int server_socket,client_socket;
		InputCB _inputCB;
		LoginCB	_loginCB;
		CloseCB _closeCB;
};

#endif
