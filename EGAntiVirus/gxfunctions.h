#ifndef gxFUNCTION_HEADER
#define gxFUNCTION_HEADER

#include <wx\wx.h>
#include <wx\dir.h>
#include <wx\wfstream.h>
#include <wx\zipstrm.h>


void gxGetLineFromTextFile(const wxString& textfilePath, size_t lineNo, wxString& LineStr);
void gxGetFirstLineFromTextFile(const wxString& textfilePath, wxString& LineStr);
void gxGetLastLineFromTextFile(const wxString& textfilePath, wxString& LineStr);

size_t gxGetTotalLineInTextFile(const wxString& textfilePath);

void gxAddLineInTextFile(const wxString& textfilePath, const wxString& lineStr);
void gxChangeLineInTextFile(const wxString& textfilePath, const wxString& LineStr, int LineNumber);
void gxRemoveLineInTextFile(const wxString& textfilePath, size_t lineNo);
void gxRemoveAllLinesInTextFile(const wxString& textfilePath, bool bIsOneByOne = false);
void gxMakeTextFile(const wxString& textfilePath);
void gxTakeAllLinesOfTextFileInWXStr(const wxString& textfilePath, wxString& strout);

void MakeFolderIfNotExist(const wxString& FolderFullPath);

typedef unsigned long long  ULLONG;

ULLONG gxGetFileSize(const wxString& filename);

void GetListOfAllDrivesString(wxString& ListAllDrive);
ULLONG gxGetDirSize(const wxString& dirName);
BOOL gxDeleteDir(const wxString& dirFullPath, bool bIsRecursive);
BOOL gxMakeDirEmpty(wxString dirFullPath);
void gxDataSizeMap(ULLONG sizeInByte, wxString& MappedByte);
void gxGetFileLastModifiedTime(const wxString& filename, wxString& LastModifiedTime, bool bDeatiled = true);

#endif
