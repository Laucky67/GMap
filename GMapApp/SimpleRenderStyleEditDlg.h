#pragma once


#include "afxwin.h"
#include "gmap-include.h"
// CSimpleRenderStyleEditDlg �Ի���

class CSimpleRenderStyleEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CSimpleRenderStyleEditDlg)

public:
	CSimpleRenderStyleEditDlg(GMap* pMap,GSimpleVectorFeatureRender* pRender, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSimpleRenderStyleEditDlg();

// �Ի�������
	enum { IDD = IDD_DEF_STYLE_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
