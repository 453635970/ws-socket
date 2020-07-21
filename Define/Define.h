// 各模块共用的类型定义，通用的不会变动的基础函数
#pragma once;

#include <Windows.h>
#include <process.h>
#include <vector>
#include <string>
#include <map>
#include <tuple>

using std::vector;
using std::string;
using std::wstring;
using std::map;
using std::tuple;
using std::get;

// 这套宏可以在编译时产生自定义警告
#define STR2(x) #x  
#define STR(x)  STR2(x)  
#define MSG(desc) message(__FILE__ "(" STR(__LINE__) "): " #desc)
#pragma comment(lib, "ws2_32.lib")

// 抑制宏参太多警告
#pragma warning(disable:4002)

/* 类型定义 */

// Window.h中应该有这个类型定义
typedef unsigned long long  QWORD;
typedef unsigned long long *PQWORD;

// 循环结束回调参数
enum LoopEndType
{
	letFriInfo,
	letContact,
	letGroupMember,
};

// 用户账号类别
enum UserType
{
	utPerson, // 普通好友
	utPublic, // 公众号
	utGroup, // 群组
	utInGroup, // 同在一个群
	utSys,
};

// 性别
enum SexType
{
	stUnknow = 0,
	stMale = 1,   //男
	stFemale = 2, //女
};

// 微信运行状态/事件
enum StatusType
{
	stQuickLogin, // 快速登陆
	stScanQrcode, // 扫描二维码
	stLogout, // 退出登录
	stLogin, // 登陆
	stDetach, // 脱离
	stNotRun, // 未运行
	stNotLogin, // 未登录
};

// 好友来源
enum FromType
{
	ftQQ = 1,
	ftSearchWXID = 3,
	ftMobileContact = 0xA,
	ftGroup = 0xE,
	ftSearchMobileNum = 0xF,
	ftCard = 0x11,
	ftNearby = 0x12,
	ftQrCode = 0x1E,
	ftFacebook = 0x1F,
};

// 信息类型
enum MsgType
{
	mtText = 1, // 文本信息
	mtImage = 3, // 图片
	mtAddFriend = 0x25, // 加好友
	mtCard = 0x2a, // 名片
	mtsound = 0x22,  //语音
	mtVideo = 0x2b, //视频
	mtLink = 0x31, // 链接、转账、文件
	mtFile = 0x31, // 文件
	mtlook = 0x2f, // 表情
	mtGroupMemChange = 0x2710, // 群成员变动
				   // mtAt = -1, // 群@
};

typedef struct
{
	BOOL bAttach; // 是否为运行时附加
	WCHAR szPath[MAX_PATH]; // HookDll所在路径
	WCHAR wxdbPath[MAX_PATH];
}HookArg, *PHookArg;

/* 为了保持兼容性，添加新事件类型必须从底部添加 */
// Hook端发起的事件类型
enum EventType
{
	etDetach = 0,
	etInitComplete,
	etInitFail,
	etQuickLogin,
	etQrcode,
	etScanLogo,
	etScanUsername,
	etLogin,
	etLogout,
	etSend,
	etRecv,
	etFriInfo,
	etRunningStatus,
	etContact,
	etLoopEnd,
	etDBPW,
	etSelete,
	etFilterText,
	etGPid = 555,
};


enum SendMsgID
{
	emtext = 0,
	emimg,
	emfile,
	emcard,
	emgif,
	emattach,
};



enum RoomID
{
	setname = 0,
	setnick,
	sendNotice,
	sendcard,
};

/* 为了保持兼容性，添加新命令类型必须从底部添加 */
// 相当重要的宏，增加命令时记得增加这儿的项目
#define CommandMacro(Macro, ...)			\
	Macro(Detach, __VA_ARGS__)				\
	Macro(GetAllFriInfo, __VA_ARGS__)		\
	Macro(GetFriInfo, __VA_ARGS__) 			\
	Macro(GetGroupMember, __VA_ARGS__) 		\
	Macro(MaxWindow, __VA_ARGS__) 			\
	Macro(MinWindow, __VA_ARGS__) 			\
	Macro(CloseWindow, __VA_ARGS__) 		\
	Macro(RestoreWindow, __VA_ARGS__) 		\
	Macro(InviteToGroup, __VA_ARGS__) 		\
	Macro(SendGroupCard, __VA_ARGS__) 		\
	Macro(SetGroupNick, __VA_ARGS__) 		\
	Macro(SetGroupName, __VA_ARGS__) 		\
	Macro(SetGroupNotice, __VA_ARGS__) 		\
	Macro(DelGroupMember, __VA_ARGS__) 		\
	Macro(MultiFun, __VA_ARGS__) 		\
	Macro(CloseProxy, __VA_ARGS__) 			\
	Macro(EnableProxy, __VA_ARGS__) 		\
	Macro(ChangeRemark, __VA_ARGS__) 		\
	Macro(GetStatus, __VA_ARGS__) 			\
	Macro(CreateGroup, __VA_ARGS__) 		\
	Macro(AcceptFriend, __VA_ARGS__) 		\
	Macro(AddFriendviaGroup, __VA_ARGS__) 	\
	Macro(AddFriendviaCard, __VA_ARGS__) 	\
	Macro(GetContact, __VA_ARGS__) 			\
	Macro(SetRead, __VA_ARGS__) 			\
	Macro(Recall, __VA_ARGS__) 				\
	Macro(DeleteFriend, __VA_ARGS__) 		\
	Macro(SendText, __VA_ARGS__) 			\
	Macro(SendImage, __VA_ARGS__) 			\
	Macro(SendFile, __VA_ARGS__)	 		\
	Macro(SendCard, __VA_ARGS__) 			\
	Macro(SendLink, __VA_ARGS__)			\
	Macro(GetDBPW, __VA_ARGS__)				\
	Macro(Logout, __VA_ARGS__)				\
	Macro(SendGif, __VA_ARGS__)             \
	Macro(SetFilterText, __VA_ARGS__)       \
	Macro(AddContact, __VA_ARGS__)        \
	Macro(SetREText, __VA_ARGS__)    \
	Macro(ISfriends, __VA_ARGS__) 	\
	Macro(GetAllGroupMember, __VA_ARGS__)  \
	Macro(PayMents, __VA_ARGS__)  \
	Macro(OpenUrl, __VA_ARGS__)   \
	Macro(RelayFile, __VA_ARGS__)  \
	Macro(SendLook, __VA_ARGS__)   \
	Macro(RelayLook, __VA_ARGS__)  \
	Macro(ViaImFri, __VA_ARGS__)   \
	Macro(SetAllRead, __VA_ARGS__)  \
	Macro(ToFav, __VA_ARGS__)   \
	Macro(RelayProgram, __VA_ARGS__)  \
	Macro(SendProgram, __VA_ARGS__)  \
	Macro(RefreshQR, __VA_ARGS__) 





#define CommandTypeCreate(Name)	\
	ct##Name,
// Control端发起的命令类型
enum CommandType
{
	CommandMacro(CommandTypeCreate)
};

// 用户信息类型
enum UserInfoType
{
	uitWXID = 0,
	uitUsername,
	uitRemark, // 好友限定
	uitUID,
	uitUIN,
	uitAddr1,
	uitAddr2,
	uitAddr3,
	uitPhone,
	uitLogo1, // Big
	uitLogo2, // Small
	uitSex, // 未实现
	uitFrom, // 未实现
	uitType, // 好友类型
	uitNick, // 群内昵称
	uitGroup, // 归属的群
	uitMaster, // 群主wxid
	uitSum, // 群成员数
	uitPname, // 拼音
};

// 用户信息结构体
typedef struct
{
	const wchar_t* pwWXID;
	const wchar_t* pwUID;
	const wchar_t* pwName;
	const wchar_t* pwRemark;
	unsigned int dwUIN;
	const wchar_t* pwAddr1;
	const wchar_t* pwAddr2;
	const wchar_t* pwAddr3;
	const wchar_t* pwPhone;
	const wchar_t* pwLogo1;
	const wchar_t* pwLogo2;
	SexType stSex;
	UserType utType;
	FromType ftFrom;
	const wchar_t* pwNick; // 群内昵称
	const wchar_t* pwGroup; // 群好友所在群
	const wchar_t* pwMaster; // 群主
	unsigned int dwSum; // 群成员数量
	const wchar_t* pPinName;// 拼音排序
} UserInfo, *PUserInfo;


struct MyDisturb
{
	PVOID p1;
	DWORD *pVa;
};
// 跨进程通信用
typedef vector< vector<byte> > DataList;
/* 函数定义 */
// 模板函数，生成时不展开。。。。
// 打印调试字符，配合DebugView使用
template<typename... T>
void DbgLog(T... args);

// 检测是指针还是指针的指针
template<typename T>
T TestPtr(T p);
wstring BV2W(const vector<byte> ba);
const wchar_t* BV2WC(vector<byte>& bv);
vector<byte> W2BV(const wstring ws);
vector<byte> W2BV(const wchar_t* pws);
vector<wstring> DL2WV(const DataList& dl);
vector<byte> I2BV(int i);
int BV2I(const vector<byte> bv);
vector<byte> L2BV(unsigned long long l);
unsigned long long BV2L(const vector<byte> bv);
int count(wstring ws1, wstring sw2);
vector<wstring> split(wstring source, wstring sep = L"^G", unsigned int maxsplit = -1);
BOOL WriteSocketHk(SOCKET sk, EventType t, DataList vvbDatas);//DataList 传引用和对象都一样, 都是push地址
BOOL WriteSocket(SOCKET sk, CommandType t, DataList vvbDatas);
DataList ReadSocketHk(SOCKET& sk);
DataList ReadSocketCt(SOCKET& sk);
wstring stdformat(const wchar_t *fmt, ...);
string stdformat(const char *fmt, ...);
int stdReplace(string& strContent, const  char *pstrRe, const char *pstrDest);
int stdReplace(wstring& strContent, const  wchar_t *pstrRe, const wchar_t *pstrDest);
wstring UTF8216(const char* pwU8);
string  UTF1628(const wchar_t* psU16);
wstring mb2u16(const char *pbuf);
string  u162mb(const wchar_t *pbuf);
string mb2u8(const char* pbuf);
string u82mb(const char* pbuf);
void splitformat(wstring& out, const wchar_t*p1, const wchar_t* p2);
void splitformat(wstring& out, const wchar_t*p1, int p2);
void TimeLimit();
wstring GetPathName(const wchar_t *p);
SYSTEMTIME TimetToSystemTime(time_t t);

vector<wstring> Format2V(wstring str, const wchar_t *pfen);

wstring substr1(const wchar_t *pdata, const wchar_t *p1, const wchar_t *p2);
wstring substr2(const wchar_t *pdata, const wchar_t *p1);




#define SocketPort 9963
#define SIZEHK 1024*10
#define SIZECT 1024*10
#define SIZEMAX 10240+300


