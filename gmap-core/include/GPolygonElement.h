#ifndef __G_POLYGON_ELEMENT_H__
#define __G_POLYGON_ELEMENT_H__

#include "GGraphicsElement.h"
#include "GPolygon.h"
#include "GStyleGroup.h"

namespace gmap{

	//!��ͼԪ
	class GMAPCORE_API GPolygonElement:public GGraphicsElement
	{
	public:
		GPolygonElement();
		GPolygonElement(GPolygon* polygon,GLineStyle* lineStyle,GFillStyle* fillStyle=NULL);
		~GPolygonElement(void);
		//!�õ�Ԫ������
		virtual EnumGraphicsElmementType GetElementType();
		//!����
		virtual void DrawElement(GMapDrawing* pDrawing);
		//!�õ���Χ��
		virtual GRectangle2dd GetBoundingBox();
		//!���߷��
		GLineStyle*	GetLineStyle();
		//!���ñ��߷��
		void	SetLineStyle(GLineStyle* style);
		//!�����
		GFillStyle* GetFillStyle();
		//!���������
		void SetFillStyle(GFillStyle* style);
		//!�õ���
		GPolygon*	GetPolygon();
		//!���浽����
		virtual void Save(IWriteFile* pStream);
		//!�����м���
		virtual void Load(IReadFile* pStream);
		//!ѡ�в���
		f64	HitTest(const GCoordinate& point);
		//!�����༭����
		GMapEditObject* CreateEditObject();
	private:
		GPolygon*		Polygon;
		GLineStyle*		LineStyle;
		GFillStyle*		FillStyle;
	};
}


#endif