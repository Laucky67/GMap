#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "IProgressUI.h"

using namespace gmap;
// CProgressDlg 对话框

class CProgressDlg : public CDialog,public gmap::IProgressUI
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProgressDlg();

// 对话框数据
	enum { IDD = IDD_PROGRESS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic CPrgTitleTxt;
	CStatic CPrgDisplayTxt;
	CProgressCtrl mProgressCtrl;

protected:
	wstring Title;
	u32 Total;
	u32 Progress;
public:
	//
	virtual void OnSetTotal(const wchar_t* id,u32 total);
	//
	virtual void OnProgress(const wchar_t* id,u32 progress);
	//!进度
	virtual void OnProgress(const wchar_t* id,u32 progress,u32 total);

	void UpdateUI();
};
