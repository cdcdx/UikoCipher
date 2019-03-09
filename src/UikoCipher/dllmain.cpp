//************************************************************
//  UikoEngine - NSIS UI Library
//
//  File: dllmain.cpp
//  Version: 2.0.0.2001
//  CreateDate: 2013-01-04
//  LastDate: 2015-09-03
//
//  Author: Garfield (542733662@qq.com)
//  WebStore: http://shop64088102.taobao.com
//
//  Copyright (c) 2012-2015, Uiko Develop Team (www.uiko.cn).
//  All Rights Reserved.
//************************************************************

#include "stdafx.h"

//HINSTANCE g_hInstance;

BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	//g_hInstance = (HINSTANCE) hInst;

#ifdef _DEBUG
 	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
 		MessageBox( 0, _T("DEBUG"), _T("Cipher-DllMain"), 0 );
#endif

    return TRUE;
}

