// ImageMerkerStyleEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "GMapApp.h"
#include "ImageMerkerStyleEditDlg.h"


// CImageMerkerStyleEditDlg �Ի���

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


// CImageMerkerStyleEditDlg ��Ϣ�������

void CImageMerkerStyleEditDlg::OnBtnOpenImageFile_Click()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CImageMerkerStyleEditDlg::OnDeltaPosXOffsetSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Rect rc(mXOffsetSpinCtrl.GetPos32(),mYOffsetSpinCtrl.GetPos32(),mWidthSpinCtrl.GetPos32(),mHeightSpinCtrl.GetPos32());
	mMarkerStyle->SetLayoutRect(rc);	
	Preview();
	*pResult = 0;
}

void CImageMerkerStyleEditDlg::OnDeltaPosYOffsetSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Rect rc(mXOffsetSpinCtrl.GetPos32(),mYOffsetSpinCtrl.GetPos32(),mWidthSpinCtrl.GetPos32(),mHeightSpinCtrl.GetPos32());
	mMarkerStyle->SetLayoutRect(rc);	
	Preview();
	*pResult = 0;
}

void CImageMerkerStyleEditDlg::OnDeltaPosWidthSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Rect rc(mXOffsetSpinCtrl.GetPos32(),mYOffsetSpinCtrl.GetPos32(),mWidthSpinCtrl.GetPos32(),mHeightSpinCtrl.GetPos32());
	mMarkerStyle->SetLayoutRect(rc);	
	Preview();
	*pResult = 0;
}

void CImageMerkerStyleEditDlg::OnDeltaPosHeightSpinCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CImageMerkerStyleEditDlg::OnEnChangeNameEditCtrl()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString name; 
	mNameEditCtrl.GetWindowText(name);
	mMarkerStyle->SetName(wstring(name.GetBuffer()));

}
