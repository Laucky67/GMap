#ifndef __G_GRAPHICS_LAYER_H__
#define __G_GRAPHICS_LAYER_H__
#include "GMapLayer.h"
#include "GStyleGroup.h"
#include "GPointElement.h"
#include "GLineStringElement.h"
#include "GPolygonElement.h"
#include "GMapEditObject.h"

namespace gmap{

	class GGraphicsElement;

	//!��ͼͼ��
	class GMAPCORE_API GGraphicsLayer:public GMapLayer
	{
	public:
		GGraphicsLayer(const wstring& name,const GRectangle2dd& defaultBox = GRectangle2dd(-1000,-1000,1000,1000));

		~GGraphicsLayer(void);

		//!ͼԪ����
		u32 GetElementCount();

		//!�õ�ͼԪ
		GGraphicsElement* GetElementAt(u32 index);

		//!�Ƴ�ͼԪ
		void RemoveElement(u32 index);

		//!���ͼԪ
		void AppendElement(GGraphicsElement* pElement);

		//!���ͼԪ
		void ClearElements();

		//!�õ���Χ��
		virtual GRectangle2dd GetBoundingBox();
		
		virtual GMapLayer::EnumLayerType GetLayerType(){return GMapLayer::LT_GRAPHICS;}

		//!����
		void Draw(GMapDrawing* pDrawing);

		//!����Ĭ�ϵĵ�ͼԪ
		void SetDefaultMarkerStyle(GMarkerStyle* style);
		
		//!����Ĭ�ϵĵ�ͼԪ
		GMarkerStyle* GetDefaultMarkerStyle();

		//!����Ĭ�ϵĵ�ͼԪ
		void SetDefaultLineStyle(GLineStyle* style);
		
		//!����Ĭ�ϵĵ�ͼԪ
		GLineStyle* GetDefaultLineStyle();

		//!����Ĭ�ϵĵ�ͼԪ
		void SetDefaultFillStyle(GFillStyle* style);
		
		//!����Ĭ�ϵĵ�ͼԪ
		GFillStyle* GetDefaultFillStyle();

		//!����
		virtual void Save(IWriteFile* pStream);
		
		//!����
		virtual void Load(IReadFile* pStream);
		
		//!���ѡ�в���
		GMapEditObject* HitTest(const GCoordinate& hitPoint,f64 hitTest);
	private:
		vector<GGraphicsElement*> Elements;
		GMarkerStyle*	DefaultMarkerStyle;
		GLineStyle*		DefaultLineStyle;
		GFillStyle*		DefaultFillStyle;
		u32				NextId;
	};

}


#endif
