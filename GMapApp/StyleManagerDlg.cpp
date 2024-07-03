// StyleManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GMapApp.h"
#include "StyleManagerDlg.h"
#include "ImageMerkerStyleEditDlg.h"
#include "SimpleLineStyleEditDlg.h"
#include "ColorFillStyleEditDlg.h"
#include "HatchFillStyleEditDlg.h"
#include "TextureFillStyleEditDlg.h"


// CStyleManagerDlg 对话框

IMPLEMENT_DYNAMIC(CStyleManagerDlg, CDialog)

CStyleManagerDlg::CStyleManagerDlg(CWnd* pParent /*=NULL*/,GStyleGroup* styleGroup,int showFlag)
: CDialog(CStyleManagerDlg::IDD, pParent)
{
	mShowTabFlag=showFlag;
	mListCtrlGraphics = NULL;
	mCurrentStyle = NULL;

	mStyleGroup  = styleGroup;

	mDashPen = new Pen(Color::Black,0);
	mDashPen->SetDashStyle(DashStyleDot);
	mFillBrush = new SolidBrush(Color(100,0,0,0));
}

CStyleManagerDlg::~CStyleManagerDlg()
{
	// mStyleGroup;


	delete mDashPen;
	delete mFillBrush;
}

void CStyleManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_NEW_STYLE, mBtnNewStyle);
	DDX_Control(pDX, IDC_STYLE_TAB, mStyleTabCtrl);
	DDX_Control(pDX, IDC_STYLE_LIST_CTRL, mStyleListCtrl);
	DDX_Control(pDX, IDC_STYLE_ID, mEditStyleID);
	DDX_Control(pDX, IDC_STYLE_NAME, mEditStyleName);
	DDX_Control(pDX, IDC_STYLE_PANEL, mStylePanelCtrl);
}


BEGIN_MESSAGE_MAP(CStyleManagerDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_NEW_STYLE, &CStyleManagerDlg::OnBnClickedBtnNewStyle)
	ON_NOTIFY(TCN_SELCHANGE, IDC_STYLE_TAB, &CStyleManagerDlg::OnTcnSelchangeTab1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_STYLE_LIST_CTRL, &CStyleManagerDlg::OnNMCustomdrawStyleListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_STYLE_LIST_CTRL, &CStyleManagerDlg::OnLvnItemchangedStyleListCtrl)
	ON_WM_PAINT()
	ON_COMMAND(ID_NEW_IMAGE_MARKER_STYLE, &CStyleManagerDlg::OnNewImageMarkerStyle)
	ON_COMMAND(ID_NEW_SIMPLE_LINE_STYLE, &CStyleManagerDlg::OnNewSimpleLineStyle)
	ON_COMMAND(ID_NEW_COLOR_FILL_STYLE, &CStyleManagerDlg::OnNewColorFillStyle)
	ON_COMMAND(ID_NEW_HATCH_FILL_STYLE, &CStyleManagerDlg::OnNewHatchFillStyle)
	ON_COMMAND(ID_NEW_TEXTURE_FILL_STYLE, &CStyleManagerDlg::OnNewTextureFillStyle)
	ON_BN_CLICKED(IDC_EDIT_BTN_CTRL, &CStyleManagerDlg::OnBnClickedEditBtnCtrl)
END_MESSAGE_MAP()


void CStyleManagerDlg::OnBnClickedBtnNewStyle()
{
	// TODO: 在此添加控件通知处理程序代码
	CMenu menu;
	menu.LoadMenu(IDR_MENU_NEW_STYLE);
	RECT rect;
	mBtnNewStyle.GetWindowRect(&rect);
	int currentStyle=0;

	if(mShowTabFlag==0)
	{
		currentStyle=0;
	}
	else if(mShowTabFlag==1)
	{
		currentStyle=1;
	}
	else if(mShowTabFlag==2)
	{		
		currentStyle=2;
	}
	else
	{
		currentStyle = mStyleTabCtrl.GetCurSel();
	}


	CMenu* subMenu = menu.GetSubMenu(currentStyle);
	subMenu->TrackPopupMenu(TPM_LEFTALIGN,rect.right,rect.top,this);
	menu.DestroyMenu();
}


void CStyleManagerDlg::FillList(int currentStyle)
{
	mStyleListCtrl.DeleteAllItems();
	for(int i=0;i<mStyleGroup->GetStyleCount();i++)
	{

		gmap::GStyle* pStyle = mStyleGroup->GetStyleAt(i);
		if(pStyle->GetStyleType()==(currentStyle+1)||currentStyle>2)
		{
			LVITEM lvi;
			ZeroMemory(&lvi,sizeof(LVITEM));
			lvi.mask = LVIF_TEXT+LVIF_PARAM;
			lvi.iItem=i;
			lvi.pszText = (wchar_t*)pStyle->GetName().c_str();
			lvi.lParam = (LPARAM)pStyle;
			mStyleListCtrl.InsertItem(&lvi);
		}
	}
	mCurrentStyle = NULL;

}

BOOL CStyleManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(mStyleGroup->GetStyleCount()==0)
	{

		{
			gmap::GImageMarkerStyle* pImageMarker = new gmap::GImageMarkerStyle(NULL,0,wstring(L"默认点符号"));
			mStyleGroup->AddStyle(pImageMarker);
			pImageMarker->SetImageFile(wstring(L"./dat/icons/8.png")); 
			pImageMarker->Release();
		}
		{
			gmap::GSimpleLineStyle* pLineStyle = new gmap::GSimpleLineStyle(NULL,0,wstring(L"默认线符号"));
			mStyleGroup->AddStyle(pLineStyle);
			pLineStyle->Release();
		}
		{
			gmap::GColorFillStyle* pFillStyle = new gmap::GColorFillStyle(NULL,0,wstring(L"默认填充符号"));
			mStyleGroup->AddStyle(pFillStyle);
			pFillStyle->Release();
		}
		{
			gmap::GSimpleLineStyle* pLineStyle = new gmap::GSimpleLineStyle(NULL,0,wstring(L"默认线符号2"));
			pLineStyle->SetColor(Color::Blue);
			mStyleGroup->AddStyle(pLineStyle);
			pLineStyle->Release();
			//pImageMarker->SetImageFile(wstring(L"D:\\portal1.bmp"));
		}
	}

	if(mShowTabFlag==0)
	{
		mStyleTabCtrl.InsertItem(0,L"点符号");		
	}
	else if(mShowTabFlag==1)
	{
		mStyleTabCtrl.InsertItem(1,L"线符号");
	}
	else if(mShowTabFlag==2)
	{		
		mStyleTabCtrl.InsertItem(2,L"面符号");
	}
	else
	{
		mStyleTabCtrl.InsertItem(0,L"点符号");
		mStyleTabCtrl.InsertItem(1,L"线符号");
		mStyleTabCtrl.InsertItem(2,L"面符号");
	}

	CRect rectClient;
	mStyleTabCtrl.GetClientRect(&rectClient);
	mStyleTabCtrl.AdjustRect(FALSE,&rectClient);
	CRect rectWnd;
	mStyleTabCtrl.GetWindowRect(&rectWnd);
	rectClient.OffsetRect(rectWnd.left,rectWnd.top);
	this->ScreenToClient(&rectClient);

	mStyleListCtrl.MoveWindow(&rectClient);


	mStyleListCtrl.SetView( LV_VIEW_TILE );
	mStyleListCtrl.Arrange(LVA_ALIGNTOP);


	LVTILEVIEWINFO lvv;
	ZeroMemory(&lvv,sizeof(lvv));
	lvv.cbSize =sizeof(lvv);
	lvv.dwMask=LVTVIM_TILESIZE;
	lvv.dwFlags=LVTVIF_FIXEDSIZE;
	lvv.sizeTile.cx =128;
	lvv.sizeTile.cy =128;
	mStyleListCtrl.SetTileViewInfo(&lvv);

	lvv.dwMask=LVTVIM_LABELMARGIN;
	//lvv.dwFlags=LVTVIF_FIXEDSIZE;
	lvv.rcLabelMargin.top=100;
	lvv.rcLabelMargin.left =0;
	lvv.rcLabelMargin.bottom=0;
	lvv.rcLabelMargin.right = 0;
	mStyleListCtrl.SetTileViewInfo(&lvv);

	FillList(mShowTabFlag);


	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CStyleManagerDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int currentStyle=0;
	if(mShowTabFlag==0)
	{
		currentStyle=0;
	}
	else if(mShowTabFlag==1)
	{
		currentStyle=1;
	}
	else if(mShowTabFlag==2)
	{		
		currentStyle=2;
	}
	else
	{
		currentStyle = mStyleTabCtrl.GetCurSel();
	}

	FillList(currentStyle);

	mEditStyleID.SetWindowText(L"");
	mEditStyleName.SetWindowText(L"");
	mEditStyleID.EnableWindow(0);
	mEditStyleName.EnableWindow(0);
	mCurrentStyle= NULL;


	CRect rect;
	mStylePanelCtrl.GetWindowRect(rect);
	this->ScreenToClient(&rect);
	this->InvalidateRect(&rect);

	*pResult = 0;
}

void CStyleManagerDlg::OnNMCustomdrawStyleListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	switch(pNMCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			mListCtrlGraphics  =  Graphics::FromHDC(pNMCD->nmcd.hdc);
		}
		break;
	case CDDS_POSTPAINT:
		{
			delete mListCtrlGraphics;
		}
		break;
	case CDDS_ITEMPREPAINT:
		{
			//pNMCD->clrFace=RGB(255,0,0);
		}
		break;

	case CDDS_ITEMPOSTPAINT:
		{
			gmap::GStyle* pStyle = (gmap::GStyle* )pNMCD->nmcd.lItemlParam;
			CRect rc1(pNMCD->nmcd.rc);
			RectF rc(rc1.left+1,rc1.top+1,rc1.Width()-2,rc1.Height()-28);
			RectF rc2=rc;
			rc2.Inflate(-2,-2);
			gmap::DrawStyle(mListCtrlGraphics,pStyle,rc2);

			if((pNMCD->nmcd.uItemState & CDIS_SELECTED)==CDIS_SELECTED)
			{
				mListCtrlGraphics->DrawRectangle(mDashPen,rc);
			}
		
		}
		break;
	default: break;    
	}


	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;
	*pResult |= CDRF_NOTIFYITEMDRAW;
	*pResult |= CDRF_NOTIFYSUBITEMDRAW;
}

void CStyleManagerDlg::OnLvnItemchangedStyleListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	if(mStyleListCtrl.GetSelectedCount()!=1)
	{
		mEditStyleID.SetWindowText(L"");
		mEditStyleName.SetWindowText(L"");
		mEditStyleID.EnableWindow(0);
		mEditStyleName.EnableWindow(0);
	}
	else
	{
		mEditStyleID.EnableWindow(1);
		mEditStyleName.EnableWindow(1);
	}


	gmap::GStyle* pStyle =(gmap::GStyle* )pNMLV->lParam;
	mCurrentStyle=NULL;

	if(NULL!=(pNMLV->uNewState & LVIS_SELECTED))
	{
		int id = pStyle->GetId();
		wstring name = pStyle->GetName();
		CString strID;
		strID.Format(L"%d",id);
		mEditStyleID.SetWindowText(strID);
		mEditStyleName.SetWindowText(name.c_str());

		mCurrentStyle= pStyle;


		CRect rect;
		mStylePanelCtrl.GetWindowRect(rect);
		this->ScreenToClient(&rect);
		this->InvalidateRect(&rect);

	}

	*pResult = 0;
}

void CStyleManagerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	if(mCurrentStyle!=NULL)
	{
		Graphics g(dc.GetSafeHdc());
		g.Clear(Color::White);

		CRect rect;
		mStylePanelCtrl.GetWindowRect(rect);
		this->ScreenToClient(&rect);
		RectF rc(rect.left+10,rect.top+10,rect.Width()-20,rect.Height()-20);
		gmap::DrawStyle(&g,mCurrentStyle,rc);
		g.DrawRectangle(mDashPen,rc);
	}

}

void CStyleManagerDlg::OnNewImageMarkerStyle()
{
	// TODO: 在此添加命令处理程序代码
	gmap::GImageMarkerStyle* style = new gmap::GImageMarkerStyle(NULL,0,L"新建点符号");
	CImageMerkerStyleEditDlg editDlg(this,style);
	if(editDlg.DoModal()==IDOK)
	{
		mStyleGroup->AddStyle(style);
		LVITEM lvi;
		ZeroMemory(&lvi,sizeof(LVITEM));
		lvi.mask = LVIF_TEXT+LVIF_PARAM;
		lvi.iItem=mStyleListCtrl.GetItemCount();
		lvi.pszText = (wchar_t*)style->GetName().c_str();
		lvi.lParam = (LPARAM)style;
		mStyleListCtrl.InsertItem(&lvi);
	}
	style->Release();

}

void CStyleManagerDlg::OnNewSimpleLineStyle()
{
	// TODO: 在此添加命令处理程序代码
	gmap::GSimpleLineStyle* style = new gmap::GSimpleLineStyle(NULL,0,L"新建线符号");
	CSimpleLineStyleEditDlg editDlg(this,style);
	if(editDlg.DoModal()==IDOK)
	{
		mStyleGroup->AddStyle(style);
		LVITEM lvi;
		ZeroMemory(&lvi,sizeof(LVITEM));
		lvi.mask = LVIF_TEXT+LVIF_PARAM;
		lvi.iItem=mStyleListCtrl.GetItemCount();
		lvi.pszText = (wchar_t*)style->GetName().c_str();
		lvi.lParam = (LPARAM)style;
		mStyleListCtrl.InsertItem(&lvi);
	}
	style->Release();

}

void CStyleManagerDlg::OnNewColorFillStyle()
{
	// TODO: 在此添加命令处理程序代码
	gmap::GColorFillStyle* style = new gmap::GColorFillStyle(NULL,0,L"新建面符号");
	CColorFillStyleEditDlg editDlg(this,style);
	if(editDlg.DoModal()==IDOK)
	{
		mStyleGroup->AddStyle(style);
		LVITEM lvi;
		ZeroMemory(&lvi,sizeof(LVITEM));
		lvi.mask = LVIF_TEXT+LVIF_PARAM;
		lvi.iItem=mStyleListCtrl.GetItemCount();
		lvi.pszText = (wchar_t*)style->GetName().c_str();
		lvi.lParam = (LPARAM)style;
		mStyleListCtrl.InsertItem(&lvi);
	}
	style->Release();
}

void CStyleManagerDlg::OnNewHatchFillStyle()
{
	// TODO: 在此添加命令处理程序代码
	gmap::GHatchFillStyle* style = new gmap::GHatchFillStyle(NULL,0,L"新建面符号");
	CHatchFillStyleEditDlg editDlg(this,style);
	if(editDlg.DoModal()==IDOK)
	{
		mStyleGroup->AddStyle(style);
		LVITEM lvi;
		ZeroMemory(&lvi,sizeof(LVITEM));
		lvi.mask = LVIF_TEXT+LVIF_PARAM;
		lvi.iItem=mStyleListCtrl.GetItemCount();
		lvi.pszText = (wchar_t*)style->GetName().c_str();
		lvi.lParam = (LPARAM)style;
		mStyleListCtrl.InsertItem(&lvi);
	}
	style->Release();
}

void CStyleManagerDlg::OnNewTextureFillStyle()
{
	// TODO: 在此添加命令处理程序代码
	gmap::GTextureFillStyle* style = new gmap::GTextureFillStyle(NULL,0,L"新建面符号");
	CTextureFillStyleEditDlg editDlg(this,style);
	if(editDlg.DoModal()==IDOK)
	{
		mStyleGroup->AddStyle(style);
		LVITEM lvi;
		ZeroMemory(&lvi,sizeof(LVITEM));
		lvi.mask = LVIF_TEXT+LVIF_PARAM;
		lvi.iItem=mStyleListCtrl.GetItemCount();
		lvi.pszText = (wchar_t*)style->GetName().c_str();
		lvi.lParam = (LPARAM)style;
		mStyleListCtrl.InsertItem(&lvi);
	}
	style->Release();
}

void CStyleManagerDlg::OnBnClickedEditBtnCtrl()
{
	// TODO: 在此添加控件通知处理程序代码
	if(mCurrentStyle!=NULL)
	{
		if(mCurrentStyle->GetStyleType()==GStyle::ST_MARKER_STYLE)
		{
			gmap::GImageMarkerStyle* style = (gmap::GImageMarkerStyle*)mCurrentStyle;
			CImageMerkerStyleEditDlg editDlg(this,style);
			editDlg.DoModal();
		}
		else if(mCurrentStyle->GetStyleType()==GStyle::ST_LINE_STYLE)
		{
			gmap::GSimpleLineStyle* style = (gmap::GSimpleLineStyle*)mCurrentStyle;
			CSimpleLineStyleEditDlg editDlg(this,style);
			editDlg.DoModal();
		}
		else if(mCurrentStyle->GetStyleType()==GStyle::ST_FILL_STYLE)
		{
			gmap::GFillStyle* fillStyle = (gmap::GFillStyle*)mCurrentStyle;
			if(fillStyle->GetFillStyleType() == GFillStyle::FTS_COLOR_FILL_STYLE)
			{
				gmap::GColorFillStyle* style = (gmap::GColorFillStyle*)mCurrentStyle;
				CColorFillStyleEditDlg editDlg(this,style);
				editDlg.DoModal();
			}
			else if(fillStyle->GetFillStyleType() == GFillStyle::FTS_HATCH_FILL_STYLE)
			{
				gmap::GHatchFillStyle* style = (gmap::GHatchFillStyle*)mCurrentStyle;
				CHatchFillStyleEditDlg editDlg(this,style);
				editDlg.DoModal();
			}	
			else if(fillStyle->GetFillStyleType() == GFillStyle::FTS_TEXTURE_FILL_STYLE)
			{
				gmap::GTextureFillStyle* style = (gmap::GTextureFillStyle*)mCurrentStyle;
				CTextureFillStyleEditDlg editDlg(this,style);
				editDlg.DoModal();
			}
		}
		mEditStyleName.SetWindowText( mCurrentStyle->GetName().c_str());



		POSITION pos = mStyleListCtrl.GetFirstSelectedItemPosition();
		int sel = mStyleListCtrl.GetNextSelectedItem(pos);
		mStyleListCtrl.SetItemText(sel,0,(wchar_t*)mCurrentStyle->GetName().c_str());

	}
	
}
