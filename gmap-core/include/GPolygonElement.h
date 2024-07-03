#ifndef __G_POLYGON_ELEMENT_H__
#define __G_POLYGON_ELEMENT_H__

#include "GGraphicsElement.h"
#include "GPolygon.h"
#include "GStyleGroup.h"

namespace gmap{

	//!点图元
	class GMAPCORE_API GPolygonElement:public GGraphicsElement
	{
	public:
		GPolygonElement();
		GPolygonElement(GPolygon* polygon,GLineStyle* lineStyle,GFillStyle* fillStyle=NULL);
		~GPolygonElement(void);
		//!得到元素类型
		virtual EnumGraphicsElmementType GetElementType();
		//!绘制
		virtual void DrawElement(GMapDrawing* pDrawing);
		//!得到包围盒
		virtual GRectangle2dd GetBoundingBox();
		//!边线风格
		GLineStyle*	GetLineStyle();
		//!设置边线风格
		void	SetLineStyle(GLineStyle* style);
		//!填充风格
		GFillStyle* GetFillStyle();
		//!设置填充风格
		void SetFillStyle(GFillStyle* style);
		//!得到点
		GPolygon*	GetPolygon();
		//!保存到流中
		virtual void Save(IWriteFile* pStream);
		//!从流中加载
		virtual void Load(IReadFile* pStream);
		//!选中测试
		f64	HitTest(const GCoordinate& point);
		//!创建编辑对象
		GMapEditObject* CreateEditObject();
	private:
		GPolygon*		Polygon;
		GLineStyle*		LineStyle;
		GFillStyle*		FillStyle;
	};
}


#endif