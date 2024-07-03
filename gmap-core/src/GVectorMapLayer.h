#ifndef __G_VECTOR_MAP_LAYER_H__
#define __G_VECTOR_MAP_LAYER_H__


#include "GCoreBase.h"
#include "GUnknown.h"
#include "GMapLayer.h"
namespace gmap{

	class GVectorDataLayer;
	class GVectorFeatureRender;
	class GVectorDataQuery;

	//!ʸ����ͼͼ��
	class GMAPCORE_API GVectorMapLayer:public GMapLayer
	{
	public:
		GVectorMapLayer();
		
		~GVectorMapLayer(void);
		//!����
		virtual void						Draw(GMapDrawing* pDrawing);
		//!�õ���Χ��
		virtual GRectangle2dd				GetBoundingBox();
		//!�õ�ͼ�������
		virtual GMapLayer::EnumLayerType	GetLayerType();
		//!�õ���Ӧ������ͼ��
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