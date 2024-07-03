#ifndef __G_FILE_STYLE_H__
#define __G_FILE_STYLE_H__

#include "GStyle.h"

namespace gmap{

	class GMAPCORE_API GFillStyle:public GStyle
	{
	public:
		enum EnumFillStyleType
		{
			FTS_COLOR_FILL_STYLE=1,
			FTS_HATCH_FILL_STYLE,
			FTS_TEXTURE_FILL_STYLE
		};
	public:
		GFillStyle(GStyleGroup* group,u32 id,const wstring& name);

		~GFillStyle(void);

		//!Style Type
		virtual GStyle::EnumStyleType GetStyleType(){return ST_FILL_STYLE;}

		virtual GFillStyle::EnumFillStyleType GetFillStyleType()=0;

		virtual void Draw(Graphics* graphics,PointF* points,UINT count);

		virtual void Draw(Graphics* graphics,GraphicsPath* path);

		virtual void Draw(Graphics* graphics,Region* region);
	protected:
		virtual void CheckDirty()=0;
	protected:
		Brush* CachedBrush;
		bool   Dirty;
	};
}

#endif