// ImageMerkerStyleEditDlg.cpp : 实现文件
//

#include "stdafx.h"

#include "GMapApp.h"
#include "ImageMerkerStyleEditDlg.h"


// CImageMerkerStyleEditDlg 对话框

IMPLEMENT_DYNAMIC(CImageMerkerStyleEditDlg, CDialog)

CImageMerkerStyleEditDlg::CImageMerkerStyleEditDlg(CWnd* pParent /*=NULL*/,GImageMarkerStyle* markerStyle)
	: CDialog(CImageMerkerStyleEditDlg::IDD, pParent)
{
	mMarkerStyle = markerStyle;
}

CImageMerkerStyleEditDlg::~CImageMerkerStyleEditDlg()
{

}

void CImageMerkerStyleEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME_EDIT_CTRL, mNameEditCtrl);
	DDX_Control(pDX, IDC_IMAGE_FILE_EDIT_CTRL, mImageFileEditCtrl);
	DDX_Control(pDX, IDC_X_OFFSET_EDIT_CTRL, mXOffsetEditCtrl);
	DDX_Control(pDX, IDC_Y_OFFSET_EDIT_CTRL, mYOffsetEditCtrl);
	DDX_Control(pDX, IDC_WIDTH_EDIT_CTRL, mWidthEditCtrl);
	DDX_Control(pDX, IDC_HEIGHT_EDIT_CTRL, mHeightEditCtrl);
	DDX_Control(pDX, IDC_X_OFFSET_SPIN_CTRL, mXOffsetSpinCtrl);
	DDX_Control(pDX, IDC_Y_OFFSET_SPIN_CTRL, mYOffsetSpinCtrl);
	DDX_Control(pDX, IDC_WIDTH_SPIN_CTRL, mWidthSpinCtrl);
	DDX_Control(pDX, IDC_HEIGHT_SPIN_CTRL, mHeightSpinCtrl);
	DDX_Control(pDX, IDC_STYLE_PREVIEW, mPreviewCtrl);
}


BEGIN_MESSAGE_MAP(CImageMerkerStyleEditDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_OPEN_IMAGE_FILE, &CImageMerkerStyleEditDlg::OnBtnOpenImageFile_Click)
	ON_NOTIFY(UDN_DELTAPOS, IDC_X_OFFSET_SPIN_CTRL, &CImageMerkerStyleEditDlg::OnDeltaPosXOffsetSpinCtrl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Y_OFFSET_SPIN_CTRL, &CImageMerkerStyleEditDlg::OnDeltaPosYOffsetSpinCtrl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_WIDTH_SPIN_CTRL, &CImageMerkerStyleEditDlg::OnDeltaPosWidthSpinCtrl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_HEIGHT_SPIN_CTRL, &CImageMerkerStyleEditDlg::OnDeltaPosHeightSpinCtrl)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CImageMerkerStyleEditDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_NAME_EDIT_CTRL, &CImageMerkerStyleEditDlg::OnEnChangeNameEditCtrl)
END_MESSAGE_MAP()


// CImageMerkerStyleEditDlg 消息处理程序

void CImageMerkerStyleEditDlg::OnBtnOpenImageFile_Click()
{
	// TODO: 在此添加控件通知处理程序代码
	static wchar_t* imageFileFilter = L"jpg(*.jpg)|*.jpg|png(*.png)|*.png|gif(*.gif)|*.gif";
	CFileDialog OpenFileDlg(TRUE,NULL,NULL,4|2,imageFileFilter,this);
	if(OpenFileDlg.DoModal() == IDOK)
	{
		CString fn = OpenFileDlg.GetPathName();
		mImageFileEditCtrl.SetWindowText(fn);
		mMarkerStyle->SetImageFile(fn.GetBuffer());
		Preview();
	}
	
}

BOOL CImageMerkerStyleEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mXOffsetSpinCtrl.SetBuddy(&mXOffsetEditCtrl);
	mYOffsetSpinCtrl.SetBuddy(&mYOffsetEditCtrl);
	mWidthSpinCtrl.SetBuddy(&mWidthEditCtrl);
	mHeightSpinCtrl.SetBuddy(&mHeightEditCtrl);

	mXOffsetSpinCtrl.SetRange(-256,256);
	mYOffsetSpinCtrl.SetRange(-256,256);
	mWidthSpinCtrl.SetRange(8,256);
	mHeightSpinCtrl.SetRange(8,256);

	Rect layoutRect = mMarkerStyle->GetLayoutRect();
	mXOffsetSpinCtrl.SetPos(layoutRect.X);
	mYOffsetSpinCtrl.SetPos(layoutRect.Y);
	mWidthSpinCtrl.SetPos(layoutRect.Width);
	mHeightSpinCtrl.SetPos(layoutRect.Height);

	mNameEditCtrl.SetWindowText(mMarkerStyle->GetName().c_str());
	mImageFileEditCtrl.SetWindowText(mMarkerStyle->GetImageFile().c_str());


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CImageMerkerStyleEditDlg::OnDeltaPosXOffsetSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	Rect rc(mXOffsetSpinCtrl.GetPos32(),mYOffsetSpinCtrl.GetPos32(),mWidthSpinCtrl.GetPos32(),mHeightSpinCtrl.GetPos32());
	mMarkerStyle->SetLayoutRect(rc);	
	Preview();
	*pResult = 0;
}

void CImageMerkerStyleEditDlg::OnDeltaPosYOffsetSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	Rect rc(mXOffsetSpinCtrl.GetPos32(),mYOffsetSpinCtrl.GetPos32(),mWidthSpinCtrl.GetPos32(),mHeightSpinCtrl.GetPos32());
	mMarkerStyle->SetLayoutRect(rc);	
	Preview();
	*pResult = 0;
}

void CImageMerkerStyleEditDlg::OnDeltaPosWidthSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	Rect rc(mXOffsetSpinCtrl.GetPos32(),mYOffsetSpinCtrl.GetPos32(),mWidthSpinCtrl.GetPos32(),mHeightSpinCtrl.GetPos32());
	mMarkerStyle->SetLayoutRect(rc);	
	Preview();
	*pResult = 0;
}

void CImageMerkerStyleEditDlg::OnDeltaPosHeightSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	Rect rc(mXOffsetSpinCtrl.GetPos32(),mYOffsetSpinCtrl.GetPos32(),mWidthSpinCtrl.GetPos32(),mHeightSpinCtrl.GetPos32());
	mMarkerStyle->SetLayoutRect(rc);
	Preview();

	*pResult = 0;
}

void CImageMerkerStyleEditDlg::Preview()
{
	CRect rect;
	mPreviewCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	this->InvalidateRect(&rect);
}
void CImageMerkerStyleEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	Graphics graphics(dc.GetSafeHdc());
	
	CRect rect;
	mPreviewCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);

	Rect rc(rect.left,rect.top,rect.Width(),rect.Height());
	SolidBrush solidBrush(Color::White);
	graphics.FillRectangle(&solidBrush,rc);
	mMarkerStyle->Draw(&graphics,PointF(rect.CenterPoint().x,rect.CenterPoint().y));

}

void CImageMerkerStyleEditDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CImageMerkerStyleEditDlg::OnEnChangeNameEditCtrl()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString name; 
	mNameEditCtrl.GetWindowText(name);
	mMarkerStyle->SetName(wstring(name.GetBuffer()));

}
