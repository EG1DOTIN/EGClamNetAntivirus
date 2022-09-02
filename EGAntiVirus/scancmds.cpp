#include "common.h"
#include "gxDelPointer.h"
#include "gxfunctions.h"
#include <wx\textfile.h>
#include "avdb.h"
#include "scancmds.h"

extern wxString workingDir;
extern wxString QuaranitineFolder;
extern wxString dbFile1;  //For antivirus settings
extern wxString dbFile2;  //For antivirus settings exclude list 
extern wxString dbFile3;  //For antivirus settings include list

const wxString ScannerExeName = wxT("clamscan.exe");

void GetRemoveOrQuarantineStr(wxString& strOut)
{
	MakeFolderIfNotExist(QuaranitineFolder);
	wxString res;
	gxGetFirstLineFromTextFile(dbFile1, res);
	if (res == AVDB_RB1)
	{
		res = wxT("");
	}
	if (res == AVDB_RB2)
	{
		res = wxT("--remove=yes");
	}
	if (res == AVDB_RB3)
	{
		res = wxT("--move=\"") + QuaranitineFolder + wxT("\"");
	}
	strOut = res;
}

void GetInExCludeStrFromDB(wxString& strOut)
{
	wxString inexcludeCMD(wxT(""));
	wxTextFile* tempfileIn = new wxTextFile(dbFile3);
	tempfileIn->Open();
	size_t tnol = tempfileIn->GetLineCount();
	if (tnol)
	{
		for (size_t i = 0; i < tnol; i++)
		{
			inexcludeCMD = inexcludeCMD + wxT("--include=(^.*\\") + tempfileIn->GetLine(i) + wxT(")$ ");
		}
	}
	else
	{
		wxTextFile* tempfileEx = new wxTextFile(dbFile2);
		tempfileEx->Open();
		tnol = tempfileEx->GetLineCount();
		if (tnol)
		{
			for (size_t j = 0; j < tnol; j++)
			{
				inexcludeCMD = inexcludeCMD + wxT("--exclude=(^.*\\") + tempfileEx->GetLine(j) + wxT(")$ ");
			}
		}
		wxString ExcludeCommonOption = wxT("infected");
		inexcludeCMD = inexcludeCMD + wxT("--exclude=(^.*\\") + ExcludeCommonOption + wxT(")$ ");
		tempfileEx->Close();
		DELETE_POINTER_WXTEXTFILE(tempfileEx);
	}
	tempfileIn->Close();
	DELETE_POINTER_WXTEXTFILE(tempfileIn);
	strOut = inexcludeCMD;
}

void GetQuickScanIncludeOption(wxString& strOut)
{
	wxString includefiles[] =
	{ 
		wxT(".EXE"), wxT(".DLL"), wxT(".SRC"),
		wxT(".SYS"), wxT(".MSI"), wxT(".REG"),
		wxT(".CAB"), wxT(".JAR"), wxT(".VBS"),
		wxT(".COM"), wxT(".BAT"), wxT(".SHS"),
		wxT(".PIF"), wxT(".SCR"), wxT(".DOC")
	};
	size_t tnos = sizeof(includefiles) / sizeof(includefiles[0]);
	wxString res(wxT(""));

	for (size_t i = 0; i < tnos; i++)
	{
		res += wxT("--include=(^.*\\") + includefiles[i] + wxT(")$ ");
	}
	strOut = res;
}

void GetMaxInfoStrFromDB(wxString& strOut)
{
	wxString res;
	std::wstring tempstdwstr;
	wxTextFile* tempfile = new wxTextFile(dbFile1);
	tempfile->Open();
	Base64DecodeN((tempfile->GetLine(2)).c_str().AsChar(), tempstdwstr);
	wxString s1(tempstdwstr);
	tempstdwstr.clear();
	Base64DecodeN((tempfile->GetLine(4)).c_str().AsChar(), tempstdwstr);
	wxString s2(tempstdwstr);
	tempstdwstr.clear();
	Base64DecodeN((tempfile->GetLine(5)).c_str().AsChar(), tempstdwstr);
	wxString s3(tempstdwstr);
	tempstdwstr.clear();
	Base64DecodeN((tempfile->GetLine(6)).c_str().AsChar(), tempstdwstr);
	wxString s4(tempstdwstr);

	if ((tempfile->GetLine(3)) == AVDB_FL)
		res = wxT("--max-filesize=") + s1 + wxT("M ");
	else
		res = wxT("--max-filesize=") + s1 + wxT("M --max-scansize=") + s2 + wxT("M --max-files=") + s3 + wxT(" --max-recursion=") + s4 + wxT(" ");
	tempfile->Close();
	DELETE_POINTER_WXTEXTFILE(tempfile);
	strOut = res;
}

void GetQuickScanMaxInfoOption(wxString& strOut)
{
	strOut = wxT("--max-filesize=5M ");
}

void GetScanCmd(wxString& strOut)
{
	wxString RemoveOrQuarantineStr;
	wxString InExCludeStrFromDB;
	wxString MaxInfoStrFromDB;
	GetRemoveOrQuarantineStr(RemoveOrQuarantineStr);
	GetInExCludeStrFromDB(InExCludeStrFromDB);
	GetMaxInfoStrFromDB(MaxInfoStrFromDB);


	strOut = ScannerExeName + wxT(" --verbose --archive-verbose --infected --bell --recursive=yes ")
		+ RemoveOrQuarantineStr + wxT(" ") + InExCludeStrFromDB + MaxInfoStrFromDB;
}

void GetFastScanCmd(wxString& strOut)
{
	//wxString RemoveOrQuarantineStr;
	//wxString InExCludeStrFromDB;
	//wxString MaxInfoStrFromDB;
	//GetRemoveOrQuarantineStr(RemoveOrQuarantineStr);

//	strOut = wxString("clamdscan.exe") + wxString(" --verbose --infected ") + RemoveOrQuarantineStr + wxString(" ");
	strOut = wxString("egcnextras.exe") + wxString(" -a 3") + wxString(" ");
}


void GetFullScanCmd(wxString& strOut)
{
	wxString scancmd;
	wxString RemoveOrQuarantineStr;
	GetRemoveOrQuarantineStr(RemoveOrQuarantineStr);
	wxString InExCludeStrFromDB;
	GetInExCludeStrFromDB(InExCludeStrFromDB);
	wxString MaxInfoStrFromDB;
	GetMaxInfoStrFromDB(MaxInfoStrFromDB);

	strOut = ScannerExeName + wxT(" --verbose --archive-verbose --infected --bell --recursive=yes ") + 
		RemoveOrQuarantineStr + wxT(" ") + InExCludeStrFromDB + MaxInfoStrFromDB;
}

void GetQuickScanCmd(wxString& strOut)
{
	wxString RemoveOrQuarantineStr;

	GetRemoveOrQuarantineStr(RemoveOrQuarantineStr);
	wxString QuickScanIncludeOption;
	GetQuickScanIncludeOption(QuickScanIncludeOption);

	strOut = ScannerExeName + wxT(" --verbose --archive-verbose --infected --bell --recursive=yes ")
		+ RemoveOrQuarantineStr + wxT(" ") + QuickScanIncludeOption + wxT("--max-filesize=4M ");
}

void GetMemoryScanCmd(wxString& strOut)
{
	wxString uImFm;
	wxString RemoveOrQuarantineStr;
	GetRemoveOrQuarantineStr(RemoveOrQuarantineStr);

	gxGetLineFromTextFile(dbFile1, 1, uImFm);

	if (uImFm == AVDB_TR)
	{
		strOut = ScannerExeName + wxT(" --verbose --infected --bell ")
			+ RemoveOrQuarantineStr + wxT(" --memory --kill --unload --max-filesize=4M ");
	}
	else
	{
		strOut = ScannerExeName + wxT(" --verbose --infected --bell ")
			+ RemoveOrQuarantineStr + wxT(" --memory --kill --max-filesize=4M ");
	}
}