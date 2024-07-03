#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "GStyleGroup.h"

using namespace gmap;

// CSimpleLineStyleEditDlg �Ի���

class CSimpleLineStyleEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CSimpleLineStyleEditDlg)

public:
	CSimpleLineStyleEditDlg(CWnd* pParent = NULL,GSimpleLineStyle* pStyle = NULL);   // ��׼���캯��
	virtual ~CSimpleLineStyleEditDlg();

// �Ի�������
	enum { IDD = IDD_SIMPLE_LINE_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit mNameEditCtrl;
	CStatic mColorPanelCtrl;
	afx_msg void OnBnClickedLineColorBtn();
	CEdit mLineWidthEditCtrl;
	CSpinButtonCtrl mLineWidthSpinCtrl;
	CComboBox mDashStyleCbxCtrl;
	CComboBox mDashCapCbxCtrl;
	CComboBox mStartCapCbxCtrl;
	CComboBox mEndCapCbxCtrl;
	CComboBox mLineJionCbxCtrl;
	CStatic mPreviewPanelCtrl;
	virtual BOOL OnInitDialog();

private:
	GSimpleLineStyle* mLineStyle;
	
public:
	afx_msg void OnCbnSelchangeDashStyleCmb();
	afx_msg void OnCbnSelchangeDashCapCmb();
	afx_msg void OnCbnSelchangeStartCapCmb();
	afx_msg void OnCbnSelchangeEndCapCmb();
	afx_msg void OnCbnSelchangeLineJoinCmb();
	afx_msg void OnEnChangeNameEditCtrl();
	afx_msg void OnPaint();
	afx_msg void OnDeltaposLineWidthSpin(NMHDR *pNMHDR, LRESULT *pResult);
};
