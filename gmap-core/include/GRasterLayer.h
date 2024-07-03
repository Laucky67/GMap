#ifndef __GRASTER_LAYER_H__
#define __GRASTER_LAYER_H__
#include "GCoreBase.h"
#include "GRasterData.h"
#include "GRasterColorConvert.h"
#include "GMapLayer.h"
namespace gmap {

	class GMAPCORE_API GRasterLayer :public GMapLayer
	{
	public:
		GRasterLayer();
		~GRasterLayer();

		virtual GMapLayer::EnumLayerType GetLayerType();
		//!����
		virtual void			Draw(GMapDrawing* pDrawing);
		//����դ��
		void					SetRasterData(GRasterData* ras);
		//��ȡդ��
		GRasterData*			GetRasterData();
		//!
		GRasterColorConvert*	GetColorConvert();
		//!
		void					SetColorConvert(GRasterColorConvert* cvt);
		//!����range
		virtual void			UpdateValueRangeFromData();
		//!
		virtual void			SetValueRange(const GRange& range);
		//!
		virtual void			AttachMap(GMap* map);

		virtual void			UpdateCache();
	private:
		void updateBoundingBox();
		void updateCacheBitmap();
	private:
		Gdiplus::Bitmap*		CacheBitmap;
		GRange					Range;
		GRasterColorConvert*	ColorConvert;
		GRasterData*			RasterData;
		
	};
}
#endif