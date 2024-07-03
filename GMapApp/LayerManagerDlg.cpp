// LayerManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LayerManagerDlg.h"
#include "GMapApp.h"
#include "GraphicsLayerPropEditDlg.h"
#include "SimpleRenderStyleEditDlg.h"
#include "VtDataSourceMDBDlg.h"
#include "CoordSysDlg.h"
#include "GImageMapLayer.h"
#include "GSimpleImageSource.h"
#include "GColorRasterData.h"
#include "GRasterLayer.h"
#include "GRasterColorConvert.h"
#include "CValueClassRenderDialog.h"
// CLayerManagerDlg �Ի���

IMPLEMENT_DYNAMIC(CLayerManagerDlg, CDialog)

CLayerManagerDlg::CLayerManagerDlg(CWnd* pParent /*=NULL*/, GMapAppDoc* pDocument)
: CDialog(CLayerManagerDlg::IDD, pParent)
{
	mDocument = pDocument;
	mMap = mDocument->GetMap();
}

CLayerManagerDlg::~CLayerManagerDlg()
{
}

void CLayerManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAYER_LIST_CTRL, mLayerListCtrl);
	DDX_Control(pDX, IDC_LAYER_VISIBLE_CHK_CTRL, mLayerVisibleChkCtrl);
	DDX_Control(pDX, IDC_LAYER_PROP_CTRL, mLayerPropBtnCtrl);
	DDX_Control(pDX, IDC_DELETE_LAYER_BTN, mDeleteLayerBtnCtrl);
	DDX_Control(pDX, IDC_NEW_LAYER_BTN, mNewLayerBtnCtrl);
}


BEGIN_MESSAGE_MAP(CLayerManagerDlg, CDialog)
	ON_BN_CLICKED(IDC_NEW_LAYER_BTN, &CLayerManagerDlg::OnBnClickedNewLayerBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LAYER_LIST_CTRL, &CLayerManagerDlg::OnLvnItemchangedLayerListCtrl)
	ON_BN_CLICKED(IDC_DELETE_LAYER_BTN, &CLayerManagerDlg::OnBnClickedDeleteLayerBtn)
	ON_BN_CLICKED(IDC_LAYER_VISIBLE_CHK_CTRL, &CLayerManagerDlg::OnBnClickedLayerVisibleChkCtrl)
	ON_BN_CLICKED(IDC_LAYER_PROP_CTRL, &CLayerManagerDlg::OnBnClickedLayerPropCtrl)
	ON_COMMAND(ID_NEW_GRAPHICS_LAYER, &CLayerManagerDlg::OnNewGraphicsLayer)
	ON_NOTIFY(NM_DBLCLK, IDC_LAYER_LIST_CTRL, &CLayerManagerDlg::OnNMDblclkLayerListCtrl)
	ON_COMMAND(ID_NEW_VECTOR_LAYER, &CLayerManagerDlg::OnNewVectorLayer)
	ON_BN_CLICKED(IDC_BTN_MAP_COORDSYS, &CLayerManagerDlg::OnBnClickedBtnMapCoordsys)
	ON_COMMAND(ID_NEW_IMAGE_LAYER, &CLayerManagerDlg::OnNewImageLayer)
	ON_COMMAND(ID_VIEW_LAYER_STYLE, &CLayerManagerDlg::OnViewLayerStyle)
	ON_COMMAND(ID_SET_SIMPLE_RENDER, &CLayerManagerDlg::OnSetSimpleRender)
	ON_COMMAND(ID_SET_CLASS_RENDER, &CLayerManagerDlg::OnSetClassRender)
END_MESSAGE_MAP()


// CLayerManagerDlg ��Ϣ�������

void CLayerManagerDlg::OnBnClickedNewLayerBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu menu;
	menu.LoadMenu(IDR_MENU_NEW_STYLE);
	RECT rect;
	mNewLayerBtnCtrl.GetWindowRect(&rect);

	CMenu* subMenu = menu.GetSubMenu(3);
	subMenu->TrackPopupMenu(TPM_LEFTALIGN,rect.right,rect.top,this);
	menu.DestroyMenu();
}

void CLayerManagerDlg::OnLvnItemchangedLayerListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if((pNMLV->uNewState&LVIS_SELECTED))
	{
		GMapLayer* layer = mMap->GetLayer(pNMLV->iItem);
		mLayerVisibleChkCtrl.SetCheck(layer->GetVisible());
	}

	*pResult = 0;
}

void CLayerManagerDlg::OnBnClickedDeleteLayerBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = mLayerListCtrl.GetFirstSelectedItemPosition();
	int item = mLayerListCtrl.GetNextSelectedItem(pos);

	if(item!=-1)
	{
		mMap->RemoveLayer(item);
		mLayerListCtrl.DeleteItem(item);
	}

}

void CLayerManagerDlg::OnBnClickedLayerVisibleChkCtrl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = mLayerListCtrl.GetFirstSelectedItemPosition();
	int item = mLayerListCtrl.GetNextSelectedItem(pos);

	if(item!=-1)
	{
		int checked = mLayerVisibleChkCtrl.GetCheck();
		mMap->GetLayer(item)->SetVisible(checked);
		wstring visible(mMap->GetLayer(item)->GetVisible()?L"��":L"��");
		mLayerListCtrl.SetItem(item,2,LVIF_TEXT,visible.c_str(),0,0,0,0,0);
	}
}

void CLayerManagerDlg::OnBnClickedLayerPropCtrl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = mLayerListCtrl.GetFirstSelectedItemPosition();
	int item = mLayerListCtrl.GetNextSelectedItem(pos);

	if(item!=-1)
	{
		GMapLayer* layer = mMap->GetLayer(item);
		if(layer->GetLayerType()==GMapLayer::LT_GRAPHICS)
		{
			CGraphicsLayerPropEditDlg dlg(this,(GGraphicsLayer*)layer,mMap);
			dlg.DoModal();
		}
		else if(layer->GetLayerType()==GMapLayer::LT_VECTOR)
		{
			CMenu menu;
			menu.LoadMenu(IDR_MENU_NEW_STYLE);
			RECT rect;
			mLayerPropBtnCtrl.GetWindowRect(&rect);

			CMenu* subMenu = menu.GetSubMenu(4);
			subMenu->TrackPopupMenu(TPM_LEFTALIGN, rect.right, rect.top, this);
			menu.DestroyMenu();
		}
	}
}

void CLayerManagerDlg::OnNewGraphicsLayer()
{
	// TODO: �ڴ���������������
	CString format;
	format.Format(L"�»�ͼͼ��[%d]",mMap->GetLayerCount()+1);
	GGraphicsLayer* pNewLayer = new GGraphicsLayer(wstring(format.GetBuffer()));
	mMap->AddLayer(pNewLayer);

	GStyleGroup* pStyleGroup = mMap->GetStyleGroup();

	for(int i=0;i<pStyleGroup->GetStyleCount();i++)
	{
		GStyle* pStyle = pStyleGroup->GetStyleAt(i);
		if(pStyle->GetStyleType()==GStyle::ST_MARKER_STYLE)
		{
			pNewLayer->SetDefaultMarkerStyle((GMarkerStyle*)pStyle);
		}
		else if(pStyle->GetStyleType()==GStyle::ST_LINE_STYLE)
		{

			pNewLayer->SetDefaultLineStyle((GLineStyle*)pStyle);
		}
		else if(pStyle->GetStyleType()==GStyle::ST_FILL_STYLE)
		{
			pNewLayer->SetDefaultFillStyle((GFillStyle*)pStyle);
		}
	}

	pNewLayer->Release();

	int id = mLayerListCtrl.InsertItem(mMap->GetLayerCount(),pNewLayer->GetName().c_str());
	wstring layerType;
	layerType=wstring(L"��ͼ");

	mLayerListCtrl.SetItem(id,1,LVIF_TEXT,layerType.c_str(),0,0,0,0,0);
	wstring visible(pNewLayer->GetVisible()?L"��":L"��");
	mLayerListCtrl.SetItem(id,2,LVIF_TEXT,visible.c_str(),0,0,0,0,0);

	RefreshCurrentLayerLabel();


}

BOOL CLayerManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	mLayerListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	mLayerListCtrl.InsertColumn(0,L"ͼ������");
	mLayerListCtrl.InsertColumn(1,L"ͼ������");
	mLayerListCtrl.InsertColumn(2,L"�Ƿ�ɼ�");
	mLayerListCtrl.InsertColumn(3,L"��ǰͼ��");
	mLayerListCtrl.SetColumnWidth(0,100);
	mLayerListCtrl.SetColumnWidth(1,100);
	mLayerListCtrl.SetColumnWidth(2,100);
	mLayerListCtrl.SetColumnWidth(3,100);

	for(int i=0;i<mMap->GetLayerCount();i++)
	{
		GMapLayer* layer=mMap->GetLayer(i);
		int id = mLayerListCtrl.InsertItem(i,layer->GetName().c_str());
		wstring layerType;
		if(layer->GetLayerType()==GMapLayer::LT_GRAPHICS)
		{
			layerType=wstring(L"��ͼ");
		}
		else  if(layer->GetLayerType()==GMapLayer::LT_VECTOR)
		{
			layerType=wstring(L"ʸ��");
		}
		else  if(layer->GetLayerType()==GMapLayer::LT_IMAGE)
		{
			layerType=wstring(L"ͼ��");
		}
		else  if (layer->GetLayerType() == GMapLayer::LT_RASTER)
		{
			layerType = wstring(L"Ӱ��");
		}
		else
		{
			layerType=wstring(L"δ֪");
		}

		mLayerListCtrl.SetItem(id,1,LVIF_TEXT,layerType.c_str(),0,0,0,0,0);

		wstring visible(layer->GetVisible()?L"��":L"��");
		mLayerListCtrl.SetItem(id,2,LVIF_TEXT,visible.c_str(),0,0,0,0,0);

		wstring currentLayer(layer==mMap->GetCurrentLayer()?L"��":L"��");
		mLayerListCtrl.SetItem(id,3,LVIF_TEXT,currentLayer.c_str(),0,0,0,0,0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLayerManagerDlg::OnNMDblclkLayerListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = mLayerListCtrl.GetFirstSelectedItemPosition();
	int item = mLayerListCtrl.GetNextSelectedItem(pos);

	if(item!=-1)
	{
		mMap->SetCurrentLayer(mMap->GetLayer(item));
	}
	RefreshCurrentLayerLabel();
	*pResult = 0;
}


void CLayerManagerDlg::RefreshCurrentLayerLabel()
{
	for(int i=0;i<mMap->GetLayerCount();i++)
	{
		GMapLayer* layer=mMap->GetLayer(i);
		wstring currentLayer(layer==mMap->GetCurrentLayer()?L"��":L"��");
		mLayerListCtrl.SetItem(i,3,LVIF_TEXT,currentLayer.c_str(),0,0,0,0,0);
	}

}
void CLayerManagerDlg::OnNewVectorLayer()
{
	// TODO: �ڴ���������������

	CVtDataSourceMDBDlg dlg(mDocument, this);
	if(dlg.DoModal()==IDOK&&dlg.mSelectDataLayer!=NULL)
	{

		GSimpleVectorFeatureRender* pRender = new GSimpleVectorFeatureRender(NULL,NULL,NULL);
		GStyleGroup* pStyleGroup = mMap->GetStyleGroup();

		for(int i=0;i<pStyleGroup->GetStyleCount();i++)
		{
			GStyle* pStyle = pStyleGroup->GetStyleAt(i);
			if(pStyle->GetStyleType()==GStyle::ST_MARKER_STYLE)
			{
				pRender->SetMarkerStyle((GMarkerStyle*)pStyle);
			}
			else if(pStyle->GetStyleType()==GStyle::ST_LINE_STYLE)
			{

				pRender->SetLineStyle((GLineStyle*)pStyle);
			}
			else if(pStyle->GetStyleType()==GStyle::ST_FILL_STYLE)
			{
				pRender->SetFillStyle((GFillStyle*)pStyle);
			}
		}

	
		GVectorMapLayer* pMapLayer = new GVectorMapLayer();
		mMap->AddLayer(pMapLayer);
		pMapLayer->SetName(dlg.mSelectDataLayer->GetName());
		pMapLayer->SetDataLayer(dlg.mSelectDataLayer);
		pMapLayer->ResetRender(pRender);

		pRender->Release();
		pMapLayer->Release();

		int id = mLayerListCtrl.InsertItem(mMap->GetLayerCount(),pMapLayer->GetName().c_str());
		wstring layerType;
		layerType=wstring(L"ʸ��");

		mLayerListCtrl.SetItem(id,1,LVIF_TEXT,layerType.c_str(),0,0,0,0,0);
		wstring visible(pMapLayer->GetVisible()?L"��":L"��");
		mLayerListCtrl.SetItem(id,2,LVIF_TEXT,visible.c_str(),0,0,0,0,0);

		RefreshCurrentLayerLabel();
	}
}

void CLayerManagerDlg::OnBnClickedBtnMapCoordsys()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CCoordSysDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.SelectedCoordinateSystem!=NULL)
		{
			mMap->SetCoordinateSystem(dlg.SelectedCoordinateSystem);
		}
	}
}

void CLayerManagerDlg::OnNewImageLayer()
{
	// TODO: �ڴ���������������

	wstring file( L".\\dat\\GmGlobe.gid");
	GRasterData* rasterData = GColorRasterData::CreateFromConfigure(file);
	GRasterColorConvert* cvt = new GRasterGrayConvert();
	GRasterLayer* pLayer = new GRasterLayer();
	pLayer->SetName(file);
	pLayer->SetRasterData(rasterData);
	pLayer->SetColorConvert(cvt);
	mMap->AddLayer(pLayer);
	rasterData->Release();
	cvt->Release();


	//GImageSource* imageSource = GSimpleImageSource::CreateFromConfigure(file);
	//GImageMapLayer* pLayer = new GImageMapLayer(imageSource);
	//mMap->AddLayer(pLayer);
	//pLayer->Release();
	//imageSource->Release();

	


	int id = mLayerListCtrl.InsertItem(mMap->GetLayerCount(),pLayer->GetName().c_str());
	wstring layerType;
	layerType=wstring(L"ͼ��");

	mLayerListCtrl.SetItem(id,1,LVIF_TEXT,layerType.c_str(),0,0,0,0,0);
	wstring visible(pLayer->GetVisible()?L"��":L"��");
	mLayerListCtrl.SetItem(id,2,LVIF_TEXT,visible.c_str(),0,0,0,0,0);
}


void CLayerManagerDlg::OnViewLayerStyle()
{
	// TODO: �ڴ���������������
	POSITION pos = mLayerListCtrl.GetFirstSelectedItemPosition();
	int item = mLayerListCtrl.GetNextSelectedItem(pos);

	if (item != -1)
	{
		GMapLayer* layer = mMap->GetLayer(item);
		GVectorMapLayer* pLayer = (GVectorMapLayer*)layer;
		GVectorFeatureRender* render = pLayer->GetRender();
		if (render->GetType() == GVectorFeatureRender::RT_SIMPLE)
		{
			CSimpleRenderStyleEditDlg dlg(mMap, (GSimpleVectorFeatureRender*)render, this);
			dlg.DoModal();
		}
		else if(render->GetType()==GVectorFeatureRender::RT_VALUE_CLASS)
		{
			CValueClassRenderDialog dlg(mMap, pLayer->GetDataLayer(), (GValueClassRender*)pLayer->GetRender(), this);
			dlg.DoModal();
		}
	}
}


void CLayerManagerDlg::OnSetSimpleRender()
{
	// TODO: �ڴ���������������
	POSITION pos = mLayerListCtrl.GetFirstSelectedItemPosition();
	int item = mLayerListCtrl.GetNextSelectedItem(pos);
	if (item != -1)
	{
		GMapLayer* layer = mMap->GetLayer(item);
		GVectorMapLayer* pLayer = (GVectorMapLayer*)layer;
		GVectorFeatureRender* render = pLayer->GetRender();
		if (render->GetType() == GVectorFeatureRender::RT_SIMPLE)
		{
			CSimpleRenderStyleEditDlg dlg(mMap, (GSimpleVectorFeatureRender*)render, this);
			dlg.DoModal();
		}
		else 
		{
			GSimpleVectorFeatureRender* render = new GSimpleVectorFeatureRender(NULL,NULL,NULL);
			CSimpleRenderStyleEditDlg dlg(mMap, (GSimpleVectorFeatureRender*)render, this);
			if (dlg.DoModal() == IDOK)
			{
				pLayer->ResetRender(render);
			}
		}
	}
}


void CLayerManagerDlg::OnSetClassRender()
{	// TODO: �ڴ���������������
	POSITION pos = mLayerListCtrl.GetFirstSelectedItemPosition();
	int item = mLayerListCtrl.GetNextSelectedItem(pos);
	if (item != -1)
	{
		GMapLayer* layer = mMap->GetLayer(item);
		GVectorMapLayer* pLayer = (GVectorMapLayer*)layer;
		GVectorFeatureRender* render = pLayer->GetRender();
		if (render->GetType() == GVectorFeatureRender::RT_VALUE_CLASS)
		{
			CValueClassRenderDialog dlg(mMap, pLayer->GetDataLayer(),(GValueClassRender*)pLayer->GetRender(), this);
			dlg.DoModal();
			pLayer->ResetRender(NULL);
		}
		else
		{
			GValueClassRender* render = new GValueClassRender();
			CValueClassRenderDialog dlg(mMap, pLayer->GetDataLayer(), render, this);
			if (dlg.DoModal() == IDOK)
			{
				pLayer->ResetRender(render);
			}
		}
	}
}
