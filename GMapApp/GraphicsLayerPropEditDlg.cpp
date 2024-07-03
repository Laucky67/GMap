// GraphicsLayerPropEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GMapApp.h"
#include "GraphicsLayerPropEditDlg.h"
#include "StyleManagerDlg.h"

// CGraphicsLayerPropEditDlg �Ի���

IMPLEMENT_DYNAMIC(CGraphicsLayerPropEditDlg, CDialog)

CGraphicsLayerPropEditDlg::CGraphicsLayerPropEditDlg(CWnd* pParent,GGraphicsLayer* layer,GMap* map)
	: CDialog(CGraphicsLayerPropEditDlg::IDD, pParent)
{
	mLayer = layer;
	mMap = map;
}

CGraphicsLayerPropEditDlg::~CGraphicsLayerPropEditDlg()
{

}

void CGraphicsLayerPropEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, mNameEditCtrl);
	DDX_Control(pDX, IDC_MARKER_STYLE_PANEL_CTRL, mMarkerStylePanelCtrl);
	DDX_Control(pDX, IDC_DEFAULT_LINE_STYLE_PANEL_CTRL, mLineStylePanelCtrl);
	DDX_Control(pDX, IDC_DEF_FILL_STYLE_PANEL_CTRL, mFillStylePanelCtrl);
}


BEGIN_MESSAGE_MAP(CGraphicsLayerPropEditDlg, CDialog)
	ON_BN_CLICKED(IDC_SEL_MARKER_STYLE_BTN, &CGraphicsLayerPropEditDlg::OnBnClickedSelMarkerStyleBtn)
	ON_BN_CLICKED(IDC_SEL_LINE_STYLE_BTN, &CGraphicsLayerPropEditDlg::OnBnClickedSelLineStyleBtn)
	ON_BN_CLICKED(IDC_SEL_FILL_STYLE_BTN, &CGraphicsLayerPropEditDlg::OnBnClickedSelFillStyleBtn)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGraphicsLayerPropEditDlg ��Ϣ�������

void CGraphicsLayerPropEditDlg::OnBnClickedSelMarkerStyleBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CStyleManagerDlg dlg(this,mMap->GetStyleGroup(),0);
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.GetSelectStyle()!=NULL)
		mLayer->SetDefaultMarkerStyle((GMarkerStyle*)dlg.GetSelectStyle());
		this->Invalidate();
	}
}

void CGraphicsLayerPropEditDlg::OnBnClickedSelLineStyleBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	CStyleManagerDlg dlg(this,mMap->GetStyleGroup(),1);
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.GetSelectStyle()!=NULL)
		mLayer->SetDefaultLineStyle((GLineStyle*)dlg.GetSelectStyle());
		this->Invalidate();
	}
}

void CGraphicsLayerPropEditDlg::OnBnClickedSelFillStyleBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CStyleManagerDlg dlg(this,mMap->GetStyleGroup(),2);
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.GetSelectStyle()!=NULL)
		mLayer->SetDefaultFillStyle((GFillStyle*)dlg.GetSelectStyle());
		this->Invalidate();
	}
}

BOOL CGraphicsLayerPropEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	mNameEditCtrl.SetWindowText(mLayer->GetName().c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGraphicsLayerPropEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	Graphics graphics(dc.GetSafeHdc());

	CRect rect;
	mMarkerStylePanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	RectF rc2(rect.left,rect.top,rect.Width(),rect.Height());
	SolidBrush brush(Color::White);
	graphics.FillRectangle(&brush,rc2);
	if(mLayer->GetDefaultMarkerStyle())
	{
		DrawStyle(&graphics,mLayer->GetDefaultMarkerStyle(),rc2);
	}

	mLineStylePanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	RectF rc3(rect.left,rect.top,rect.Width(),rect.Height());
	//SolidBrush brush(Color::White);
	graphics.FillRectangle(&brush,rc3);
	if(mLayer->GetDefaultLineStyle())
	{
		DrawStyle(&graphics,mLayer->GetDefaultLineStyle(),rc3);
	}

	mFillStylePanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	RectF rc1(rect.left,rect.top,rect.Width(),rect.Height());
	//SolidBrush brush(Color::White);
	graphics.FillRectangle(&brush,rc1);
	if(mLayer->GetDefaultFillStyle())
	{
		DrawStyle(&graphics,mLayer->GetDefaultFillStyle(),rc1);
	}
}
