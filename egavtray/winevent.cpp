#include "winevent.h"
#include "SendMsgToService.h"

IMPLEMENT_CLASS(WinEventTestFrame, wxFrame)

WinEventTestFrame::WinEventTestFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
	m_bShutdownOrLogoffEvent = false;
}

WinEventTestFrame::~WinEventTestFrame()
{
}

WXLRESULT WinEventTestFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
	switch (message)
	{
	case WM_POWERBROADCAST:
		switch (wParam)
		{
		case PBT_APMSUSPEND:
		case PBT_APMRESUMESUSPEND:
		case PBT_APMRESUMEAUTOMATIC:
			SendMsgToService(SERVICE_CONTROL_CUSTOM_MSG_RESTART_IT);
			break;
			/*
		case PBT_APMSUSPEND:
			wxMessageBox("System is suspending operation...");
			break;
		case PBT_APMRESUMESUSPEND:
			wxMessageBox("Operation is resuming from a low-power state...");
			break;
		case PBT_APMRESUMEAUTOMATIC:
			wxMessageBox("Operation is resuming automatically from a low-power state...");
			break;
		case PBT_APMQUERYSUSPEND:
			wxMessageBox("Request permission to suspend system...");
			break;
		case PBT_APMQUERYSTANDBY:
			wxMessageBox("Request permission to standby system...");
			break;
		case PBT_APMQUERYSUSPENDFAILED:
			wxMessageBox("Permission to suspend system is denied...");
			break;
		case PBT_APMQUERYSTANDBYFAILED:
			wxMessageBox("Permission to standby system is denied...");
			break;
		case PBT_APMSTANDBY:
			wxMessageBox("System is going in standby operation...");
			break;
		case PBT_APMRESUMECRITICAL:
			wxMessageBox("System has resumed due to power failure...");
			break;

		case PBT_APMRESUMESTANDBY:
			wxMessageBox("System is going in standby operation...");
			break;
		case PBT_APMBATTERYLOW:
			wxMessageBox("Battery power is low...");
			break;
		case PBT_APMPOWERSTATUSCHANGE:
			wxMessageBox("Power status has changed...");
			break;
		case PBT_APMOEMEVENT:
			wxMessageBox("The APM BIOS has signaled an APM OEM event");
			break;
		case PBT_POWERSETTINGCHANGE:
			wxMessageBox("A power setting change event has been received...");
			break;
			*/
		}
		break;
	case WM_QUERYENDSESSION:
		if (lParam == 0)
		{
			//On Shutdown
			m_bShutdownOrLogoffEvent = true;
		}
		if ((lParam & ENDSESSION_LOGOFF) == ENDSESSION_LOGOFF)
		{
			// On Logoff user
			m_bShutdownOrLogoffEvent = true;
		}
		break;
	default:
		break;
	}
	
	return wxFrame::MSWWindowProc(message, wParam, lParam);
}