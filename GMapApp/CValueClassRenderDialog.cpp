// CValueClassRenderDialog.cpp: 实现文件
//

#include "stdafx.h"
#include "GMapApp.h"
#include "CValueClassRenderDialog.h"
#include "StyleManagerDlg.h"
#include "afxdialogex.h"
#include <set>
#include <algorithm>
// CValueClassRenderDialog 对话框

IMPLEMENT_DYNAMIC(CValueClassRenderDialog, CDialogEx)

CValueClassRenderDialog::CValueClassRenderDialog(GMap* map,GVectorDataLayer* dataLayer, GValueClassRender* classRender, CWnd* pParent)
	: CDialogEx(IDD_VALUE_CLASS_RENDER_DLG, pParent)
{
	Map = map;
	DataLayer = dataLayer;
	ClassRender = classRender;
	CurrentStyleItem = NULL;
}

CValueClassRenderDialog::~CValueClassRenderDialog()
{

}

void CValueClassRenderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_FIELD, m_cbFields);
	DDX_Control(pDX, IDC_LIST_VALUE_CLASS, m_lvValueClass);
	DDX_Control(pDX, IDC_ST_MSTYLE, m_stMStyle);
	DDX_Control(pDX, IDC_ST_LSTYLE, m_stLStyle);
	DDX_Control(pDX, IDC_ST_FSTYLE, m_stFStyle);
}

void CValueClassRenderDialog::updateClassIds()
{
	ClassIDs.clear();

	if (ClassField.size()==0)
		return;
	
	set<int> ids;
	GVectorDataQuery* query=DataLayer->CreateQueryCommand(false);
	query->AddField(ClassField);
	GVectorDataQueryResult* rs = query->Execute();

	while (!rs->IsEOF())
	{
		_variant_t val;
		rs->GetFieldValue(0, val);
		int clsid = val;
		ids.insert(clsid);
		if (ids.size() > ClassIDs.size())
		{
			ClassIDs.push_back(pair<int,bool>(clsid,true));
		}
		rs->MoveNext();
	}
	rs->Release();
	query->Release();

	for (int i = 0; i < ClassRender->GetCount(); i++)
	{
		StyleClassItem* item = ClassRender->GetItem(i);
		int clsid = item->ClassValue;
		ids.insert(clsid);
		if (ids.size() > ClassIDs.size())
		{
			ClassIDs.push_back(pair<int, bool>(clsid, false));
		}
	}
	ids.insert(-1);
	if (ids.size() > ClassIDs.size())
	{
		ClassIDs.push_back(pair<int, bool>(-1, false));
	}
	std::sort(ClassIDs.begin(), ClassIDs.end(), [](auto a, auto b)->int {return a.first > b.first; });
}

const wchar_t* Yz = L"√";
const wchar_t* Nz = L"X";
void CValueClassRenderDialog::updateListView()
{
	int i = 0;
	USES_CONVERSION;
	while(m_lvValueClass.GetItemCount())
		m_lvValueClass.DeleteItem(0);
	for (auto it = ClassIDs.begin(), end = ClassIDs.end(); it != end; it++)
	{
		char sbuf[100];
		sprintf_s(sbuf, "%d", it->first);
		int id = m_lvValueClass.InsertItem(i++, A2T(sbuf));

		m_lvValueClass.SetItem(id, 1, LVIF_TEXT, it->second?Yz:Nz, 0, 0, 0, 0, 0);
		m_lvValueClass.SetItem(id, 2, LVIF_TEXT, ClassRender->GetItemByClass(it->first)?Yz:Nz, 0, 0, 0, 0, 0);
	}
}

BEGIN_MESSAGE_MAP(CValueClassRenderDialog, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_LSTYLE, &CValueClassRenderDialog::OnClickedBtnLstyle)
	ON_BN_CLICKED(IDC_BTN_MSTYLE, &CValueClassRenderDialog::OnClickedBtnMstyle)
	ON_BN_CLICKED(IDC_BTN_FSTYLE, &CValueClassRenderDialog::OnClickedBtnFstyle)
	ON_CBN_SELCHANGE(IDC_CB_FIELD, &CValueClassRenderDialog::OnSelchangeCbField)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VALUE_CLASS, &CValueClassRenderDialog::OnClickListValueClass)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_VALUE_CLASS, &CValueClassRenderDialog::OnDblclkListValueClass)
END_MESSAGE_MAP()

// CValueClassRenderDialog 消息处理程序

bool isIntergerField(GFieldDef* field)
{
	return (field->GetFieldType()== GFieldDef::ftTinyInt ||
			field->GetFieldType() == GFieldDef::ftSmallInt ||
			field->GetFieldType() == GFieldDef::ftInteger ||
			field->GetFieldType() == GFieldDef::ftBigInt ||
			field->GetFieldType() == GFieldDef::ftUnsignedTinyInt ||
			field->GetFieldType() == GFieldDef::ftUnsignedSmallInt ||
			field->GetFieldType() == GFieldDef::ftUnsignedInt ||
			field->GetFieldType()== GFieldDef::ftUnsignedBigInt);
}
BOOL CValueClassRenderDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	USES_CONVERSION;
	for (int i = 0; i < DataLayer->GetFieldCount(); i++)
	{
		GFieldDef* fd = DataLayer->GetFieldDef(i);
		if (isIntergerField(fd))
		{
			int ix = m_cbFields.AddString(fd->GetName().c_str());
			m_cbFields.SetItemData(ix,i);
		}
	}
	ClassField = ClassRender->GetClassField();
	if (ClassRender->GetClassField().size() < 1)
	{
		if (m_cbFields.GetCount()>0)
		{
			m_cbFields.SetCurSel(0);
		}
	}
	else
	{
		m_cbFields.SelectString(-1, ClassRender->GetClassField().c_str());
	}

	// TODO:  在此添加额外的初始化
	m_lvValueClass.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	int i = 0;
	m_lvValueClass.InsertColumn(i++, L"类别ID",0,100);			//0
	m_lvValueClass.InsertColumn(i++, L"数据项",0,100);			//0
	m_lvValueClass.InsertColumn(i++, L"已设置",0,100);
	
	if (ClassField.size()>0)
	{
		updateClassIds();
		updateListView();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CValueClassRenderDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting	Graphics graphics(dc.GetSafeHdc());
	Graphics graphics(dc.GetSafeHdc());	
	
	StyleClassItem* item=CurrentStyleItem;
	if (item == NULL)
		return;
	
	CRect rect;
	m_stMStyle.GetWindowRect(&rect);
	ScreenToClient(&rect);
	RectF rc2(rect.left, rect.top, rect.Width(), rect.Height());
	SolidBrush brush(Color::White);
	graphics.FillRectangle(&brush, rc2);
	
	if (item->MStyle)
	{
		DrawStyle(&graphics, item->MStyle, rc2);
	}

	m_stLStyle.GetWindowRect(&rect);
	ScreenToClient(&rect);
	RectF rc3(rect.left, rect.top, rect.Width(), rect.Height());
	graphics.FillRectangle(&brush, rc3);
	if (item->LStyle)
	{
		DrawStyle(&graphics, item->LStyle, rc3);
	}

	m_stFStyle.GetWindowRect(&rect);
	ScreenToClient(&rect);
	RectF rc1(rect.left, rect.top, rect.Width(), rect.Height());
	//SolidBrush brush(Color::White);
	graphics.FillRectangle(&brush, rc1);
	if (item->FStyle)
	{
		DrawStyle(&graphics, item->FStyle, rc1);
	}
}


void CValueClassRenderDialog::OnClickedBtnMstyle()
{
	if (CurrentStyleItem == NULL)
		return;
	CStyleManagerDlg dlg(this, Map->GetStyleGroup(), 0);
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.GetSelectStyle() != NULL)
		{
			CurrentStyleItem->setMStyle((GMarkerStyle*)dlg.GetSelectStyle());
		}
		this->Invalidate();
	}
}

void CValueClassRenderDialog::OnClickedBtnLstyle()
{
	if (CurrentStyleItem == NULL)
		return;
	CStyleManagerDlg dlg(this, Map->GetStyleGroup(), 1);
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.GetSelectStyle() != NULL)
		{
			CurrentStyleItem->setLStyle((GLineStyle*)dlg.GetSelectStyle());
		}
		this->Invalidate();
	}
}

void CValueClassRenderDialog::OnClickedBtnFstyle()
{
	if (CurrentStyleItem == NULL)
		return;
	CStyleManagerDlg dlg(this, Map->GetStyleGroup(), 2);
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.GetSelectStyle() != NULL)
		{
			CurrentStyleItem->setFStyle((GFillStyle*)dlg.GetSelectStyle());
		}
		this->Invalidate();
	}
}

void CValueClassRenderDialog::OnSelchangeCbField()
{
	// TODO: 在此添加控件通知处理程序代码
	ClassField = L"";
	int cursel = m_cbFields.GetCurSel();
	int nField = m_cbFields.GetItemData(cursel);
	if (cursel > -1)
	{
		ClassField = DataLayer->GetFieldDef(nField)->GetName();
		ClassRender->SetClassField(ClassField);
	}
	updateClassIds();
	updateListView();
	CurrentStyleItem = NULL;
	Invalidate();
}


void CValueClassRenderDialog::OnClickListValueClass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_lvValueClass.GetFirstSelectedItemPosition();
	int item = m_lvValueClass.GetNextSelectedItem(pos);
	if (item != -1)
	{
		auto it=ClassIDs[item];
		CurrentStyleItem = ClassRender->GetItemByClass(it.first);
	}
	Invalidate();
	*pResult = 0;
}


void CValueClassRenderDialog::OnDblclkListValueClass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_lvValueClass.GetFirstSelectedItemPosition();
	int item = m_lvValueClass.GetNextSelectedItem(pos);
	if (item != -1)
	{
		auto it = ClassIDs[item];
		CurrentStyleItem = ClassRender->GetItemByClass(it.first);
		if (CurrentStyleItem)
		{
			ClassRender->RemoveByClass(it.first);
			CurrentStyleItem = NULL;
			m_lvValueClass.SetItem(item, 2, LVIF_TEXT,Nz,0,0,0,0,0);
		}
		else
		{
			CurrentStyleItem= ClassRender->AddItem(it.first);
			m_lvValueClass.SetItem(item, 2, LVIF_TEXT, Yz, 0, 0, 0, 0, 0);
		}
	}
	Invalidate();
	*pResult = 0;
}
