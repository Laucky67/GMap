// SimpleLineEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GMapApp.h"
#include "SimpleLineStyleEditDlg.h"


// CSimpleLineStyleEditDlg 对话框
DashStyle DashStyles[]=
{	
	DashStyleSolid,          // 0
    DashStyleDash,           // 1
    DashStyleDot,            // 2
    DashStyleDashDot,        // 3
    DashStyleDashDotDot,     // 4
    DashStyleCustom          // 5
};

int GetDashStyleIndex(DashStyle style)
{
	for(int i=0;i<6;i++)
	{
		if(DashStyles[i]==style)
			return i;
	}
	return 0;
}

DashCap DashCaps[]=
{
	DashCapFlat             ,
    DashCapRound            ,
    DashCapTriangle         
};

int GetDashCapIndex(DashCap style)
{
	for(int i=0;i<3;i++)
	{
		if(DashCaps[i]==style)
			return i;
	}
	return 0;
}
LineCap LineCaps[]=
{
    LineCapFlat             ,
    LineCapSquare           ,
    LineCapRound            ,
    LineCapTriangle         ,

    LineCapNoAnchor         , // corresponds to flat cap
    LineCapSquareAnchor     , // corresponds to square cap
    LineCapRoundAnchor      , // corresponds to round cap
    LineCapDiamondAnchor    , // corresponds to triangle cap
    LineCapArrowAnchor       // no correspondence
};

int GetLineCapIndex(LineCap style)
{
	for(int i=0;i<9;i++)
	{
		if(LineCaps[i]==style)
			return i;
	}
	return 0;
}

LineJoin LineJoins[]=
{
    LineJoinMiter        ,
    LineJoinBevel        ,
    LineJoinRound        ,
    LineJoinMiterClipped 
};

int GetLineJoinIndex(LineJoin style)
{
	for(int i=0;i<4;i++)
	{
		if(LineJoins[i]==style)
			return i;
	}
	return 0;
}

IMPLEMENT_DYNAMIC(CSimpleLineStyleEditDlg, CDialog)

CSimpleLineStyleEditDlg::CSimpleLineStyleEditDlg(CWnd* pParent /*=NULL*/,GSimpleLineStyle* pStyle)
	: CDialog(CSimpleLineStyleEditDlg::IDD, pParent)
{
	mLineStyle = pStyle;
}

CSimpleLineStyleEditDlg::~CSimpleLineStyleEditDlg()
{
}

void CSimpleLineStyleEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME_EDIT_CTRL, mNameEditCtrl);
	DDX_Control(pDX, IDC_LINE_COLOR_PANEL, mColorPanelCtrl);
	DDX_Control(pDX, IDC_LINE_WIDTH_EDIT, mLineWidthEditCtrl);
	DDX_Control(pDX, IDC_LINE_WIDTH_SPIN, mLineWidthSpinCtrl);
	DDX_Control(pDX, IDC_DASH_STYLE_CMB, mDashStyleCbxCtrl);
	DDX_Control(pDX, IDC_DASH_CAP_CMB, mDashCapCbxCtrl);
	DDX_Control(pDX, IDC_START_CAP_CMB, mStartCapCbxCtrl);
	DDX_Control(pDX, IDC_END_CAP_CMB, mEndCapCbxCtrl);
	DDX_Control(pDX, IDC_LINE_JOIN_CMB, mLineJionCbxCtrl);
	DDX_Control(pDX, IDC_PREVIEW_PANEL, mPreviewPanelCtrl);
}


BEGIN_MESSAGE_MAP(CSimpleLineStyleEditDlg, CDialog)
	ON_BN_CLICKED(IDC_LINE_COLOR_BTN, &CSimpleLineStyleEditDlg::OnBnClickedLineColorBtn)
	ON_CBN_SELCHANGE(IDC_DASH_STYLE_CMB, &CSimpleLineStyleEditDlg::OnCbnSelchangeDashStyleCmb)
	ON_CBN_SELCHANGE(IDC_DASH_CAP_CMB, &CSimpleLineStyleEditDlg::OnCbnSelchangeDashCapCmb)
	ON_CBN_SELCHANGE(IDC_START_CAP_CMB, &CSimpleLineStyleEditDlg::OnCbnSelchangeStartCapCmb)
	ON_CBN_SELCHANGE(IDC_END_CAP_CMB, &CSimpleLineStyleEditDlg::OnCbnSelchangeEndCapCmb)
	ON_CBN_SELCHANGE(IDC_LINE_JOIN_CMB, &CSimpleLineStyleEditDlg::OnCbnSelchangeLineJoinCmb)
	ON_EN_CHANGE(IDC_NAME_EDIT_CTRL, &CSimpleLineStyleEditDlg::OnEnChangeNameEditCtrl)
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_LINE_WIDTH_SPIN, &CSimpleLineStyleEditDlg::OnDeltaposLineWidthSpin)
END_MESSAGE_MAP()


// CSimpleLineStyleEditDlg 消息处理程序

void CSimpleLineStyleEditDlg::OnBnClickedLineColorBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog colorDialog(mLineStyle->GetColor().GetValue(),NULL,this);
	if(colorDialog.DoModal()==IDOK)
	{
		COLORREF rgb = colorDialog.GetColor();
		Color color(GetRValue(rgb),GetGValue(rgb),GetBValue(rgb));
		mLineStyle->SetColor(color);
		this->Invalidate();
	}
}

BOOL CSimpleLineStyleEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mNameEditCtrl.SetWindowText(mLineStyle->GetName().c_str());
	mLineWidthSpinCtrl.SetBuddy(&mLineWidthEditCtrl);
	mLineWidthSpinCtrl.SetRange(1,16);

	//
	mDashStyleCbxCtrl.InsertString(0,L"Solid");
	mDashStyleCbxCtrl.InsertString(1,L"Dash");
	mDashStyleCbxCtrl.InsertString(2,L"Dot");
	mDashStyleCbxCtrl.InsertString(3,L"DashDot");
	mDashStyleCbxCtrl.InsertString(4,L"DashDotDot");

	mDashCapCbxCtrl.InsertString(0,L"DashCapFlat");
	mDashCapCbxCtrl.InsertString(1,L"DashCapRound");
	mDashCapCbxCtrl.InsertString(2,L"DashCapTriangle");

	mStartCapCbxCtrl.InsertString(0,L"LineCapFlat");
	mStartCapCbxCtrl.InsertString(1,L"LineCapSquare");
	mStartCapCbxCtrl.InsertString(2,L"LineCapRound");
	mStartCapCbxCtrl.InsertString(3,L"LineCapTriangle");
	mStartCapCbxCtrl.InsertString(4,L"LineCapNoAnchor");
	mStartCapCbxCtrl.InsertString(5,L"LineCapSquareAnchor");
	mStartCapCbxCtrl.InsertString(6,L"LineCapRoundAnchor");
	mStartCapCbxCtrl.InsertString(7,L"LineCapDiamondAnchor");
	mStartCapCbxCtrl.InsertString(8,L"LineCapArrowAnchor");

	mEndCapCbxCtrl.InsertString(0,L"LineCapFlat");
	mEndCapCbxCtrl.InsertString(1,L"LineCapSquare");
	mEndCapCbxCtrl.InsertString(2,L"LineCapRound");
	mEndCapCbxCtrl.InsertString(3,L"LineCapTriangle");
	mEndCapCbxCtrl.InsertString(4,L"LineCapNoAnchor");
	mEndCapCbxCtrl.InsertString(5,L"LineCapSquareAnchor");
	mEndCapCbxCtrl.InsertString(6,L"LineCapRoundAnchor");
	mEndCapCbxCtrl.InsertString(7,L"LineCapDiamondAnchor");
	mEndCapCbxCtrl.InsertString(8,L"LineCapArrowAnchor");

	mLineJionCbxCtrl.InsertString(0,L"LineJoinMiter");
	mLineJionCbxCtrl.InsertString(1,L"LineJoinBevel");
	mLineJionCbxCtrl.InsertString(2,L"LineJoinRound");
	mLineJionCbxCtrl.InsertString(4,L"LineJoinMiterClipped");
	
	mLineWidthSpinCtrl.SetPos(mLineStyle->GetLineWidth());
	mDashStyleCbxCtrl.SetCurSel(GetDashStyleIndex(mLineStyle->GetDashStyle()));
	mDashCapCbxCtrl.SetCurSel(GetDashCapIndex(mLineStyle->GetDashCap()));
	mStartCapCbxCtrl.SetCurSel(GetLineCapIndex(mLineStyle->GetStartCap()));
	mEndCapCbxCtrl.SetCurSel(GetLineCapIndex(mLineStyle->GetEndCap()));
	mLineJionCbxCtrl.SetCurSel(GetLineJoinIndex(mLineStyle->GetLineJoin()));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSimpleLineStyleEditDlg::OnCbnSelchangeDashStyleCmb()
{
	// TODO: 在此添加控件通知处理程序代码
	mLineStyle->SetDashStyle(DashStyles[mDashStyleCbxCtrl.GetCurSel()]);
	this->Invalidate();
}

void CSimpleLineStyleEditDlg::OnCbnSelchangeDashCapCmb()
{
	// TODO: 在此添加控件通知处理程序代码
	mLineStyle->SetDashCap(DashCaps[mDashCapCbxCtrl.GetCurSel()]);	
	this->Invalidate();
}

void CSimpleLineStyleEditDlg::OnCbnSelchangeStartCapCmb()
{
	// TODO: 在此添加控件通知处理程序代码
	mLineStyle->SetStartCap(LineCaps[mStartCapCbxCtrl.GetCurSel()]);
	this->Invalidate();
}

void CSimpleLineStyleEditDlg::OnCbnSelchangeEndCapCmb()
{
	// TODO: 在此添加控件通知处理程序代码
	mLineStyle->SetEndCap(LineCaps[mEndCapCbxCtrl.GetCurSel()]);
	this->Invalidate();
}

void CSimpleLineStyleEditDlg::OnCbnSelchangeLineJoinCmb()
{
	// TODO: 在此添加控件通知处理程序代码
	mLineStyle->SetLineJoin(LineJoins[mLineJionCbxCtrl.GetCurSel()]);
	this->Invalidate();
}

void CSimpleLineStyleEditDlg::OnEnChangeNameEditCtrl()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString text;
	mNameEditCtrl.GetWindowText(text);
	mLineStyle->SetName(wstring(text.GetBuffer()));
}

void CSimpleLineStyleEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	Graphics graphics(dc.GetSafeHdc());

	CRect rect;
	mColorPanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);

	RectF rc1(rect.left,rect.top,rect.Width(),rect.Height());
	SolidBrush whiteBrush(Color::White);
	graphics.FillRectangle(&whiteBrush,rc1);

	SolidBrush solidBrush(mLineStyle->GetColor());
	graphics.FillRectangle(&solidBrush,rc1);
	
	mPreviewPanelCtrl.GetWindowRect(&rect);
	//mPreviewPanelCtrl.GetClientRect(&rect);
	ScreenToClient(&rect);
	
	RectF rc2(rect.left+10,rect.top+10,rect.Width()-20,rect.Height()-20);	
	
	graphics.FillRectangle(&whiteBrush,rc2);
	rc2.Inflate(-10,0);
	DrawStyle(&graphics,mLineStyle,rc2);
}

void CSimpleLineStyleEditDlg::OnDeltaposLineWidthSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	mLineStyle->SetLineWidth(mLineWidthSpinCtrl.GetPos32());
	this->Invalidate();
	*pResult = 0;
}
