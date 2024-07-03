// ProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GMapApp.h"
#include "ProgressDlg.h"


// CProgressDlg 对话框

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
: CDialog(CProgressDlg::IDD, pParent)
{

}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TX_TITLE, CPrgTitleTxt);
	DDX_Control(pDX, IDC_TX_DISPLAY_PROG, CPrgDisplayTxt);
	DDX_Control(pDX, IDC_PROGRESS_BAR, mProgressCtrl);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
END_MESSAGE_MAP()


// CProgressDlg 消息处理程序
//
void  CProgressDlg::OnSetTotal(const wchar_t* id,u32 total)
{
	Title = id;
	Total = total;
}

//
void  CProgressDlg::OnProgress(const wchar_t* id,u32 progress)
{
	Progress = progress;
}
//!进度
void  CProgressDlg::OnProgress(const wchar_t* id,u32 progress,u32 total)
{
	Total = total;
	Progress = progress;
}


void CProgressDlg::UpdateUI()
{
	if(Progress%10==0)
	{
		CString fmt;
		CPrgTitleTxt.SetWindowText(Title.c_str());

		fmt.Format(L"%d/%d",Progress,Total);
		CPrgTitleTxt.SetWindowText(fmt);

		mProgressCtrl.SetRange(0,Total);
		mProgressCtrl.SetPos(Progress);


		MSG msg;
		while(PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			AfxGetThread()->PumpMessage();
		}
	}
	
}