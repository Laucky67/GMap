#ifndef __G_VECTOR_MAP_LAYER_H__
#define __G_VECTOR_MAP_LAYER_H__


#include "GCoreBase.h"
#include "GUnknown.h"
#include "GMapLayer.h"
namespace gmap{

	class GVectorDataLayer;
	class GVectorFeatureRender;
	class GVectorDataQuery;

	//!矢量地图图层
	class GMAPCORE_API GVectorMapLayer:public GMapLayer
	{
	public:
		GVectorMapLayer();
		
		~GVectorMapLayer(void);
		//!绘制
		virtual void						Draw(GMapDrawing* pDrawing);
		//!得到包围盒
		virtual GRectangle2dd				GetBoundingBox();
		//!得到图层的类型
		virtual GMapLayer::EnumLayerType	GetLayerType();
		//!得到对应的数据图层
		GVectorDataLayer*					GetDataLayer();
		//!
		void								SetDataLayer(GVectorDataLayer* dataLayer);
		//!
		GVectorFeatureRender*				GetRender();
		//!
		void								ResetRender(GVectorFeatureRender* render=NULL);
	private:
		GVectorDataLayer*					DataLayer;
		GVectorFeatureRender*				FeatureRender;
		GVectorDataQuery*                   CacheQuery;
	};
}


#endif