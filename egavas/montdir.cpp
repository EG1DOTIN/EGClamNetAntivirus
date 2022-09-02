#include "common.h"
#include "montdir.h"
#include <wx\textfile.h>
#include <vector>

wxThread *gThreadMonitorDir1 = NULL;
wxThread *gThreadMonitorDir2 = NULL;

std::vector<wxString> paths1, paths2;

#define MAX_DIRS 25
#define MAX_FILES 255
#define MAX_BUFFER 4096

extern "C" {
	WINBASEAPI BOOL WINAPI
		ReadDirectoryChangesW(HANDLE hDirectory,
		LPVOID lpBuffer, DWORD nBufferLength,
		BOOL bWatchSubtree, DWORD dwNotifyFilter,
		LPDWORD lpBytesReturned,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
		);
}

// all purpose structure to contain directory information and provide
// the input buffer that is filled with file change data

typedef struct _DIRECTORY_INFO {
	HANDLE hDir;
	TCHAR lpszDirName[MAX_PATH];
	CHAR lpBuffer[MAX_BUFFER];
	DWORD dwBufLength;
	OVERLAPPED Overlapped;
}DIRECTORY_INFO, *PDIRECTORY_INFO, *LPDIRECTORY_INFO;

DIRECTORY_INFO DirInfo1[MAX_DIRS];   // Buffer for of the directory
DIRECTORY_INFO DirInfo2[MAX_DIRS];   // Buffer for of the directory

/////////////////////////////////////////////////////////////

bool GetExCludeOrNot(const wxString& strIn)
{
	wxString ext = strIn.AfterLast('.');
	wxString excludefiles[] =
	{
		wxT("jpg"), wxT("jpeg"), wxT("gif"), wxT("png"), wxT("tif"),
		wxT("tiff"), wxT("txt"), wxT("docx"), wxT("pdf"), wxT("mp3"),
		wxT("mp4"), wxT("wav"), wxT("avi"), wxT("mpg"), wxT("mpeg"),
		wxT("wmv"), wxT("vob"), wxT("log"), wxT("tmp")
	};
	size_t tnos = sizeof(excludefiles) / sizeof(excludefiles[0]);

	if (tnos)
	{
		for (size_t j = 0; j < tnos; j++)
		{
			if (ext == excludefiles[j])
				return true;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////
void* ThreadMonitorDir1::Entry()
{
	std::wstring AppDataDir;
	GetAppDataFolderPath(AppDataDir);
	LPCWSTR path = AppDataDir.c_str();
	SuccessAppConLogW(L"EGAVAS: AppData Dir Path: %s", path);

	char buf[2048];
	DWORD nRet;
	BOOL result = TRUE;
	char filename[MAX_PATH];
	DirInfo1[0].hDir = CreateFile(path, GENERIC_READ | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL);

	if (DirInfo1[0].hDir == INVALID_HANDLE_VALUE)
	{
		return (wxThread::ExitCode)0;  //cannot open folder
	}

	lstrcpy(DirInfo1[0].lpszDirName, path);
	OVERLAPPED PollingOverlap;

	FILE_NOTIFY_INFORMATION* pNotify;
	int offset;
	PollingOverlap.OffsetHigh = 0;
	PollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	while (result)
	{
		result = ReadDirectoryChangesW(
			DirInfo1[0].hDir,// handle to the directory to be watched
			&buf,// pointer to the buffer to receive the read results
			sizeof(buf),// length of lpBuffer
			TRUE,// flag for monitoring directory or directory tree
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_SIZE |
			//FILE_NOTIFY_CHANGE_LAST_WRITE |
			FILE_NOTIFY_CHANGE_LAST_ACCESS ,
			//FILE_NOTIFY_CHANGE_CREATION,
			&nRet,// number of bytes returned
			&PollingOverlap,// pointer to structure needed for overlapped I/O
			NULL);

		if (TestDestroy())
			break;

		WaitForSingleObject(PollingOverlap.hEvent, INFINITE);

		if (TestDestroy())
			break;

		offset = 0;
#ifndef NDEBUG
		_chdir("C:\\Program Files\\EG1\\EGClamNetAV");
#endif

		do
		{
			pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buf + offset);
			strcpy(filename, "");
			int filenamelen = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength / 2, filename, sizeof(filename), NULL, NULL);
			filename[pNotify->FileNameLength / 2] = L'';

			wxString cmd;
			wxString wxStrFileName = wxString(filename);
			switch (pNotify->Action)
			{
			case FILE_ACTION_ADDED:
				//printf("\nThe file is added to the directory: [%s] \n", filename);
				if (!GetExCludeOrNot(wxStrFileName))
				{
					if (paths1.size() >= 100)
					{
						paths1.clear();
					}

					if (std::find(paths1.begin(), paths1.end(), wxStrFileName) == paths1.end())
					{
						paths1.push_back(wxStrFileName);
						cmd = wxString(wxT("clamdscan.exe --move=\"")) + wxString(AppDataDir) + wxT("\\EGAntiVirus\\Vault\" \"") + wxString(path) + wxT("\\") + wxStrFileName + wxString("\"");
						system(cmd.c_str().AsChar());
					}
				}
				break;
			}
			offset += pNotify->NextEntryOffset;


		} while (pNotify->NextEntryOffset); //(offset != 0);
	}

	return (wxThread::ExitCode)0;     // success
}

wxThread *CreateThreadMonitorDir1()
{
	if (!gThreadMonitorDir1)
	{
		//Create thread
		gThreadMonitorDir1 = new ThreadMonitorDir1();

		//Start thread
		gThreadMonitorDir1->Create();
		gThreadMonitorDir1->Run();
		return gThreadMonitorDir1;
	}
	return ((wxThread *)(NULL));
}

void DeleteThreadMonitorDir1()
{
	if (gThreadMonitorDir1 != NULL)
	{
		//gThreadRenameFolder->Pause();
		//gThreadRenameFolder->Delete();
		gThreadMonitorDir1->Kill();
		CloseHandle(DirInfo1[0].hDir);
		DELETE_POINTER(wxThread, gThreadMonitorDir1);
	}
}

////////////////////////////////////////////////////////////
void* ThreadMonitorDir2::Entry()
{
	std::wstring AppDataDir, foldertomont;
	GetAppDataFolderPath(AppDataDir);
	GetUsersProfileDir(foldertomont);
	LPCWSTR path = foldertomont.c_str();
	SuccessAppConLogW(L"EGAVAS: Profile Dir Path: %s", path);

	char buf[2048];
	DWORD nRet;
	BOOL result = TRUE;
	char filename[MAX_PATH];
	DirInfo2[0].hDir = CreateFile(path, GENERIC_READ | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL);

	if (DirInfo2[0].hDir == INVALID_HANDLE_VALUE)
	{
		return (wxThread::ExitCode)0;  //cannot open folder
	}

	lstrcpy(DirInfo2[0].lpszDirName, path);
	OVERLAPPED PollingOverlap;

	FILE_NOTIFY_INFORMATION* pNotify;
	int offset;
	PollingOverlap.OffsetHigh = 0;
	PollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	while (result)
	{
		result = ReadDirectoryChangesW(
			DirInfo2[0].hDir,// handle to the directory to be watched
			&buf,// pointer to the buffer to receive the read results
			sizeof(buf),// length of lpBuffer
			TRUE,// flag for monitoring directory or directory tree
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_SIZE |
			//FILE_NOTIFY_CHANGE_LAST_WRITE |
			FILE_NOTIFY_CHANGE_LAST_ACCESS,
			//FILE_NOTIFY_CHANGE_CREATION,
			&nRet,// number of bytes returned
			&PollingOverlap,// pointer to structure needed for overlapped I/O
			NULL);

		if (TestDestroy())
			break;

		WaitForSingleObject(PollingOverlap.hEvent, INFINITE);

		if (TestDestroy())
			break;

		offset = 0;
#ifndef NDEBUG
		_chdir("C:\\Program Files\\EG1\\EGClamNetAV");
#endif
		do
		{
			pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buf + offset);
			strcpy(filename, "");
			int filenamelen = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength / 2, filename, sizeof(filename), NULL, NULL);
			filename[pNotify->FileNameLength / 2] = L'';

			wxString cmd;
			wxString wxStrFileName = wxString(filename);
			switch (pNotify->Action)
			{
			case FILE_ACTION_ADDED:
				//printf("\nThe file is added to the directory: [%s] \n", filename);
				//if ((wxStrFileName.Contains("\\AppData\\")) || (wxStrFileName.Contains("\\Desktop\\")) || (wxStrFileName.Contains("\\Downloads\\")))
				if(shouldMonitor(wxStrFileName))
				{
					if (!GetExCludeOrNot(wxStrFileName))
					{
						if (paths2.size() >= 100)
						{
							paths2.clear();
						}

						if (std::find(paths2.begin(), paths2.end(), wxStrFileName) == paths2.end())
						{
							paths2.push_back(wxStrFileName);
							cmd = wxString(wxT("clamdscan.exe --move=\"")) + wxString(AppDataDir) + wxT("\\EGAntiVirus\\Vault\" \"") + wxString(path) + wxT("\\") + wxStrFileName + wxString("\"");
							system(cmd.c_str().AsChar());
						}

					}
				}
				break;
			case FILE_NOTIFY_CHANGE_LAST_ACCESS:
				SuccessFunctionLogsW(L"File Accessed: ", wxStrFileName);
				break;
			}
			offset += pNotify->NextEntryOffset;

		} while (pNotify->NextEntryOffset); //(offset != 0);
	}

	return (wxThread::ExitCode)0;     // success
}

wxThread *CreateThreadMonitorDir2()
{
	if (!gThreadMonitorDir2)
	{
		//Create thread
		gThreadMonitorDir2 = new ThreadMonitorDir2();

		//Start thread
		gThreadMonitorDir2->Create();
		gThreadMonitorDir2->Run();
		return gThreadMonitorDir2;
	}
	return ((wxThread *)(NULL));
}

void DeleteThreadMonitorDir2()
{
	if (gThreadMonitorDir2 != NULL)
	{
		gThreadMonitorDir2->Kill();
		CloseHandle(DirInfo2[0].hDir);
		DELETE_POINTER(wxThread, gThreadMonitorDir2);
	}
}

bool shouldMonitor(const wxString& filepath)
{
	bool ret = false;
	std::wstring AppDataDir;
	GetAppDataFolderPath(AppDataDir);
	wxString dbFile6 = wxString(AppDataDir) + wxT("\\EGAntiVirus\\db\\db6");

	wxTextFile* temp = new wxTextFile(dbFile6);
	if (!(temp->Exists()))
	{
		ErrorAppWinLogW(L"Monitoring folder list file not found, please restart antivirus.");
		return ret;
	}
	temp->Open();
	for (wxString line = temp->GetFirstLine(); !temp->Eof(); line = temp->GetNextLine())
	{
		if (filepath.Contains(line))
		{
			ret = true;
			break;
		}
	}
	temp->Close();
	DELETE_POINTER(wxTextFile, temp);
	return ret;
}
