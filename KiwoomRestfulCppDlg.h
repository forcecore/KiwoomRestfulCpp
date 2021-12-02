
// KiwoomRestfulCppDlg.h: 헤더 파일
//

#pragma once


// CKiwoomRestfulCppDlg 대화 상자
class CKiwoomRestfulCppDlg : public CDialogEx
{
// 생성입니다.
public:
	CKiwoomRestfulCppDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// KIwoom stuff
private:
	// crow::SimpleApp *app;
	void *crowApp; // Sorry man! It's not easy to include crow_all.h here!
	CKHOpenAPI kiwoom;
	bool kiwoomConnected;
	int reqno;

public:
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KIWOOMRESTFULCPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void KiwoomOnEventConnect(long nErrCode);
	afx_msg void OnDestroy();
};
