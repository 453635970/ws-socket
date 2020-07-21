// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。



wstring GetModulePathW()
{
	wchar_t wspath[MAX_PATH] = { 0 };
	GetModuleFileNameW(GetModuleHandle(0), wspath, MAX_PATH);
	wstring wsFullPath = wspath;
	wstring wxre;
	int nPos = wsFullPath.rfind('\\');
	if (nPos != -1)
		wxre = wsFullPath.substr(0, nPos);
	return wxre;
}
void WriteLog(const char * err)
{
	wstring pa = GetModulePathW();
	pa += L"\\record.log";
	SYSTEMTIME st;
	GetLocalTime(&st);
	FILE *fpread = 0;
	fpread = fopen(u162mb(pa.c_str()).c_str(), "a+");
	if (fpread) {
		string time = stdformat("%d/%d/%d-%d:%d：%s\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, err);
		fwrite(time.c_str(), time.size(), 1, fpread);
		fflush(fpread);
		fclose(fpread);
		fpread = 0;
	}
}
string g_iphost = "ws://192.168.77.210:9695"; 
