
// KiwoomRestfulCppDlg.cpp: 구현 파일
//
#include "pch.h"
#include "framework.h"
#include "KiwoomRestfulCpp.h"
#include "KiwoomRestfulCppDlg.h"
#include "afxdialogex.h"
#include <string>
#include <map>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <boost/locale.hpp>

#define CROW_MAIN
#include "crow_all.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// trim from start (in place)
static inline void ltrim(std::wstring& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
static inline void rtrim(std::wstring& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::wstring& s)
{
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
static inline std::wstring ltrim_copy(std::wstring s)
{
	ltrim(s);
	return s;
}

// trim from end (copying)
static inline std::wstring rtrim_copy(std::wstring s)
{
	rtrim(s);
	return s;
}

// trim from both ends (copying)
static inline std::wstring trim_copy(std::wstring s)
{
	trim(s);
	return s;
}

std::wstring cstring2int2str(const CString &num)
{
	if (num == L"")
		return L"0";
	int x = std::stoi(std::wstring(num));
	return std::to_wstring(x);
}

bool startswith(const std::wstring& ss, const std::wstring& prefix)
{
	if (ss.size() < prefix.size())
		return false;

	for (int i = 0; i < prefix.size(); ++i)
	{
		if (prefix[i] != ss[i])
			return false;
	}
	return true;
}


// CKiwoomRestfulCppDlg 대화 상자

UINT CrowThreadProc(LPVOID Param)
{
	crow::SimpleApp* app = (crow::SimpleApp*) Param;
	app->multithreaded().run();
	return 0;
}

CKiwoomRestfulCppDlg::CKiwoomRestfulCppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KIWOOMRESTFULCPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKiwoomRestfulCppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// 이거 안 하면 winocc.cpp line 377 에서 에러 뜸
	DDX_Control(pDX, IDC_KHOPENAPICTRL1, this->kiwoom);
}

BEGIN_MESSAGE_MAP(CKiwoomRestfulCppDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CKiwoomRestfulCppDlg 메시지 처리기

void CKiwoomRestfulCppDlg::initCrowHandlers(void *voidCrowApp)
{
	crow::SimpleApp* crowApp = (crow::SimpleApp*)voidCrowApp;

	CROW_ROUTE((*crowApp), "/")([]()
	{
		return "Kiwoom Restful";
	});

	CROW_ROUTE((*crowApp), "/balance")
	.methods("POST"_method)
	([this](const crow::request& req)
	{
		auto x = crow::json::load(req.body);
		if (!x) {
			return crow::response(400);
		}

		this->reqno++;
		std::wstring str_reqno = std::to_wstring(this->reqno);
		std::wstring rqname = std::wstring(L"req") + str_reqno;
		//std::string accno_str = std::string(x["accno"].s().begin());
		std::wstring accno;
		// wstr to str, thanks to https://wendys.tistory.com/40
		accno.assign(x["accno"].s().begin(), x["accno"].s().end());

		// Delayed connectedness check for faster debug of the crow part
		if (!this->kiwoomConnected) {
			return crow::response{ "{\"error\": \"Kiwoom not connected yet\"}" };
		}
	
		this->kiwoom.SetInputValue(L"계좌번호", (LPCTSTR)accno.c_str());
		this->kiwoom.SetInputValue(L"비밀번호", L"");
		this->kiwoom.SetInputValue(L"상장폐지조회구분", L"1");
		this->kiwoom.SetInputValue(L"비밀번호입력매체구분", L"00");
		int errCode = this->kiwoom.CommRqData((LPCTSTR)rqname.c_str(), L"OPW00004", 0, (LPCTSTR)str_reqno.c_str());

		// Busy for data arrival
		// Semaphore같은거 쓰면 좋을수도 있는데 귀찮다
		// C++11 지원되게 컴파일하면 일단 reading은 tread-safe하니 OK...?
		reqMap[rqname] = false;
		while (!reqMap[rqname]);

		reqMap.erase(reqMap.find(rqname));
		std::string resp = boost::locale::conv::utf_to_utf<char>(resultMap[rqname]);
		resultMap.erase(resultMap.find(rqname));
		return crow::response{ resp };
	});

	CROW_ROUTE((*crowApp), "/order")
	.methods("POST"_method)
	([this](const crow::request& req)
	{
		auto x = crow::json::load(req.body);
		if (!x) {
			return crow::response(400);
		}

		this->reqno++;
		std::wstring str_reqno = std::to_wstring(this->reqno);

		std::wstring rqname, screenno, accno, code, hogagb, orgorderno;
		rqname = std::wstring(L"sendorder") + str_reqno;
		screenno = str_reqno;
		// str to wstr, thanks to https://wendys.tistory.com/40
		accno.assign(x["accno"].s().begin(), x["accno"].s().end());
		code.assign(x["code"].s().begin(), x["code"].s().end());
		int order_type = x["ordertype"].i();
		int qty = x["qty"].i();
		int price = x["price"].i();
		hogagb.assign(x["hogagb"].s().begin(), x["hogagb"].s().end());

		// Delayed connectedness check for faster debug of the crow part
		if (!this->kiwoomConnected) {
			return crow::response{ "{\"error\": \"Kiwoom not connected yet\"}" };
		}

		int errCode = this->kiwoom.SendOrder(
			(LPCTSTR) rqname.c_str(),
			(LPCTSTR) screenno.c_str(),
			(LPCTSTR) accno.c_str(),
			order_type,
			(LPCTSTR) code.c_str(),
			qty,
			price,
			(LPCTSTR) hogagb.c_str(),
			(LPCTSTR) orgorderno.c_str() // 원주문번호, 정정 때 쓰임
		);
	
		// Busy for data arrival
		// Semaphore같은거 쓰면 좋을수도 있는데 귀찮다
		// C++11 지원되게 컴파일하면 일단 reading은 tread-safe하니 OK...?
		reqMap[rqname] = false;
		while (!reqMap[rqname]);

		reqMap.erase(reqMap.find(rqname));
		std::string resp = boost::locale::conv::utf_to_utf<char>(resultMap[rqname]);
		resultMap.erase(resultMap.find(rqname));
		return crow::response{ "{\"message\": \"" + resp + "\"}" };
	});
}

BOOL CKiwoomRestfulCppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// 로그인 ㄱㄱ
	this->kiwoomConnected = false; // Will become true when KiwoomOnEventConnect() received.
	this->reqno = 0;
	this->kiwoom.CommConnect();

	// https://int-i.github.io/cpp/2020-07-22/vcpkg-boost/
	// Install boost with vcpkg!
	// .\vcpkg install boost boost:x64-windows
	// .\vcpkg integrate install
	auto crowApp = new crow::SimpleApp();
	this->crowApp = (void*)crowApp;
	this->initCrowHandlers(crowApp);
	crowApp->bindaddr(theApp.bindAddr);
	crowApp->port(theApp.portNumber);
	AfxBeginThread(CrowThreadProc, (LPVOID) crowApp);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CKiwoomRestfulCppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CKiwoomRestfulCppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKiwoomRestfulCppDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// Delete crow
	crow::SimpleApp* app = (crow::SimpleApp*) this->crowApp;
	app->stop();
	delete app;
}


BEGIN_EVENTSINK_MAP(CKiwoomRestfulCppDlg, CDialogEx)
	ON_EVENT(CKiwoomRestfulCppDlg, IDC_KHOPENAPICTRL1, 5, CKiwoomRestfulCppDlg::KiwoomOnEventConnect, VTS_I4)
	ON_EVENT(CKiwoomRestfulCppDlg, IDC_KHOPENAPICTRL1, 2, CKiwoomRestfulCppDlg::KiwoomOnReceiveRealData, VTS_BSTR VTS_BSTR VTS_BSTR)
	ON_EVENT(CKiwoomRestfulCppDlg, IDC_KHOPENAPICTRL1, 3, CKiwoomRestfulCppDlg::KiwoomOnReceiveMsg, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	ON_EVENT(CKiwoomRestfulCppDlg, IDC_KHOPENAPICTRL1, 4, CKiwoomRestfulCppDlg::KiwoomOnReceiveChejanData, VTS_BSTR VTS_I4 VTS_BSTR)
	ON_EVENT(CKiwoomRestfulCppDlg, IDC_KHOPENAPICTRL1, 6, CKiwoomRestfulCppDlg::KiwoomOnReceiveInvestRealData, VTS_BSTR)
	ON_EVENT(CKiwoomRestfulCppDlg, IDC_KHOPENAPICTRL1, 7, CKiwoomRestfulCppDlg::KiwoomOnReceiveRealCondition, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	ON_EVENT(CKiwoomRestfulCppDlg, IDC_KHOPENAPICTRL1, 8, CKiwoomRestfulCppDlg::KiwoomOnReceiveTrCondition, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4)
	ON_EVENT(CKiwoomRestfulCppDlg, IDC_KHOPENAPICTRL1, 9, CKiwoomRestfulCppDlg::KiwoomOnReceiveConditionVer, VTS_I4 VTS_BSTR)
	ON_EVENT(CKiwoomRestfulCppDlg, IDC_KHOPENAPICTRL1, 1, CKiwoomRestfulCppDlg::KiwoomOnReceiveTrData, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()


void CKiwoomRestfulCppDlg::KiwoomOnEventConnect(long nErrCode)
{
	this->kiwoomConnected = true;
	if (nErrCode != 0)
	{
		std::string msg = std::string("Kiwoom connection failed with code=") + std::to_string(nErrCode);
		AfxMessageBox((LPCTSTR) msg.c_str(), MB_OK | MB_ICONSTOP);
	}
	else
	{
		CWnd* label = GetDlgItem(IDC_STATIC);
		label->SetWindowText(L"Kiwoom Connected");
	}
}


void CKiwoomRestfulCppDlg::KiwoomOnReceiveRealData(LPCTSTR sRealKey, LPCTSTR sRealType, LPCTSTR sRealData)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	return;
}


void CKiwoomRestfulCppDlg::KiwoomOnReceiveMsg(LPCTSTR sScrNo, LPCTSTR sRQName, LPCTSTR sTrCode, LPCTSTR sMsg)
{
	// For some reason sRQName isn't requested by me.
	if (reqMap.find(sRQName) == reqMap.end())
		return;

	std::wstring result;
	if (startswith(sRQName, L"sendorder"))
	{
		resultMap[sRQName] = sMsg;
		reqMap[sRQName] = true; // Mark ready. Order matters!
	}

	return;
}


void CKiwoomRestfulCppDlg::KiwoomOnReceiveChejanData(LPCTSTR sGubun, long nItemCnt, LPCTSTR sFIdList)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	return;
}


void CKiwoomRestfulCppDlg::KiwoomOnReceiveInvestRealData(LPCTSTR sRealKey)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	return;
}


void CKiwoomRestfulCppDlg::KiwoomOnReceiveRealCondition(LPCTSTR sTrCode, LPCTSTR strType, LPCTSTR strConditionName, LPCTSTR strConditionIndex)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	return;
}


void CKiwoomRestfulCppDlg::KiwoomOnReceiveTrCondition(LPCTSTR sScrNo, LPCTSTR strCodeList, LPCTSTR strConditionName, long nIndex, long nNext)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	return;
}


void CKiwoomRestfulCppDlg::KiwoomOnReceiveConditionVer(long lRet, LPCTSTR sMsg)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	return;
}


void CKiwoomRestfulCppDlg::KiwoomOnReceiveTrData(LPCTSTR sScrNo, LPCTSTR sRQName, LPCTSTR sTrCode, LPCTSTR sRecordName, LPCTSTR sPrevNext, long nDataLength, LPCTSTR sErrorCode, LPCTSTR sMessage, LPCTSTR sSplmMsg)
{
	// For some reason sRQName isn't requested by me.
	if (reqMap.find(sRQName) == reqMap.end())
		return;

	CString trcode(sTrCode);
	std::wstring result;

	if (trcode == "OPW00004") // cash... 어차피 정확하게 알기 힘듦... my자산 들어가서 나오는 양은 대체 어케 본담?
	{
		std::wstring cash = cstring2int2str(kiwoom.GetCommData(sTrCode, sRQName, 0, L"d+2추정예수금"));
		result = L"{";
		result += L"\"cash\":" + cash;
		int cnt = kiwoom.GetRepeatCnt(sTrCode, sRQName);
		for (int i = 0; i < cnt; ++i)
		{
			std::wstring code = kiwoom.GetCommData(sTrCode, sRQName, i, L"종목코드");
			trim(code);
			std::wstring qty = cstring2int2str(kiwoom.GetCommData(sTrCode, sRQName, i, L"보유수량"));
			result += L",\"" + code + L"\":" + qty;
		}
		result += L"}";
	}

	resultMap[sRQName] = result;
	reqMap[sRQName] = true; // Mark ready. Order matters!
}
