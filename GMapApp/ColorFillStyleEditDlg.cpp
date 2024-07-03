// ColorFillStyleEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GMapApp.h"
#include "ColorFillStyleEditDlg.h"


// CColorFillStyleEditDlg �Ի���

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


// CColorFillStyleEditDlg ��Ϣ�������

void CColorFillStyleEditDlg::OnBnClickedForeColorBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	mNameEditCtrl.SetWindowText(mFillStyle->GetName().c_str());
	mAlphaSliderCtrl.SetBuddy(&mAlphaEditCtrl);
	mAlphaSliderCtrl.SetRange(0,100);

	mAlphaSliderCtrl.SetPos(100.0*(mFillStyle->GetFillColor().GetAlpha()/255.0f));

	


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CColorFillStyleEditDlg::OnEnChangeNameEditCtrl()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString text;
	mNameEditCtrl.GetWindowText(text);
	mFillStyle->SetName(wstring(text.GetBuffer()));
}

void CColorFillStyleEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Color color = mFillStyle->GetFillColor();
	color = Color(255*(mAlphaSliderCtrl.GetPos()/100.0f),color.GetR(),color.GetG(),color.GetB());
	mFillStyle->SetFillColor(color);

	this->Invalidate();
	*pResult = 0;
}
