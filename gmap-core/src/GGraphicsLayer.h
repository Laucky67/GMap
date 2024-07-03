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

	//!绘图图层
	class GMAPCORE_API GGraphicsLayer:public GMapLayer
	{
	public:
		GGraphicsLayer(const wstring& name,const GRectangle2dd& defaultBox = GRectangle2dd(-1000,-1000,1000,1000));

		~GGraphicsLayer(void);

		//!图元个数
		u32 GetElementCount();

		//!得到图元
		GGraphicsElement* GetElementAt(u32 index);

		//!移除图元
		void RemoveElement(u32 index);

		//!添加图元
		void AppendElement(GGraphicsElement* pElement);

		//!清除图元
		void ClearElements();

		//!得到包围盒
		virtual GRectangle2dd GetBoundingBox();
		
		virtual GMapLayer::EnumLayerType GetLayerType(){return GMapLayer::LT_GRAPHICS;}

		//!绘制
		void Draw(GMapDrawing* pDrawing);

		//!设置默认的点图元
		void SetDefaultMarkerStyle(GMarkerStyle* style);
		
		//!设置默认的点图元
		GMarkerStyle* GetDefaultMarkerStyle();

		//!设置默认的点图元
		void SetDefaultLineStyle(GLineStyle* style);
		
		//!设置默认的点图元
		GLineStyle* GetDefaultLineStyle();

		//!设置默认的点图元
		void SetDefaultFillStyle(GFillStyle* style);
		
		//!设置默认的点图元
		GFillStyle* GetDefaultFillStyle();

		//!保存
		virtual void Save(IWriteFile* pStream);
		
		//!加载
		virtual void Load(IReadFile* pStream);
		
		//!点击选中测试
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
