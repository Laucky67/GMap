#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "GStyleGroup.h"

using namespace gmap;

// CColorFillStyleEditDlg �Ի���

class CColorFillStyleEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CColorFillStyleEditDlg)

public:
	CColorFillStyleEditDlg(CWnd* pParent = NULL,GColorFillStyle* pFillStyle=NULL);   // ��׼���캯��
	virtual ~CColorFillStyleEditDlg();

// �Ի�������
	enum { IDD = IDD_COLOR_FILL_STYLE_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
