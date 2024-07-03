#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "GStyleGroup.h"
using namespace gmap;


// CImageMerkerStyleEditDlg 对话框

class CImageMerkerStyleEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageMerkerStyleEditDlg)

public:
	CImageMerkerStyleEditDlg(CWnd* pParent = NULL,GImageMarkerStyle* markerStyle=NULL);   // 标准构造函数
	virtual ~CImageMerkerStyleEditDlg();

// 对话框数据
	enum { IDD = IDD_IMAGE_MARKER_STYLE_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit mNameEditCtrl;
	CEdit mImageFileEditCtrl;
	afx_msg void OnBtnOpenImageFile_Click();
	CEdit mXOffsetEditCtrl;
	CEdit mYOffsetEditCtrl;
	CEdit mWidthEditCtrl;
	CEdit mHeightEditCtrl;
	CSpinButtonCtrl mXOffsetSpinCtrl;
	CSpinButtonCtrl mYOffsetSpinCtrl;
	CSpinButtonCtrl mWidthSpinCtrl;
	CSpinButtonCtrl mHeightSpinCtrl;

private:
	GImageMarkerStyle* mMarkerStyle;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaPosXOffsetSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaPosYOffsetSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaPosWidthSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaPosHeightSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	void Preview();
	CStatic mPreviewCtrl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeNameEditCtrl();
};
