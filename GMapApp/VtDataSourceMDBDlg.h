#pragma once
#include "afxcmn.h"
#include "gmap-include.h"
#include "IProgressUI.h"
#include "GMapAppDoc.h"
#include "afxwin.h"

// CDataSourceMDBDlg �Ի���

class CVtDataSourceMDBDlg : public CDialog,public gmap::IProgressUI
{
	DECLARE_DYNAMIC(CVtDataSourceMDBDlg)

public:
	CVtDataSourceMDBDlg(GMapAppDoc* pAppDoc,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVtDataSourceMDBDlg();

// �Ի�������
	enum { IDD = IDD_DATA_SOURCE_MDB_DLG };

	GMapAppDoc*		   mDocument;
	GVectorDataSource* mDataSource;
	GVectorDataLayer*  mSelectDataLayer;

	void FillDataLayerList();
	void FillDataSourceCBX();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnImport();
	CListCtrl mDataLayerListCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnHdnItemdblclickDataLayerListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkDataLayerListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnRebuildSpatialIndex();

private:
	wstring Title;
	u32 Total;
	u32 Progress;
	int SelectId;
public:
	//
	virtual void OnSetTotal(const wchar_t* id,u32 total);
	//
	virtual void OnProgress(const wchar_t* id,u32 progress); 
	//!����
	virtual void OnProgress(const wchar_t* id,u32 progress,u32 total); 

	void UpdateProgressUI();
	afx_msg void OnBnClickedBtnSetCs();
	afx_msg void OnCbnSelchangeDataSourceCombox();
	CComboBox mDataSourceCBX;
	afx_msg void OnBnClickedButtonDataSource();
	CButton mButtonDataSource;
	afx_msg void OnOpenVtDatasource();
	afx_msg void OnCreateVtDatasource();
};
