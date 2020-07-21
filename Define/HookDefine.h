// 只有Hook端才用得到的变量定义
#include <Windows.h>
#include "../Define/Define.h"
#pragma comment(lib, "version.lib")


/*
	宏函数
	部分宏在需要的位置定义
*/

// 初始化微信函数指针
#define Setpwxf(Name, Offset)	\
	pp->pwxf##Name = (ft##Name)((DWORD)pBase + Offset)
// 初始化微信数据指针
#define Setpwx(Name, Type, Offset)	\
	pp->pwx##Name = (Type)((DWORD)pBase + Offset)
#define SetNULL(Name)			\
	pp->pwxf##Name = NULL;
// Hook处理函数生成
#define HookHandlerCreate(Name)		\
	PVOID hh##Name(PVOID pArg);
// 命令处理函数生成
#define CommandHandlerCreate(Name)	\
	VOID ch##Name(const DataList& dl);
// 设定Hook处理函数及处理方法
#define SetHookHandler(Name, HandleType, phh)	\
	phh->hhc##Name.ht = HandleType;	\
	phh->hhc##Name.fthh = hh##Name;
// 设定命令处理函数
#define SetCommandHandler(Name, pch)	\
	pch[ct##Name] = ch##Name;
// 保存需要的微信函数指针
#define StoreFuncPtr(Name, pp)				\
	pwxf##Name = pp->##pwxf##Name;
// 保存需要的微信数据指针
#define StoreDataPtr(Name, Type, pp)				\
	pwx##Name = pp->##pwx##Name;
// 相当重要的宏，增加Hook时记得增加这儿的项目
#define HookFuncMacro(Macro, ...)		\
	Macro(InitLoginWin, __VA_ARGS__)	\
	Macro(GetQrcode, __VA_ARGS__)		\
	Macro(AfterScan, __VA_ARGS__)		\
	Macro(ScanUserData, __VA_ARGS__)	\
	Macro(ChatView, __VA_ARGS__)		\
	Macro(_ChatView, __VA_ARGS__)		\
	Macro(MutexCheck, __VA_ARGS__)		\
	Macro(OnRecv, __VA_ARGS__)			\
	Macro(OnSend, __VA_ARGS__)		    \
	Macro(OnSelect, __VA_ARGS__)        \
	Macro(OnCaseText, __VA_ARGS__)      \
	Macro(OnShadeWnd, __VA_ARGS__)    \
	Macro(OnNcHit, __VA_ARGS__)      \
	Macro(OnTrayClick, __VA_ARGS__)  \
	Macro(IsFriendCall, __VA_ARGS__)  \
	Macro(AutoDlImg, __VA_ARGS__)   \
	Macro(AutoDlVideo, __VA_ARGS__)   \
	Macro(DelFriend, __VA_ARGS__)   \
	Macro(RoamMsg, __VA_ARGS__)  \
	Macro(Monitor1, __VA_ARGS__)  \
	Macro(GetImgSid, __VA_ARGS__)  \
	Macro(GetImgFile, __VA_ARGS__)  \
	Macro(GetVoice, __VA_ARGS__) \
	Macro(Dtray, __VA_ARGS__)    \
	Macro(DtrayHide, __VA_ARGS__)  \
	Macro(DumpGif, __VA_ARGS__)   \
	Macro(OnUrlKey, __VA_ARGS__)  \
    Macro(MonRemark, __VA_ARGS__)






// 微信函数指针宏
#define WXFuncMacro(Macro, ...)		\
	Macro(InitLoginWin, __VA_ARGS__)		\
	Macro(GetQrcode, __VA_ARGS__)		\
	Macro(AfterScan, __VA_ARGS__)		\
	Macro(ScanUserData, __VA_ARGS__)		\
	Macro(SetRichEditText, __VA_ARGS__)		\
	Macro(GetRichEditText, __VA_ARGS__)		\
	Macro(SetEditText, __VA_ARGS__)		\
	Macro(GetEditText, __VA_ARGS__)		\
	Macro(EnableProxy, __VA_ARGS__)		\
	Macro(ProxyNotify, __VA_ARGS__)		\
	Macro(TopRightNotify, __VA_ARGS__)		\
	Macro(RestoreWindow, __VA_ARGS__)		\
	Macro(ChatOnTop, __VA_ARGS__)		\
	Macro(MuteMessage, __VA_ARGS__)		\
	Macro(GetFriInfo, __VA_ARGS__)		\
	Macro(FreeFriInfo, __VA_ARGS__)		\
	Macro(GetGroupInfo, __VA_ARGS__)		\
	Macro(InviteToGroup, __VA_ARGS__)		\
	Macro(SendGroupCard, __VA_ARGS__)		\
	Macro(DelGroupMember, __VA_ARGS__)		\
	Macro(ChatView, __VA_ARGS__)		\
	Macro(_ChatView, __VA_ARGS__)		\
	Macro(ChangeRemark, __VA_ARGS__)		\
	Macro(MutexCheck, __VA_ARGS__)		\
	Macro(SetTalker, __VA_ARGS__)		\
	Macro(SetGroupNick, __VA_ARGS__)		\
	Macro(SetGroupName, __VA_ARGS__)		\
	Macro(SetGroupNotice, __VA_ARGS__)		\
	Macro(CreateGroup, __VA_ARGS__)		\
	Macro(AcceptFriend, __VA_ARGS__)		\
	Macro(SendTextMsg, __VA_ARGS__)		\
	Macro(DBPassword, __VA_ARGS__)		\
	Macro(OnRecv, __VA_ARGS__)		    \
	Macro(GetExtraInfo, __VA_ARGS__)	\
	Macro(FreeMsg, __VA_ARGS__)			\
	Macro(SendImage, __VA_ARGS__)		\
	Macro(SendFile, __VA_ARGS__)		\
	Macro(SendFileArg, __VA_ARGS__)		\
	Macro(GetContact1, __VA_ARGS__)		\
	Macro(GetContact2, __VA_ARGS__)		\
	Macro(SetRead, __VA_ARGS__)			\
	Macro(SendLink, __VA_ARGS__)		\
	Macro(OnSend, __VA_ARGS__)			\
	Macro(GetFriLogo, __VA_ARGS__)		\
	Macro(Recall, __VA_ARGS__)			\
	Macro(AddTask, __VA_ARGS__)			\
	Macro(AddTaskArg, __VA_ARGS__)		\
	Macro(FormatCard, __VA_ARGS__)		\
	Macro(SendGif, __VA_ARGS__)         \
	Macro(OnSelect, __VA_ARGS__)		 \
	Macro(OnCaseText, __VA_ARGS__)       \
	Macro(OnShadeWnd, __VA_ARGS__)       \
	Macro(SendImagePara1, __VA_ARGS__)   \
	Macro(OnNcHit, __VA_ARGS__)     \
	Macro(AddContact, __VA_ARGS__)     \
	Macro(OnTrayClick, __VA_ARGS__)   \
	Macro(AddFriReady, __VA_ARGS__)  \
	Macro(Getest1, __VA_ARGS__)   \
	Macro(IsFriendCall, __VA_ARGS__)   \
	Macro(GetIMInfo1, __VA_ARGS__)	\
	Macro(GetIMInfo2, __VA_ARGS__)   \
	Macro(SetDisturb, __VA_ARGS__)   \
	Macro(SaveInLlist, __VA_ARGS__)   \
	Macro(QuitGroupParam1, __VA_ARGS__)  \
	Macro(QuitGroupParam6, __VA_ARGS__)   \
	Macro(QuitGroup, __VA_ARGS__)   \
	Macro(GetFriBlock, __VA_ARGS__)   \
	Macro(IsDisturb, __VA_ARGS__)    \
	Macro(IsChatOnTop, __VA_ARGS__)  \
	Macro(DelFriend, __VA_ARGS__)   \
	Macro(AutoDlImg, __VA_ARGS__)   \
	Macro(AutoDlVideo, __VA_ARGS__)   \
	Macro(GetImgB1, __VA_ARGS__)   \
	Macro(GetImgB2, __VA_ARGS__)   \
	Macro(RoamMsg, __VA_ARGS__)  \
	Macro(Monitor1, __VA_ARGS__)  \
	Macro(GetImgSid, __VA_ARGS__)  \
	Macro(GetImgFile, __VA_ARGS__) \
	Macro(GetVoice, __VA_ARGS__)   \
	Macro(Dtray, __VA_ARGS__)   \
	Macro(DtrayHide, __VA_ARGS__) \
	Macro(GetGinfo, __VA_ARGS__)   \
    Macro(DumpGif, __VA_ARGS__)    \
	Macro(PayMents, __VA_ARGS__)  \
	Macro(OpenUrl, __VA_ARGS__)   \
	Macro(OnUrlKey, __VA_ARGS__)   \
	Macro(SendLook, __VA_ARGS__)   \
	Macro(RelayFile, __VA_ARGS__)  \
	Macro(RelayLook, __VA_ARGS__)  \
	Macro(SetReadAll, __VA_ARGS__)  \
	Macro(ViaImFriendArg, __VA_ARGS__)  \
	Macro(ViaImFriend, __VA_ARGS__)   \
	Macro(ToFav, __VA_ARGS__)  \
	Macro(AddTag, __VA_ARGS__)   \
	Macro(MsgView, __VA_ARGS__)   \
    Macro(RelayProgram, __VA_ARGS__)  \
    Macro(SendProgram, __VA_ARGS__) \
	Macro(ControlVisible, __VA_ARGS__)   \
	Macro(RefreshQR, __VA_ARGS__)  \
	Macro(MonRemark, __VA_ARGS__)








	


// 微信数据指针宏
#define WXDataMacro(Macro, ...)		\
	Macro(WXID, PSTR*, __VA_ARGS__)		\
	Macro(Username, PSTR, __VA_ARGS__)		\
	Macro(UIN, PDWORD, __VA_ARGS__)		\
	Macro(Sex, SexType*, __VA_ARGS__)		\
	Macro(Phone, PSTR, __VA_ARGS__)		\
	Macro(Addr1, PSTR, __VA_ARGS__)		\
	Macro(Addr2, PSTR, __VA_ARGS__)		\
	Macro(Addr3, PSTR, __VA_ARGS__)		\
	Macro(Logo, PSTR*, __VA_ARGS__)		\
	Macro(UID, PSTR, __VA_ARGS__)		\
	Macro(Heap, PHANDLE, __VA_ARGS__)		\
	Macro(Friend, PVOID, __VA_ARGS__)		\
	Macro(InviteArg, PVOID, __VA_ARGS__)		\
	Macro(CreateGroupArg, PVOID, __VA_ARGS__)		\
	Macro(AcceptFriendArg, PVOID*, __VA_ARGS__)		\
	Macro(UserData, PVOID*, __VA_ARGS__)		\
	Macro(SetReadArg, PVOID, __VA_ARGS__)		\
	Macro(SendLinkArg, PVOID, __VA_ARGS__)		\
	Macro(DBPW, PBYTE*, __VA_ARGS__)			\
	Macro(DelGroupMemberArg, PVOID, __VA_ARGS__)  \
	Macro(WxPath, PSTR*, __VA_ARGS__)  \
    Macro(RefreshQRArg, PVOID, __VA_ARGS__)	 \
	Macro(SendLookArg, PVOID, __VA_ARGS__)	 \
	Macro(RelayArg, PVOID, __VA_ARGS__)  \
	Macro(Cefobj, PVOID, __VA_ARGS__)


/* 
	数据类型定义 
*/

// 主要用来伪造事件主体名(模拟DuiLib::CString)
typedef struct
{
	PWSTR pwName;
	WCHAR szName[100]; // 这个长度是我随便写的
					   // 这段的长度应该是动态管理的
}*FakeNotifyString;

// 返回伪造的事件主题名称
typedef FakeNotifyString(__fastcall* FakeFunc)(PVOID, DWORD, FakeNotifyString);

// 伪造的Notify函数参数
typedef struct
{
	/* +   0 */ PWSTR pwType; // 事件类型指针
	/* +   4 */ WCHAR szType[16]; //事件类型，最长应为14字符（含句尾零字符)，保持对齐
	/* +  24 */ CHAR _1[96]; // 保留，凑偏移量
	/* +  84 */ PVOID pFakeVTable; // 指向伪造的虚表，基于对节省空间的考虑，直接指向自己就可以了
	/* +  88 */ FakeFunc pFakeFunc; // 虚表中的第二个函数，返回Object的名称
	/* +  8C */ PWSTR pFakeName; // 指向下面哪个字符串
	/* +  90 */ WCHAR szFakeName[60]; // 伪造的类名
	/* + 108 */ PVOID pFakeObject; // 指向伪造的object，也就是指向pFakeVTable就可以了
}FakeNotify, *PFakeNotify;

// 微信中广泛使用的字符串类
typedef struct
{
	const wchar_t* pwStr;
	SIZE_T stLen;
	SIZE_T stMax;
	DWORD dwZero1;
	DWORD dwZero2;
}FakeString, *PFakeString;


typedef struct
{
	DWORD dwx1;
	DWORD dwx2;
	const wchar_t* pwStr;
	SIZE_T stLen;
	SIZE_T stMax;
	DWORD dwZero1;
	DWORD dwZero2;
}FakeString2, *PFakeString2;

// 个人好友、公众号使用的链表类型
typedef struct _LinkList
{
	struct _LinkList* pNext;
	struct _LinkList* pPre;
	const wchar_t** pWXID;
}LinkList, *PLinkList;

// 群组使用的平衡二叉树类型
typedef struct _BinTree
{
	struct _BinTree* pLeft; // 左(小)
	struct _BinTree* pParent; // 父
	struct _BinTree* pRight; // 右(大)
	char _1; // 占位
	char bEnd; // 是否终止
	char _2[2]; // 占位
	const wchar_t* pwWXID;
}BinTree, *PBinTree;

// 发送链接时使用的结构体
typedef struct
{
	FakeString fsTitle;
	FakeString fsURL;
	BYTE _1[0x2c];
	FakeString fsImage;
	BYTE _2[0x14];
	FakeString fsDesc;
	BYTE _3[0x80];
}FakeLink, *PFakeLink;




// 微信好友信息
typedef struct
{
	/* 0 */ DWORD _1[2];
	/* 8 */ FakeString fsWXID;
	/* 1C */ FakeString fsUID;
	/* 30 */ FakeString fsV1;
	/* 44 */ DWORD _3;
	/* 48 */ DWORD dwUserType1; // 0: Sys; 2: Group; 3: Normal or public.
	/* 4c */ DWORD dwUserType2; // 0: Normal or Group or Sys; 18: Public
	/* 50 */ FakeString fsRemark;
	/* 64 */ FakeString fsUsername;
	/* 78 */ FakeString _5;
	/* 8C */ DWORD _6[6];
	/* A4 */ FakeString fsPUsername;
	/* B8 */ FakeString fspUsername;
	/* CC */ FakeString fsPRemark;
	/* E0 */ FakeString fspRemark;
	/* F4 */ FakeString fsLogo1;
	/* 108 */ FakeString fsLogo2;
	/* 11C */ DWORD _7[16];
	/* 15C */ SexType stSex;       
	/* 160 */ BYTE _8[0x34];
	/* 194 */ FakeString fsWP;  //签名
	/* 1A8 */ FakeString fsAddr1;   
	/* 1BC */ FakeString fsAddr2;   
	/* 1D0 */ FakeString fsAddr3;   
	/* 1E4 */ FromType ftFrom;   
	/* 1E8 */ DWORD _9[200];
} FakeUserInfo, *PFakeUserInfo;

/*-------------------------微信2.7.1.82及以前-----------------------------------------------------------------------*/
//typedef struct
//{
//	/* 0 */ byte _1[40];
//	/* 28 */
//	union
//	{
//		struct { DWORD dwSrvIDL; DWORD dwSrvIDH; } dwSrvID;
//		QWORD qwSrvID;
//	} SrvID;
//	/* 30 */ MsgType mtType;
//	/* 34 */ DWORD dwIsSend; // 0: Recv; 1: Send
//	/* 38 */ DWORD dwIsDl; // 1: Upload; 2: Download
//	/* 3C */ DWORD dwTime;
//	/* 40 */ FakeString fsTalker;
//	/* 54 */ DWORD _3[5];
//	/* 68 */ FakeString fsContent;
//	/* 7C */ BYTE _7[8];
//	/* 84 */ FakeString fsimgpath;
//	/* 98 */ BYTE _4[124];
//	/* 114 */ FakeString fsSenderInGroup; 
//	/* 128 */ FakeString fsLocalID;
//	/* 13C */ FakeString fsThumb; // 略缩图
//	/* 150 */ FakeString fsOrigin; // 原文件
//	/* 164 */ DWORD _5;
//	/* 168 */ FakeString fsAtlist; //at人列表
//	          DWORD _6[100];
//	/* and more */
//} FakeMsg, *PFakeMsg;
/*-------------------------微信2.7.1.82-----------------------------------------------------------------------*/






/*-------------------------微信2.8.0.133开始-----------------------------------------------------------------------*/
typedef struct
{
	/* 0 */ byte _1[40];
	/* 28 */
	union
	{
		struct { DWORD dwSrvIDL; DWORD dwSrvIDH; } dwSrvID;
		QWORD qwSrvID;
	} SrvID;
	/* 30 */ MsgType mtType;
	/* 34 */ DWORD dwIsSend; // 0: Recv; 1: Send
	/* 38 */ DWORD dwIsDl; // 1: Upload; 2: Download
	/* 3C */ DWORD dwTime;
	/* 40 */ FakeString fsTalker;
	/* 54 */ DWORD _3[5];
	/* 68 */ FakeString fsContent;
	/* 7C */ BYTE _7[8];
	/* 84 */ FakeString fsimgpath;
	/* 98 */ BYTE _4[148];
	/* 12C */ FakeString fsSenderInGroup;
	/* 140 */ FakeString fsLocalID;
	/* 154 */ FakeString fsThumb; // 略缩图
	/* 168 */ FakeString fsOrigin; // 原文件
	/* 17c */ DWORD _5;
	/* 180 */ FakeString fsAtlist; //at人列表
	DWORD _6[100];
	/* and more */
} FakeMsg, *PFakeMsg;
/*-------------------------微信2.8.0.133-----------------------------------------------------------------------*/

//124
enum InfoType
{
	itWXID = 0,
	itUsername,
	itRemark, // 好友限定
	itUID,
	itUIN,
	itAddr1,
	itAddr2,
	itAddr3,
	itPhone,
	itLogo1, // Big
	itLogo2, // Small
	itSex, // 未实现
	itFrom, // 未实现
	itType, // 好友类型
	itNick, // 群内昵称
	itGroup, // 归属的群
	itMaster, // 群主wxid
	itSum, // 群成员数
	itPname, // 拼音
	itSignname, //签名
	itType1, // 好友类型1
};

/* 
	微信的函数类型定义以及用于Hook的参数 
*/

/* 
	需要Hook的
*/

// 初始化登陆窗口
typedef PVOID(__fastcall*ftInitLoginWin)(PVOID, DWORD, PVOID);
/* 调用此函数后即可读取快速登陆信息 */

typedef PVOID(__fastcall*ftGetQrcode)(PVOID, DWORD, PVOID);
typedef PVOID(__fastcall*ftRefreshQR)(PVOID, DWORD);


/* 估计二维码链接访问一次就失效了，所以直接发送图片的二进制信息 */
// 二维码扫描完成
typedef PVOID(__fastcall*ftAfterScan)(PVOID, DWORD, PVOID);
// 扫描完成后此函数调用两次，第一次获取头像链接，第二次是用户名
// AfterScan调用后设置此Hook，调用两次后再取消
typedef PWSTR*(__fastcall*ftScanUserData)(PVOID, DWORD, PVOID, PVOID);
// ExtendChatView，登录后重要的界面，作为登陆成功的标志
typedef PVOID(__fastcall*ftChatView)(PVOID, DWORD);
// ExtendChatView的析构函数（大概），作为Logout的标志
typedef PVOID(__fastcall*ft_ChatView)(PVOID, DWORD);
// 多开判断，想要多开就返回FALSE即可
typedef PVOID(__stdcall*ftMutexCheck)();
// 数据库密钥
typedef PVOID(__stdcall*ftDBPassword)(PVOID, PVOID, PBYTE);
// 下载信息
typedef PVOID(__fastcall*ftOnRecv)(PVOID, DWORD);
// 上传信息
typedef PVOID(__stdcall*ftOnSend)(DWORD, PVOID);
typedef PVOID(__fastcall*ftOnSelect)(PVOID, DWORD, PVOID, DWORD);
typedef PVOID(__fastcall*ftOnFilterText)(PVOID, PVOID);
typedef PVOID(__fastcall*ftOnCaseText)(PVOID, PVOID, PVOID);
typedef PVOID(__fastcall*ftOnTrayClick)(PVOID, PVOID, PVOID, PVOID, PVOID,PVOID, PVOID);
typedef PVOID(__fastcall*ftIsFriendCall)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID);
typedef PVOID(__fastcall*ftGetImgSid)(PVOID, PVOID, PVOID, PVOID);
typedef PVOID(__fastcall*ftGetImgFile)(PVOID, PVOID);
typedef PVOID(__fastcall*ftGetVoice)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID);

//去阴影
typedef PVOID (__fastcall*ftOnShadeWnd)(PVOID, PVOID, PVOID);
//禁止移动
typedef PVOID (__fastcall*ftOnNcHit)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID);

typedef PVOID(__fastcall*ftDtray)(PVOID, PVOID);

typedef PVOID(__fastcall*ftDtrayHide)(PVOID, PVOID);


typedef PVOID(__fastcall*ftDumpGif)(PVOID, PVOID, PVOID);

typedef PVOID(__fastcall*ftOnUrlKey)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID);




typedef struct
{
	PWSTR pwName;
	WCHAR szName[5]; 
}*FakeString1;



typedef bool(__fastcall*ftControlVisible)(PVOID, DWORD);

// 设置富文本内容
//typedef VOID(__fastcall*ftSetRichEditText)(PVOID, DWORD, PVOID);

typedef VOID(__fastcall*ftSetRichEditText)(PVOID, DWORD, PVOID, PVOID);
// 获取富文本内容
typedef FakeString1(__fastcall*ftGetRichEditText)(PVOID, DWORD, PVOID);
//typedef VOID(__fastcall*ftAddFriReady)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, DWORD, DWORD, PVOID, PVOID, DWORD, DWORD);
typedef VOID(__stdcall*ftAddFriReady)();

typedef VOID(__fastcall*ftGetest1)(PVOID, PVOID, PVOID, PVOID);

// 设置输入框内容
typedef VOID(__fastcall*ftSetEditText)(PVOID, DWORD, const wchar_t*);
// 获取输入框内容
typedef FakeString(__fastcall*ftGetEditText)(PVOID, DWORD);
// 启用代理
typedef PVOID(__fastcall*ftEnableProxy)(PVOID, DWORD);
// Notify通用格式
typedef PVOID(__fastcall*ftNotify)(PVOID, DWORD, PFakeNotify);
// 代理设置界面
typedef ftNotify ftProxyNotify;
// 右上角按钮
typedef ftNotify ftTopRightNotify;
// 恢复窗口
typedef VOID(__fastcall*ftRestoreWindow)(PVOID, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
// 设定对话人
typedef VOID(__fastcall*ftSetTalker)(PVOID, DWORD, PVOID, DWORD);
// 置顶聊天（是否置顶）
typedef VOID(__stdcall*ftChatOnTop)(PVOID, BOOL);
// 屏蔽（是否接收信息）
typedef VOID(__stdcall*ftMuteMessage)(PVOID, BOOL);
// 通过WXID获取权限内的所有信息
typedef VOID(__stdcall*ftGetFriInfo)(const wchar_t*, SIZE_T, DWORD, DWORD, DWORD, PFakeUserInfo);
// Free好友信息，字符串时放到堆上的
typedef VOID(__fastcall*ftFreeFriInfo)(PFakeUserInfo, DWORD);
// 获取群信息
typedef PVOID(__stdcall*ftGetGroupInfo)();
// 邀请入群(<40人群)
typedef VOID(__fastcall*ftInviteToGroup)(PVOID, DWORD, PFakeString*, const wchar_t*, SIZE_T, SIZE_T, DWORD, DWORD);
// 发送群信息（邀请入群(>40人)）
typedef VOID(__fastcall*ftSendGroupCard)(PVOID, DWORD, PFakeString*, const wchar_t*, SIZE_T, SIZE_T, DWORD, DWORD);
// 删除群员
typedef VOID(__fastcall*ftDelGroupMember)(PVOID, DWORD, PFakeString*, const wchar_t*, SIZE_T, SIZE_T, DWORD, DWORD);
// 修改群昵称
typedef VOID(__stdcall*ftSetGroupNick)(const wchar_t*, SIZE_T, SIZE_T, DWORD, DWORD, const wchar_t*, SIZE_T, SIZE_T, DWORD, DWORD, const wchar_t*, SIZE_T, SIZE_T, DWORD, DWORD);
// 修改群名
typedef void(__fastcall*ftSetGroupName)(PFakeString, PFakeString);
// 群公告（要有这个权限）
typedef void(__stdcall*ftSetGroupNotice)(PFakeString*);
// 修改备注
typedef VOID(__stdcall*ftChangeRemark)(PFakeUserInfo, DWORD);
// 创建群
typedef VOID(__fastcall*ftCreateGroup)(PVOID, DWORD, PVOID, DWORD, PVOID, DWORD, DWORD, DWORD);
// 同意加好友
typedef VOID(__fastcall*ftAcceptFriend)(PVOID, DWORD, PVOID, SIZE_T, SIZE_T, DWORD, DWORD, DWORD, PVOID, DWORD, DWORD, PVOID, PVOID, PVOID, PVOID,
	PVOID, PVOID, PVOID, DWORD, DWORD, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID);

// 发送基于文本的信息
//typedef VOID(__stdcall*ftSendTextMsg)(PFakeMsg, PVOID, PVOID, PFakeString[3], MsgType);
typedef VOID(__stdcall*ftSendTextMsg)();

// 获取额外好友信息
typedef VOID(__fastcall*ftGetExtraInfo)(PFakeUserInfo, DWORD);
// 释放发送信息回执资源
typedef VOID(__fastcall*ftFreeMsg)(PFakeMsg, DWORD);


// 发送图片
typedef VOID(__fastcall*ftSendImage)(PVOID, PVOID, PFakeMsg, PFakeString, PFakeString);
typedef PVOID(__cdecl *ftSendImagePara1)(PVOID);





// 发送文件
#define ftSendFile PVOID
// typedef PVOID ftSendFile;
// typedef VOID(__fastcall*ftSendFile)(PVOID, DWORD, PVOID, const wchar_t*, SIZE_T, SIZE_T, DWORD, DWORD, const wchar_t*, SIZE_T, SIZE_T, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
// 
typedef PVOID(__stdcall*ftSendFileArg)();
//获取最近联系人
typedef PVOID(__stdcall*ftGetContact1)();
typedef PWSTR**(__fastcall*ftGetContact2)(PVOID, DWORD);
// 撤回
typedef VOID(__stdcall*ftRecall)(PFakeMsg);
// 好友头像信息
typedef VOID(__fastcall*ftGetFriLogo)(PFakeUserInfo, DWORD, PFakeString);
// 发送链接信息
typedef VOID(__fastcall*ftSendLink)(PVOID, DWORD, PFakeMsg, const wchar_t*, SIZE_T, SIZE_T, DWORD, DWORD, PFakeLink);
// 标记已读
typedef VOID(__fastcall*ftSetRead)(PVOID _this, DWORD NOUSE, PFakeString pfiWXID, PDWORD arg1, DWORD arg2);
// 添加下一消息队列的任务，当前只用来实现logout
typedef PVOID(*ftAddTaskArg)();
typedef VOID(__fastcall*ftAddTask)(PVOID _this, DWORD NOUSE, DWORD, DWORD, DWORD, DWORD);
// 格式化名片信息
typedef VOID(__fastcall*ftFormatCard)(PFakeString, PFakeUserInfo);
//发送GIF图片
typedef VOID(__stdcall*ftSendGif)(PVOID, DWORD, DWORD, DWORD, DWORD, PVOID, DWORD, DWORD, DWORD, DWORD, PVOID, DWORD
	, DWORD, DWORD, DWORD, DWORD, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID);
//添加联系人,有问题,懒得去看了，没用到
typedef VOID(__fastcall*ftAddContact)(PVOID, PVOID, PVOID);
//获取企业微信群成员
typedef int(__fastcall*ftGetIMInfo1)(PVOID, PVOID);
typedef int(__fastcall*ftGetIMInfo2)(PVOID, PVOID, PVOID, PVOID);

//消息免打扰
typedef VOID(__stdcall*ftSetDisturb)(PVOID, BOOL);
//保存到通讯录
typedef VOID(__stdcall*ftSaveInLlist)(PVOID, DWORD, DWORD, DWORD, DWORD,PVOID, DWORD, DWORD, DWORD, DWORD, BOOL);

//退群
typedef PVOID(__fastcall*ftQuitGroupParam6)(PVOID, PVOID, PVOID);//参数6
typedef PVOID(__stdcall*ftQuitGroupParam1)();//参数1
typedef PVOID(__fastcall*ftQuitGroup)(PVOID, PVOID, DWORD, PVOID, PVOID, PVOID);//主功能函数
typedef PVOID(__stdcall*ftGetFriBlock)(PVOID, PVOID);

typedef PVOID(__cdecl * ftPayMents)(PVOID, DWORD, DWORD, DWORD, DWORD, PVOID, DWORD, DWORD, DWORD, DWORD, PVOID, PVOID);
typedef PVOID(__fastcall * ftOpenUrl)(PVOID, PVOID);//必须定义成__fastcall, 实际是__cdecl


typedef PVOID(__fastcall * ftSetReadAll)(PVOID, PVOID);

typedef PVOID(__stdcall * ftViaImFriendArg)();
typedef PVOID(__fastcall * ftViaImFriend)(PVOID, PVOID, PVOID, PVOID);


typedef BOOL(__fastcall*ftIsDisturb)(PVOID, PVOID);//消息免打扰是否开启
typedef BOOL(__stdcall* ftIsChatOnTop)(PVOID);//消息置顶是否开启

typedef PVOID(__stdcall* ftDelFriend)(PVOID, PVOID);

typedef PVOID(__fastcall*ftAutoDlImg)(PVOID, PVOID);
typedef PVOID(__fastcall*ftAutoDlVideo)(PVOID, PVOID);

//群成员变动 实时监控
typedef PVOID(__fastcall*ftMonitor1)(PVOID, PVOID);
typedef PVOID(__fastcall*ftMonRemark)(PVOID, PVOID, PVOID, PVOID);


//漫游消息
typedef PVOID(__fastcall*ftRoamMsg)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID);
//获取企业微信群成员
typedef PVOID(__stdcall*ftGetImgB1)();
typedef PVOID(__fastcall*ftGetImgB2)(PVOID, PVOID, PVOID, PVOID, PVOID);
typedef PVOID(__stdcall*ftGetGinfo)();


//文件转发
typedef PVOID(__fastcall * ftRelayFile)(PVOID, PVOID, PVOID, PVOID);
//表情转发
typedef PVOID(__fastcall * ftRelayLook)(PVOID, PVOID);
//小程序转发
typedef PVOID(__fastcall * ftRelayProgram)(PVOID, PVOID, PVOID, PVOID);
//发送表情
typedef PVOID(__fastcall * ftSendLook)(PVOID, PVOID, PVOID, PVOID, PVOID);
//发小程序
typedef PVOID(__stdcall * ftSendProgram)(PVOID, DWORD, DWORD, PVOID, PVOID, PVOID, PVOID, PVOID, DWORD, DWORD, DWORD, DWORD);

//添加到收藏
typedef PVOID(__stdcall*ftToFav)(PVOID);

//添加标签
typedef PVOID(__fastcall * ftAddTag)(PVOID, PVOID, PVOID);


typedef PVOID(__stdcall * ftMsgView)();




#define WXFuncCreate(Name)		\
	ft##Name pwxf##Name;
#define WXDataCreate(Name, Type)	\
	Type pwx##Name;
typedef struct
{

	//PHANDLE pwxHeap;
	//PVOID pwxFriend;
	//PSTR* pwxWXID;
	//PSTR pwxUsername;
	//PDWORD pwxUIN;
	//SexType* pwxSex;
	//PSTR pwxPhone;
	//PSTR pwxAddr1;
	//PSTR pwxAddr2;
	//PSTR pwxAddr3;
	//PSTR* pwxLogo;
	//PSTR pwxUID;
	//PVOID* pwxUserData;
	//PVOID pwxInviteArg;
	//PVOID pwxCreateGroupArg;
	//PVOID* pwxAcceptFriendArg;
	//PVOID pwxSetReadArg;
	//PVOID pwxSendLinkArg;
	//PBYTE* pwxDBPW;
	WXDataMacro(WXDataCreate);
	WXFuncMacro(WXFuncCreate);
	DWORD _1[30]; // 保留缓冲
}WXPtrs, *PWXPtrs;


// HookBase函数如何调用HookHandler函数
enum HandleType
{
	htNo = 0, // 不调用处理函数
	htReplace, // 越俎代庖
	htBefore, // 先调用HookHandler，再调用原函数
	htAfter, // 先调用原函数，然后调用HookHandler
};

// HookHandler
typedef PVOID(*ftHookHandler)(PVOID); // 传入一个指针，指针怎么解析由函数自行解决

// CommandHandler
typedef void(*ftCommandHandler)(const DataList& dl);


//
typedef struct
{
	HandleType ht;
	ftHookHandler fthh;
}HookHandleControl, *PHookHandleControl;


#define HookHandlersCreate(Name)	\
	HookHandleControl hhc##Name;

// 初始化HookBase Handler用的参数结构体 该结构体的任意改动都将导致新旧版本无法兼容
typedef struct
{
	HookFuncMacro(HookHandlersCreate);
}HookHandlers, *PHookHandlers;

#define HookFuncsCreate(Name)		\
	ft##Name pwxf##Name;
// Hook函数指针，传参用于初始化Hook
typedef struct
{
	HookFuncMacro(HookFuncsCreate);
	DWORD _1[10]; // 保留
}HookFuncs, *PHookFuncs;

// 初始化子模块的参数 // 如果是结构体嵌套结构体，那么其中一个结构体的更改会导致后面剩下结构体的偏移量全部变化导致兼容性问题，全部使用指针
typedef struct
{
	PCWSTR pwWXVer; // 微信版本
	DWORD dwVer; // 本模块版本
	SOCKET sk; // 通信用Socket
	PVOID pBase; // WeChatWin.dll的基地址
	PHookFuncs phf; // 需要Hook的函数指针
	PHookHandlers phh; // Hook的处理方法
	ftCommandHandler* cfCmdHs; // Command的处理方法
	PWXPtrs pp; // 微信指针
	VOID (*pfOnMsgIDMap)(const wstring&, const wchar_t*); // 函数指针
	PCWSTR pwWXPath;
}SubInitArg, *PSubInitArg;

static SOCKET skControl = 0;

extern PHANDLE pwxHeap;
// 在微信的堆上申请一块内存
PVOID WXHeapAlloc(SIZE_T stMemSize);
// 在微信的堆上申请一块内存然后写入字符串
const wchar_t* WXStringAlloc(const wchar_t* pwStr);
const char* WXStringAlloc(const char* psStr);

// 初始化FakeNotify
BOOL MakeFakeNotify(PFakeNotify pFakeNotify, const wchar_t* pwEvent, const wchar_t* pwObject);

// 伪造的获取类名函数
FakeNotifyString __fastcall FakeGetName(PVOID pObject, DWORD NOUSE, FakeNotifyString pMem);
