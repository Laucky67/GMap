#ifndef __G_LINE_ELEMENT_H__
#define __G_LINE_ELEMENT_H__

#include "GGraphicsElement.h"
#include "GLineString.h"
#include "GStyleGroup.h"

namespace gmap{

	//!��ͼԪ
	class GMAPCORE_API GLineStringElement:public GGraphicsElement
	{
	public:
		GLineStringElement();

		GLineStringElement(GLineString* lineString,GLineStyle* style);


		~GLineStringElement(void);
		
		//!�õ�Ԫ������
		virtual EnumGraphicsElmementType GetElementType();

		//!����
		virtual void DrawElement(GMapDrawing* pDrawing);

		//!�õ���Χ��
		virtual GRectangle2dd GetBoundingBox();

		//!���
		GLineStyle*	GetLineStyle();

		//!���÷��
		void	SetLineStyle(GLineStyle* style);

		//!�õ���
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