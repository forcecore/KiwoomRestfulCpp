
// KiwoomRestfulCppDlg.h: 헤더 파일
//

#pragma once

#include <string>
#include <map>


// CKiwoomRestfulCppDlg 대화 상자
class CKiwoomRestfulCppDlg : public CDialogEx
{
// 생성입니다.
public:
	CKiwoomRestfulCppDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// KIwoom stuff
private:
	void initCrowHandlers(void *voidCrowApp);

	// crow::SimpleApp *app;
	void *crowApp; // Sorry man! It's not easy to include crow_all.h here!
	CKHOpenAPI kiwoom;
	bool kiwoomConnected;
	int reqno;
	std::map<std::wstring, bool> reqMap;
	std::map<std::wstring, std::wstring> resultMap;

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
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void KiwoomOnEventConnect(long nErrCode);
	void KiwoomOnReceiveRealData(LPCTSTR sRealKey, LPCTSTR sRealType, LPCTSTR sRealData);
	void KiwoomOnReceiveMsg(LPCTSTR sScrNo, LPCTSTR sRQName, LPCTSTR sTrCode, LPCTSTR sMsg);
	void KiwoomOnReceiveChejanData(LPCTSTR sGubun, long nItemCnt, LPCTSTR sFIdList);
	void KiwoomOnReceiveInvestRealData(LPCTSTR sRealKey);
	void KiwoomOnReceiveRealCondition(LPCTSTR sTrCode, LPCTSTR strType, LPCTSTR strConditionName, LPCTSTR strConditionIndex);
	void KiwoomOnReceiveTrCondition(LPCTSTR sScrNo, LPCTSTR strCodeList, LPCTSTR strConditionName, long nIndex, long nNext);
	void KiwoomOnReceiveConditionVer(long lRet, LPCTSTR sMsg);
	void KiwoomOnReceiveTrData(LPCTSTR sScrNo, LPCTSTR sRQName, LPCTSTR sTrCode, LPCTSTR sRecordName, LPCTSTR sPrevNext, long nDataLength, LPCTSTR sErrorCode, LPCTSTR sMessage, LPCTSTR sSplmMsg);
};
