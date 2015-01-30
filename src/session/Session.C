#include <util/Util.H>
#include <Session.H>
#include <stdio.h>
#include <util/CConfig.H>

APISession* APISession::s_pInstance = NULL;
APISession::APISession(void)
: m_hSession(0)
, m_hLastRequest(0)
{
	m_hSession = ATCreateSession();
	s_pInstance = this;
}

APISession::~APISession(void)
{
	ATDestroySession(m_hSession);
	s_pInstance = NULL;
}

bool APISession::Init(const ATGUID& apiUserid, const std::string& serverIpAddress, uint32_t serverPort, const wchar16_t* userid, const wchar16_t* password)
{
	if(Util::StringLength(userid) >= _countof(m_userid) || Util::StringLength(password) >= _countof(m_password))
		return false;

	Destroy();

	Util::CopyString(m_userid, userid);
	Util::CopyString(m_password, password);

	bool rc = ATSetAPIUserId(m_hSession, (LPATGUID)&apiUserid);

	if(rc == true)
		rc = ATInitSession(m_hSession, serverIpAddress.c_str(), serverIpAddress.c_str(), serverPort, ATSessionStatusChangeCallback, true);
	return false;

	return rc;
}

bool APISession::Destroy()
{
	ATShutdownSession(m_hSession);
	return true;
}

void APISession::ATSessionStatusChangeCallback(uint64_t hSession, ATSessionStatusType statusType)
{
	std::string strStatusType;
	switch(statusType)
	{
	case SessionStatusConnected: strStatusType = "SessionStatusConnected"; break;
	case SessionStatusDisconnected: strStatusType = "SessionStatusDisconnected"; break;
	case SessionStatusDisconnectedDuplicateLogin: strStatusType = "SessionStatusDisconnectedDuplicateLogin"; break;
	default: break;
	}

	printf("RECV Status change [%s]\n", strStatusType.c_str());

	if(statusType == SessionStatusConnected)
	{
		APISession::s_pInstance->m_hLastRequest = ATCreateLoginRequest(hSession, APISession::s_pInstance->m_userid, APISession::s_pInstance->m_password, APISession::ATLoginResponseCallback);
		bool rc = ATSendRequest(hSession, APISession::s_pInstance->m_hLastRequest, DEFAULT_REQUEST_TIMEOUT, APISession::ATRequestTimeoutCallback);

		printf("SEND (%llu): Login request [%s] (rc=%d)\n", APISession::s_pInstance->m_hLastRequest, Util::ConvertString(APISession::s_pInstance->m_userid, 
			Util::StringLength(APISession::s_pInstance->m_userid)).c_str(), rc);
	}
}

void APISession::ATLoginResponseCallback(uint64_t hSession, uint64_t hRequest, LPATLOGIN_RESPONSE pResponse)
{
	std::string strLoginResponseType;
	bool success = false;
	switch(pResponse->loginResponse)
	{
	case LoginResponseSuccess: strLoginResponseType = "LoginResponseSuccess"; success = true;break;
	case LoginResponseInvalidUserid: strLoginResponseType = "LoginResponseInvalidUserid"; break;
	case LoginResponseInvalidPassword: strLoginResponseType = "LoginResponseInvalidPassword"; break;
	case LoginResponseInvalidRequest: strLoginResponseType = "LoginResponseInvalidRequest"; break;
	case LoginResponseLoginDenied: strLoginResponseType = "LoginResponseLoginDenied"; break;
	case LoginResponseServerError: strLoginResponseType = "LoginResponseServerError"; break;
	default: strLoginResponseType = "unknown"; break;
	}

	printf("RECV (%llu): Login response [%s]\n", hRequest, strLoginResponseType.c_str());
	if(success)
		APISession::s_pInstance->_communicationCB();
}

void APISession::ATServerTimeUpdateCallback(LPATTIME pServerTime)
{
	printf("RECV: Server time update [%0.2d:%0.2d:%0.2d %0.2d/%0.2d/%0.4d\n",
		pServerTime->hour, pServerTime->minute, pServerTime->second,
		pServerTime->month, pServerTime->day, pServerTime->year);
}

void APISession::ATRequestTimeoutCallback(uint64_t hOrigRequest)
{
	printf("(%I64u): Request timed-out\n", hOrigRequest);
}

void APISession::startLogin()
{
        uint32_t serverPort = Util::FromString(CConfig::Config_Get("serverport"));
        ATGUID guidApiUserid = Util::StringToATGuid(CConfig::Config_Get("key"));
        bool rc = this->Init(guidApiUserid, CConfig::Config_Get("serverip"), serverPort, &Util::ConvertString(CConfig::Config_Get("username")).front(), &Util::ConvertString(CConfig::Config_Get("userpassword")).front());
}

void APISession::close()
{
}

void APISession::setCommunicationCB(CommunicationCB communicationCB_)
{
	_communicationCB = communicationCB_;
}
