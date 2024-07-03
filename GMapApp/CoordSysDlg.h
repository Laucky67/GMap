#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "GCoordinateSystem.h"

// CCoordSysDlg 对话框

class CCoordSysDlg : public CDialog
{
	DECLARE_DYNAMIC(CCoordSysDlg)

public:
	CCoordSysDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCoordSysDlg();

// 对话框数据
	enum { IDD = IDD_COORDSYS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl mCoordSysTree;
	CEdit mCoordSysParameterEdit;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	CImageList ImageList;
	CButton mOKButton;
	afx_msg void OnTvnSelchangedCoordSysTree(NMHDR *pNMHDR, LRESULT *pResult);

	gmap::GCoordinateSystem* SelectedCoordinateSystem;
};
