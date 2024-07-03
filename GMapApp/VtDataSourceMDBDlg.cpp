// DataSourceMDBDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GMapApp.h"
#include "VtDataSourceMDBDlg.h"
#include "ProgressDlg.h"
#include "CoordSysDlg.h"
#include "ogrsf_frmts.h"
#include "GOGRImporter.h"
#include <fstream>

// CDataSourceMDBDlg 对话框

IMPLEMENT_DYNAMIC(CVtDataSourceMDBDlg, CDialog)

CVtDataSourceMDBDlg::CVtDataSourceMDBDlg(GMapAppDoc* pAppDoc ,CWnd* pParent /*=NULL*/)
	: CDialog(CVtDataSourceMDBDlg::IDD, pParent)
{
	mDocument = pAppDoc;
	mDataSource = NULL;
	mSelectDataLayer = NULL;
}

CVtDataSourceMDBDlg::~CVtDataSourceMDBDlg()
{
}

void CVtDataSourceMDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_LAYER_LIST_CTRL, mDataLayerListCtrl);
	DDX_Control(pDX, IDC_COMBOX_DATA_SOURCES, mDataSourceCBX);
	DDX_Control(pDX, IDC_BUTTON_DATA_SOURCE, mButtonDataSource);
}


BEGIN_MESSAGE_MAP(CVtDataSourceMDBDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CVtDataSourceMDBDlg::OnBnClickedBtnImport)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CVtDataSourceMDBDlg::OnHdnItemdblclickDataLayerListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_DATA_LAYER_LIST_CTRL, &CVtDataSourceMDBDlg::OnNMDblclkDataLayerListCtrl)
	ON_BN_CLICKED(IDC_BTN_REBUILD_SPATIAL_INDEX, &CVtDataSourceMDBDlg::OnBnClickedBtnRebuildSpatialIndex)
	ON_BN_CLICKED(IDC_BTN_SET_CS, &CVtDataSourceMDBDlg::OnBnClickedBtnSetCs)
	ON_CBN_SELCHANGE(IDC_COMBOX_DATA_SOURCES, &CVtDataSourceMDBDlg::OnCbnSelchangeDataSourceCombox)
	ON_BN_CLICKED(IDC_BUTTON_DATA_SOURCE, &CVtDataSourceMDBDlg::OnBnClickedButtonDataSource)
	ON_COMMAND(ID_OPEN_VT_DATASOURCE, &CVtDataSourceMDBDlg::OnOpenVtDatasource)
	ON_COMMAND(ID_CREATE_VT_DATASOURCE, &CVtDataSourceMDBDlg::OnCreateVtDatasource)
END_MESSAGE_MAP()


// CDataSourceMDBDlg 消息处理程序

void CVtDataSourceMDBDlg::OnBnClickedBtnImport()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加命令处理程序代码
	static wchar_t* shapeFilter = L"shp文件(*.shp)|*.shp";
	CFileDialog OpenFileDlg(TRUE,NULL,NULL,4|2,shapeFilter,NULL);
	if(OpenFileDlg.DoModal() == IDOK)
	{
		CString fn = OpenFileDlg.GetPathName();
		USES_CONVERSION;

		wstring pathName(fn.GetBuffer());
		OGRDataSource* pDataSource = OGRSFDriverRegistrar::Open(W2A(fn.GetBuffer()), FALSE );
		
		int extPos = pathName.find_last_of(wstring(L"."));
		int slashPos = pathName.find_last_of(wstring(L"\\"));

		wstring layerName = pathName.substr(slashPos+1,extPos-slashPos-1);
		wstring cpg = pathName.substr(0, extPos) + L".cpg";
		std::ifstream stream(cpg);
		string cp;
		if (stream.is_open())
		{
			stream >> cp;
		}
		bool utf8 = cp == "UTF-8";
		GVectorDataLayer* pLayer = GOGRImporter::Import(layerName,pDataSource->GetLayer(0),mDataSource,NULL, utf8);

		OGRDataSource::DestroyDataSource(pDataSource);
	}
	FillDataLayerList();
}

BOOL CVtDataSourceMDBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
		// TODO:  在此添加额外的初始化
	mDataLayerListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	int i=0;
	mDataLayerListCtrl.InsertColumn(i++,L"序号");    //0
	mDataLayerListCtrl.InsertColumn(i++,L"图层名称");//1
	mDataLayerListCtrl.InsertColumn(i++,L"图层类型");//2
	mDataLayerListCtrl.InsertColumn(i++,L"空间参考");//3
	mDataLayerListCtrl.InsertColumn(i++,L"空间索引");//4
	mDataLayerListCtrl.InsertColumn(i++,L"MinX");
	mDataLayerListCtrl.InsertColumn(i++,L"MinY");
	mDataLayerListCtrl.InsertColumn(i++,L"MaxX");
	mDataLayerListCtrl.InsertColumn(i++,L"MaxY");
	
	i=0;
	mDataLayerListCtrl.SetColumnWidth(i++,40);
	mDataLayerListCtrl.SetColumnWidth(i++,100);
	mDataLayerListCtrl.SetColumnWidth(i++,100);
	mDataLayerListCtrl.SetColumnWidth(i++,100);
	mDataLayerListCtrl.SetColumnWidth(i++,100);
	mDataLayerListCtrl.SetColumnWidth(i++,100);
	mDataLayerListCtrl.SetColumnWidth(i++,100);
	mDataLayerListCtrl.SetColumnWidth(i++,100);
	mDataLayerListCtrl.SetColumnWidth(i++,100);

	USES_CONVERSION;
	for each(auto it in mDocument->GetVectorDataSources())
	{
		mDataSourceCBX.AddString(it->GetName().c_str());
	}
	mDataSourceCBX.SetCurSel(0);

	const vector<GVectorDataSource*>& datasources = mDocument->GetVectorDataSources();
	if (datasources.size() > 0)
	{
		mDataSource = datasources.front();
	}
	FillDataLayerList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CVtDataSourceMDBDlg::FillDataLayerList()
{
	if (!mDataSource)
		return;
	mDataLayerListCtrl.DeleteAllItems();
	for(int i=0;i<mDataSource->GetVectorLayerCount();i++)
	{
		GVectorDataLayer* layer=mDataSource->GetLayer(i);

		CString format;
		format.Format(L"%d",i+1);
		int id = mDataLayerListCtrl.InsertItem(i,format.GetBuffer());

		int n=1;
		mDataLayerListCtrl.SetItem(id,n++,LVIF_TEXT,layer->GetName().c_str(),0,0,0,0,0);
		
		
		wstring layerType;
		if(layer->GetLayerType()==GVectorDataLayer::VLT_POINT)
		{
			layerType=wstring(L"Point");
		}
		else  if(layer->GetLayerType()==GVectorDataLayer::VLT_LINE_STRING)
		{
			layerType=wstring(L"Line");
		}
		else  if(layer->GetLayerType()==GVectorDataLayer::VLT_POLYGON)
		{
			layerType=wstring(L"Polygon");
		}
		else
		{
			layerType=wstring(L"Unknown");
		}
		
		mDataLayerListCtrl.SetItem(id,n++,LVIF_TEXT,layerType.c_str(),0,0,0,0,0);

		string csname("Unknown");
		if(layer->GetCoordinateSystem())
		{
			csname = layer->GetCoordinateSystem()->GetName();
		}

		USES_CONVERSION;
		mDataLayerListCtrl.SetItem(id,n++,LVIF_TEXT,A2T(csname.c_str()),0,0,0,0,0);

		wstring spatialIndex = (layer->GetSpatialIndexType()==GVectorDataLayer::SIT_NONE?wstring(L"无"):wstring(L"RTREE"));
		mDataLayerListCtrl.SetItem(id,n++,LVIF_TEXT,spatialIndex.c_str(),0,0,0,0,0);

		GRectangle2dd box = layer->GetBoundingBox();

		format.Format(L"%f",box.MinEdge.X);	
		mDataLayerListCtrl.SetItem(id,n++,LVIF_TEXT,format.GetBuffer(),0,0,0,0,0);

		format.Format(L"%f",box.MinEdge.Y);	
		mDataLayerListCtrl.SetItem(id,n++,LVIF_TEXT,format.GetBuffer(),0,0,0,0,0);
		
		format.Format(L"%f",box.MaxEdge.X);	
		mDataLayerListCtrl.SetItem(id,n++,LVIF_TEXT,format.GetBuffer(),0,0,0,0,0);

		
		format.Format(L"%f",box.MaxEdge.Y);	
		mDataLayerListCtrl.SetItem(id,n++,LVIF_TEXT,format.GetBuffer(),0,0,0,0,0);
	}

}
void CVtDataSourceMDBDlg::FillDataSourceCBX()
{
	for each(auto it in mDocument->GetVectorDataSources())
	{
		auto name = it->GetName();
	}
}
void CVtDataSourceMDBDlg::OnHdnItemdblclickDataLayerListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CVtDataSourceMDBDlg::OnNMDblclkDataLayerListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = mDataLayerListCtrl.GetFirstSelectedItemPosition();
	int item = mDataLayerListCtrl.GetNextSelectedItem(pos);

	if(item!=-1)
	{
		mSelectDataLayer=mDataSource->GetLayer(item);
		SelectId = item;
	}
	*pResult = 0;
	*pResult = 0;
}

void CVtDataSourceMDBDlg::OnBnClickedBtnRebuildSpatialIndex()
{
	// TODO: 在此添加控件通知处理程序代码

	if(mSelectDataLayer!=NULL)
	{
		mSelectDataLayer->UpdateSpatialIndex(this);

		mDataLayerListCtrl.SetItem(SelectId,4,LVIF_TEXT,L"RTREE",0,0,0,0,0);
	}

}


// CProgressDlg 消息处理程序
//
void  CVtDataSourceMDBDlg::OnSetTotal(const wchar_t* id,u32 total)
{
	Title = id;
	Total = total;
}

//
void  CVtDataSourceMDBDlg::OnProgress(const wchar_t* id,u32 progress)
{
	Progress = progress;
	UpdateProgressUI();

}
//!进度
void  CVtDataSourceMDBDlg::OnProgress(const wchar_t* id,u32 progress,u32 total)
{
	Total = total;
	Progress = progress;
	UpdateProgressUI();
}


void CVtDataSourceMDBDlg::UpdateProgressUI()
{
	if(Progress%10==0)
	{
		CString fmt;
		fmt.Format(L"构建索引中[%s:%d/%d]",Title.c_str(),Progress,Total);
		mDataLayerListCtrl.SetItem(SelectId,4,LVIF_TEXT,fmt.GetBuffer(),0,0,0,0,0);

		MSG msg;
		while(PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			AfxGetThread()->PumpMessage();
		}
	}
}
void CVtDataSourceMDBDlg::OnBnClickedBtnSetCs()
{
	// TODO: 在此添加控件通知处理程序代码
	if(mSelectDataLayer!=NULL)
	{
		CCoordSysDlg dlg;
		if(dlg.DoModal()==IDOK)
		{
			//if(dlg.SelectedCoordinateSystem!=NULL)
			{
				mSelectDataLayer->SetCoordinateSystem(dlg.SelectedCoordinateSystem);

				string csname("Unknown");
				if(mSelectDataLayer->GetCoordinateSystem())
				{
					csname = mSelectDataLayer->GetCoordinateSystem()->GetName();
				}

				USES_CONVERSION;
				mDataLayerListCtrl.SetItem(SelectId,3,LVIF_TEXT,A2T(csname.c_str()),0,0,0,0,0);
			}
		}
	}
}


void CVtDataSourceMDBDlg::OnCbnSelchangeDataSourceCombox()
{
	// TODO: 在此添加控件通知处理程序代码
	if (mDataSourceCBX.GetCurSel() < mDocument->GetVectorDataSources().size())
	{
		mDataSource = mDocument->GetVectorDataSources()[mDataSourceCBX.GetCurSel()];
	}
	FillDataLayerList();
}


void CVtDataSourceMDBDlg::OnBnClickedButtonDataSource()
{
	// TODO: 在此添加控件通知处理程序代码
	CMenu menu;
	menu.LoadMenuW(IDR_MENU_VT_DATA_SOURCE);
	RECT rect;
	mButtonDataSource.GetWindowRect(&rect);

	CMenu* subMenu = menu.GetSubMenu(0);
	subMenu->TrackPopupMenu(TPM_LEFTALIGN, rect.right, rect.top, this);
	menu.DestroyMenu();
}


void CVtDataSourceMDBDlg::OnOpenVtDatasource()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE, _T("*.mdb"), NULL, 6UL, _T("access file 2007-(*.mdb)|*.mdb|access 2007+(*.accdb)|*.accdb||"));
	if (dlg.DoModal() == IDOK)
	{
		CString fileName = dlg.GetPathName();
		wstring wfn = fileName.GetBuffer();
		const vector<GVectorDataSource*>& datasources = mDocument->GetVectorDataSources();
		if (mDocument->OpenVtDataSourceMDB(wfn))
		{
			mDataSourceCBX.AddString(datasources.back()->GetName().c_str());
			mDataSourceCBX.SetCurSel(datasources.size()-1);
			mDataSource = datasources.back();
			FillDataLayerList();
		}
	}
}


void CVtDataSourceMDBDlg::OnCreateVtDatasource()
{
	// TODO: 在此添加命令处理程序代码
}
