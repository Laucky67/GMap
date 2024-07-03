#ifndef __G_LINE_STYLE_H__
#define __G_LINE_STYLE_H__

#include "GStyle.h"

namespace gmap{

	class GMAPCORE_API GLineStyle:public GStyle
	{
	public:
		enum EnumLineStyleType
		{
			LST_SIMPLE_LINE_STYLE=1,
			LST_COMPOUND_LINE_STYLE=2,
			LST_MARKER_LINE_STYLE=3
		};
	public:
		GLineStyle(GStyleGroup* group,u32 id,const wstring& name);

		virtual ~GLineStyle(void);

		//!Style Type
		virtual GStyle::EnumStyleType			GetStyleType(){return GStyle::ST_LINE_STYLE;}

		//!线的风格类型
		virtual GLineStyle::EnumLineStyleType	GetLineStyleType()=0;

		//!绘制一条线
		virtual void							Draw(Graphics* graphics,PointF* points,UINT count,bool closed)=0;

		//!绘制一条线
		virtual void							Draw(Graphics* graphics,GraphicsPath* path)=0;
	};

}
#endif