#ifndef MONITOR_THREAD_HEADER
#define MONITOR_THREAD_HEADER

#include <wx/wx.h>

/////////////////////////////////////////////////////////////////////
//Thread for lock target folder 1
class ThreadMonitorDir1 : public wxThread
{
public:
	ThreadMonitorDir1() : wxThread(wxTHREAD_JOINABLE)
	{

	}
private:
	ThreadMonitorDir1(const ThreadMonitorDir1 &copy);

public:
	virtual void *Entry();

};

wxThread *CreateThreadMonitorDir1();

void DeleteThreadMonitorDir1();

/////////////////////////////////////////////////////////////////////
//Thread for lock target folder 2
class ThreadMonitorDir2 : public wxThread
{
public:
	ThreadMonitorDir2() : wxThread(wxTHREAD_JOINABLE)
	{

	}
private:
	ThreadMonitorDir2(const ThreadMonitorDir2 &copy);

public:
	virtual void *Entry();

};

wxThread *CreateThreadMonitorDir2();

void DeleteThreadMonitorDir2();

bool shouldMonitor(const wxString& filepath);
/////////////////////////////////////////////////////////////////////
#endif