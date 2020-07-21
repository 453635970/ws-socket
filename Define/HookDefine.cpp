#include "HookDefine.h"
#include <Psapi.h>
#pragma comment (lib,"Psapi.lib")

BOOL GetWXVersion(PWSTR szVersion)
{
	DWORD dwSize = 0;
	BYTE *pbVersionInfo = NULL;
	VS_FIXEDFILEINFO *pFileInfo = NULL;
	UINT puLenFileInfo = 0;

	WCHAR szPath[MAX_PATH] = { 0 };
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());
	if (hProcess && !GetModuleFileNameEx(hProcess, NULL, szPath, MAX_PATH))
		return FALSE;
	*wcsrchr(szPath, '\\') = 0; // 路径中不可能没'\'
	wcscat_s(szPath, L"\\WeChatWin.dll");
	
	dwSize = GetFileVersionInfoSizeW(szPath, NULL);
	if (dwSize == 0)
		return FALSE;
	
	pbVersionInfo = new BYTE[dwSize];
	memset(pbVersionInfo, 0, dwSize);
	if (!GetFileVersionInfoW(szPath, 0, dwSize, pbVersionInfo))
	{
		//delete[] pbVersionInfo;
		return FALSE;
	}
	if (!VerQueryValueW(pbVersionInfo, TEXT("\\"), (LPVOID*)&pFileInfo, &puLenFileInfo))
	{
		//delete[] pbVersionInfo;
		return FALSE;
	}
	wsprintfW(szVersion, L"%d.%d.%d.%d",
		(pFileInfo->dwFileVersionMS >> 16) & 0xffff,
		(pFileInfo->dwFileVersionMS >> 0) & 0xffff,
		(pFileInfo->dwFileVersionLS >> 16) & 0xffff,
		(pFileInfo->dwFileVersionLS >> 0) & 0xffff
	);
	//delete[] pbVersionInfo;
	return TRUE;
}


//微信 全局堆上(非CRT堆)
PVOID WXHeapAlloc(SIZE_T stMemSize)
{
	//if (!pwxHeap)
	//	return NULL;
	//return HeapAlloc(*pwxHeap, 0, stMemSize);
	HANDLE hHeap = GetProcessHeap();
	if (hHeap)
	{
		LPVOID pAlloc = HeapAlloc(hHeap, 0, stMemSize);
		if (pAlloc)
		{
			memset(pAlloc, 0, stMemSize);
			return pAlloc;
		}
	}
	return 0;
}

const wchar_t* WXStringAlloc(const wchar_t* pwStr)
{
	size_t stLen = wcslen(pwStr) + 1;
	PWSTR szStr = (PWSTR)WXHeapAlloc(stLen * 2);
	if (!szStr)
		return NULL;
	wcscpy_s(szStr, stLen, pwStr);
	return szStr;
}

const char* WXStringAlloc(const char* psStr)
{
	size_t stLen = strlen(psStr) + 1;
	PSTR szStr = (PSTR)WXHeapAlloc(stLen + 1);
	if (!szStr)
		return NULL;
	strcpy_s(szStr, stLen, psStr);
	return szStr;
}

FakeNotifyString __fastcall FakeGetName(PVOID pObject, DWORD NOUSE, FakeNotifyString pMem)
{
	/*pMem->pwName = pMem->szName;
	PWSTR pwFakeStr = (PWSTR)((DWORD)pObject + 12);
	wcscpy_s(pMem->pwName, 100, pwFakeStr);
	return pMem;*/

	pMem->pwName = pMem->szName;
	PWSTR pwFakeStr = (PWSTR)((DWORD)pObject + 12);
	lstrcpyW(pMem->pwName, pwFakeStr);
	return pMem;
} 

//BOOL MakeFakeNotify(PFakeNotify pfn, const wchar_t* pwEvent, const wchar_t* pwObject)
//{
//	if (!pfn)
//		return FALSE;
//	pfn->pwType = (pfn->szType);
//	wcscpy_s(pfn->szType, wcslen(pwEvent), pwEvent);
//	pfn->pFakeObject = &(pfn->pFakeVTable);
//	pfn->pFakeVTable = &(pfn->pFakeVTable);
//	pfn->pFakeFunc = FakeGetName;
//	pfn->pFakeName = (pfn->szFakeName);
//	wcscpy_s(pfn->pFakeName, wcslen(pwObject), pwObject);
//	return TRUE;
//}

BOOL MakeFakeNotify(PFakeNotify pfn, const wchar_t* pwEvent, const wchar_t* pwObject)
{
	if (!pfn)
		return FALSE;
	pfn->pwType = (pfn->szType);
	lstrcpyW(pfn->szType, pwEvent);

	pfn->pFakeObject = &(pfn->pFakeVTable);
	pfn->pFakeVTable = &(pfn->pFakeVTable);
	pfn->pFakeFunc = (FakeFunc)FakeGetName;
	pfn->pFakeName = (pfn->szFakeName);
	lstrcpyW(pfn->pFakeName, pwObject);
	return TRUE;
} // MakeFakeNotify