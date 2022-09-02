#include "common.h"
#include "uniqid.h"   

// we just need this for purposes of unique machine id. So any one or two mac's is       
// fine. 
u16 hashMacAddress(PIP_ADAPTER_INFO info)
{
	u16 hash = 0;
	for (u32 i = 0; i < info->AddressLength; i++)
	{
		hash += (info->Address[i] << ((i & 1) * 8));
	}
	return hash;
}

void getMacHash(u16& mac1, u16& mac2)
{
	IP_ADAPTER_INFO AdapterInfo[32];
	DWORD dwBufLen = sizeof(AdapterInfo);

	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	if (dwStatus != ERROR_SUCCESS)
		return; // no adapters.      

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	mac1 = hashMacAddress(pAdapterInfo);
	if (pAdapterInfo->Next)
		mac2 = hashMacAddress(pAdapterInfo->Next);

	// sort the mac addresses. We don't want to invalidate     
	// both macs if they just change order.           
	if (mac1 > mac2)
	{
		u16 tmp = mac2;
		mac2 = mac1;
		mac1 = tmp;
	}
}

u16 getVolumeHash()
{
	DWORD serialNum = 0;

	// Determine if this volume uses an NTFS file system.      
	GetVolumeInformation(L"c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0);
	u16 hash = (u16)((serialNum + (serialNum >> 16)) & 0xFFFF);

	return hash;
}

u16 getCpuHash()
{
	int cpuinfo[4] = { 0, 0, 0, 0 };
	__cpuid(cpuinfo, 0);
	u16 hash = 0;
	u16* ptr = (u16*)(&cpuinfo[0]);
	for (u32 i = 0; i < 8; i++)
		hash += ptr[i];

	return hash;
}

const char* getMachineName()
{
	static char computerName[1024];
	DWORD size = 1024;
	GetComputerNameA(computerName, &size);
	return &(computerName[0]);
}

u16 mask[5] = { 0x4e25, 0xf4a1, 0x5437, 0xab41, 0x0000 };

static void smear(u16* id)
{
	for (u32 i = 0; i < 5; i++)
		for (u32 j = i; j < 5; j++)
			if (i != j)
				id[i] ^= id[j];

	for (u32 i = 0; i < 5; i++)
		id[i] ^= mask[i];
}

static void unsmear(u16* id)
{
	for (u32 i = 0; i < 5; i++)
		id[i] ^= mask[i];

	for (u32 i = 0; i < 5; i++)
		for (u32 j = 0; j < i; j++)
			if (i != j)
				id[4 - i] ^= id[4 - j];
}

static u16* computeSystemUniqueId()
{
	static u16 id[5];
	static bool computed = false;

	if (computed) return id;

	// produce a number that uniquely identifies this system.
	id[0] = getCpuHash();
	id[1] = getVolumeHash();
	getMacHash(id[2], id[3]);

	// fifth block is some checkdigits
	id[4] = 0;
	for (u32 i = 0; i < 4; i++)
		id[4] += id[i];

	smear(id);

	computed = true;
	return id;
}

void getSystemUniqueId(std::string& uid)
{
	// get the name of the computer
	std::string buf;
	buf.append(getMachineName());

	u16* id = computeSystemUniqueId();
	for (u32 i = 0; i < 5; i++)
	{
		char num[16];
		_snprintf(num, 16, "%x", id[i]);
		buf.append("-");
		switch (strlen(num))
		{
		case 1: buf.append("000"); break;
		case 2: buf.append("00");  break;
		case 3: buf.append("0");   break;
		}
		buf.append(num);
	}

	char* p = (char*)(buf.c_str());
	while (*p) { *p = toupper(*p); p++; }

	uid = buf;
}


bool ValidateUID(std::string testIdString)
{
	// unpack the given string. parse failures return false.
	std::string testString;
	testString.append(testIdString);
	char* testName = strtok((char*)(testString.c_str()), "-");
	if (!testName) return false;

	u16 testId[5];
	for (u32 i = 0; i < 5; i++)
	{
		char* testNum = strtok(NULL, "-");
		if (!testNum) return false;
		testId[i] = (u16)(strtol(testNum, NULL, 16));
	}
	unsmear(testId);

	// make sure this id is valid - by looking at the checkdigits
	u16 check = 0;
	for (u32 i = 0; i < 4; i++)
		check += testId[i];
	if (check != testId[4]) return false;

	// get the current system information
	u16 systemId[5];
	memcpy(systemId, computeSystemUniqueId(), sizeof(systemId));
	unsmear(systemId);

	// now start scoring the match
	u32 score = 0;

	for (u32 i = 0; i < 4; i++)
		if (testId[i] == systemId[i])
			score++;

	if (!strcmp(getMachineName(), testName))
		score++;

	// if we score 3 points or more then the id matches.
	return (score >= 3) ? true : false;
}

DWORD GetPhysicalDriveSerialNumber(std::string& strSerialNumber, UINT nDriveNumber)
{
	DWORD dwResult = NO_ERROR;
	strSerialNumber.empty();

	// Format physical drive path (may be '\\.\PhysicalDrive0', '\\.\PhysicalDrive1' and so on).
	std::string strDrivePath;
	char drivenum[2];
	_itoa(nDriveNumber, drivenum, 10);
	strDrivePath.append("\\\\.\\PhysicalDrive");
	strDrivePath.append(drivenum);

	// call CreateFile to get a handle to physical drive
	HANDLE hDevice = ::CreateFileA(strDrivePath.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == hDevice)
		return ::GetLastError();

	// set the input STORAGE_PROPERTY_QUERY data structure
	STORAGE_PROPERTY_QUERY storagePropertyQuery;
	ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
	storagePropertyQuery.PropertyId = StorageDeviceProperty;
	storagePropertyQuery.QueryType = PropertyStandardQuery;

	// get the necessary output buffer size
	STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader = { 0 };
	DWORD dwBytesReturned = 0;
	if (!::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
		&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
		&storageDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER),
		&dwBytesReturned, NULL))
	{
		dwResult = ::GetLastError();
		::CloseHandle(hDevice);
		return dwResult;
	}

	// allocate the necessary memory for the output buffer
	const DWORD dwOutBufferSize = storageDescriptorHeader.Size;
	BYTE* pOutBuffer = new BYTE[dwOutBufferSize];
	ZeroMemory(pOutBuffer, dwOutBufferSize);

	// get the storage device descriptor
	if (!::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
		&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
		pOutBuffer, dwOutBufferSize,
		&dwBytesReturned, NULL))
	{
		dwResult = ::GetLastError();
		delete[]pOutBuffer;
		::CloseHandle(hDevice);
		return dwResult;
	}

	// Now, the output buffer points to a STORAGE_DEVICE_DESCRIPTOR structure
	// followed by additional info like vendor ID, product ID, serial number, and so on.
	STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pOutBuffer;
	const DWORD dwSerialNumberOffset = pDeviceDescriptor->SerialNumberOffset;
	if (dwSerialNumberOffset != 0)
	{
		// finally, get the serial number
		unsigned char* finalstr = pOutBuffer + dwSerialNumberOffset;
		strSerialNumber = std::string((char*)(finalstr));
	}

	// perform cleanup and return
	delete[]pOutBuffer;
	::CloseHandle(hDevice);
	return dwResult;
}

void getMacAddress(std::string& adapinfo)
{
	PIP_ADAPTER_INFO AdapterInfo;
	DWORD dwBufLen = sizeof(AdapterInfo);
	char mac_addr[MAXBYTE];

	AdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	if (AdapterInfo == NULL)
	{
		//ErrorCommonLogsW(L"Error allocating memory needed to call GetAdaptersinfo\n");
		adapinfo = "ERROR";
		return;
	}

	// Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen     variable
	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
		if (AdapterInfo == NULL)
		{
			//ErrorCommonLogsW(L"Error allocating memory needed to call GetAdaptersinfo\n");
			adapinfo = "ERROR";
			return;
		}
	}

	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR)
	{
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
		do {
			sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
				pAdapterInfo->Address[0], pAdapterInfo->Address[1],
				pAdapterInfo->Address[2], pAdapterInfo->Address[3],
				pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
			//SuccessCommonLogsW(L"Address: %S, mac: %S", pAdapterInfo->IpAddressList.IpAddress.String, mac_addr);
			pAdapterInfo = pAdapterInfo->Next;
		} while (pAdapterInfo);
	}
	adapinfo = std::string(mac_addr);
	free(AdapterInfo);
}

void getUniqueId(std::string& uniqid)
{
	std::string strCpuHash, strDiskSN, strMacAddress;

	u16 cpuhash = getCpuHash();

	char temp[10];

	strCpuHash = std::string(_itoa(cpuhash, temp, 10));
	DWORD dwResult = GetPhysicalDriveSerialNumber(strDiskSN);
	
	if (dwResult != NO_ERROR)
	{
		//ErrorCommonLogsW(L"Error in getUniqueId: GetPhysicalDriveSerialNumber failed: Error: %u", dwResult);
		strDiskSN = "ERROR";
	}

	//getMacAddress(strMacAddress);

	uniqid.append(strCpuHash);
	uniqid.append("-");
	uniqid.append(strDiskSN);
	//uniqid.append("-");
	//uniqid.append(strMacAddress);

	RemoveWhiteSpace(uniqid);
	return;
}

