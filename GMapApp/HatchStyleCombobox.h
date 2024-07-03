#pragma once


// CHatchStyleComboBox

class CHatchStyleComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CHatchStyleComboBox)

public:
	CHatchStyleComboBox();
	virtual ~CHatchStyleComboBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

	HatchBrush *mBrush;

	virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
};


