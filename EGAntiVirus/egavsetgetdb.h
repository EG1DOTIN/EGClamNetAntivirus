#ifndef EGAVSETGETDB_HEADER
#define EGAVSETGETDB_HEADER

bool SetEGAVDBForAutoUpdateOnOff(bool bTRUEFALSE);
bool GetEGAVDBForAutoUpdateOnOff();
bool SetEGAVDBForLiveShieldOnOff(bool bTRUEFALSE);
bool GetEGAVDBForLiveShieldOnOff();
void SetDefaultDBForUpdateAndLiveShieldInDB();
void SetEGAVMachineIdInDB();
bool GetEGAVMachineIdFromDB(std::string& mid);

#endif