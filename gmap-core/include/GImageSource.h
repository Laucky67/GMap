#ifndef __G_IMAGE_SOURCE_H__
#define __G_IMAGE_SOURCE_H__

#include "GCoreBase.h"
#include "GGeometry.h"
#include "Gdiplus.h"
#include "GCoordinateSystem.h"

using namespace Gdiplus;

namespace gmap{

	//Ӱ������Դ
	class GMAPCORE_API GImageSource:public GUnknown
	{
	public:
		//!���캯��
		GImageSource(const wstring& name,GCoordinateSystem* pCS,const GRectangle2dd& bbox);

		//!��������
		virtual ~GImageSource(void);

		//!��ȡ����
		const wstring& GetName();

		//!�õ�����ϵ
		GCoordinateSystem* GetCoordinateSystem();

		//!�õ���Χ
		const GRectangle2dd& GetBoundingBox();		

		//!����ͼ������
		virtual void LockRegion(const GRectangle2dd& lockRegion,Bitmap* pLockBitmap,IGeoTransform* pTransformer)=0;


	protected:
		GCoordinateSystem*	CoordinateSystem;
		GRectangle2dd		BoundingBox;
		wstring				Name;
	};

}


#endif
