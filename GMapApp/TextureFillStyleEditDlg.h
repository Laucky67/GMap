#pragma once
#include "afxwin.h"
#include "GStyleGroup.h"
using namespace gmap;

// CTextureFillStyleEditDlg 对话框

class CTextureFillStyleEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CTextureFillStyleEditDlg)

public:
	CTextureFillStyleEditDlg(CWnd* pParent = NULL,GTextureFillStyle* fillStyle = NULL);   // 标准构造函数
	virtual ~CTextureFillStyleEditDlg();

// 对话框数据
	enum { IDD = IDD_TEXTURE_FILL_STYLE_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit mNameEditCtrl;
	CEdit mImageFileEditCtrl;
	afx_msg void OnBnClickedBtnOpenImageFile();
	CComboBox mWrapModeCbxCtrl;
	CStatic mPreviewPanelCtrl;

private:
	GTextureFillStyle* mFillStyle;
	

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeWrapModeCbx();
	afx_msg void OnNewTextureFillStyle();
};
