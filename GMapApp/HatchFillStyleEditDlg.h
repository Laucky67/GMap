#pragma once
#include "afxwin.h"
#include "HatchStyleComboBox.h"
#include "GStyleGroup.h"
using namespace gmap;

// CHatchFillStyleEditDlg 对话框

class CHatchFillStyleEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CHatchFillStyleEditDlg)

public:
	CHatchFillStyleEditDlg(CWnd* pParent = NULL,GHatchFillStyle* fillStyle=NULL);   // 标准构造函数
	virtual ~CHatchFillStyleEditDlg();

// 对话框数据
	enum { IDD = IDD_HATCH_FILL_STYLE_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit mNameEditCtrl;
	CStatic mForeColorPanelCtrl;
	afx_msg void OnBnClickedForeColorBtn();
	afx_msg void OnBnClickedBkColorBtn();
	CStatic mBkColorPanelCtrl;
	CHatchStyleComboBox mHatchStyleCbxCtrl;
	CStatic mPreviewPanelCtrl;
	virtual BOOL OnInitDialog();

private:
	GHatchFillStyle* mFillStyle;
public:
	afx_msg void OnCbnSelchangeHatchStyleCmb();
	afx_msg void OnPaint();
	afx_msg void OnEnChangeNameEditCtrl();
};
