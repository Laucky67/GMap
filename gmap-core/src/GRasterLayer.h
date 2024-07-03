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
		//!绘制
		virtual void			Draw(GMapDrawing* pDrawing);
		//设置栅格
		void					SetRasterData(GRasterData* ras);
		//获取栅格
		GRasterData*			GetRasterData();
		//!
		GRasterColorConvert*	GetColorConvert();
		//!
		void					SetColorConvert(GRasterColorConvert* cvt);
		//!更新range
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