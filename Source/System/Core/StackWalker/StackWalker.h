// #pragma once is supported starting with _MCS_VER 1000, 
// so we need not to check the version (because we only support _MSC_VER >= 1100)!
#pragma once

#include <windows.h>


class StackWalkerInternal;  // forward


/*
*
*/
class StackWalker
{
	friend StackWalkerInternal;

	public:

	typedef enum StackWalkOptions
	{
		RetrieveNone		= 0,	// No addition info will be retrived (only the address is available)
		RetrieveSymbol		= 1,	// Try to get the symbol-name
		RetrieveLine		= 2,	// Try to get the line for this symbol
		RetrieveModuleInfo	= 4,	// Try to retrieve the module-infos
		RetrieveFileVersion	= 8,	// Also retrieve the version for the DLL/EXE
		RetrieveVerbose		= 0xF,	// Contains all the above
		SymBuildPath		= 0x10,	// Generate a "good" symbol-search-path
		SymUseSymSrv		= 0x20,	// Also use the public Microsoft-Symbol-Server
		SymAll				= 0x30,	// Contains all the abouve "Sym"-options
		OptionsAll			= 0x3F	// Contains all options (default)
	} StackWalkOptions;

	StackWalker(
		int		options		= OptionsAll, // 'int' is by design, to combine the enum-flags
		LPCSTR	szSymPath	= NULL, 
		DWORD	dwProcessId	= GetCurrentProcessId(), 
		HANDLE	hProcess	= GetCurrentProcess()
	);

	StackWalker( DWORD dwProcessId, HANDLE hProcess );

	virtual ~StackWalker();

	typedef BOOL ( __stdcall *PReadProcessMemoryRoutine )(
		HANDLE      hProcess,
		DWORD64     qwBaseAddress,
		PVOID       lpBuffer,
		DWORD       nSize,
		LPDWORD     lpNumberOfBytesRead,
		LPVOID      pUserData  // optional data, which was passed in "ShowCallstack"
    );

	BOOL LoadModules();

	BOOL ShowCallstack(
		HANDLE hThread = GetCurrentThread(), 
		const CONTEXT *context = NULL, 
		PReadProcessMemoryRoutine readMemoryFunction = NULL,
		LPVOID pUserData = NULL,// optional to identify some data in the 'readMemoryFunction'-callback
		char *	pDestBuffer	= 0,// буффер в который запишется текст call stack
		int		nMaxSize	= 0	// максимальная длинна текста (включая null-term)
	);

	protected:

	enum { STACKWALK_MAX_NAMELEN = 1024 }; // max name length for found symbols

	typedef struct CallstackEntry
	{
		DWORD64 offset;  // if 0, we have no valid entry
		CHAR	name[STACKWALK_MAX_NAMELEN];
		CHAR	undName[STACKWALK_MAX_NAMELEN];
		CHAR	undFullName[STACKWALK_MAX_NAMELEN];
		DWORD64	offsetFromSmybol;
		DWORD	offsetFromLine;
		DWORD	lineNumber;
		CHAR	lineFileName[STACKWALK_MAX_NAMELEN];
		DWORD	symType;
		LPCSTR	symTypeString;
		CHAR	moduleName[STACKWALK_MAX_NAMELEN];
		DWORD64	baseOfImage;
		CHAR	loadedImageName[STACKWALK_MAX_NAMELEN];
	} CallstackEntry;

	typedef enum CallstackEntryType
	{
		firstEntry,
		nextEntry,
		lastEntry
	};

	virtual void OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName);
	virtual void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion);
	virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry);
	virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr);
	virtual void OnOutput(LPCSTR szText);

	virtual int	CallstackEntryToString(
		const CallstackEntry &	entry,			// ссылка на запись
		char *					pDestBuffer	= 0,// буффер в который запишется текст call stack
		uint					nMaxSize	= 0	// максимальная длинна текста (включая null-term)
	);

	StackWalkerInternal *m_sw;
	HANDLE m_hProcess;
	DWORD m_dwProcessId;
	BOOL m_modulesLoaded;
	LPSTR m_szSymPath;

	int m_options;

	static BOOL __stdcall myReadProcMem(HANDLE hProcess, DWORD64 qwBaseAddress, PVOID lpBuffer, DWORD nSize, LPDWORD lpNumberOfBytesRead);
};


// The "ugly" assembler-implementation is needed for systems before XP
// If you have a new PSDK and you only compile for XP and later, then you can use 
// the "RtlCaptureContext"
// Currently there is no define which determines the PSDK-Version... 
// So we just use the compiler-version (and assumes that the PSDK is 
// the one which was installed by the VS-IDE)

// INFO: If you want, you can use the RtlCaptureContext if you only target XP and later...
//       But I currently use it in x64/IA64 environments...
//#if defined(_M_IX86) && (_WIN32_WINNT <= 0x0500) && (_MSC_VER < 1400)

#if defined(_M_IX86)

	// The following should be enough for walking the callstack...
	#define GET_CURRENT_CONTEXT(c, contextFlags)\
	do											\
	{											\
		memset(&c, 0, sizeof(CONTEXT));			\
		c.ContextFlags = contextFlags;			\
		__asm    call x							\
		__asm x: pop eax						\
		__asm    mov c.Eip, eax					\
		__asm    mov c.Ebp, ebp					\
		__asm    mov c.Esp, esp					\
	} while( 0 );

#else

	// The following is defined for x86 (XP and higher), x64 and IA64:
	#define GET_CURRENT_CONTEXT(c, contextFlags)\
	do											\
	{											\
		memset(&c, 0, sizeof(CONTEXT));			\
		c.ContextFlags = contextFlags;			\
		RtlCaptureContext(&c);					\
	} while( 0 );

#endif