#ifndef __G_MARKER_STYLE_H__
#define __G_MARKER_STYLE_H__

#include "GStyle.h"

namespace gmap{

	class GStyleGroup;

	//!µã·ûºÅ
	class GMAPCORE_API GMarkerStyle:public GStyle
	{
	public:
		enum EnumMarkerStyleType
		{
			MST_IMAGE_STYLE=1,
			MST_TRUE_TYPE_STYLE=2
		};
	public:
		GMarkerStyle(GStyleGroup* group,u32 id,const wstring& name);

		~GMarkerStyle(void);

		//!Style Type
		virtual GStyle::EnumStyleType GetStyleType(){return GStyle::ST_MARKER_STYLE;}

		virtual GMarkerStyle::EnumMarkerStyleType	GetMarkerStyleType()=0;

		virtual void Draw(Graphics* graphics,const PointF& point)=0;
	};
}


#endif