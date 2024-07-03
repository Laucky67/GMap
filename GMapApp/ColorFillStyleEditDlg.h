#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "GStyleGroup.h"

using namespace gmap;

// CColorFillStyleEditDlg 对话框

class CColorFillStyleEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CColorFillStyleEditDlg)

public:
	CColorFillStyleEditDlg(CWnd* pParent = NULL,GColorFillStyle* pFillStyle=NULL);   // 标准构造函数
	virtual ~CColorFillStyleEditDlg();

// 对话框数据
	enum { IDD = IDD_COLOR_FILL_STYLE_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit mNameEditCtrl;
	CEdit mAlphaEditCtrl;
	CSliderCtrl mAlphaSliderCtrl;
	CStatic mColorPanelCtrl;
	CStatic mPreviewPanelCtrl;

	afx_msg void OnNMCustomdrawAlphaSliderEditCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedForeColorBtn();
	
	virtual BOOL OnInitDialog();

private:
	GColorFillStyle* mFillStyle;
public:
	afx_msg void OnEnChangeNameEditCtrl();
	afx_msg void OnPaint();
	afx_msg void OnNMReleasedcaptureAlphaSliderEditCtrl(NMHDR *pNMHDR, LRESULT *pResult);
};
