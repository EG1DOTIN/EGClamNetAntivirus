#ifndef DBCMD_HEADER
#define DBCMD_HEADER

#include <wx\wx.h>

void GetRemoveOrQuarantineStr(wxString& strOut);
void GetInExCludeStrFromDB(wxString& strOut);
void GetMaxInfoStrFromDB(wxString& strOut);
void GetQuickScanIncludeOption(wxString& strOut);
void GetQuickScanMaxInfoOption(wxString& strOut);
void GetScanCmd(wxString& strOut);
void GetFastScanCmd(wxString& strOut);
void GetFullScanCmd(wxString& strOut);
void GetQuickScanCmd(wxString& strOut);
void GetMemoryScanCmd(wxString& strOut);
#endif