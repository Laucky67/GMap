#ifndef __G_POINT_ELEMENT_H__
#define __G_POINT_ELEMENT_H__

#include "GGraphicsElement.h"
#include "GPoint.h"
#include "GStyleGroup.h"

namespace gmap{

	//!��ͼԪ
	class GMAPCORE_API GPointElement:public GGraphicsElement
	{
	public:
		GPointElement();

		GPointElement(GPoint* point,GMarkerStyle* style);

		GPointElement(f64 x,f64 y,GMarkerStyle* style);

		~GPointElement(void);
		
		//!�õ�Ԫ������
		virtual EnumGraphicsElmementType GetElementType();

		//!����
		virtual void DrawElement(GMapDrawing* pDrawing);

		//!�õ���Χ��
		virtual GRectangle2dd GetBoundingBox();

		//!���
		GMarkerStyle*	GetMarkerStyle();

		//!���÷��
		void	SetMarkerStyle(GMarkerStyle* style);

		//!�õ���
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