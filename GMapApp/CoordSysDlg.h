#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "GCoordinateSystem.h"

// CCoordSysDlg �Ի���

class CCoordSysDlg : public CDialog
{
	DECLARE_DYNAMIC(CCoordSysDlg)

public:
	CCoordSysDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCoordSysDlg();

// �Ի�������
	enum { IDD = IDD_COORDSYS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
