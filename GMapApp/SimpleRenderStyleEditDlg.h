#pragma once


#include "afxwin.h"
#include "gmap-include.h"
// CSimpleRenderStyleEditDlg 对话框

class CSimpleRenderStyleEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CSimpleRenderStyleEditDlg)

public:
	CSimpleRenderStyleEditDlg(GMap* pMap,GSimpleVectorFeatureRender* pRender, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSimpleRenderStyleEditDlg();

// 对话框数据
	enum { IDD = IDD_DEF_STYLE_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	GSimpleVectorFeatureRender* mRender;
	GMap*			mMap;
	
public:
	CEdit   mNameEditCtrl;
	CStatic mMarkerStylePanelCtrl;
	CStatic mLineStylePanelCtrl;
	CStatic mFillStylePanelCtrl;
	afx_msg void OnBnClickedSelMarkerStyleBtn();
	afx_msg void OnBnClickedSelLineStyleBtn();
	afx_msg void OnBnClickedSelFillStyleBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
