#ifndef __G_IMAGE_MAP_LAYER_H__
#define __G_IMAGE_MAP_LAYER_H__

#include "GMapLayer.h"
#include "GImageSource.h"
namespace gmap{
	class GMAPCORE_API GImageMapLayer:public GMapLayer
	{
	public:
		//!构造函数
		GImageMapLayer(GImageSource* imageSource);

		//!析构函数
		~GImageMapLayer(void);

		//!绘制
		virtual void						Draw(GMapDrawing* pDrawing);

		//!得到包围盒
		virtual GRectangle2dd				GetBoundingBox();

		//!得到图层的类型
		virtual GMapLayer::EnumLayerType	GetLayerType();

		//!得到图形源
		GImageSource*						GetImageSource();

	protected:
		GImageSource*						ImageSource;
	};
}

#endif
