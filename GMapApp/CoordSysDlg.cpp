// CoordSysDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GMapApp.h"
#include "CoordSysDlg.h"
#include "GCoordinateSystem.h"
using namespace gmap;

// CCoordSysDlg �Ի���

IMPLEMENT_DYNAMIC(CCoordSysDlg, CDialog)

CCoordSysDlg::CCoordSysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCoordSysDlg::IDD, pParent)
{
	ImageList.Create(IDB_BITMAP_COORD_SYS,16,ILC_COLOR24,0X00000000);
	SelectedCoordinateSystem = NULL;
}

CCoordSysDlg::~CCoordSysDlg()
{
}

void CCoordSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COORD_SYS_TREE, mCoordSysTree);
	DDX_Control(pDX, IDC_PARAMETERS_EDIT, mCoordSysParameterEdit);
	DDX_Control(pDX, IDOK, mOKButton);
}


BEGIN_MESSAGE_MAP(CCoordSysDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCoordSysDlg::OnBnClickedOk)
	ON_NOTIFY(TVN_SELCHANGED, IDC_COORD_SYS_TREE, &CCoordSysDlg::OnTvnSelchangedCoordSysTree)
END_MESSAGE_MAP()


// CCoordSysDlg ��Ϣ�������

void CCoordSysDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

BOOL CCoordSysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	
	mCoordSysTree.SetImageList(&ImageList,TVSIL_NORMAL);

	UINT  mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_IMAGE|TVIF_TEXT|TVIF_PARAM;

	HTREEITEM root = mCoordSysTree.InsertItem(mask,L"����ϵ",0,0,0,0,-1,TVI_ROOT,TVI_LAST);

	HTREEITEM nullCS = mCoordSysTree.InsertItem(mask,L"Unknown",  2,2,0,0,0,root,TVI_LAST);

	HTREEITEM geoCS = mCoordSysTree.InsertItem(mask,L"��������ϵ",1,1,0,0,-1,root,TVI_LAST);

	HTREEITEM prjCS = mCoordSysTree.InsertItem(mask,L"ͶӰ����ϵ",1,1,0,0,-1,root,TVI_LAST);

	int count = gmap::GCoordinateSystem::GetEPGSCoordSysCount();

	USES_CONVERSION;
	for(int i=0;i<count;i++)
	{
		GCoordinateSystem* pCS = GCoordinateSystem::GetEPSGCoordinateSystem(i);
		if(pCS->GetType()==GCoordinateSystem::CST_LONLAT)
		{
			string item = pCS->GetName()+string("->")+pCS->GetWkName();
			mCoordSysTree.InsertItem(mask,A2T(item.c_str()),2,2,0,0,(LPARAM)pCS,geoCS,TVI_LAST);
		}
		else if(pCS->GetType()==GCoordinateSystem::CST_PROJECTION)
		{
			GCoordinateSystem* proj = pCS;
			string item = proj->GetName()+string("->")+pCS->GetWkName();
			mCoordSysTree.InsertItem(mask,A2T(item.c_str()),2,2,0,0,(LPARAM)pCS,prjCS,TVI_LAST);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCoordSysDlg::OnTvnSelchangedCoordSysTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	TVITEMW item =  pNMTreeView->itemNew;
	
	if(item.lParam!=-1)
	{
		::EnableWindow(mOKButton.GetSafeHwnd(),TRUE);
		if(item.lParam!=0)
		{
			GCoordinateSystem* pCS =(GCoordinateSystem*)item.lParam;
			USES_CONVERSION;			
			mCoordSysParameterEdit.SetLimitText(4024);
			if(pCS->GetType()==GCoordinateSystem::CST_LONLAT)
			{
				mCoordSysParameterEdit.SetWindowText(A2T(pCS->GetParameters().c_str()));
			}
			else if(pCS->GetType()==GCoordinateSystem::CST_PROJECTION)
			{
				GCoordinateSystem* proj = (GCoordinateSystem*)pCS;
				mCoordSysParameterEdit.SetWindowText(A2T(proj->GetParameters().c_str()));
			}
			SelectedCoordinateSystem = pCS;
		}
		else
		{	
			mCoordSysParameterEdit.SetWindowText(L"δ����");			
			SelectedCoordinateSystem = NULL;
		}
	}
	else
	{
		::EnableWindow(mOKButton.GetSafeHwnd(),FALSE);
	}

	*pResult = 0;
}
