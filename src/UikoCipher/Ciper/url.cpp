#include "url.h"

//typedef unsigned char	uchar;

uchar HttpUtility::toHex(const uchar &x)
{
	return x > 9 ? x -10 + 'A': x + '0';
}

uchar HttpUtility::fromHex(const uchar &x)
{
	return isdigit(x) ? x-'0' : x-'A'+10;
}

string HttpUtility::URLEncode(const string &sIn)
{
	string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{      
		uchar buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (uchar)sIn[ix] ) )
		{      
			buf[0] = sIn[ix];
		}
		//else if ( isspace( (BYTE)sIn[ix] ) ) //貌似把空格编码成%20或者+都可以
		//{
		//    buf[0] = '+';
		//}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (uchar)sIn[ix] >> 4 );
			buf[2] = toHex( (uchar)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
}

string HttpUtility::URLDecode(const string &sIn)
{
	string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{
		uchar ch = 0;
		if(sIn[ix]=='%')
		{
			ch = (fromHex(sIn[ix+1])<<4);
			ch |= fromHex(sIn[ix+2]);
			ix += 2;
		}
		else if(sIn[ix] == '+')
		{
			ch = ' ';
		}
		else
		{
			ch = sIn[ix];
		}
		sOut += (char)ch;
	}
	return sOut;
}