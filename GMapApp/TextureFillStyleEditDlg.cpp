// TextureFillStyleEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GMapApp.h"
#include "TextureFillStyleEditDlg.h"


// CTextureFillStyleEditDlg �Ի���

IMPLEMENT_DYNAMIC(CTextureFillStyleEditDlg, CDialog)

CTextureFillStyleEditDlg::CTextureFillStyleEditDlg(CWnd* pParent /*=NULL*/,GTextureFillStyle* fillStyle)
	: CDialog(CTextureFillStyleEditDlg::IDD, pParent)
{
	mFillStyle = fillStyle;
}

CTextureFillStyleEditDlg::~CTextureFillStyleEditDlg()
{
}

void CTextureFillStyleEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME_EDIT_CTRL, mNameEditCtrl);
	DDX_Control(pDX, IDC_IMAGE_FILE_EDIT_CTRL, mImageFileEditCtrl);
	DDX_Control(pDX, IDC_WRAP_MODE_CBX, mWrapModeCbxCtrl);
	DDX_Control(pDX, IDC_PREVIEW_PANEL, mPreviewPanelCtrl);
}


BEGIN_MESSAGE_MAP(CTextureFillStyleEditDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_OPEN_IMAGE_FILE, &CTextureFillStyleEditDlg::OnBnClickedBtnOpenImageFile)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_WRAP_MODE_CBX, &CTextureFillStyleEditDlg::OnCbnSelchangeWrapModeCbx)

END_MESSAGE_MAP()


// CTextureFillStyleEditDlg ��Ϣ�������

void CTextureFillStyleEditDlg::OnBnClickedBtnOpenImageFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		static wchar_t* imageFileFilter = L"jpg�ļ�(*.jpg)|*.jpg|png�ļ�(*.png)|*.png|gif�ļ�(*.gif)|*.gif";
	CFileDialog OpenFileDlg(TRUE,NULL,NULL,4|2,imageFileFilter,this);
	if(OpenFileDlg.DoModal() == IDOK)
	{
		CString fn = OpenFileDlg.GetPathName();
		mImageFileEditCtrl.SetWindowText(fn);
		mFillStyle->SetImageFile(wstring(fn.GetBuffer()));
		this->Invalidate();
	}	
}

BOOL CTextureFillStyleEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	mWrapModeCbxCtrl.AddString(L"WrapModeTile");
	mWrapModeCbxCtrl.AddString(L"WrapModeTileFlipX");
	mWrapModeCbxCtrl.AddString(L"WrapModeTileFlipY");
	mWrapModeCbxCtrl.AddString(L"WrapModeTileFlipXY");
	mWrapModeCbxCtrl.AddString(L"WrapModeClamp");
	

	mNameEditCtrl.SetWindowText(mFillStyle->GetName().c_str());
	mImageFileEditCtrl.SetWindowText(mFillStyle->GetImageFile().c_str());
	mWrapModeCbxCtrl.SetCurSel((int)mFillStyle->GetWrapMode());

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CTextureFillStyleEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	Graphics graphics(dc.GetSafeHdc());
	CRect rect;
	
	
	mPreviewPanelCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);

	RectF rc1(rect.left+10,rect.top+10,rect.Width()-20,rect.Height()-20);
	DrawStyle(&graphics,mFillStyle,rc1);
}

void CTextureFillStyleEditDlg::OnCbnSelchangeWrapModeCbx()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mFillStyle->SetWrapMode((WrapMode)mWrapModeCbxCtrl.GetCurSel());
	this->Invalidate();	
}

void CTextureFillStyleEditDlg::OnNewTextureFillStyle()
{
	// TODO: �ڴ���������������

}
