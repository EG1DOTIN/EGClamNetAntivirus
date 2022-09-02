#ifndef AVDB_HEADER
#define AVDB_HEADER

#define AVDB_RB1	wxT("lteH")		//BaseEncodeN("RB1") ->Radio Button1
#define AVDB_RB2	wxT("lte+")		//BaseEncodeN("RB2") ->Radio Button2
#define AVDB_RB3	wxT("lte0")		//BaseEncodeN("RB3") ->Radio Button3
#define AVDB_TR		wxT("mBe=")		//BaseEncodeN("TR")  ->True
#define AVDB_FL		wxT("Vtz=")		//BaseEncodeN("FL")  ->False

void MakeDefaultAVDBIfNotExist();
void MakeDefaultExcludeListFile(const wxString& dbFileFullPath);
void MakeDefaultIncludeListFile(const wxString& dbFileFullPath);

#endif



//Default Settings for anti virus settings database

//dbFile2

//	Control            values			Default Value		Meaning

//0. RadioButton ->  RB1 /RB2 /RB3		RB3					Report Only,Remove (Use Carefully),Move To Quarantine Folder
//1. CheckBox1   ->  TRUE / FALSE		TRUE				Unload Infected Program from Computer Memory
//2. SpinCtrl1   ->  1-30				30					Do Not Scan Files Larger Than (MB)
//3. CheckBox2   ->  TRUE /FALSE		TRUE				Extract Files From Archives
//4. SpinCtrl2   ->  1-50				50					Do Not Extract More Than (MB)
//5. SpinCtrl3   ->  1-100				100					Do Not Extract More Than (Files)
//6. SpinCtrl4   ->  1-10				10					Do Not Extract More Than (Sub Archives)
//7. CheckBox4   ->  TRUE / FALSE		TRUE				Real-Time Protection On/Off
//8. CheckBox3   ->  TRUE / FALSE		TRUE				Automatic Updates On/Off
//9. CheckBox5   ->  TRUE / FALSE       FALSE				Do Not Reload VSDB (Fast Scan)

//dbFile2
// Exclude list of file extension 

//dbFile3
// include list for scaning only

