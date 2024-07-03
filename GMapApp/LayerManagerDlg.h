#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "GMapApp.h"
#include "GMapAppDoc.h"
#include "gmap-include.h"
// CLayerManagerDlg 对话框

class CLayerManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CLayerManagerDlg)

public:
	CLayerManagerDlg(CWnd* pParent,GMapAppDoc* pDocument);   // 标准构造函数
	virtual ~CLayerManagerDlg();

// 对话框数据
	enum { IDD = IDD_LAYER_MANAGER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl mLayerListCtrl;
	afx_msg void OnBnClickedNewLayerBtn();
	afx_msg void OnLvnItemchangedLayerListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDeleteLayerBtn();
	afx_msg void OnBnClickedLayerVisibleChkCtrl();
	afx_msg void OnBnClickedLayerPropCtrl();


	CButton mLayerVisibleChkCtrl;
	CButton mLayerPropBtnCtrl;
	CButton mDeleteLayerBtnCtrl;
	afx_msg void OnNewGraphicsLayer();
	CButton mNewLayerBtnCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkLayerListCtrl(NMHDR *pNMHDR, LRESULT *pResult);

	void RefreshCurrentLayerLabel();
private:
	GMap*					mMap;
	GMapAppDoc*				mDocument;
public:
	afx_msg void OnNewVectorLayer();
	afx_msg void OnBnClickedBtnMapCoordsys();
	afx_msg void OnNewImageLayer();
	afx_msg void OnViewLayerStyle();
	afx_msg void OnSetSimpleRender();
	afx_msg void OnSetClassRender();
};
