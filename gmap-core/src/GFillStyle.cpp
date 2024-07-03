
#include "GFillStyle.h"
namespace gmap{
	GFillStyle::GFillStyle(GStyleGroup* group,u32 id,const wstring& name)
		:GStyle(group,id,name)
	{
		CachedBrush=NULL;
		Dirty = true;
	}

	GFillStyle::~GFillStyle(void)
	{
		if(CachedBrush)
		{
			delete CachedBrush;
		}
	}

	void GFillStyle::Draw(Graphics* graphics,PointF* points,UINT count)
	{
		CheckDirty();
		if(CachedBrush)
		{
			graphics->FillPolygon(CachedBrush,points,count);
		}
	}

	void GFillStyle::Draw(Graphics* graphics,GraphicsPath* path)
	{	
		CheckDirty();
		if(CachedBrush)
		{
			graphics->FillPath(CachedBrush,path);
		}
	}

	void GFillStyle::Draw(Graphics* graphics,Region* region)
	{
		CheckDirty();
		if(CachedBrush)
		{
			graphics->FillRegion(CachedBrush,region);
		}
	}
}
