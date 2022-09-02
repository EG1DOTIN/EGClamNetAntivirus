#ifndef GX_DEL_POINTER_HEADER
#define GX_DEL_POINTER_HEADER

#include "common.h"

#define DELETE_POINTER_WXTEXTFILE(varName) DELETE_POINTER(wxTextFile,varName)
#define DELETE_POINTER_WXFILENAME(varName) DELETE_POINTER(wxFileName,varName)
#define DELETE_POINTER_WXTHREAD(varName) DELETE_POINTER(wxThread,varName)

#endif