#ifndef _EXDLL_H_
#define _EXDLL_H_

#include <stdio.h>
#include <windows.h>

#if defined(__GNUC__)
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

////////////////////////////////////////////////////////////////////////////////////////////////

// Starting with NSIS 2.42, you can check the version of the plugin API in exec_flags->plugin_api_version
// The format is 0xXXXXYYYY where X is the major version and Y is the minor version (MAKELONG(y,x))
// When doing version checks, always remember to use >=, ex: if (pX->exec_flags->plugin_api_version >= NSISPIAPIVER_1_0) {}

#define NSISPIAPIVER_1_0 0x00010000
#define NSISPIAPIVER_CURR NSISPIAPIVER_1_0

// NSIS Plug-In Callback Messages
enum NSPIM 
{
	NSPIM_UNLOAD,    // This is the last message a plugin gets, do final cleanup
	NSPIM_GUIUNLOAD, // Called after .onGUIEnd
};

// Prototype for callbacks registered with extra_parameters->RegisterPluginCallback()
// Return NULL for unknown messages
// Should always be __cdecl for future expansion possibilities
typedef UINT_PTR (*NSISPLUGINCALLBACK)(enum NSPIM);

typedef struct { 
	int autoclose;      // 安装结束是否自动关闭/进入下一页：SetAutoClose true/false 
	int all_user_var;   // 外壳变量的作用环境：SetShellVarContext all/current 
	int exec_error;     // 检测或设置错误标记：IfErrors、ClearErrors 
	int abort;          // 函数执行中是否调用Abort终止 
	int exec_reboot;    // 重启标记相关 
	int reboot_called;  // 重启标记相关 
	int cur_insttype;   // depreacted 					                // used to be XXX_cur_insttype
	int plugin_api_version; // 插件版本：NSISPIAPIVER_CURR 				// used to be XXX_insttype_changed
	int silent;             // 检测或设置静默安装标记 
	int instdir_error;  // 目标文件夹是否无效：GetInstDirError 
	int rtl;            // 所选语言是否从右到左 
	int errlvl;         // 检测或设置错误级别：SetErrorLevel 
	int alter_reg_view; // 检测或设置注册表访问权限：SetRegView 64/32 
	int status_update;  // 安装详情界面正在更新释放文件的百分比 
} exec_flags_type;

typedef struct {
  exec_flags_type *exec_flags;
  int (__stdcall *ExecuteCodeSegment)(int, HWND); // 用来执行NSIS脚本中的函数，这样使得插件更容易和交互。该函数有两个参数，第一个参数就是要执行的代码块首地址减去 1，第二个参数是窗口句柄，我们传 NULL 即可。
  void (__stdcall *ValidateFilename)(char *); // ValidateFilename 这个函数就是内部用来处理安装路径的，传入这个函数当调用结束其中不合法字符就被过滤掉了。
  int (__stdcall *RegisterPluginCallback)(HMODULE, NSISPLUGINCALLBACK); // returns 0 on success, 1 if already registered and < 0 on errors   // NSIS 设计了这个注册 DLL 的回调函数，把 FreeLibrary 统一交给 NSIS 来管理。
} extra_parameters;

// Definitions for page showing plug-ins
// See Ui.c to understand better how they're used

// sent to the outer window to tell it to go to the next inner window
#define WM_NOTIFY_OUTER_NEXT (WM_USER+0x8)

// custom pages should send this message to let NSIS know they're ready
#define WM_NOTIFY_CUSTOM_READY (WM_USER+0xd)

// sent as wParam with WM_NOTIFY_OUTER_NEXT when user cancels - heed its warning
#define NOTIFY_BYE_BYE 'x'

////////////////////////////////////////////////////////////////////////////////////////////////

// only include this file from one place in your DLL.
// (it is all static, if you use it in two places it will fail)
#define EXDLL_INIT()           {  \
        g_stringsize=string_size; \
        g_stacktop=stacktop;      \
        g_variables=variables; }

typedef struct _stack_t {
  struct _stack_t *next;
  TCHAR text[1]; // this should be the length of string_size
} stack_t;

enum
{
INST_0,         // $0
INST_1,         // $1
INST_2,         // $2
INST_3,         // $3
INST_4,         // $4
INST_5,         // $5
INST_6,         // $6
INST_7,         // $7
INST_8,         // $8
INST_9,         // $9
INST_R0,        // $R0
INST_R1,        // $R1
INST_R2,        // $R2
INST_R3,        // $R3
INST_R4,        // $R4
INST_R5,        // $R5
INST_R6,        // $R6
INST_R7,        // $R7
INST_R8,        // $R8
INST_R9,        // $R9
INST_CMDLINE,   // $CMDLINE
INST_INSTDIR,   // $INSTDIR
INST_OUTDIR,    // $OUTDIR
INST_EXEDIR,    // $EXEDIR
INST_LANG,      // $LANGUAGE
__INST_LAST
};

static unsigned int g_stringsize;
static stack_t **g_stacktop;
static TCHAR *g_variables;

static int __stdcall popstring(TCHAR *str) UNUSED; // 0 on success, 1 on empty stack
static void __stdcall pushstring(const TCHAR *str) UNUSED; // with length limit, pass 0 for g_stringsize
static TCHAR * __stdcall getuservariable(const int varnum) UNUSED;
static void __stdcall setuservariable(const int varnum, const TCHAR *var) UNUSED;

#ifdef _UNICODE

#define PopStringW(x) popstring(x)
#define PushStringW(x) pushstring(x)
#define SetUserVariableW(x,y) setuservariable(x,y)

static int __stdcall PopStringA(char* ansiStr);
static void __stdcall PushStringA(const char* ansiStr);
static void __stdcall GetUserVariableW(const int varnum, wchar_t* wideStr);
static void __stdcall GetUserVariableA(const int varnum, char* ansiStr);
static void __stdcall SetUserVariableA(const int varnum, const char* ansiStr);

#else  // ANSI defs

#define PopStringA(x) popstring(x)
#define PushStringA(x) pushstring(x)
#define SetUserVariableA(x,y) setuservariable(x,y)

static int __stdcall PopStringW(wchar_t* wideStr);
static void __stdcall PushStringW(wchar_t* wideStr);
static void __stdcall GetUserVariableW(const int varnum, wchar_t* wideStr);
static void __stdcall GetUserVariableA(const int varnum, char* ansiStr);
static void __stdcall SetUserVariableW(const int varnum, const wchar_t* wideStr);

#endif

////////////////////////////////////////////////////////////////////////////////////////////////

// utility functions (not required but often useful)
static int __stdcall popstring(TCHAR *str)
{
	stack_t *th;
	if (!g_stacktop || !*g_stacktop) return 1;
	th=(*g_stacktop);
	if (str) lstrcpy(str,th->text);
	*g_stacktop = th->next;
	GlobalFree((HGLOBAL)th);
	return 0;
}

static int __stdcall popstringn(TCHAR *str, int n)
{
  stack_t *th;
  if (!g_stacktop || !*g_stacktop) return 1;
  th=(*g_stacktop);
  lstrcpyn(str,th->text,n);
  *g_stacktop = th->next;
  GlobalFree((HGLOBAL)th);
  return 0;
}

static void __stdcall pushstring(const TCHAR *str)
{
  stack_t *th;
  if (!g_stacktop) return;
  th=(stack_t*)GlobalAlloc(GPTR,sizeof(stack_t)+g_stringsize);
  lstrcpyn(th->text,str,g_stringsize);
  th->next=*g_stacktop;
  *g_stacktop=th;
}

static TCHAR * __stdcall getuservariable(const int varnum)
{
  if (varnum < 0 || varnum >= __INST_LAST) return NULL;
  return g_variables+varnum*g_stringsize;
}

static void __stdcall setuservariable(const int varnum, const TCHAR *var)
{
	if (var != NULL && varnum >= 0 && varnum < __INST_LAST) 
		lstrcpy(g_variables + varnum*g_stringsize, var);
}

// playing with integers
static int __stdcall myatoi(const TCHAR *s)
{
	int v=0;
	if (*s == '0' && (s[1] == 'x' || s[1] == 'X'))
	{
		s++;
		for (;;)
		{
			int c=*(++s);
			if (c >= '0' && c <= '9') c-='0';
			else if (c >= 'a' && c <= 'f') c-='a'-10;
			else if (c >= 'A' && c <= 'F') c-='A'-10;
			else break;
			v<<=4;
			v+=c;
		}
	}
	else if (*s == '0' && s[1] <= '7' && s[1] >= '0')
	{
		for (;;)
		{
			int c=*(++s);
			if (c >= '0' && c <= '7') c-='0';
			else break;
			v<<=3;
			v+=c;
		}
	}
	else
	{
		int sign=0;
		if (*s == '-') sign++; else s--;
		for (;;)
		{
			int c=*(++s) - '0';
			if (c < 0 || c > 9) break;
			v*=10;
			v+=c;
		}
		if (sign) v = -v;
	}

	return v;
}

static unsigned __stdcall myatou(const TCHAR *s)
{
	unsigned int v=0;

	for (;;)
	{
		unsigned int c=*s++;
		if (c >= '0' && c <= '9') c-='0';
		else break;
		v*=10;
		v+=c;
	}
	return v;
}

static int __stdcall myatoi_or(const TCHAR *s)
{
	int v=0;
	if (*s == '0' && (s[1] == 'x' || s[1] == 'X'))
	{
		s++;
		for (;;)
		{
			int c=*(++s);
			if (c >= '0' && c <= '9') c-='0';
			else if (c >= 'a' && c <= 'f') c-='a'-10;
			else if (c >= 'A' && c <= 'F') c-='A'-10;
			else break;
			v<<=4;
			v+=c;
		}
	}
	else if (*s == '0' && s[1] <= '7' && s[1] >= '0')
	{
		for (;;)
		{
			int c=*(++s);
			if (c >= '0' && c <= '7') c-='0';
			else break;
			v<<=3;
			v+=c;
		}
	}
	else
	{
		int sign=0;
		if (*s == '-') sign++; else s--;
		for (;;)
		{
			int c=*(++s) - '0';
			if (c < 0 || c > 9) break;
			v*=10;
			v+=c;
		}
		if (sign) v = -v;
	}

	// Support for simple ORed expressions
	if (*s == '|') 
	{
		v |= myatoi_or(s+1);
	}

	return v;
}

static int __stdcall popint()
{
	TCHAR buf[128];
	if (popstringn(buf,sizeof(buf)))
		return 0;

	return myatoi(buf);
}

static int __stdcall popint_or()
{
	TCHAR buf[128];
	if (popstringn(buf,sizeof(buf)))
		return 0;

	return myatoi_or(buf);
}

static void __stdcall pushint(int value)
{
	TCHAR buffer[1024];
#ifdef UNICODE
	wsprintf(buffer, _T("%d"), value);
#else
	sprintf_s(buffer, "%d", value);
#endif

	pushstring(buffer);
}

#endif//_EXDLL_H_
