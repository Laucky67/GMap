#include "GColorFillStyle.h"
namespace gmap{

	GColorFillStyle::GColorFillStyle(GStyleGroup* group,u32 id,const wstring& name)
		:GFillStyle(group,id,name)
	{
		FillColor = Color(255,rand()%255,rand()%255,rand()%255);
	}

	GColorFillStyle::~GColorFillStyle(void)
	{

	}


	GFillStyle::EnumFillStyleType GColorFillStyle::GetFillStyleType()
	{
		return GFillStyle::FTS_COLOR_FILL_STYLE;
	}

	Color GColorFillStyle::GetFillColor()
	{
		return FillColor;
	}

	void GColorFillStyle::SetFillColor(const Color& color){
		FillColor = color;
		Dirty = true;
	}

	void GColorFillStyle::CheckDirty()
	{
		if(Dirty)
		{
			if(CachedBrush)
			{
				delete CachedBrush;
				CachedBrush = NULL;
			}
			CachedBrush = new SolidBrush(FillColor);

			Dirty = false;
		}
	}

	//!保存到流中
	void GColorFillStyle::Save(IWriteFile* pStream)
	{
		pStream->Write(FillColor);
	}

	//!从流中读取
	void GColorFillStyle::Load(IReadFile* pStream)
	{
		pStream->Read(FillColor);
		Dirty=true;
	}
}
