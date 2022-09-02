#ifndef UNIQ_ID_HEADER
#define UNIQ_ID_HEADER

//#define WIN32_LEAN_AND_MEAN        
#include <windows.h>      
#include <intrin.h>       
#include <iphlpapi.h>  
#include <cstdint>
#include <string.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "iphlpapi.lib")

typedef std::uint16_t u16;
typedef std::uint32_t u32;


u16 hashMacAddress(PIP_ADAPTER_INFO info);
void getMacHash(u16& mac1, u16& mac2);
u16 getVolumeHash();
u16 getCpuHash();
const char* getMachineName();
static void smear(u16* id);
static void unsmear(u16* id);
static u16* computeSystemUniqueId();
void getSystemUniqueId(std::string& uid);
bool ValidateUID(std::string testIdString);
DWORD GetPhysicalDriveSerialNumber(std::string& strSerialNumber, UINT nDriveNumber = 0);
void getMacAddress(std::string& adapinfo);
void getUniqueId(std::string& uniqid);

#endif