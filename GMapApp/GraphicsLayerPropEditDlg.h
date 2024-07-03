#pragma once


#include "afxwin.h"
#include "gmap-include.h"
// CGraphicsLayerPropEditDlg �Ի���

class CGraphicsLayerPropEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CGraphicsLayerPropEditDlg)

public:
	CGraphicsLayerPropEditDlg(CWnd* pParent,GGraphicsLayer* layer,GMap* map);   // ��׼���캯��
	virtual ~CGraphicsLayerPropEditDlg();

// �Ի�������
	enum { IDD = IDD_DEF_STYLE_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
