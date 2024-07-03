#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "gmap-include.h"


// CStyleManagerDlg 对话框

class CStyleManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CStyleManagerDlg)

public:
	CStyleManagerDlg(CWnd* pParent,GStyleGroup* styleGroup,int showFlag=4);   // 标准构造函数
	virtual ~CStyleManagerDlg();

// 对话框数据
	enum { IDD = IDD_STYLE_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnNewStyle();
	CButton mBtnNewStyle;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);


	CTabCtrl mStyleTabCtrl;
	CListCtrl mStyleListCtrl;
	int		  mShowTabFlag;
	afx_msg void OnNMCustomdrawStyleListCtrl(NMHDR *pNMHDR, LRESULT *pResult);

	void FillList(int currentStyle);

private:
	Graphics* mListCtrlGraphics;
	Pen	       *mDashPen;
	SolidBrush *mFillBrush;
	gmap::GStyleGroup*	mStyleGroup;
	gmap::GStyle*		mCurrentStyle;

public:
	afx_msg void OnLvnItemchangedStyleListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit mEditStyleID;
	CEdit mEditStyleName;
	CStatic mStylePanelCtrl;
	afx_msg void OnPaint();
	afx_msg void OnNewImageMarkerStyle();
	afx_msg void OnNewSimpleLineStyle();
	afx_msg void OnNewColorFillStyle();
	afx_msg void OnNewHatchFillStyle();
	afx_msg void OnNewTextureFillStyle();
	afx_msg void OnBnClickedEditBtnCtrl();

	GStyle*	GetSelectStyle(){return mCurrentStyle;}

	
};
