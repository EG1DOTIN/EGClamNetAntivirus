#include "common.h"
#include "gxDelPointer.h"
#include "gxfunctions.h"
#include "egavsetgetdb.h"
#include "avdb.h"
#include "wx\textfile.h"

extern wxString dbFile1;  //For antivirus settings
extern wxString dbFile2;  //For antivirus settings exclude list 
extern wxString dbFile3;  //For antivirus settings include list

void MakeDefaultAVDBIfNotExist()
{
	std::string tempstdstr;
	if (!(wxFile::Exists(dbFile1)))
	{
		wxTextFile* tempfile = new wxTextFile(dbFile1);
		tempfile->Create();
		tempfile->Open();
		tempfile->AddLine(AVDB_RB3);
		tempfile->AddLine(AVDB_TR);
		Base64EncodeN(L"30", tempstdstr);
		tempfile->AddLine(wxString(tempstdstr));
		tempstdstr.clear();
		tempfile->AddLine(AVDB_TR);
		Base64EncodeN(L"50", tempstdstr);
		tempfile->AddLine(wxString(tempstdstr));
		tempstdstr.clear();
		Base64EncodeN(L"100", tempstdstr);
		tempfile->AddLine(wxString(tempstdstr));
		tempstdstr.clear();
		Base64EncodeN(L"10", tempstdstr);
		tempfile->AddLine(wxString(tempstdstr));
		tempstdstr.clear();
		tempfile->AddLine(AVDB_TR);
		tempfile->AddLine(AVDB_TR);
		tempfile->AddLine(AVDB_FL);
		tempfile->Write();
		tempfile->Close();
		DELETE_POINTER_WXTEXTFILE(tempfile);
		SetDefaultDBForUpdateAndLiveShieldInDB();
	}
	if (!(wxFile::Exists(dbFile2)))
		MakeDefaultExcludeListFile(dbFile2);
	gxMakeTextFile(dbFile3);
}


void MakeDefaultExcludeListFile(const wxString& dbFileFullPath)
{
	wxString excludefiles[] =
	{
		wxT("jpg"), wxT("jpeg"), wxT("gif"), wxT("png"), wxT("tif"),wxT("tiff"), wxT("txt"), wxT("docx"), wxT("pptx"), wxT("xlsx"),
		wxT("pdf"), wxT("mp3"), wxT("mp4"), wxT("wav"), wxT("avi"), wxT("mpg"), wxT("mpeg"), wxT("wmv"), wxT("vob"), wxT("3gp"), 
		wxT("webm"), wxT("flv"), wxT("mkv"), wxT("vob"), wxT("ogv"), wxT("ogg"), wxT("gifv"), wxT("mng"), wxT("qt"), wxT("yuv"), 
		wxT("amv"), wxT("m4p"), wxT("m4v"), wxT("mp2"), wxT("mpe"), wxT("mpv"), wxT("m2v"), wxT("3g2"), wxT("f4v"), wxT("f4p"), 
		wxT("f4a"), wxT("f4b"), wxT("dbx"), wxT("tbb"), wxT("pst"), wxT("dat"), wxT("log"), wxT("nsf"), wxT("ntf"),
	};
	size_t tnos = sizeof(excludefiles) / sizeof(excludefiles[0]);

	wxTextFile* temp = new wxTextFile(dbFileFullPath);
	if (!(temp->Exists()))
		temp->Create();
	temp->Open();
	temp->Clear();

	for (size_t i = 0; i < tnos; i++)
	{
		temp->AddLine(excludefiles[i]);
	}

	temp->Write();
	temp->Close();
	DELETE_POINTER_WXTEXTFILE(temp);
	
}


void MakeDefaultIncludeListFile(const wxString& dbFileFullPath)
{
	wxString includefiles[] =
	{
		wxT("VB"), wxT("JS"), wxT("WS"), wxT("7Z"),

		wxT("ADE"), wxT("ADP"), wxT("APP"), wxT("ASA"), wxT("ASP"), wxT("BAS"),
		wxT("BAT"), wxT("CAB"), wxT("CER"), wxT("CHM"), wxT("COM"), wxT("CPL"),
		wxT("CMD"), wxT("CRT"), wxT("CSH"), wxT("DOC"), wxT("DLL"), wxT("EXE"),
		wxT("FXP"), wxT("HLP"), wxT("HTA"), wxT("HTM"), wxT("HTR"), wxT("INF"),
		wxT("INS"), wxT("ISP"), wxT("ITS"), wxT("JAR"), wxT("JSE"), wxT("KSH"),
		wxT("LNK"), wxT("MAD"), wxT("MAF"), wxT("MAG"), wxT("MAM"), wxT("MAQ"),
		wxT("MAR"), wxT("MAS"), wxT("MAT"), wxT("MAU"), wxT("MAV"), wxT("MAW"),
		wxT("MDA"), wxT("MDB"), wxT("MDE"), wxT("MDT"), wxT("MDW"), wxT("MDZ"),
		wxT("MHT"), wxT("MSC"), wxT("MSH"), wxT("MSI"), wxT("MSP"), wxT("MST"),
		wxT("OCX"), wxT("OPS"), wxT("PCD"), wxT("PIF"), wxT("PPT"), wxT("PRF"),
		wxT("PRG"), wxT("PS1"), wxT("PS2"), wxT("RAR"), wxT("REG"), wxT("SCT"),
		wxT("SCF"), wxT("SHB"), wxT("SHS"), wxT("SCR"), wxT("SRC"), wxT("SYS"),
		wxT("TMP"), wxT("URL"), wxT("VBE"), wxT("VBS"), wxT("VBX"), wxT("VSS"),
		wxT("VST"), wxT("VSW"), wxT("WSF"), wxT("WSC"), wxT("WSH"), wxT("XLS"),
		wxT("ZIP"),

		wxT("PSC1"), wxT("PSC2"), wxT("MSH1"), wxT("MSH2"), wxT("DOCM"), wxT("DOTM"),
		wxT("XLSM"), wxT("XLTM"), wxT("XLAM"), wxT("PPTM"), wxT("POTM"), wxT("PPAM"),
		wxT("PPSM"), wxT("SLDM"), wxT("HTML"), wxT("MHTM"),

		wxT("MHTML"), wxT("VSMACROS"), wxT("MSHXML"), wxT("MSH1XML"), wxT("MSH2XML"),
		wxT("PS1XML"), wxT("PS2XML"), wxT("GADGET"), wxT("APPLICATION"),

	};
	size_t tnos = sizeof(includefiles) / sizeof(includefiles[0]);

	wxTextFile* temp = new wxTextFile(dbFileFullPath);
	if (!(temp->Exists()))
		temp->Create();
	temp->Open();
	temp->Clear();
	for (size_t i = 0; i < tnos; i++)
	{
		temp->AddLine(includefiles[i]);
	}

	temp->Write();
	temp->Close();
	DELETE_POINTER_WXTEXTFILE(temp);
}