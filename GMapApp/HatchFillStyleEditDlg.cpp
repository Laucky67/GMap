// HatchFillStyleEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GMapApp.h"
#include "HatchFillStyleEditDlg.h"


// CHatchFillStyleEditDlg 对话框

IMPLEMENT_DYNAMIC(CHatchFillStyleEditDlg, CDialog)

CHatchFillStyleEditDlg::CHatchFillStyleEditDlg(CWnd* pParent /*=NULL*/,GHatchFillStyle* fillStyle)
	: CDialog(CHatchFillStyleEditDlg::IDD, pParent)
{
	mFillStyle = fillStyle;
}

CHatchFillStyleEditDlg::~CHatchFillStyleEditDlg()
{
}

void CHatchFillStyleEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME_EDIT_CTRL, mNameEditCtrl);
	DDX_Control(pDX, IDC_FORE_COLOR_PANEL, mForeColorPanelCtrl);
	DDX_Control(pDX, IDC_BK_COLOR_PANEL, mBkColorPanelCtrl);
	DDX_Control(pDX, IDC_HATCH_STYLE_CMB, mHatchStyleCbxCtrl);
	DDX_Control(pDX, IDC_PREVIEW_PANEL_CTRL, mPreviewPanelCtrl);
}


BEGIN_MESSAGE_MAP(CHatchFillStyleEditDlg, CDialog)
	ON_BN_CLICKED(IDC_FORE_COLOR_BTN, &CHatchFillStyleEditDlg::OnBnClickedForeColorBtn)
	ON_BN_CLICKED(IDC_BK_COLOR_BTN, &CHatchFillStyleEditDlg::OnBnClickedBkColorBtn)
	ON_CBN_SELCHANGE(IDC_HATCH_STYLE_CMB, &CHatchFillStyleEditDlg::OnCbnSelchangeHatchStyleCmb)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_NAME_EDIT_CTRL, &CHatchFillStyleEditDlg::OnEnChangeNameEditCtrl)
END_MESSAGE_MAP()


// CHatchFillStyleEditDlg 消息处理程序

void CHatchFillStyleEditDlg::OnBnClickedForeColorBtn()
{
	// TODO: 在此添加控件通知处理程序代码
		CColorDialog colorDialog(mFillStyle->GetForeColor().ToCOLORREF(),NULL,this);
	if(colorDialog.DoModal()==IDOK)
	{
		COLORREF rgb = colorDialog.GetColor();
		Color color(255,GetRValue(rgb),GetGValue(rgb),GetBValue(rgb));
		mFillStyle->SetForeColor(color);
		this->Invalidate();
	}
}

void CHatchFillStyleEditDlg::OnBnClickedBkColorBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog colorDialog(mFillStyle->GetBackColor().ToCOLORREF(),NULL,this);
	if(colorDialog.DoModal()==IDOK)
	{
		COLORREF rgb = colorDialog.GetColor();
		Color color(255,GetRValue(rgb),GetGValue(rgb),GetBValue(rgb));
		mFillStyle->SetBackColor(color);
		this->Invalidate();
	}
}

HatchStyle HatchStyles[]=
{
    HatchStyleHorizontal,                   // 0
    HatchStyleVertical,                     // 1
    HatchStyleForwardDiagonal,              // 2
    HatchStyleBackwardDiagonal,             // 3
    HatchStyleCross,                        // 4
    HatchStyleDiagonalCross,                // 5
    HatchStyle05Percent,                    // 6
    HatchStyle10Percent,                    // 7
    HatchStyle20Percent,                    // 8
    HatchStyle25Percent,                    // 9
    HatchStyle30Percent,                    // 10
    HatchStyle40Percent,                    // 11
    HatchStyle50Percent,                    // 12
    HatchStyle60Percent,                    // 13
    HatchStyle70Percent,                    // 14
    HatchStyle75Percent,                    // 15
    HatchStyle80Percent,                    // 16
    HatchStyle90Percent,                    // 17
    HatchStyleLightDownwardDiagonal,        // 18
    HatchStyleLightUpwardDiagonal,          // 19
    HatchStyleDarkDownwardDiagonal,         // 20
    HatchStyleDarkUpwardDiagonal,           // 21
    HatchStyleWideDownwardDiagonal,         // 22
    HatchStyleWideUpwardDiagonal,           // 23
    HatchStyleLightVertical,                // 24
    HatchStyleLightHorizontal,              // 25
    HatchStyleNarrowVertical,               // 26
    HatchStyleNarrowHorizontal,             // 27
    HatchStyleDarkVertical,                 // 28
    HatchStyleDarkHorizontal,               // 29
    HatchStyleDashedDownwardDiagonal,       // 30
    HatchStyleDashedUpwardDiagonal,         // 31
    HatchStyleDashedHorizontal,             // 32
    HatchStyleDashedVertical,               // 33
    HatchStyleSmallConfetti,                // 34
    HatchStyleLargeConfetti,                // 35
    HatchStyleZigZag,                       // 36
    HatchStyleWave,                         // 37
    HatchStyleDiagonalBrick,                // 38
    HatchStyleHorizontalBrick,              // 39
    HatchStyleWeave,                        // 40
    HatchStylePlaid,                        // 41
    HatchStyleDivot,                        // 42
    HatchStyleDottedGrid,                   // 43
    HatchStyleDottedDiamond,                // 44
    HatchStyleShingle,                      // 45
    HatchStyleTrellis,                      // 46
    HatchStyleSphere,                       // 47
    HatchStyleSmallGrid,                    // 48
    HatchStyleSmallCheckerBoard,            // 49
    HatchStyleLargeCheckerBoard,            // 50
    HatchStyleOutlinedDiamond,              // 51
    HatchStyleSolidDiamond,                 // 52

};
BOOL CHatchFillStyleEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	for(UINT i=0;i<53;i++)
	{
		CString text;
		text.Format(L"图案[%d]",i);
		int id = mHatchStyleCbxCtrl.AddString(text);
		mHatchStyleCbxCtrl.SetItemData(id,i);
	}

	mNameEditCtrl.SetWindowText(mFillStyle->GetName().c_str());

	int idx = (int)mFillStyle->GetHatchStyle();
	mHatchStyleCbxCtrl.SetCurSel(idx);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHatchFillStyleEditDlg::OnCbnSelchangeHatchStyleCmb()
{
	// TODO: 在此添加控件通知处理程序代码
	mFillStyle->SetHatchStyle((HatchStyle)mHatchStyleCbxCtrl.GetCurSel());
	this->Invalidate();
}

void CHatchFillStyleEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	Graphics graphics(dc.GetSafeHdc());
	CRect rect;
	mForeColorPanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);

	Rect rc1(rect.left,rect.top,rect.Width(),rect.Height());
	SolidBrush brush1(mFillStyle->GetForeColor());
	graphics.FillRectangle(&brush1,rc1);

	mBkColorPanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);

	Rect rc2(rect.left,rect.top,rect.Width(),rect.Height());
	SolidBrush brush2(mFillStyle->GetBackColor());
	graphics.FillRectangle(&brush2,rc2);
	
	mPreviewPanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);

	RectF rc3(rect.left+10,rect.top+10,rect.Width()-20,rect.Height()-20);
	DrawStyle(&graphics,mFillStyle,rc3);
}

void CHatchFillStyleEditDlg::OnEnChangeNameEditCtrl()
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
