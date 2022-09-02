#ifndef CLEANING_THREADS_HEADER
#define CLEANING_THREADS_HEADER

#include <Windows.h>

void CleaningMain();
DWORD WINAPI CleaningMainThreadFunction(LPVOID lparm);

#endif