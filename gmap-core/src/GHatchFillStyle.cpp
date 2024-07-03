#include "GHatchFillStyle.h"

namespace gmap{


	GHatchFillStyle::GHatchFillStyle(GStyleGroup* group,u32 id,const wstring& name)
		:GFillStyle(group,id,name)
	{
		FillHatchStyle = HatchStyleCross;
		ForeColor = Color::Black;
		BackColor=  Color::Transparent;
	}

	GHatchFillStyle::~GHatchFillStyle(void)
	{

	}

	GFillStyle::EnumFillStyleType GHatchFillStyle::GetFillStyleType()
	{
		return GFillStyle::FTS_HATCH_FILL_STYLE;
	}

	Color GHatchFillStyle::GetForeColor()
	{
		return ForeColor;
	}

	void GHatchFillStyle::SetForeColor(const Color& color)
	{
		ForeColor = color;
		Dirty = true;
	}

	Color GHatchFillStyle::GetBackColor()
	{
		return BackColor;
	}

	void GHatchFillStyle::SetBackColor(const Color& color)
	{
		BackColor = color;
		Dirty = true;
	}
	HatchStyle	GHatchFillStyle::GetHatchStyle()
	{
		return FillHatchStyle;
	}

	void		GHatchFillStyle::SetHatchStyle(HatchStyle hatchStyle)
	{
		FillHatchStyle = hatchStyle;
		Dirty = true;
	}

	void GHatchFillStyle::CheckDirty()
	{
		if(Dirty)
		{
			if(CachedBrush)
			{
				delete CachedBrush;
				CachedBrush = NULL;
			}
			CachedBrush = new HatchBrush(FillHatchStyle,ForeColor,BackColor);
			Dirty = false;
		}
	}

	//!保存到流中
	void GHatchFillStyle::Save(IWriteFile* pStream)
	{
		pStream->Write(FillHatchStyle);
		pStream->Write(ForeColor);
		pStream->Write(BackColor);
	}

	//!从流中读取
	void GHatchFillStyle::Load(IReadFile* pStream)
	{
		pStream->Read(FillHatchStyle);
		pStream->Read(ForeColor);
		pStream->Read(BackColor);
		Dirty = true;
	}

}

