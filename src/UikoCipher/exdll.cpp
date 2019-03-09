//************************************************************
//  UikoCipher - NSIS Cipher Library
//
//  File: Cipher.cpp
//  Version: 2.0.0.2001
//  CreateDate: 2013-01-04
//  LastDate: 2014-09-03
//
//  Author: Garfield
//
//  Copyright (c) 2012-2015, Uiko Develop Team.
//  All Rights Reserved.
//************************************************************

#include <windows.h>
#include <string>
#include <atlstr.h>

#include "exdll.h"

//Uiko Core
//#include "core.h"

#include ".\Ciper\OpenSSL.h"
#include ".\Ciper\AESFile.h"
//#include ".\Ciper\Base64.h"
//#include ".\Ciper\SHA_1.h"
#include ".\Ciper\MD5.h"
#include ".\Ciper\crc32.h"

#include ".\Ciper\url.h"
#include ".\Ciper\encode.h"

//#pragma warning(disable:4099)

using namespace std;

#define MAX_STRLEN       1024

TCHAR strarg[1024*sizeof(TCHAR)];
unsigned char staticCnvBuffer[1024*2];

#ifdef UNICODE
#define wcslen lstrlenW
#define wcscpy lstrcpyW
#define wcsncpy lstrcpynW
#define wcscat lstrcatW
#define wcscmp lstrcmpW
#define wcscmpi lstrcmpiW
//char * _T2A(unsigned short *wideStr)
//{
//	WideCharToMultiByte(CP_ACP, 0, wideStr, -1, (char *)staticCnvBuffer, sizeof(staticCnvBuffer), NULL, NULL);
//	return (char *)staticCnvBuffer;
//}
//#define _A2T(x) _A2U(x)
#else
#define strlen lstrlenA
#define strcpy lstrcpyA
#define strncpy lstrcpynA
#define strcat lstrcatA
#define strcmp lstrcmpA
#define strcmpi lstrcmpiA
#define _T2A(x) (x)
#define _A2T(x) (x)
#endif
unsigned short * _A2U(char *ansiStr)
{
	MultiByteToWideChar(CP_ACP, 0, ansiStr, -1, (LPWSTR)staticCnvBuffer, sizeof(staticCnvBuffer)/2);
	return (unsigned short *)staticCnvBuffer;
}

#define PRIVATEKEY "rsa_private_key.key"
#define PUBLICKEY "rsa_public_key.key"


//RSA密钥生成
extern "C" void __declspec(dllexport) RSA_Creat_Key(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{

	EXDLL_INIT();
	{
		CryptObject rsa;
		rsa.generateKey();
	}

}
//RSA公钥加密
extern "C" void __declspec(dllexport) RSA_Encrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_encode   [MAX_STRLEN] = _T("");
	TCHAR  string_encoded  [MAX_STRLEN] = _T("");
    
	EXDLL_INIT();
	{
        //popstring(string_encode);
        //char* cc = rsa.rsa_encrypt(string_encode);
		//strcpy_s(string_encoded,cc);
		
        popstring(string_encode);
        
		CryptObject rsa;
#ifdef UNICODE
        USES_CONVERSION;
        char* cc = rsa.rsa_encrypt(T2A(string_encode));
        wcscpy_s(string_encoded,A2T(cc));
#else
        char* cc = rsa.rsa_encrypt(string_encode);
        strcpy_s(string_encoded,cc);
#endif
        
		pushstring(string_encoded);
	}

}
//RSA私钥解密
extern "C" void __declspec(dllexport) RSA_Decrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_decode   [MAX_STRLEN] = _T("");
	TCHAR  string_decoded  [MAX_STRLEN] = _T("");
    
	EXDLL_INIT();
	{
		//popstring(string_decode);
		//CryptObject rsa;
		//char* cc = rsa.rsa_decrypt(string_decode);
		//strcpy_s(string_decoded,cc);
		
        popstring(string_decode);
        
		CryptObject rsa;
#ifdef UNICODE
        USES_CONVERSION;
        char* cc = rsa.rsa_decrypt(T2A(string_decode));
        wcscpy_s(string_decoded, A2T(cc));
#else
        char* cc = rsa.rsa_decrypt(string_decode);
        strcpy_s(string_decoded, cc);
#endif
		
		pushstring(string_decoded);	
	}
}


//RSA公钥文件加密
extern "C" void __declspec(dllexport) RSA_EncryptFile(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_encode   [MAX_STRLEN] = _T("");
	TCHAR  string_encoded  [MAX_STRLEN] = _T("");
    
	EXDLL_INIT();
	{
		//popstring(string_encode);
		//CryptObject rsa;
		//char* cc = rsa.rsa_encrypt(string_encode);
		//strcpy_s(string_encoded,cc);
		
        popstring(string_encode);
        
		CryptObject rsa;
#ifdef UNICODE
        USES_CONVERSION;
        char* cc = rsa.rsa_encrypt(T2A(string_encode));
        wcscpy_s(string_encoded, A2T(cc));
#else
        char* cc = rsa.rsa_encrypt(string_encode);
        strcpy_s(string_encoded, cc);
#endif
		
		pushstring(string_encoded);
	}

}
//RSA私钥文件解密
extern "C" void __declspec(dllexport) RSA_DecryptFile(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_decode   [MAX_STRLEN] = _T("");
	TCHAR  string_decoded  [MAX_STRLEN] = _T("");
    
	EXDLL_INIT();
	{
		//popstring(string_decode);
		//CryptObject rsa;
		////char* cc = rsa.privatekey_decode(string_decode);
		//char* cc = rsa.rsa_decrypt(string_decode);
		//strcpy_s(string_decoded,cc);
		
        popstring(string_decode);
        
		CryptObject rsa;
#ifdef UNICODE
        USES_CONVERSION;
        char* cc = rsa.rsa_decrypt(T2A(string_decode));
        wcscpy_s(string_decoded, A2T(cc));
#else
        char* cc = rsa.rsa_decrypt(string_decode);
        strcpy_s(string_decoded, cc);
#endif
        
		pushstring(string_decoded);	
	}
}


//AES加密 CBC iv="6543210987654321"
extern "C" void __declspec(dllexport) AES_Encrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_encode   [MAX_STRLEN] = _T("");
	TCHAR  string_encoded  [MAX_STRLEN] = _T("");
	TCHAR  aeskey          [256] = _T("");
    
	EXDLL_INIT();
	{
		//popstring(string_encode);
		//popstring(aeskey);
		//CryptObject aes;
		//char* cc = aes.aes_encode(string_encode, aeskey);
		//strcpy_s(string_encoded,cc);
        
        popstring(string_encode);
        
		CryptObject rsa;
#ifdef UNICODE
        USES_CONVERSION;
        char* cc = rsa.aes_encode(T2A(string_encode), T2A(aeskey));
        wcscpy_s(string_encoded, A2T(cc));
#else
        char* cc = rsa.aes_encode(string_encode, aeskey);
        strcpy_s(string_encoded, cc);
#endif
        
		pushstring(string_encoded);
	}

}
//AES解密 CBC iv="6543210987654321"
extern "C" void __declspec(dllexport) AES_Decrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_decode   [MAX_STRLEN] = _T("");
	TCHAR  string_decoded  [MAX_STRLEN] = _T("");
	TCHAR  aeskey          [256] = _T("");
    
	EXDLL_INIT();
	{
		//popstring(string_decode);
		//popstring(aeskey);
		//CryptObject aes;
		//char* cc = aes.aes_decode(string_decode,aeskey);
		//strcpy_s(string_decoded,cc);
		
        popstring(string_decode);
        
		CryptObject rsa;
#ifdef UNICODE
        USES_CONVERSION;
        char* cc = rsa.aes_decode(T2A(string_decode), T2A(aeskey));
        wcscpy_s(string_decoded, A2T(cc));
#else
        char* cc = rsa.aes_decode(string_decode, aeskey);
        strcpy_s(string_decoded, cc);
#endif
        
		pushstring(string_decoded);	
	}
}

//AES文件加密 CBC iv="6543210987654321"
extern "C" void __declspec(dllexport) AES_EncryptFile(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_file1  [MAX_STRLEN] = _T("");
	TCHAR  string_file2  [MAX_STRLEN] = _T("");
	TCHAR  aeskey        [256] = _T("");

	EXDLL_INIT();
	{
		popstring(string_file1);
		popstring(string_file2);
		popstring(aeskey);
        
        clock_t start, finish;
        double duration;
        start = clock(); 
        
#ifdef UNICODE
        USES_CONVERSION;
        if (AES_Encrypt_File( T2A(string_file1), T2A(string_file2), NID_aes_128_cbc, (unsigned char*)T2A(aeskey)) ) 
		{
			pushstring(_T("ok"));
		} else {
			pushstring(_T("unsuccessfull"));
		}
#else
        if (AES_Encrypt_File( string_file1, string_file2, NID_aes_128_cbc, (unsigned char*)aeskey) ) 
		{
			pushstring("ok");
		} else {
			pushstring("unsuccessfull");
		}
#endif
        
		finish = clock(); 
		duration = (double)(finish - start) / CLOCKS_PER_SEC; 
	}
}
//AES文件解密 CBC iv="6543210987654321"
extern "C" void __declspec(dllexport) AES_DecryptFile(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_file1  [MAX_STRLEN] = _T("");
	TCHAR  string_file2  [MAX_STRLEN] = _T("");
	TCHAR  aeskey        [256] = _T("");

	EXDLL_INIT();
	{
		popstring(string_file1);
		popstring(string_file2);
		popstring(aeskey);

		clock_t start, finish;
		double duration;
		start = clock(); 
        
#ifdef UNICODE
        USES_CONVERSION;
        if (AES_Decrypt_File( T2A(string_file1), T2A(string_file2), (unsigned char*)T2A(aeskey)) ) 
		{
			pushstring(_T("ok"));
		} else {
			pushstring(_T("unsuccessfull"));
		}
#else
        if (AES_Decrypt_File( string_file1, string_file2, (unsigned char*)aeskey) ) 
		{
			pushstring("ok");
		} else {
			pushstring("unsuccessfull");
		}
#endif
        
		finish = clock(); 
		duration = (double)(finish - start) / CLOCKS_PER_SEC; 
	}
}
//Base64编码
extern "C" void __declspec(dllexport) Base64_Encrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_encode    [MAX_STRLEN] = _T("");
	TCHAR  string_encoded   [MAX_STRLEN] = _T("");
    
	EXDLL_INIT();
	{
        //popstring (string_to_encode);
        //CryptObject base64;
        //char* cc = base64.base64_encode( string_to_encode, strlen(string_to_encode) );
        //strcpy_s(string_to_encode, cc);
        
        popstring (string_encode);
        
        CryptObject base64;
#ifdef UNICODE
        USES_CONVERSION;
        char* cc = base64.base64_encode( T2A(string_encode), wcslen(string_encode) );
        wcscpy_s(string_encoded, A2T(cc));
#else
        char* cc = base64.base64_encode( string_encode, strlen(string_encode) );
        strcpy_s(string_encoded, cc);
#endif
		
		pushstring (string_encoded);
	}

}

//Base64解码
extern "C" void __declspec(dllexport) Base64_Decrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_decode    [MAX_STRLEN] = _T("");
	TCHAR  string_decoded   [MAX_STRLEN] = _T("");
    
	EXDLL_INIT();
	{
		//Uiko Core
		//Init(1);
        
        //popstring (string_decode);
        //int outlen = 0;
        //CryptObject base64;
        //char* cc = base64.base64_decode( string_decode, strlen(string_to_decode), outlen );
        //strcpy_s(string_decoded,cc);
		
		popstring (string_decode);
        
        int outlen = 0;
        CryptObject base64;
#ifdef UNICODE
        USES_CONVERSION;
        char* cc = base64.base64_decode( T2A(string_decode), wcslen(string_decode), outlen );
        wcscpy_s(string_decode, A2T(cc));
#else
        char* cc = base64.base64_decode( string_decode, strlen(string_decode), outlen );
        strcpy_s(string_decode, cc);
#endif
		
		pushstring (string_decoded);	
	}
}

//ASCII编码 字符串转ASCII
extern "C" void __declspec(dllexport) ASCII_Encrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_to_ascii  [MAX_STRLEN] = _T("");
    CString ascii = _T("");

	EXDLL_INIT();
	{
		popstring(string_to_ascii);
        
#ifdef UNICODE
        USES_CONVERSION;
		for(int i = 0; i < wcslen(string_to_ascii); i++)
		{
			CString cc = _T("");
			cc.Format(_T("%d,"),string_to_ascii[i]);
			ascii += cc;
		}
#else
		for(int i = 0; i < strlen(string_to_ascii); i++)
		{
			CString cc = _T("");
			cc.Format("%d,",string_to_ascii[i]);
			ascii += cc;
		}
#endif
        
		pushstring (ascii);
	}
}
//ASCII解码 ASCII转字符串
extern "C" void __declspec(dllexport) ASCII_Decrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_ascii  [MAX_STRLEN] = _T("");
	CString  ascii  = _T("");
	CString  string = _T("");

	EXDLL_INIT();
	{
		popstring(string_ascii);
		ascii.Format(_T("%s"), string_ascii);
		
		do
		{
#ifdef UNICODE
            USES_CONVERSION;
			int dd = _wtoi( ascii.Left(ascii.Find(',')) );
			if(dd == 0)
			{
				dd = _wtoi( ascii );
				ascii = _T("");
			}
#else
			int dd = atoi( ascii.Left(ascii.Find(',')) );
			if(dd == 0)
			{
				dd = atoi( ascii );
				ascii = _T("");
			}
#endif
			if(dd == 0) break;
			string += (unsigned char)dd;
			ascii = ascii.Mid( ascii.Find(',')+1, ascii.GetLength()-ascii.Find(',')-1 );
		}
		while ( ascii.Find(',') );
        
		pushstring (string);
	}
}

//url编码字符串编码函数 gb2312
extern "C" void __declspec(dllexport) URL_Encrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_to_encode  [MAX_STRLEN] = _T("");

	EXDLL_INIT();
	{
		popstring (string_to_encode);
        
		CString cc = URLEncode( string_to_encode );
        
		pushstring (cc);
	}

}

//url编码字符串解码函数 gb2312
extern "C" void __declspec(dllexport) URL_Decrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_to_decode  [MAX_STRLEN] = _T("");

	EXDLL_INIT();
	{
		popstring (string_to_decode);
        
		CString cc = UrlDecode( string_to_decode );
        
		pushstring (cc);	
	}
}

//unicode编码字符串编码函数
extern "C" void __declspec(dllexport) UNICODE_Encrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_to_encode  [MAX_STRLEN] = _T("");
	TCHAR  string_encoded    [MAX_STRLEN] = _T("");

	EXDLL_INIT();
	{
		popstring (string_to_encode);
        
#ifdef UNICODE
        USES_CONVERSION;
        wchar_t* cc = GB2312ToUnicode( T2A(string_to_encode) );
        wcscpy_s(string_encoded, cc);
#else
        USES_CONVERSION;
        wchar_t* cc = GB2312ToUnicode( string_to_encode );
        strcpy_s(string_encoded, W2A(cc));
#endif
        
		pushstring (string_encoded);
	}

}

//unicode编码字符串解码函数
extern "C" void __declspec(dllexport) UNICODE_Decrypt(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_to_decode  [MAX_STRLEN] = _T("");
	TCHAR  string_decoded    [MAX_STRLEN] = _T("");
    
	EXDLL_INIT();
	{
		popstring (string_to_decode);
        
#ifdef UNICODE
        USES_CONVERSION;
        char* cc = UnicodeToGB2312( (const wchar_t *)string_to_decode );
        wcscpy_s(string_decoded, A2T(cc));
#else
        USES_CONVERSION;
        char* cc = UnicodeToGB2312( (const wchar_t *)A2T(string_to_decode) );
        strcpy_s(string_decoded, cc);
#endif
        
		pushstring (string_decoded);
	}
}

//MD5文件编码
extern "C" void __declspec(dllexport) GetMD5File(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	EXDLL_INIT();
	{
		popstring(strarg);
        
        char *buf = NULL;
#ifdef UNICODE
        USES_CONVERSION;
        buf = MD5File(T2A(strarg));
        pushstring(A2T(buf));
#else
        buf = MD5File(strarg);
        pushstring(buf);
#endif
		
		delete buf;
	}
}
//MD5编码
extern "C" void __declspec(dllexport) GetMD5String(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	EXDLL_INIT();
	{
		popstring(strarg);
        
        char *buf = NULL;
#ifdef UNICODE
        USES_CONVERSION;
        buf = MD5String(T2A(strarg));
        pushstring(A2T(buf));
#else
        buf = MD5String(strarg);
        pushstring(buf);
#endif
		
		delete buf;
	}
}
//MD5随机码
extern "C" void __declspec(dllexport) GetMD5Random(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	EXDLL_INIT();
	{
		Sleep(1);
        DWORD dwTickCount = GetTickCount();
        TCHAR lTickCount[1024];
		wsprintf(lTickCount, _T("%d"), dwTickCount);
		
        char *buf = NULL;
#ifdef UNICODE
        USES_CONVERSION;
        buf = MD5String(T2A(lTickCount));
        pushstring(A2T(buf));
#else
        buf = MD5String(lTickCount);
        pushstring(buf);
#endif

		delete buf;
	}
}


//CRC32文件编码    //产生CRC32值存在问题
extern "C" void __declspec(dllexport) GetCRC32File(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_encode   [MAX_STRLEN] = _T("");
	TCHAR  string_encoded  [MAX_STRLEN] = _T("");

	EXDLL_INIT();
	{
		popstring(string_encode);

		HANDLE file;
		ISO_13818_CRC32 alg;
		unsigned long nLen;
		unsigned char chBuffer[1024];

		for (register int i = 0; i < 256; i++) ((uint4 *)chBuffer)[i] = 0UL;

		if ((file = CreateFile(string_encode, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0)) != INVALID_HANDLE_VALUE )
		{
			//DWORD dwSize = GetFileSize(file, NULL);
			while ((ReadFile(file, chBuffer, 1024, &nLen, NULL) != FALSE) && nLen)
			{
				alg.update(chBuffer, nLen);
			}
			CloseHandle(file);

			unsigned long crc = alg.get_value();
            
#ifdef UNICODE
            USES_CONVERSION;
            itoa(crc, T2A(string_encoded), 16); 
#else
            itoa(crc, string_encoded, 16); 
#endif
			
			pushstring(string_encoded);
		}
	}
}
//CRC32编码
extern "C" void __declspec(dllexport) GetCRC32String(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_encode   [MAX_STRLEN] = _T("");
	TCHAR  string_encoded  [MAX_STRLEN] = _T("");

	EXDLL_INIT();
	{
		popstring(string_encode);


#ifdef UNICODE
        USES_CONVERSION;
        unsigned long crc = CRC32( (unsigned char*)T2A(string_encode), wcslen(string_encode) );
        itoa(crc, T2A(string_encoded), 16); 
#else
        unsigned long crc = CRC32( (unsigned char*)string_encode, strlen(string_encode) );
        itoa(crc, string_encoded, 16); 
#endif
		
		pushstring(string_encoded);
	}

}
//SHA1编码
extern "C" void __declspec(dllexport) GetSHA1String(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_encode   [MAX_STRLEN] = _T("");
	TCHAR  string_encoded  [MAX_STRLEN] = _T("");

	EXDLL_INIT();
	{
		popstring(string_encode);

        CryptObject sha;
#ifdef UNICODE
        USES_CONVERSION;
        char* cc = sha.sha1_encode((unsigned char*)T2A(string_encode));
        wcscpy_s(string_encoded, A2T(cc));
#else
        char* cc = sha.sha1_encode((unsigned char*)string_encode);
        strcpy_s(string_encoded, cc);
#endif
		
		pushstring (string_encoded);
	}
}
//EVP编码 "md4","md5","sha1","sha224","sha256","sha384","sha512"
extern "C" void __declspec(dllexport) GetEVPString(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
{
	TCHAR  string_encode   [MAX_STRLEN] = _T("");
	TCHAR  string_encoded  [MAX_STRLEN] = _T("");
	TCHAR  encodetype      [16] = _T("");

	EXDLL_INIT();
	{
		popstring(string_encode);
		popstring(encodetype);
        
        CryptObject sha;
#ifdef UNICODE
        USES_CONVERSION;
        char* cc = sha.evp_encode(T2A(string_encode), T2A(encodetype) );
        wcscpy_s(string_encoded, A2T(cc));
#else
        char* cc = sha.evp_encode(string_encode, encodetype );
        strcpy_s(string_encoded, cc);
#endif
		
		pushstring (string_encoded);
	}
}


//extern "C" void __declspec(dllexport) OpenURL(HWND hwndParent, int string_size, TCHAR *variables, stack_t **stacktop)
//{
//	char  string_url  [MAX_STRLEN]="";
//
//	EXDLL_INIT();
//	{
//		popstring(string_url);
//
//		//打开网站0
//		HKEY hkRoot,hSubKey;
//		char ValueName[256];
//		unsigned char DataValue[256];
//		unsigned long chValueName=256;
//		unsigned long chDataValue=256;
//		char ShellChar[256];//定义命令行
//		CString csUrl;
//		DWORD dwType;
//		if(RegOpenKey(HKEY_CLASSES_ROOT,NULL, &hkRoot) == ERROR_SUCCESS)
//		{
//			if(RegOpenKeyEx( hkRoot, "htmlfile\\shell\\open\\command", 	//子目录
//				0L,											//必须取0
//				KEY_ALL_ACCESS,
//				&hSubKey )==ERROR_SUCCESS)
//			{
//				//获取键值，默认浏览器的路径
//				RegEnumValue(hSubKey,0,ValueName,&chValueName,NULL,&dwType,DataValue,&chDataValue);
//				csUrl.Format( "%s", DataValue );
//				int startpos = csUrl.Find("\"", 0);
//				int endpos = csUrl.Find(".exe", 0);
//				CString csIE = csUrl.Mid( startpos+1, endpos+4-1 );
//
//				STARTUPINFO si;  
//				GetStartupInfo(&si);  
//				PROCESS_INFORMATION pi;  
//				si.dwFlags = STARTF_USESHOWWINDOW;  
//				si.wShowWindow = SW_HIDE; 
//				if (!CreateProcess(csIE, string_url, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {  
//					wprintf(L"CreateProcess error:%d\n", GetLastError());  
//					return ;  
//				}  
//				//Sleep(5000);  
//				//TerminateProcess(pi.hProcess, 0);  
//			}
//			else
//				OutputDebugString("Web浏览器打开错误");
//		}
//		else
//			OutputDebugString("Web浏览器打开错误");
//		RegCloseKey(hSubKey);//关闭子健
//		RegCloseKey(hkRoot);//关闭主键
//
//
//		////打开网站1
//		//ShellExecute( NULL, "open", string_url, NULL, NULL, SW_SHOW );
//
//
//		////打开网站2
//		//HKEY hkRoot,hSubKey;
//		//char ValueName[256];
//		//unsigned char DataValue[256];
//		//unsigned long chValueName=256;
//		//unsigned long chDataValue=256;
//		//char ShellChar[256];//定义命令行
//		//CString csUrl;
//		//DWORD dwType;
//		//if(RegOpenKey(HKEY_CLASSES_ROOT,NULL, &hkRoot) == ERROR_SUCCESS)
//		//{
//		//	if(RegOpenKeyEx( hkRoot, "htmlfile\\shell\\open\\command", 	//子目录
//		//		0L,											//必须取0
//		//		KEY_ALL_ACCESS,
//		//		&hSubKey )==ERROR_SUCCESS)
//		//	{
//		//		//获取键值，默认浏览器的路径
//		//		RegEnumValue(hSubKey,0,ValueName,&chValueName,NULL,&dwType,DataValue,&chDataValue);
//		//		csUrl.Format( "%s", DataValue );
//		//		if (csUrl.Find("%1"))
//		//		{
//		//			csUrl.Replace( "%1", string_url );
//		//			WinExec(csUrl,SW_SHOW);//执行命令
//		//		}
//		//		else
//		//		{
//		//			strcpy(ShellChar,(char*)DataValue);
//		//			strcat(ShellChar,(char*)string_url);
//		//			WinExec(ShellChar,SW_SHOW);//执行命令
//		//		}
//		//	}
//		//	else
//		//		OutputDebugString("Web浏览器打开错误");
//		//}
//		//else
//		//	OutputDebugString("Web浏览器打开错误");
//		//RegCloseKey(hSubKey);//关闭子健
//		//RegCloseKey(hkRoot);//关闭主键
//	}
//
//}
