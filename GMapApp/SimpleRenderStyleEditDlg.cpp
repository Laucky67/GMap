// SimpleRenderStyleEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GMapApp.h"
#include "SimpleRenderStyleEditDlg.h"
#include "StyleManagerDlg.h"


// CSimpleRenderStyleEditDlg 对话框

IMPLEMENT_DYNAMIC(CSimpleRenderStyleEditDlg, CDialog)

CSimpleRenderStyleEditDlg::CSimpleRenderStyleEditDlg(GMap* pMap,GSimpleVectorFeatureRender* pRender, CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleRenderStyleEditDlg::IDD, pParent)
{
	mMap = pMap;
	mRender = pRender;
}

CSimpleRenderStyleEditDlg::~CSimpleRenderStyleEditDlg()
{
}
void CSimpleRenderStyleEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, mNameEditCtrl);
	DDX_Control(pDX, IDC_MARKER_STYLE_PANEL_CTRL, mMarkerStylePanelCtrl);
	DDX_Control(pDX, IDC_DEFAULT_LINE_STYLE_PANEL_CTRL, mLineStylePanelCtrl);
	DDX_Control(pDX, IDC_DEF_FILL_STYLE_PANEL_CTRL, mFillStylePanelCtrl);
}


BEGIN_MESSAGE_MAP(CSimpleRenderStyleEditDlg, CDialog)
	ON_BN_CLICKED(IDC_SEL_MARKER_STYLE_BTN, &CSimpleRenderStyleEditDlg::OnBnClickedSelMarkerStyleBtn)
	ON_BN_CLICKED(IDC_SEL_LINE_STYLE_BTN, &CSimpleRenderStyleEditDlg::OnBnClickedSelLineStyleBtn)
	ON_BN_CLICKED(IDC_SEL_FILL_STYLE_BTN, &CSimpleRenderStyleEditDlg::OnBnClickedSelFillStyleBtn)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSimpleRenderStyleEditDlg 消息处理程序

void CSimpleRenderStyleEditDlg::OnBnClickedSelMarkerStyleBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CStyleManagerDlg dlg(this,mMap->GetStyleGroup(),0);
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.GetSelectStyle()!=NULL)
		mRender->SetMarkerStyle((GMarkerStyle*)dlg.GetSelectStyle());
		this->Invalidate();
	}
}

void CSimpleRenderStyleEditDlg::OnBnClickedSelLineStyleBtn()
{
	// TODO: 在此添加控件通知处理程序代码	
	CStyleManagerDlg dlg(this,mMap->GetStyleGroup(),1);
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.GetSelectStyle()!=NULL)
		mRender->SetLineStyle((GLineStyle*)dlg.GetSelectStyle());
		this->Invalidate();
	}
}

void CSimpleRenderStyleEditDlg::OnBnClickedSelFillStyleBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CStyleManagerDlg dlg(this,mMap->GetStyleGroup(),2);
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.GetSelectStyle()!=NULL)
		mRender->SetFillStyle((GFillStyle*)dlg.GetSelectStyle());
		this->Invalidate();
	}
}

BOOL CSimpleRenderStyleEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//mNameEditCtrl.SetWindowText(mRender->GetName().c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSimpleRenderStyleEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	Graphics graphics(dc.GetSafeHdc());

	CRect rect;
	mMarkerStylePanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	RectF rc2(rect.left,rect.top,rect.Width(),rect.Height());
	SolidBrush brush(Color::White);
	graphics.FillRectangle(&brush,rc2);
	if(mRender->GetMarkerStyle())
	{
		DrawStyle(&graphics,mRender->GetMarkerStyle(),rc2);
	}

	mLineStylePanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	RectF rc3(rect.left,rect.top,rect.Width(),rect.Height());
	graphics.FillRectangle(&brush,rc3);
	if(mRender->GetLineStyle())
	{
		DrawStyle(&graphics,mRender->GetLineStyle(),rc3);
	}

	mFillStylePanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	RectF rc1(rect.left,rect.top,rect.Width(),rect.Height());
	//SolidBrush brush(Color::White);
	graphics.FillRectangle(&brush,rc1);
	if(mRender->GetFillStyle())
	{
		DrawStyle(&graphics,mRender->GetFillStyle(),rc1);
	}
}

