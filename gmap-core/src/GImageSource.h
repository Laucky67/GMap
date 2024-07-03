#ifndef __G_IMAGE_SOURCE_H__
#define __G_IMAGE_SOURCE_H__

#include "GCoreBase.h"
#include "GGeometry.h"
#include "Gdiplus.h"
#include "GCoordinateSystem.h"

using namespace Gdiplus;

namespace gmap{

	//影像数据源
	class GMAPCORE_API GImageSource:public GUnknown
	{
	public:
		//!构造函数
		GImageSource(const wstring& name,GCoordinateSystem* pCS,const GRectangle2dd& bbox);

		//!析构函数
		virtual ~GImageSource(void);

		//!获取名字
		const wstring& GetName();

		//!得到坐标系
		GCoordinateSystem* GetCoordinateSystem();

		//!得到范围
		const GRectangle2dd& GetBoundingBox();		

		//!锁定图形区域
		virtual void LockRegion(const GRectangle2dd& lockRegion,Bitmap* pLockBitmap,IGeoTransform* pTransformer)=0;


	protected:
		GCoordinateSystem*	CoordinateSystem;
		GRectangle2dd		BoundingBox;
		wstring				Name;
	};

}


#endif
