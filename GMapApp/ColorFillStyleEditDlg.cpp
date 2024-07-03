// ColorFillStyleEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GMapApp.h"
#include "ColorFillStyleEditDlg.h"


// CColorFillStyleEditDlg 对话框

IMPLEMENT_DYNAMIC(CColorFillStyleEditDlg, CDialog)

CColorFillStyleEditDlg::CColorFillStyleEditDlg(CWnd* pParent /*=NULL*/,GColorFillStyle* pFillStyle)
	: CDialog(CColorFillStyleEditDlg::IDD, pParent)
{
	mFillStyle = pFillStyle;
}

CColorFillStyleEditDlg::~CColorFillStyleEditDlg()
{
}

void CColorFillStyleEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME_EDIT_CTRL, mNameEditCtrl);
	DDX_Control(pDX, IDC_APHLA_EDIT_CTRL, mAlphaEditCtrl);
	DDX_Control(pDX, IDC_ALPHA_SLIDER_EDIT_CTRL, mAlphaSliderCtrl);
	DDX_Control(pDX, IDC_FORE_COLOR_PANEL, mColorPanelCtrl);
	DDX_Control(pDX, IDC_PREVIEW_PANEL_CTRL, mPreviewPanelCtrl);
}


BEGIN_MESSAGE_MAP(CColorFillStyleEditDlg, CDialog)
	ON_BN_CLICKED(IDC_FORE_COLOR_BTN, &CColorFillStyleEditDlg::OnBnClickedForeColorBtn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ALPHA_SLIDER_EDIT_CTRL, &CColorFillStyleEditDlg::OnNMCustomdrawAlphaSliderEditCtrl)
	ON_EN_CHANGE(IDC_NAME_EDIT_CTRL, &CColorFillStyleEditDlg::OnEnChangeNameEditCtrl)
	ON_WM_PAINT()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_ALPHA_SLIDER_EDIT_CTRL, &CColorFillStyleEditDlg::OnNMReleasedcaptureAlphaSliderEditCtrl)
END_MESSAGE_MAP()


// CColorFillStyleEditDlg 消息处理程序

void CColorFillStyleEditDlg::OnBnClickedForeColorBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog colorDialog(mFillStyle->GetFillColor().ToCOLORREF(),NULL,this);
	if(colorDialog.DoModal()==IDOK)
	{
		COLORREF rgb = colorDialog.GetColor();
		Color color(255*(mAlphaSliderCtrl.GetPos()/100.0f),GetRValue(rgb),GetGValue(rgb),GetBValue(rgb));
		mFillStyle->SetFillColor(color);
		this->Invalidate();
	}
}

void CColorFillStyleEditDlg::OnNMCustomdrawAlphaSliderEditCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	Color color = mFillStyle->GetFillColor();
	int pos = mAlphaSliderCtrl.GetPos();
	CString format;
	format.Format(L"%d",pos);
	color = Color(255*(mAlphaSliderCtrl.GetPos()/100.0f),color.GetR(),color.GetG(),color.GetB());
	mFillStyle->SetFillColor(color);
	
	CRect rect;
	mPreviewPanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);

	mAlphaEditCtrl.SetWindowText(format);

	this->InvalidateRect(&rect);
	*pResult = 0;
}


BOOL CColorFillStyleEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mNameEditCtrl.SetWindowText(mFillStyle->GetName().c_str());
	mAlphaSliderCtrl.SetBuddy(&mAlphaEditCtrl);
	mAlphaSliderCtrl.SetRange(0,100);

	mAlphaSliderCtrl.SetPos(100.0*(mFillStyle->GetFillColor().GetAlpha()/255.0f));

	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CColorFillStyleEditDlg::OnEnChangeNameEditCtrl()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString text;
	mNameEditCtrl.GetWindowText(text);
	mFillStyle->SetName(wstring(text.GetBuffer()));
}

void CColorFillStyleEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	Graphics graphics(dc.GetSafeHdc());
	
	CRect rect;
	mColorPanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);

	RectF rc1(rect.left,rect.top,rect.Width(),rect.Height());
	Color color;
	color.SetFromCOLORREF(mFillStyle->GetFillColor().ToCOLORREF());
	SolidBrush brush(color);
	graphics.FillRectangle(&brush,rc1);


	
	mPreviewPanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);

	RectF rc2(rect.left+10,rect.top+20,rect.Width()-20,rect.Height()-20);
	DrawStyle(&graphics,mFillStyle,rc2);
}

void CColorFillStyleEditDlg::OnNMReleasedcaptureAlphaSliderEditCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	Color color = mFillStyle->GetFillColor();
	color = Color(255*(mAlphaSliderCtrl.GetPos()/100.0f),color.GetR(),color.GetG(),color.GetB());
	mFillStyle->SetFillColor(color);

	this->Invalidate();
	*pResult = 0;
}
