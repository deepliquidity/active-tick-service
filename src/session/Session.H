#pragma once
#include <ActiveTickServerAPI/ActiveTickServerAPI.h>
#include <string>
#include <functional>

#define _countof(array) (sizeof(array)/sizeof(array[0]))

class APISession
{
	public:
		APISession(void);
		typedef std::function<void()>  CommunicationCB;
		virtual ~APISession(void);
		uint64_t GetSessionHandle() const { return m_hSession; }
		bool Init(const ATGUID& apiUserid, const std::string& serverIpAddress, uint32_t serverPort, const wchar16_t* userid, const wchar16_t* password);
		bool Destroy();
		void startLogin();
		void close();
		void setCommunicationCB(CommunicationCB);

	private:
		static void ATSessionStatusChangeCallback(uint64_t hSession, ATSessionStatusType statusType);
		static void ATLoginResponseCallback(uint64_t hSession, uint64_t hRequest, LPATLOGIN_RESPONSE pResponse);
		static void ATServerTimeUpdateCallback(LPATTIME pServerTime);
		static void ATRequestTimeoutCallback(uint64_t hOrigRequest);	

	private:
		uint64_t		m_hSession;
		uint64_t		m_hLastRequest;
		wchar16_t		m_userid[50];
		wchar16_t		m_password[50];
		static APISession*	s_pInstance;
		CommunicationCB		_communicationCB;
};
