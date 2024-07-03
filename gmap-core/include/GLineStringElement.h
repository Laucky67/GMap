#ifndef __G_LINE_ELEMENT_H__
#define __G_LINE_ELEMENT_H__

#include "GGraphicsElement.h"
#include "GLineString.h"
#include "GStyleGroup.h"

namespace gmap{

	//!点图元
	class GMAPCORE_API GLineStringElement:public GGraphicsElement
	{
	public:
		GLineStringElement();

		GLineStringElement(GLineString* lineString,GLineStyle* style);


		~GLineStringElement(void);
		
		//!得到元素类型
		virtual EnumGraphicsElmementType GetElementType();

		//!绘制
		virtual void DrawElement(GMapDrawing* pDrawing);

		//!得到包围盒
		virtual GRectangle2dd GetBoundingBox();

		//!风格
		GLineStyle*	GetLineStyle();

		//!设置风格
		void	SetLineStyle(GLineStyle* style);

		//!得到点
		GLineString*	GetLineString();

		virtual void Save(IWriteFile* pStream);

		virtual void Load(IReadFile* pStream);
		
		f64	HitTest(const GCoordinate& point);

		GMapEditObject* CreateEditObject();
	private:
		GLineString*	LineString;
		GLineStyle*		LineStyle;
	};

}


#endif