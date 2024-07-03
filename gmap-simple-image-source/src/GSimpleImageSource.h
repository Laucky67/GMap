#ifndef __G_SIMPLE_IMAGE_SOURCE_H__
#define __G_SIMPLE_IMAGE_SOURCE_H__
#include "GImageSource.h"

#ifdef GMAPSIMPLEIMAGESOURCE_EXPORTS
	#define GMAPSIMPLEIMAGESOURCE_API	__declspec(dllexport)
#else
	#define GMAPSIMPLEIMAGESOURCE_API	__declspec(dllimport)
#endif

namespace gmap{

	class GMAPSIMPLEIMAGESOURCE_API GSimpleImageSource:public GImageSource
	{
	public:
		//!构造函数
		GSimpleImageSource(const wstring& imgSrc,GCoordinateSystem* pCS,const GRectangle2dd& bbox);
			
		//!构造函数
		GSimpleImageSource(const wstring& name,Bitmap* pBitmap,GCoordinateSystem* pCS,const GRectangle2dd& bbox);
		
		
		~GSimpleImageSource(void);
		
		//!锁定图形区域
		virtual void LockRegion(const GRectangle2dd& lockRegion,Bitmap* pLockBitmap,IGeoTransform* pTransformer);

	public:
		static GImageSource* Create(const wstring& imgSrc, GCoordinateSystem* pCS, const GRectangle2dd& bbox);
		static GImageSource* CreateFromConfigure(const wstring& fileName);
	protected:
		Bitmap* Bitmap;
	};
}

#endif
