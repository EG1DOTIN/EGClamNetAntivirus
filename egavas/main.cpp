#include "common.h"
#include "montdir.h"

int main()
{
	wxInitialize();

	for (;;)
	{
#ifdef NDEBUG
		if (!IsServiceRunningW(L"WinSEGAV AutoConfig"))
#else
		if (GetAsyncKeyState('Q'))
#endif
		{
			break;
		}

		CreateThreadMonitorDir1();
		CreateThreadMonitorDir2();

		wxMilliSleep(100);

	}

	DeleteThreadMonitorDir1();
	DeleteThreadMonitorDir2();

	wxUninitialize();

	return 0;
}