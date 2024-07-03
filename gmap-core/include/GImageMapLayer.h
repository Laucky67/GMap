#ifndef __G_IMAGE_MAP_LAYER_H__
#define __G_IMAGE_MAP_LAYER_H__

#include "GMapLayer.h"
#include "GImageSource.h"
namespace gmap{
	class GMAPCORE_API GImageMapLayer:public GMapLayer
	{
	public:
		//!���캯��
		GImageMapLayer(GImageSource* imageSource);

		//!��������
		~GImageMapLayer(void);

		//!����
		virtual void						Draw(GMapDrawing* pDrawing);

		//!�õ���Χ��
		virtual GRectangle2dd				GetBoundingBox();

		//!�õ�ͼ�������
		virtual GMapLayer::EnumLayerType	GetLayerType();

		//!�õ�ͼ��Դ
		GImageSource*						GetImageSource();

	protected:
		GImageSource*						ImageSource;
	};
}

#endif
