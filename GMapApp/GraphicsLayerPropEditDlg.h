#pragma once


#include "afxwin.h"
#include "gmap-include.h"
// CGraphicsLayerPropEditDlg 对话框

class CGraphicsLayerPropEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CGraphicsLayerPropEditDlg)

public:
	CGraphicsLayerPropEditDlg(CWnd* pParent,GGraphicsLayer* layer,GMap* map);   // 标准构造函数
	virtual ~CGraphicsLayerPropEditDlg();

// 对话框数据
	enum { IDD = IDD_DEF_STYLE_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	GGraphicsLayer* mLayer;
	GMap*			mMap;
	
public:
	CEdit mNameEditCtrl;
	CStatic mMarkerStylePanelCtrl;
	CStatic mLineStylePanelCtrl;
	CStatic mFillStylePanelCtrl;
	afx_msg void OnBnClickedSelMarkerStyleBtn();
	afx_msg void OnBnClickedSelLineStyleBtn();
	afx_msg void OnBnClickedSelFillStyleBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
