#pragma once

#include "afxwin.h"
#include "gmap-include.h"

// CValueClassRenderDialog 对话框
using StyleClassItem = GValueClassRender::StyleClassItem;

class CValueClassRenderDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CValueClassRenderDialog)

public:
	CValueClassRenderDialog(GMap* map,GVectorDataLayer* dataLayer,GValueClassRender* classRender,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CValueClassRenderDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VALUE_CLASS_RENDER_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	void  updateClassIds();
	void  updateListView();
public:
	CComboBox	m_cbFields;
	CListCtrl	m_lvValueClass;
	CStatic		m_stMStyle;
	CStatic		m_stLStyle;
	CStatic		m_stFStyle;

public:
	GMap*						Map;
	GVectorDataLayer*			DataLayer;
	wstring						ClassField;
	GValueClassRender*			ClassRender;
	StyleClassItem*				CurrentStyleItem;
	vector<pair<int,bool>>		ClassIDs;
	virtual BOOL				OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClickedBtnLstyle();
	afx_msg void OnClickedBtnMstyle();
	afx_msg void OnClickedBtnFstyle();
	afx_msg void OnSelchangeCbField();
	afx_msg void OnClickListValueClass(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListValueClass(NMHDR *pNMHDR, LRESULT *pResult);
};
