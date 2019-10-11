#pragma once

///////////////////////////////////////////////////////////////////////////////
//
// AutoItX v3
//
// Copyright (C)1999-2013:
//		- Jonathan Bennett <jon at autoitscript dot com>
//		- See "AUTHORS.txt" for contributors.
//
// This file is part of AutoItX.  Use of this file and the AutoItX DLL is subject
// to the terms of the AutoItX license details of which can be found in the helpfile.
//
// When using the AutoItX3.dll as a standard DLL this file contains the definitions,
// and function declarations required to use the DLL and AutoItX3_DLL.lib file.
//
///////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
	#define AU3_API extern "C"
#else
	#define AU3_API
#endif


// Definitions
#define AU3_INTDEFAULT			(-2147483647)	// "Default" value for _some_ int parameters (largest negative number)

//
// nBufSize
// When used for specifying the size of a resulting string buffer this is the number of CHARACTERS 
// in that buffer, including the null terminator.  For example:
//
// WCHAR szBuffer[10];
// AU3_ClipGet(szBuffer, 10);
//
// The resulting string will be truncated at 9 characters with the the terminating null in the 10th.
//


///////////////////////////////////////////////////////////////////////////////
// Exported functions
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>

AU3_API void WINAPI AU3_Init(void);
AU3_API int AU3_error(void);

AU3_API int WINAPI AU3_AutoItSetOption(LPCWSTR szOption, int nValue);

AU3_API void WINAPI AU3_Send(LPCWSTR szSendText, int nMode);
///////////////////////////////////////////////////////////////////////////////
