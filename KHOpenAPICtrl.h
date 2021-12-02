// KHOpenAPI.h  : Microsoft Visual C++로 만든 ActiveX 컨트롤 래퍼 클래스의 선언입니다.

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CKHOpenAPI

class CKHOpenAPI : public CWnd
{
protected:
	DECLARE_DYNCREATE(CKHOpenAPI)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xA1574A0D, 0x6BFA, 0x4BD7, { 0x90, 0x20, 0xDE, 0xD8, 0x87, 0x11, 0x81, 0x8D } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 특성입니다.
public:

// 작업입니다.
public:

	long CommConnect()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void CommTerminate()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long CommRqData(LPCTSTR sRQName, LPCTSTR sTrCode, long nPrevNext, LPCTSTR sScreenNo)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sRQName, sTrCode, nPrevNext, sScreenNo);
		return result;
	}
	CString GetLoginInfo(LPCTSTR sTag)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sTag);
		return result;
	}
	long SendOrder(LPCTSTR sRQName, LPCTSTR sScreenNo, LPCTSTR sAccNo, long nOrderType, LPCTSTR sCode, long nQty, long nPrice, LPCTSTR sHogaGb, LPCTSTR sOrgOrderNo)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sRQName, sScreenNo, sAccNo, nOrderType, sCode, nQty, nPrice, sHogaGb, sOrgOrderNo);
		return result;
	}
	long SendOrderFO(LPCTSTR sRQName, LPCTSTR sScreenNo, LPCTSTR sAccNo, LPCTSTR sCode, long lOrdKind, LPCTSTR sSlbyTp, LPCTSTR sOrdTp, long lQty, LPCTSTR sPrice, LPCTSTR sOrgOrdNo)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sRQName, sScreenNo, sAccNo, sCode, lOrdKind, sSlbyTp, sOrdTp, lQty, sPrice, sOrgOrdNo);
		return result;
	}
	void SetInputValue(LPCTSTR sID, LPCTSTR sValue)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sID, sValue);
	}
	long SetOutputFID(LPCTSTR sID)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sID);
		return result;
	}
	CString CommGetData(LPCTSTR sJongmokCode, LPCTSTR sRealType, LPCTSTR sFieldName, long nIndex, LPCTSTR sInnerFieldName)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sJongmokCode, sRealType, sFieldName, nIndex, sInnerFieldName);
		return result;
	}
	void DisconnectRealData(LPCTSTR sScnNo)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sScnNo);
	}
	long GetRepeatCnt(LPCTSTR sTrCode, LPCTSTR sRecordName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sTrCode, sRecordName);
		return result;
	}
	long CommKwRqData(LPCTSTR sArrCode, long bNext, long nCodeCount, long nTypeFlag, LPCTSTR sRQName, LPCTSTR sScreenNo)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sArrCode, bNext, nCodeCount, nTypeFlag, sRQName, sScreenNo);
		return result;
	}
	CString GetAPIModulePath()
	{
		CString result;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetCodeListByMarket(LPCTSTR sMarket)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sMarket);
		return result;
	}
	long GetConnectState()
	{
		long result;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetMasterCodeName(LPCTSTR sTrCode)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sTrCode);
		return result;
	}
	long GetMasterListedStockCnt(LPCTSTR sTrCode)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sTrCode);
		return result;
	}
	CString GetMasterConstruction(LPCTSTR sTrCode)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sTrCode);
		return result;
	}
	CString GetMasterListedStockDate(LPCTSTR sTrCode)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sTrCode);
		return result;
	}
	CString GetMasterLastPrice(LPCTSTR sTrCode)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sTrCode);
		return result;
	}
	CString GetMasterStockState(LPCTSTR sTrCode)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sTrCode);
		return result;
	}
	long GetDataCount(LPCTSTR strRecordName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strRecordName);
		return result;
	}
	CString GetOutputValue(LPCTSTR strRecordName, long nRepeatIdx, long nItemIdx)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strRecordName, nRepeatIdx, nItemIdx);
		return result;
	}
	CString GetCommData(LPCTSTR strTrCode, LPCTSTR strRecordName, long nIndex, LPCTSTR strItemName)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strTrCode, strRecordName, nIndex, strItemName);
		return result;
	}
	CString GetCommRealData(LPCTSTR sTrCode, long nFid)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sTrCode, nFid);
		return result;
	}
	CString GetChejanData(long nFid)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nFid);
		return result;
	}
	CString GetThemeGroupList(long nType)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nType);
		return result;
	}
	CString GetThemeGroupCode(LPCTSTR strThemeCode)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strThemeCode);
		return result;
	}
	CString GetFutureList()
	{
		CString result;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetFutureCodeByIndex(long nIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nIndex);
		return result;
	}
	CString GetActPriceList()
	{
		CString result;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetMonthList()
	{
		CString result;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetOptionCode(LPCTSTR strActPrice, long nCp, LPCTSTR strMonth)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BSTR ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strActPrice, nCp, strMonth);
		return result;
	}
	CString GetOptionCodeByMonth(LPCTSTR sTrCode, long nCp, LPCTSTR strMonth)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BSTR ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sTrCode, nCp, strMonth);
		return result;
	}
	CString GetOptionCodeByActPrice(LPCTSTR sTrCode, long nCp, long nTick)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sTrCode, nCp, nTick);
		return result;
	}
	CString GetSFutureList(LPCTSTR strBaseAssetCode)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strBaseAssetCode);
		return result;
	}
	CString GetSFutureCodeByIndex(LPCTSTR strBaseAssetCode, long nIndex)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strBaseAssetCode, nIndex);
		return result;
	}
	CString GetSActPriceList(LPCTSTR strBaseAssetGb)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strBaseAssetGb);
		return result;
	}
	CString GetSMonthList(LPCTSTR strBaseAssetGb)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strBaseAssetGb);
		return result;
	}
	CString GetSOptionCode(LPCTSTR strBaseAssetGb, LPCTSTR strActPrice, long nCp, LPCTSTR strMonth)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strBaseAssetGb, strActPrice, nCp, strMonth);
		return result;
	}
	CString GetSOptionCodeByMonth(LPCTSTR strBaseAssetGb, LPCTSTR sTrCode, long nCp, LPCTSTR strMonth)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strBaseAssetGb, sTrCode, nCp, strMonth);
		return result;
	}
	CString GetSOptionCodeByActPrice(LPCTSTR strBaseAssetGb, LPCTSTR sTrCode, long nCp, long nTick)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strBaseAssetGb, sTrCode, nCp, nTick);
		return result;
	}
	CString GetSFOBasisAssetList()
	{
		CString result;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetOptionATM()
	{
		CString result;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetSOptionATM(LPCTSTR strBaseAssetGb)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strBaseAssetGb);
		return result;
	}
	CString GetBranchCodeName()
	{
		CString result;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long CommInvestRqData(LPCTSTR sMarketGb, LPCTSTR sRQName, LPCTSTR sScreenNo)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sMarketGb, sRQName, sScreenNo);
		return result;
	}
	long SendOrderCredit(LPCTSTR sRQName, LPCTSTR sScreenNo, LPCTSTR sAccNo, long nOrderType, LPCTSTR sCode, long nQty, long nPrice, LPCTSTR sHogaGb, LPCTSTR sCreditGb, LPCTSTR sLoanDate, LPCTSTR sOrgOrderNo)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sRQName, sScreenNo, sAccNo, nOrderType, sCode, nQty, nPrice, sHogaGb, sCreditGb, sLoanDate, sOrgOrderNo);
		return result;
	}
	CString KOA_Functions(LPCTSTR sFunctionName, LPCTSTR sParam)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sFunctionName, sParam);
		return result;
	}
	long SetInfoData(LPCTSTR sInfoData)
	{
		long result;
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sInfoData);
		return result;
	}
	long SetRealReg(LPCTSTR strScreenNo, LPCTSTR strCodeList, LPCTSTR strFidList, LPCTSTR strOptType)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strScreenNo, strCodeList, strFidList, strOptType);
		return result;
	}
	long GetConditionLoad()
	{
		long result;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetConditionNameList()
	{
		CString result;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL SendCondition(LPCTSTR strScrNo, LPCTSTR strConditionName, int nIndex, int nSearch)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I2 VTS_I2;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strScrNo, strConditionName, nIndex, nSearch);
		return result;
	}
	void SendConditionStop(LPCTSTR strScrNo, LPCTSTR strConditionName, int nIndex)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I2;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strScrNo, strConditionName, nIndex);
	}
	VARIANT GetCommDataEx(LPCTSTR strTrCode, LPCTSTR strRecordName)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, strTrCode, strRecordName);
		return result;
	}
	void SetRealRemove(LPCTSTR strScrNo, LPCTSTR strDelCode)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strScrNo, strDelCode);
	}
	long GetMarketType(LPCTSTR strCode)
	{
		long result;
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strCode);
		return result;
	}
};
