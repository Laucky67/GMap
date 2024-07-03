// HatchStyleCombobox.cpp : 实现文件
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



// CHatchStyleComboBox 消息处理程序



void CHatchStyleComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
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

	// TODO:  添加您的代码以确定指定项的排序顺序
	// 返回 -1 表示项 1 排在项 2 之前
	// 返回 0 表示项 1 和项 2 顺序相同
	// 返回 1 表示项 1 排在项 2 之后

	return 0;
}

void CHatchStyleComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

	//lpMeasureItemStruct->
	// TODO:  添加您的代码以确定指定项的大小
}
