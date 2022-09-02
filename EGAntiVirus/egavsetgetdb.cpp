#include <iostream>
#include "common.h"
#include "egavsetgetdb.h"
#include "gxfunctions.h"
#include "avdb.h"
#include "uniqid.h"

extern wxString dbFile1;  //For antivirus settings
extern wxString dbFile5;  //For machineID	

bool SetEGAVDBForAutoUpdateOnOff(bool bTRUEFALSE)
{
	wxString bval = bTRUEFALSE ? AVDB_TR : AVDB_FL;
	// line number 8 is for Automatic Updates On/Off in db settings file.
	gxChangeLineInTextFile(dbFile1, bval, 8);
	return true;
}

bool GetEGAVDBForAutoUpdateOnOff()
{
	wxString bval;
	gxGetLineFromTextFile(dbFile1, 8, bval);
	return (bval==AVDB_TR);
}

bool SetEGAVDBForLiveShieldOnOff(bool bTRUEFALSE)
{
	wxString bval = bTRUEFALSE ? AVDB_TR : AVDB_FL;
	// line number 7 is for Real-Time Protection On/Off in db settings file.
	gxChangeLineInTextFile(dbFile1, bval, 7);
	return true;
}

void SetDefaultDBForUpdateAndLiveShieldInDB()
{
	SetEGAVDBForAutoUpdateOnOff(true);
	SetEGAVDBForLiveShieldOnOff(true);
}

bool GetEGAVDBForLiveShieldOnOff()
{
	wxString bval;
	gxGetLineFromTextFile(dbFile1, 7, bval);
	return (bval == AVDB_TR);
}

void SetEGAVMachineIdInDB()
{
	std::string uid, euid;
	getUniqueId(uid);
	MyEncode64(uid, euid);
	gxAddLineInTextFile(dbFile5, euid);

	gxAddLineInTextFile(dbFile5, AVDB_FL); //  for machine key EGFAV2020
}

bool GetEGAVMachineIdFromDB(std::string& mid)
{
	std::string stdstruid;
	wxString wxstrmid; 
	gxGetFirstLineFromTextFile(dbFile5, wxstrmid);
	MyDecode64(wxstrmid.c_str().AsChar(), stdstruid);
	mid = stdstruid;
	return true;
}

