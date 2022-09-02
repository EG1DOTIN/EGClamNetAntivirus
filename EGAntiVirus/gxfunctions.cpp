#include "common.h"
#include "gxDelPointer.h"
#include "gxFunctions.h"
#include "wx/filename.h"
#include "wx/textfile.h"
#include <wx/fs_arc.h>
#include <wx/filesys.h>
#include <Shldisp.h>
#include <xmemory>
#include <vector>
#include <TlHelp32.h>
#include <Psapi.h>

//Remove Comment below macro to build for xp else remain comment
//#define BUILD_4XP


void gxGetLineFromTextFile(const wxString& textfilePath, size_t lineNo, wxString& LineStr)
{
	LineStr = wxEmptyString;
	wxTextFile* tempfile = new wxTextFile(textfilePath);
	tempfile->Open();
	if (tempfile->GetLineCount())
	{
		LineStr = tempfile->GetLine(lineNo);
	}
	tempfile->Close();
	DELETE_POINTER_WXTEXTFILE(tempfile);
}

void gxGetFirstLineFromTextFile(const wxString& textfilePath, wxString& LineStr)
{
	LineStr = wxEmptyString;
	wxTextFile* tempfile = new wxTextFile(textfilePath);
	tempfile->Open();
	if (tempfile->GetLineCount())
	{
		LineStr = tempfile->GetFirstLine();
	}
	tempfile->Close();
	DELETE_POINTER_WXTEXTFILE(tempfile);
}

void gxGetLastLineFromTextFile(const wxString& textfilePath, wxString& LineStr)
{
	LineStr = wxEmptyString;
	wxTextFile* tempfile = new wxTextFile(textfilePath);
	tempfile->Open();
	if (tempfile->GetLineCount())
	{
		LineStr = tempfile->GetLastLine();
	}
	tempfile->Close();
	DELETE_POINTER_WXTEXTFILE(tempfile);
}

size_t gxGetTotalLineInTextFile(const wxString & textfilePath)
{
	size_t nlines;
	wxTextFile* tempfile = new wxTextFile(textfilePath);
	if (!tempfile->Exists())
		tempfile->Create();
	tempfile->Open();
	nlines = tempfile->GetLineCount();
	tempfile->Close();
	DELETE_POINTER_WXTEXTFILE(tempfile);
	return nlines;
}


void gxAddLineInTextFile(const wxString& textfilePath, const wxString& lineStr)
{
	wxTextFile* tempfile = new wxTextFile(textfilePath);
	if (!tempfile->Exists())
		tempfile->Create();
	tempfile->Open();
	tempfile->AddLine(lineStr);
	tempfile->Write();
	tempfile->Close();
	DELETE_POINTER_WXTEXTFILE(tempfile);
}

void gxChangeLineInTextFile(const wxString& textfilePath, const wxString& LineStr, int LineNumber)
{
	wxTextFile* tempfile = new wxTextFile(textfilePath);
	tempfile->Open();
	tempfile->RemoveLine(LineNumber);
	tempfile->InsertLine(LineStr, LineNumber);
	tempfile->Write();
	tempfile->Close();
	DELETE_POINTER_WXTEXTFILE(tempfile);
}

void gxRemoveLineInTextFile(const wxString& textfilePath, size_t lineNo)
{
	wxTextFile* tempfile = new wxTextFile(textfilePath);
	tempfile->Open();
	tempfile->RemoveLine(lineNo);
	tempfile->Write();
	tempfile->Close();
	DELETE_POINTER_WXTEXTFILE(tempfile);
}

void gxRemoveAllLinesInTextFile(const wxString& textfilePath, bool bIsOneByOne)
{
	wxTextFile *temP = new wxTextFile(textfilePath);
	temP->Open();
	size_t count = temP->GetLineCount();
	if (count)
	{
		if (bIsOneByOne)
		{
			for (size_t i = count; i > 0; i--)
			{
				temP->RemoveLine(i - 1);
			}
		}
		else
		{
			temP->Clear();
		}

		temP->Write();
	}
	temP->Close();
	DELETE_POINTER_WXTEXTFILE(temP);
}

void gxMakeTextFile(const wxString& textfilePath)
{
	if ((!wxFile::Exists(textfilePath)))
	{
		wxTextFile* tempfile = new wxTextFile(textfilePath);
		tempfile->Create();
		DELETE_POINTER_WXTEXTFILE(tempfile);
	}
}

void gxTakeAllLinesOfTextFileInWXStr(const wxString& textfilePath, wxString& strout)
{
	if (wxFile::Exists(textfilePath))
	{
		wxTextFile* tempfile = new wxTextFile(textfilePath);
		tempfile->Open();
		size_t count = tempfile->GetLineCount();
		if (count)
		{
			for (size_t i = 0; i < count; i++)
			{
				strout.append(tempfile->GetLine(i));
				strout.append(wxT("\n"));
			}
		}
		tempfile->Close();
		DELETE_POINTER_WXTEXTFILE(tempfile);
	}
}

void MakeFolderIfNotExist(const wxString& FolderFullPath)
{
	//if AppData dir doesnot exists create it
	if (!wxDirExists(FolderFullPath))
	{
		wxFileName::Mkdir(FolderFullPath, 511, wxPATH_MKDIR_FULL);
	}
}

ULLONG gxGetFileSize(const wxString& filename)
{
	ULLONG res = 0;
	if (!wxFileExists(filename))
		return res;
	wxULongLong sizeoffile = wxFileName::GetSize(filename);
	if (sizeoffile != wxInvalidSize)
		res = sizeoffile.GetValue();
	else
	{
		wxStructStat structStat;
		wxStat(filename, &structStat);
		wxFileOffset filelen = structStat.st_size;
		//wxDateTime last_modifiedtime(structStat.st_mtime);
		//filelen probably 64bit int
		//wxLogDebug(wxT("%d  -  %s"), (unsigned long)filelen, last_modifiedtime.Format(wxT("%d-%b-%Y at %H:%M:%S")));
		res = (ULLONG)filelen;
	}
	return res;
}

void GetListOfAllDrivesString(wxString& ListAllDrive)
{
	wxChar driveBuffer[256];
	size_t n = (size_t)GetLogicalDriveStrings(255, driveBuffer);
	size_t i = 0;
	wxString dr;
	while (i < n)
	{
		dr.Printf("%c", driveBuffer[i]);
		ListAllDrive += dr + wxT(":\\ ");
		dr.Clear();

		while (driveBuffer[i] != wxT('\0'))
			i++;
		i++;
		if (driveBuffer[i] == wxT('\0'))
			break;
	}
}

ULLONG gxGetDirSize(const wxString& dirName)
{
	ULLONG res = 0;
	if (wxDir::Exists(dirName))
	{
		res = wxDir::GetTotalSize(dirName).GetValue();
		return res;
	}
	else
	{
		return res;
	}
}

BOOL gxDeleteDir(const wxString& dirFullPath, bool bIsRecursive)
{
	if (wxDir::Exists(dirFullPath))
	{
		if (bIsRecursive)
		{
			return (wxDir::Remove(dirFullPath, wxPATH_RMDIR_RECURSIVE));
		}
		else
		{
			return (wxDir::Remove(dirFullPath, wxPATH_RMDIR_FULL));
		}
	}
	else
	{
		return FALSE;
	}

}

BOOL gxMakeDirEmpty(wxString dirFullPath)
{
	if (!wxDir::Exists(dirFullPath))
	{
		wxLogError("Does not exist folder \"" + dirFullPath + "\"");
		return FALSE;
	}
	else
	{
		if (dirFullPath[dirFullPath.length() - 1] != wxFILE_SEP_PATH)
		{
			dirFullPath += wxFILE_SEP_PATH;
		}
		wxDir* dir = new wxDir(dirFullPath);
		if (dir == NULL)
		{
			wxLogError("Could not open folder \"" + dirFullPath + "\"");
			delete dir;
			dir = (wxDir*)(NULL);
			return FALSE;
		}
		wxString filename;
		bool cont = dir->GetFirst(&filename);
		if (cont)
		{
			do
			{
				if (wxDirExists(dirFullPath + filename))
				{
					if (!gxDeleteDir(dirFullPath + filename, true))
						wxLogError("Could not remove folder \"" + dirFullPath + filename + "\"");
				}
				else
				{
					if (!wxRemoveFile(dirFullPath + filename))
					{
						wxLogError("Could not remove file \"" + dirFullPath + filename + "\"");
					}
				}
			} while (dir->GetNext(&filename));

			/*
			if (!wxFileName::Rmdir(dirFullPath))
			{
			wxLogError("Could not remove directory " + dirFullPath);
			delete dir;
			dir = (wxDir*)(NULL);
			return FALSE;
			}
			else
			{
			delete dir;
			dir = (wxDir*)(NULL);
			return TRUE;
			}
			*/
		}
		delete dir;
		dir = (wxDir*)(NULL);
		return TRUE;
	}
}

void gxDataSizeMap(ULLONG sizeInByte, wxString& MappedByte)
{
	if (sizeInByte == 0)
	{
		MappedByte = wxT("0 byte");
		return;
	}

	if (sizeInByte == 1)
	{
		MappedByte = wxT("1 byte");
		return;
	}

	if ((sizeInByte > 1) && (sizeInByte < 1024))
	{
		MappedByte.Printf("%u", sizeInByte);
		MappedByte += wxT(" bytes");
		return;
	}

	// 1024 x 1024 = 1048576  
	if ((sizeInByte >= 1024) && (sizeInByte < 1048576))
	{
		double p = (sizeInByte *1.0) / 1024.0;
		MappedByte.Printf("%0.2f", p);
		MappedByte += wxT(" KB");
		return;
	}

	//1048576 x 1024 = 1073741824
	if ((sizeInByte >= 1048576) && (sizeInByte < 1073741824))
	{
		double p = (sizeInByte*1.0) / 1048576.0;
		MappedByte.Printf("%0.2f", p);
		MappedByte += wxT(" MB");
		return;
	}

	//1073741824 x 1024 = 1099511627776
	if ((sizeInByte >= 1073741824) && (sizeInByte < 1099511627776))
	{
		double p = (sizeInByte*1.0) / 1073741824.0;
		MappedByte.Printf("%0.2f", p);
		MappedByte += wxT(" GB");
		return;
	}

	// 1099511627776 x 1024 = 1125899906842624
	if ((sizeInByte >= 1099511627776) && (sizeInByte < 1125899906842624))
	{
		double p = (sizeInByte*1.0) / 1099511627776.0;
		MappedByte.Printf("%0.2f", p);
		MappedByte += wxT(" TB");
		return;
	}
	else
		MappedByte = wxT("Error reading size");
}

void gxGetFileLastModifiedTime(const wxString& filename, wxString& LastModifiedTime, bool bDeatiled)
{
	if (!wxFileExists(filename))
		return;

	wxStructStat structStat;
	wxStat(filename, &structStat);

	wxDateTime fileMtime = structStat.st_mtime;

	if (bDeatiled)
		LastModifiedTime = fileMtime.Format(wxT("%d-%b-%Y at %H:%M:%S"));
	else
		LastModifiedTime = fileMtime.Format(wxT("%d/%m/%Y"));
}