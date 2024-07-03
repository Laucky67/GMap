#ifndef __G_POINT_ELEMENT_H__
#define __G_POINT_ELEMENT_H__

#include "GGraphicsElement.h"
#include "GPoint.h"
#include "GStyleGroup.h"

namespace gmap{

	//!点图元
	class GMAPCORE_API GPointElement:public GGraphicsElement
	{
	public:
		GPointElement();

		GPointElement(GPoint* point,GMarkerStyle* style);

		GPointElement(f64 x,f64 y,GMarkerStyle* style);

		~GPointElement(void);
		
		//!得到元素类型
		virtual EnumGraphicsElmementType GetElementType();

		//!绘制
		virtual void DrawElement(GMapDrawing* pDrawing);

		//!得到包围盒
		virtual GRectangle2dd GetBoundingBox();

		//!风格
		GMarkerStyle*	GetMarkerStyle();

		//!设置风格
		void	SetMarkerStyle(GMarkerStyle* style);

		//!得到点
		GPoint*	GetPoint();

		virtual void Save(IWriteFile* pStream);

		virtual void Load(IReadFile* pStream);

		f64	HitTest(const GCoordinate& point);

		GMapEditObject* CreateEditObject();

	private:
		GPoint*			Point;
		GMarkerStyle*	MarkerStyle;
	};
}


#endif