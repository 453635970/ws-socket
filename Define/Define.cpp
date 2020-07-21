#include "Define.h"
#include <time.h>
template<typename... T>
void DbgLog(T... args)
{
#ifdef _DEBUG
	WCHAR szTemp[512];
	swprintf_s(szTemp, args...);
	OutputDebugStringW(szTemp);
	return;
#else
	return;
#endif 
}

template<typename T>
T TestPtr(T p)
{
	__try
	{
		if (!IsBadWritePtr(*(T*)p, 128))
			return (T)p;
		else
			return *(T*)p;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return (T)p;
	}
}



vector<wstring> DL2WV(const DataList& dl)
{
	vector<wstring> wv;
	for (auto i = dl.begin(); i < dl.end(); i++)
		wv.push_back(BV2W(*i));
	return wv;
}


vector<byte> W2BV(const wstring ws)
{
	auto p = reinterpret_cast<const byte*>(ws.c_str());
	return vector<byte>(p, p + ws.size() * 2);
}

vector<byte> W2BV(const wchar_t* pws)
{
	wstring ws = pws;
	auto p = reinterpret_cast<const byte*>(ws.c_str());
	return vector<byte>(p, p + ws.size() * 2);
}


wstring BV2W(const vector<byte> bv)
{
	if (bv.size() > 0) {
		auto p = reinterpret_cast<const wchar_t*>(bv.data());
		return wstring(p, p + bv.size() / 2);
	}
	else 
		return wstring(L"");
}


const wchar_t* BV2WC(vector<byte>& bv)
{
	bv.push_back(0);
	bv.push_back(0);
	auto r = reinterpret_cast<const wchar_t*>(bv.data());
	bv.pop_back();
	bv.pop_back();
	return r;
}

vector<wstring> split(wstring source, wstring sep, unsigned int maxsplit)
{
	unsigned int i = 0;
	vector<wstring> vs;
	size_t st1, st2, st_seq = sep.size();
	st1 = source.find(sep);
	if (st1 == wstring::npos)
		return vector<wstring>();
	vs.push_back(source.substr(0, st1));
	i++;
	st2 = source.find(sep, st1 + st_seq);
	while (st2 != wstring::npos && i < maxsplit)
	{
		i++;
		vs.push_back(source.substr(st1 + st_seq, st2 - st1 - st_seq));
		st1 = st2;
		st2 = source.find(sep, st1 + st_seq);
	}
	if (st1 + st_seq != source.size())
	{
		vs.push_back(source.substr(st1 + st_seq));
	}
	return vs;
}

vector<byte> I2BV(int i)
{
	auto inter = reinterpret_cast<byte*>(&i);
	vector<byte> bv(inter, inter + 4);
	return bv;
}

int BV2I(const vector<byte> bv)
{
	if (bv.size()>0)
		return *reinterpret_cast<const int*>(bv.data());
	return 0;
}

vector<byte> L2BV(unsigned long long l)
{
	auto inter = reinterpret_cast<byte*>(&l);
	vector<byte> bv(inter, inter + 8);
	return bv;
}

unsigned long long BV2L(const vector<byte> bv)
{
	if (bv.size()>0)
		return *reinterpret_cast<const unsigned long long*>(bv.data());
	return 0;
}

int count(wstring ws1, wstring ws2)
{
	int i = 0;
	size_t st1, st2;
	st1 = ws1.find(ws2);
	if (st1 == wstring::npos)
		return 0;
	i++;
	st2 = ws1.find(ws2, st1 + ws2.size());
	while (st2 != wstring::npos)
	{
		i++;
		st1 = st2;
		st2 = ws1.find(ws2, st1 + ws2.size());
	}
	return i;
}


/*单向10车道 稳定的socket通信---------------------------------------------------------------------------------------------------------------------------------*/
//控制端写
BOOL WriteSocket(SOCKET sk, CommandType ct, DataList vvbDatas)
{
	__try
	{
		if (sk < 1)
			return FALSE;
		SIZE_T stMax = SIZECT;
		vector<byte> vBuf;
		vBuf.reserve(SIZEMAX);
		PBYTE pMem = (PBYTE)vBuf.data();
		*(PDWORD)pMem = 0x11111111;
		pMem += 4;
		*(PDWORD)pMem = ct;
		pMem += 4;
		*(PDWORD)pMem = vvbDatas.size();
		pMem += 4;

		/* 4个bit(固定:0x11111111) |  4bit(事件类型)| 4bit(元素个数) | (元素长度(4bit)|内容)| ...|(元素长度(4bit)|内容) */
		for (auto iter = vvbDatas.begin(); iter < vvbDatas.end(); iter++)
		{
			*(PDWORD)pMem = iter->size();
			pMem += 4;
			if (iter->size() + (pMem - vBuf.data()) > SIZECT)
				return FALSE;
			memcpy(pMem, iter->data(), iter->size());
			pMem += iter->size();
		}
		//if (send(sk, (const char *)pWriteBuf, pMem - pWriteBuf, 0) == SOCKET_ERROR)
		if (send(sk, (const char *)vBuf.data(), SIZECT, 0) < 1)
		{
			DWORD err = GetLastError();
			DbgLog(L"WX: WriteSocket Error 控制: %d", err);
			return FALSE;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{

	}
}

//HOOK端写
BOOL WriteSocketHk(SOCKET sk, EventType et, DataList vvbDatas)
{
	__try
	{
		if (sk < 1)
			return FALSE;
		vector<byte> vBuf;
		vBuf.reserve(SIZEMAX);
		PBYTE pMem = (PBYTE)vBuf.data();
		*(PDWORD)pMem = 0x11111111;
		pMem += 4;
		*(PDWORD)pMem = et;
		pMem += 4;
		*(PDWORD)pMem = vvbDatas.size();
		pMem += 4;
		/* 事件类型 | 元素个数 | （元素长度|内容） | （同上）。。。 */
		for (auto iter = vvbDatas.begin(); iter < vvbDatas.end(); iter++)
		{
			*(PDWORD)pMem = iter->size();
			pMem += 4;
			if (iter->size() + (pMem - vBuf.data()) > SIZEHK)
				return FALSE;
			memcpy(pMem, iter->data(), iter->size());
			pMem += iter->size();
		}

		if (send(sk, (const char *)vBuf.data(), SIZEHK, 0) < 1)
		{
			DWORD err = GetLastError();
			DbgLog(L"WX: WriteSocket Error hook: %d", err);
			return FALSE;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{

	}
}

int  __cou = 0;
byte _cbuf2level1[SIZEMAX] = { 0 };
byte _cbuf2level2[SIZEMAX] = { 0 };
//HOOK 读取控制端
/* 校验 | 事件类型 | 数组长度 | （元素长度|内容） | （同上）。。。 */
DataList ReadSocketHk(SOCKET& sk)
{
	vector< vector<byte> > vv;
	
	BYTE *pReadBuf = (BYTE *)malloc(SIZEMAX);
	memset(pReadBuf, 0, SIZEMAX);
	if (sk > 0)
	{
		int ret = recv(sk, (char*)pReadBuf, SIZECT, 0);
		PBYTE pMem = (PBYTE)pReadBuf;
		if (ret == -1 || ret == 0)
		{
			byte bts[8] = { 0x22, 0x23,0x24,0x33,0x34,0x34,0x34 };
			vv.push_back(vector<byte>(bts, bts + 5));
			free(pReadBuf);
			return vv;
		}

		BOOL isok = FALSE;
		if (ret < SIZECT)
		{
			//包头,固定缓存区|收|发 大小
			if (*(PDWORD)pMem == 0x11111111)
			{
				__cou = 0;
				ZeroMemory(_cbuf2level1, sizeof(_cbuf2level1));
				memcpy(_cbuf2level1, pReadBuf, ret);
				__cou += ret;
			}
			else
			{
				int sur = SIZECT - __cou;
				if (sur > ret)//一个包内
				{
					memcpy(_cbuf2level1 + __cou, pReadBuf, ret);
					__cou += ret;
					free(pReadBuf);
					return vv;//返回一个空
				}
				else if (sur == ret)//一个包完结
				{
					memcpy(_cbuf2level1 + __cou, pReadBuf, ret);
					__cou = 0;
					pMem = (PBYTE)_cbuf2level1;
					if (*(PDWORD)pMem == 0x11111111)
						isok = TRUE;
				}
				else if (sur < ret)//2个包尾 和头在一起
				{
					memcpy(_cbuf2level1 + __cou, pReadBuf, sur);
					ZeroMemory(_cbuf2level2, sizeof(_cbuf2level2));
					memcpy(_cbuf2level2, _cbuf2level1, SIZECT);
					pMem = (PBYTE)_cbuf2level2;
					if (*(PDWORD)pMem == 0x11111111)
					{
						isok = TRUE;
						__cou = 0;
						ZeroMemory(_cbuf2level1, sizeof(_cbuf2level1));
						memcpy(_cbuf2level1, pReadBuf + sur, ret - sur);
						__cou += (ret - sur);
					}
				}
			}
		}

		if (ret == SIZECT && (*(PDWORD)pMem == 0x11111111))
			isok = TRUE;
		/*---------------------------------------------------------*/
		if (isok)
		{
			pMem += 4;
			int nEVENT = (*(PDWORD)(pMem));// 事件类型
			pMem += 4;
			SIZE_T nMaxCou = *(PDWORD)pMem; 

			SIZE_T nlen=0;                //元素长度
			pMem += 4;
			for (SIZE_T i = 0; i < nMaxCou; i++)
			{
				nlen = *(PDWORD)pMem;
				pMem += 4;
				vv.push_back(vector<byte>(pMem, pMem + nlen));
				pMem += nlen;
			}
			vv.push_back(I2BV(nEVENT)); //事件类型
			free(pReadBuf);
			return vv;
		}
	}
	free(pReadBuf);
	return vv;
}

map<int, byte *>  _pbuf2level1;
map<int, byte *>  _pbuf2level2;
map<int, int>     _cou1M;
//读hook端的写
/* 校验 | 事件类型 | 数组长度 | （元素长度|内容） | （同上）。。。 */
DataList ReadSocketCt(SOCKET& sk)
{
	int nNo = (int)sk;
	if (_pbuf2level1.find(nNo) == _pbuf2level1.end())
	{
		byte *pt = (byte *)malloc(SIZEMAX);
		ZeroMemory(pt, SIZEMAX);
		_pbuf2level1.insert(map<int, byte *>::value_type(nNo, pt));
	}
	if (_pbuf2level2.find(nNo) == _pbuf2level2.end())
	{
		byte *pt1 = (byte *)malloc(SIZEMAX);
		ZeroMemory(pt1, SIZEMAX);
		_pbuf2level2.insert(map<int, byte *>::value_type(nNo, pt1));
	}
	if (_cou1M.find(nNo) == _cou1M.end())
	{
		int tcou = 0;
		_cou1M.insert(map<int, int>::value_type(nNo, tcou));
	}

	byte* _hbuf2level1 = _pbuf2level1[nNo];
	byte* _hbuf2level2 = _pbuf2level2[nNo];
	vector<vector<byte>> vv;

	BYTE *pReadBuf = (BYTE *)malloc(SIZEMAX);
	memset(pReadBuf, 0, SIZEMAX);
	if (sk>0)
	{
		int ret = recv(sk, (char*)pReadBuf, SIZEHK, 0);
		PBYTE pMem = (PBYTE)pReadBuf;
		if (ret == -1 || ret == 0)
		{//socket已完蛋

			byte bts[8] = {0x22, 0x23,0x24,0x33,0x34,0x34,0x34 };
			vv.push_back(vector<byte>(bts, bts + 5));
			free(pReadBuf);
			return vv;
		}

		BOOL isok = FALSE;
		//出现了粘包
		if (ret < SIZEHK)
		{
			//包头,固定缓存区|收|发 大小
			if (*(PDWORD)pMem == 0x11111111)
			{
				_cou1M[nNo] = 0;
				ZeroMemory(_hbuf2level1, SIZEHK);
				memcpy(_hbuf2level1, pReadBuf, ret);
				_cou1M[nNo] += ret;
			}
			else
			{
				int sur = SIZEHK - _cou1M[nNo];
				if (sur > ret)//一个包内
				{
					memcpy(_hbuf2level1 + _cou1M[nNo], pReadBuf, ret);
					_cou1M[nNo] += ret;
					free(pReadBuf);
					return vv;//返回一个空
				}
				else if (sur == ret)//一个包完结
				{
					memcpy(_hbuf2level1 + _cou1M[nNo], pReadBuf, ret);
					_cou1M[nNo] = 0;
					pMem = (PBYTE)_hbuf2level1;
					if (*(PDWORD)pMem == 0x11111111)
						isok = TRUE;
				}
				else if (sur < ret)//2个包尾 和头在一起
				{
					memcpy(_hbuf2level1 + _cou1M[nNo], pReadBuf, sur);
					ZeroMemory(_hbuf2level2, SIZEHK);
					memcpy(_hbuf2level2, _hbuf2level1, SIZEHK);
					pMem = (PBYTE)_hbuf2level2;
					if (*(PDWORD)pMem == 0x11111111)
					{
						isok = TRUE;
						_cou1M[nNo] = 0;
						ZeroMemory(_hbuf2level1, SIZEHK);
						memcpy(_hbuf2level1, pReadBuf + sur, ret - sur);
						_cou1M[nNo] += (ret - sur);
					}
				}
			}
		}

		if (ret == SIZECT && (*(PDWORD)pMem == 0x11111111))
			isok = TRUE;
		
		if (isok)
		{
			pMem += 4;
			int nEVENT = (*(PDWORD)(pMem));//事件类型
			pMem += 4;
			SIZE_T nMaxCou = *(PDWORD)pMem; 
			SIZE_T nlen=0;//元素长度
			pMem += 4;
			for (SIZE_T i = 0; i < nMaxCou; i++)
			{
				nlen = *(PDWORD)pMem;
				pMem += 4;
				vv.push_back(vector<byte>(pMem, pMem + nlen));
				pMem += nlen;
			}
			vv.push_back(I2BV(nEVENT)); //事件类型
			free(pReadBuf);
			return vv;
		}
	}
	free(pReadBuf);
	return vv;
}
/*-------------------------------------------------------------------------------------------------------------------------------*/



/*完美字符集转换---------------------------------------------------------------------------------------------------------------------------------*/
string u162mb(const wchar_t *pbuf)
{
	string strText;
	if (pbuf && wcslen(pbuf) > 0)
	{
		wstring str = pbuf;
		int iLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
		char*  pText = new char[iLen];
		memset((void*)pText, 0, iLen);
		::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pText, iLen, NULL, NULL);
		strText = pText;
		delete pText;
	}
	return strText;
}
wstring mb2u16(const char *pbuf)
{
	wstring  strText;
	if (pbuf && strlen(pbuf) > 0)
	{
		string str = pbuf;
		int  iLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t * pText = new  wchar_t[iLen];
		memset(pText, 0, iLen * 2);
		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pText, iLen);
		strText = (wchar_t*)pText;
		delete  pText;
	}
	return  strText;
}
wstring UTF8216(const char* pbuf)
{
	wstring  strText;
	if (pbuf && strlen(pbuf) > 0)
	{
		string str = pbuf;
		int ilen = MultiByteToWideChar(CP_UTF8, NULL, str.c_str(), -1, NULL, NULL);
		wchar_t* pText = new wchar_t[ilen];
		memset((void*)pText, 0, ilen * 2);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pText, ilen);
		strText = pText;
		delete pText;
	}
	return strText;
}
string UTF1628(const wchar_t* pbuf)
{
	string  strText;
	if (pbuf && wcslen(pbuf) > 0)
	{
		wstring str = pbuf;
		int len = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, 0, 0, 0, 0);
		char*  pText = new char[len];
		memset((void*)pText, 0, len);
		::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pText, len, 0, 0);
		strText = pText;
		delete pText;
	}
	return strText;
}
string mb2u8(const char* pbuf)
{
	string sre;
	wstring  strText = mb2u16(pbuf);
	sre = UTF1628(strText.c_str());
	return sre;
}
string u82mb(const char* pbuf)
{
	string sre;
	wstring strText = UTF8216(pbuf);
	sre = u162mb(strText.c_str());
	return sre;
}
/*-------------------------------------------------------------------------------------------------------------------------------*/




int stdReplace(wstring& strContent, const  wchar_t *pstrRe, const wchar_t *pstrDest)
{
	wstring  strReplace = pstrRe;
	while (true)
	{
		size_t pos = strContent.find(strReplace);
		if (pos != std::wstring::npos)
		{
			WCHAR pBuf[1] = { L'\0' };
			strContent.replace(pos, strReplace.length(), pBuf, 0);
			strContent.insert(pos, pstrDest);
		}
		else
		{
			break;
		}
	}
	return 0;
}

int stdReplace(string& strContent, const  char *pstrRe, const char *pstrDest)
{
	string  strReplace = pstrRe;
	while (true)
	{
		size_t pos = strContent.find(strReplace);
		if (pos != std::string::npos)
		{
			char pBuf[1] = { '\0' };
			strContent.replace(pos, strReplace.length(), pBuf, 0);
			strContent.insert(pos, pstrDest);
		}
		else
			break;
	}
	return 0;
}

string stdformat(const char *fmt, ...)
{
	std::string strResult = "";
	if (NULL != fmt)
	{
		va_list marker = NULL;
		va_start(marker, fmt);
		size_t nLength = _vscprintf(fmt, marker) + 1;
		std::vector<char> vBuffer(nLength, '\0');
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker);
	}
	return strResult;
}

wstring stdformat(const wchar_t *fmt, ...)
{
	std::wstring strResult = L"";
	if (NULL != fmt)
	{
		va_list marker = NULL;
		va_start(marker, fmt);
		size_t nLength = _vscwprintf(fmt, marker) + 1;
		std::vector<wchar_t> vBuffer(nLength, L'\0');
		int nWritten = _vsnwprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker);
	}
	return strResult;
}

void splitformat(wstring& out, const wchar_t*p1, const wchar_t* p2)
{
	out += '\"';
	out += p1;
	out += '\"';
	out += ':';
	out += '\"';
	out += p2;
	out += '\"';
	out += L"@sdk|@";
}
void splitformat(wstring& out, const wchar_t*p1, int p2)
{
	out += '\"';
	out += p1;
	out += '\"';
	out += ':';
	wchar_t sbuf[64] = { 0 };
	swprintf_s(sbuf, L"%d", p2);
	out += sbuf;
	out += L"@sdk|@";
}


SYSTEMTIME TimetToSystemTime(time_t t)
{
	FILETIME ft;
	SYSTEMTIME pst;
	LONGLONG nLL = Int32x32To64(t, 10000000) + 116444736000000000;
	ft.dwLowDateTime = (DWORD)nLL;
	ft.dwHighDateTime = (DWORD)(nLL >> 32);
	FileTimeToSystemTime(&ft, &pst);
	return pst;
}


void TimeLimit()
{
	if (1)
	{
	}
	else
	{
		struct tm t = { 0 };
		time_t now;
		time(&now);
		localtime_s(&t, &now);
		if ((t.tm_year + 1900 != 2019) && (t.tm_mon + 1 > 5) && ((t.tm_mon + 1 == 5) && (t.tm_mday > 20)))
		{
			exit(0);
			ExitProcess(0);
		}
	}
}



wstring GetPathName(const wchar_t *p)
{
	wstring re;
	wstring st = p;
	int sub = st.rfind(L"\\");
	if (sub != -1)
		re = st.substr(sub + 1, st.length() - sub - 1);
	return re;
}


vector<wstring> Format2V(wstring str, const wchar_t *pfen)
{
	vector<wstring> V;
	wstring sjx = str;
	while (1)
	{
		int sub = sjx.find(pfen);
		if (sub != -1) {
			wstring st = sjx.substr(0, sub);
			V.push_back(st);
			sjx = sjx.substr(sub + 1, sjx.length());
		}
		else {
			if (sjx.size()>0)
				V.push_back(sjx);
			break;
		}
	}
	return V;
}


wstring substr1(const wchar_t *pdata, const wchar_t *p1, const wchar_t *p2)
{
	wstring re;
	wstring data = pdata;
	int sub = data.find(p1);
	int sub1 = data.find(p2);
	if (sub != -1 && sub1 != -1 && sub1 > sub)
		re = data.substr(sub, sub1 - sub);
	return re;
}

wstring substr2(const wchar_t *pdata, const wchar_t *p1)
{
	wstring re;
	wstring data = pdata;
	int sub = data.find(p1);
	if (sub != -1)
	{
		int len = wcslen(p1);
		int sub1 = data.find(L"\"", len + sub);
		if (sub1 != -1 && sub1 > (sub + len))
			re = data.substr(sub+ len, sub1 - sub- len);
	}
	return re;
}