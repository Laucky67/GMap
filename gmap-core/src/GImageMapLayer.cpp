#include "GImageMapLayer.h"
#include "GCoordinateSystem.h"
#include "GMap.h"
#include "GMapDrawing.h"

namespace gmap{

	//20037508.342789244
	//20037508.342789244
	//!���캯��
	GImageMapLayer::GImageMapLayer(GImageSource* imageSource)
	{
		ImageSource=imageSource;
		if(ImageSource)
		{
			Name = ImageSource->GetName();
			ImageSource->AddRef();
		}
	}

	//!��������
	GImageMapLayer::~GImageMapLayer(void)
	{
		if(ImageSource)
		{
			ImageSource->Release();
		}
	}

	//!����
	void GImageMapLayer::Draw(GMapDrawing* pDrawing)
	{
		Bitmap* bitmap = new Bitmap(pDrawing->GetViewPort().Width,
									pDrawing->GetViewPort().Height,
									PixelFormat32bppARGB);
		
		IGeoTransform* pTrans = 
							GCoordinateSystem::CreateTransform(Map->GetCoordinateSystem()
																,ImageSource->GetCoordinateSystem());


		ImageSource->LockRegion(pDrawing->GetViewBounds(),bitmap,pTrans);

		pDrawing->GetGraphics()->DrawImage(bitmap,0,0);

		if(pTrans)
		{
			pTrans->Release();
		}
	
		delete bitmap;
	}

	//!�õ���Χ��
	GRectangle2dd GImageMapLayer::GetBoundingBox()
	{
		IGeoTransform* pTrans = 
							GCoordinateSystem::CreateTransform(ImageSource->GetCoordinateSystem()
																,Map->GetCoordinateSystem());
		
		GRectangle2dd box = ImageSource->GetBoundingBox();

		if(pTrans)
		{
			double x0,y0,x1,y1;
			
			pTrans->Transform(box.MinEdge.X,box.MinEdge.Y,&x0,&y0);
			pTrans->Transform(box.MaxEdge.X,box.MaxEdge.Y,&x1,&y1);

			GRectangle2dd box2(x0,y0,x1,y1);
			box2.Repair();

			pTrans->Release();

			return box2;
		}
		else
		{
			return box;
		}

	}

	//!�õ�ͼ�������
	GMapLayer::EnumLayerType GImageMapLayer::GetLayerType()
	{
		return GMapLayer::LT_IMAGE;
	}

	//!�õ�ͼ��Դ
	GImageSource* GImageMapLayer::GetImageSource()
	{
		return ImageSource;
	}

}
