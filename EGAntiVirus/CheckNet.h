#ifndef CHECK_NET_HEADER
#define CHECK_NET_HEADER

// example from:
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms737591(v=vs.85).aspx
#define WIN32_LEAN_AND_MEAN

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "80"

int CheckInternet();	

#endif