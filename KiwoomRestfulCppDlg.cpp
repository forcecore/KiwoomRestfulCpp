
// KiwoomRestfulCppDlg.cpp: 구현 파일
//
#include "pch.h"
#include "framework.h"
#include "KiwoomRestfulCpp.h"
#include "KiwoomRestfulCppDlg.h"
#include "afxdialogex.h"
#include <string>

#define CROW_MAIN
#include "crow_all.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

BOOL CKiwoomRestfulCppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// Will become true when the dialog is created and CommConnect is invoked there.
	// See KiwoomRestfulCppDlg.cpp
	this->kiwoomConnected = false;
	this->reqno = 0;

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// 로그인 ㄱㄱ
	// 성공여부는 event handler에서 알 수 있음.
	this->kiwoom.CommConnect();

	auto crowApp = new crow::SimpleApp();
	this->crowApp = (void*)crowApp;

	// https://int-i.github.io/cpp/2020-07-22/vcpkg-boost/
	// Install boost with vcpkg!
	// .\vcpkg install boost boost:x64-windows
	// .\vcpkg integrate install
	CROW_ROUTE((*crowApp), "/")([]()
	{
		return "Kiwoom Restful";
	});

	CROW_ROUTE((*crowApp), "/balance")
	.methods("POST"_method)
	([this](const crow::request& req)
	{
		if (!this->kiwoomConnected) {
			return crow::response{ "{\"error\": \"Kiwoom not connected yet\"}" };
		}
		auto x = crow::json::load(req.body);
		if (!x) {
			return crow::response(400);
		}

		this->reqno++;
		const char* num = std::to_string(this->reqno).c_str();
		const char* rqname = (std::string("req") + num).c_str();
		this->kiwoom.SetInputValue(L"계좌번호", (LPCTSTR) x["accno"].s().begin());
		this->kiwoom.SetInputValue(L"비밀번호", L"");
		this->kiwoom.SetInputValue(L"비밀번호입력매체구분", L"00");
		this->kiwoom.SetInputValue(L"조회구분", L"2");
		this->kiwoom.CommRqData((LPCTSTR)rqname, L"OPW00001", 0, (LPCTSTR)num);

		return crow::response{ "/balance receive WIP" };
	});

	crowApp->port(12233);
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

BEGIN_EVENTSINK_MAP(CKiwoomRestfulCppDlg, CDialogEx)
	ON_EVENT(CKiwoomRestfulCppDlg, IDC_KHOPENAPICTRL1, 5, CKiwoomRestfulCppDlg::KiwoomOnEventConnect, VTS_I4)
END_EVENTSINK_MAP()


void CKiwoomRestfulCppDlg::KiwoomOnEventConnect(long nErrCode)
{
	this->kiwoomConnected = true;
	if (nErrCode != 0)
	{
		std::string msg = std::string("Kiwoom connection failed with code=") + std::to_string(nErrCode);
		AfxMessageBox((LPCTSTR) msg.c_str(), MB_OK | MB_ICONSTOP);
	}
}


void CKiwoomRestfulCppDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// Delete crow
	crow::SimpleApp* app = (crow::SimpleApp*) this->crowApp;
	delete app;
}
