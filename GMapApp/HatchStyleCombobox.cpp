// HatchStyleCombobox.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "GMapApp.h"
#include "HatchStyleCombobox.h"


// CHatchStyleComboBox

IMPLEMENT_DYNAMIC(CHatchStyleComboBox, CComboBox)

CHatchStyleComboBox::CHatchStyleComboBox()
{
}

CHatchStyleComboBox::~CHatchStyleComboBox()
{
}


BEGIN_MESSAGE_MAP(CHatchStyleComboBox, CComboBox)
END_MESSAGE_MAP()



// CHatchStyleComboBox ��Ϣ�������



void CHatchStyleComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  ������Ĵ����Ի���ָ����
	Graphics graphics(lpDrawItemStruct->hDC);
	HatchStyle style = (HatchStyle)lpDrawItemStruct->itemID;
	HatchBrush brush(style,Color::Black,Color::White);

	CRect rect = lpDrawItemStruct->rcItem;
	Rect rc0(rect.left,rect.top,rect.Width(),rect.Height());
	SolidBrush bkBrush(Color::White);
	graphics.FillRectangle(&bkBrush,rc0);

	Rect rc(rect.left+1,rect.top+1,rect.Width()-2,rect.Height()-2);

	graphics.FillRectangle(&brush,rc);

	if((lpDrawItemStruct->itemState&ODS_SELECTED))
	{
		Pen pen(Color::Red);	
		graphics.DrawRectangle(&pen,rc);
	}
	//if((lpDrawItemStruct->itemState&ODS_FOCUS))
	//{	
	//	Pen pen(Color::Red);	
	//	graphics.DrawRectangle(&pen,rc);
	//}

}

int CHatchStyleComboBox::CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/)
{

	// TODO:  ������Ĵ�����ȷ��ָ���������˳��
	// ���� -1 ��ʾ�� 1 ������ 2 ֮ǰ
	// ���� 0 ��ʾ�� 1 ���� 2 ˳����ͬ
	// ���� 1 ��ʾ�� 1 ������ 2 ֮��

	return 0;
}

void CHatchStyleComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

	//lpMeasureItemStruct->
	// TODO:  ������Ĵ�����ȷ��ָ����Ĵ�С
}
