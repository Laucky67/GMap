#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "IProgressUI.h"

using namespace gmap;
// CProgressDlg �Ի���

class CProgressDlg : public CDialog,public gmap::IProgressUI
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProgressDlg();

// �Ի�������
	enum { IDD = IDD_PROGRESS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	//!����
	virtual void OnProgress(const wchar_t* id,u32 progress,u32 total);

	void UpdateUI();
};
