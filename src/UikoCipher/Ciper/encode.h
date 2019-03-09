#ifndef _ENCODE_H_
#define _ENCODE_H_

#include <xstring>

//GB2312 转换成 Unicode：
wchar_t* GB2312ToUnicode(const char* szGBString)
{    
	UINT nCodePage = 936; //GB2312    
	int nLength=MultiByteToWideChar(nCodePage,0,szGBString,-1,NULL,0);    
	wchar_t* pBuffer = new wchar_t[nLength+1];    
	MultiByteToWideChar(nCodePage,0,szGBString,-1,pBuffer,nLength);    
	pBuffer[nLength]=0;    
	return pBuffer;
}

//BIG5 转换成 Unicode：
wchar_t* BIG5ToUnicode(const char* szBIG5String)
{    
	UINT nCodePage = 950; //BIG5    
	int nLength=MultiByteToWideChar(nCodePage,0,szBIG5String,-1,NULL,0);    
	wchar_t* pBuffer = new wchar_t[nLength+1];    
	MultiByteToWideChar(nCodePage,0,szBIG5String,-1,pBuffer,nLength);    
	pBuffer[nLength]=0;   
	return pBuffer;
}

//Unicode 转换成 GB2312：
char* UnicodeToGB2312(const wchar_t* szUnicodeString)
{    
	UINT nCodePage = 936; //GB2312    
	int nLength=WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,NULL,0,NULL,NULL);    
	char* pBuffer=new char[nLength+1];    
	WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,pBuffer,nLength,NULL,NULL);    
	pBuffer[nLength]=0;    
	return pBuffer;
}

//Unicode 转换成 BIG5：
char* UnicodeToBIG5(const wchar_t* szUnicodeString)
{    
	UINT nCodePage = 950; //BIG5    
	int nLength=WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,NULL,0,NULL,NULL);    
	char* pBuffer=new char[nLength+1];    
	WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,pBuffer,nLength,NULL,NULL);    
	pBuffer[nLength]=0;    
	return pBuffer;
}
//繁体中文BIG5 转换成 简体中文 GB2312
char* BIG5ToGB2312(const char* szBIG5String)
{    
	LCID lcid = MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_PRC);    
	wchar_t* szUnicodeBuff = BIG5ToUnicode(szBIG5String);    
	char* szGB2312Buff = UnicodeToGB2312(szUnicodeBuff);    
	int nLength = LCMapStringA(lcid,LCMAP_SIMPLIFIED_CHINESE, szGB2312Buff,-1,NULL,0);    
	char* pBuffer = new char[nLength + 1];    
	LCMapStringA(0x0804,LCMAP_SIMPLIFIED_CHINESE,szGB2312Buff,-1,pBuffer,nLength);    
	pBuffer[nLength] = 0;         
	delete[] szUnicodeBuff;    
	delete[] szGB2312Buff;   
	return pBuffer;
}
//简体中文 GB2312 转换成 繁体中文BIG5
char* GB2312ToBIG5(const char* szGBString)
{    
	LCID lcid = MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_PRC);    
	int nLength = LCMapStringA(lcid,LCMAP_TRADITIONAL_CHINESE,szGBString,-1,NULL,0);    
	char* pBuffer=new char[nLength+1];    
	LCMapStringA(lcid,LCMAP_TRADITIONAL_CHINESE,szGBString,-1,pBuffer,nLength);    
	pBuffer[nLength]=0;    
	wchar_t* pUnicodeBuff = GB2312ToUnicode(pBuffer);    
	char* pBIG5Buff = UnicodeToBIG5(pUnicodeBuff);    
	delete[] pBuffer;    
	delete[] pUnicodeBuff;    
	return pBIG5Buff;
}
#endif //_ENCODE_H_
