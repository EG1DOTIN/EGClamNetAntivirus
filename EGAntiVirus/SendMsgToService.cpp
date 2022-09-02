#include "common.h"
#include "SendMsgToService.h"

int SendMsgToService(unsigned int msg)
{
	SC_HANDLE hMyService, hSCM;
	BOOL bSuccess;
	SERVICE_STATUS status;
	hSCM = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (!hSCM)
	{
		std::string errstr;
		GetLastErrorAsString(GetLastError(), errstr);
		ErrorAppTrayLogW(L"Open SCM failed with error %S",errstr.c_str());
	}
	hMyService = OpenService(hSCM, SERVICE_NAME, SERVICE_USER_DEFINED_CONTROL);
	if (!hMyService)
	{
		std::string errstr;
		GetLastErrorAsString(GetLastError(), errstr);
		ErrorAppTrayLogW(L"Open SCM failed with error %S", errstr.c_str());
	}
	bSuccess = ControlService(hMyService, msg, &status);
	if (!bSuccess)
	{
		std::string errstr;
		GetLastErrorAsString(GetLastError(), errstr);
		ErrorAppTrayLogW(L"Control Service failed with error %S", errstr.c_str());
	}
	CloseServiceHandle(hMyService);
	CloseServiceHandle(hSCM);
	return 0;
}