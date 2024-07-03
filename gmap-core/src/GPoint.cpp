#include "GPoint.h"
namespace gmap{
	//!构造函数
	GPoint::GPoint(f64 x,f64 y)
	{
		X = x;
		Y = y;
	}

	//!析构函数
	GPoint::~GPoint(void)
	{


	}

	//!X
	f64 GPoint::GetX()
	{
		return X;
	}

	//!Y
	f64 GPoint::GetY()
	{
		return Y;
	}

	//!得到坐标
	void GPoint::GetCoordinate(GCoordinate* pCoordinate)
	{
		if(pCoordinate)
		{
			pCoordinate->X = X;
			pCoordinate->Y = Y;
		}
	}

	//!设置点
	void GPoint::SetCoordinate(f64 x,f64 y)
	{
		X = x;
		Y = y;
	}

	//!设置坐标
	void GPoint::SetCoordinate(const GCoordinate& coordinate)
	{
		X = coordinate.X;
		Y = coordinate.Y;
	}

	//!得到包围盒
	GRectangle2dd GPoint::GetBoundingBox()
	{
		return GRectangle2dd(X,Y,X,Y);
	}

	//!Clone
	GGeometry* GPoint::Clone()
	{
		return GPoint::Create(X,Y);
	}

	//!得到几何类型
	GGeometry::EnumGeometryType GPoint::GetGeometryType()
	{
		return GGeometry::GT_POINT;
	}

	//---------------------------------------------------
	//!Save
	void GPoint::Save(IWriteFile* pStream)
	{
		pStream->Write(&X,sizeof(X));
		pStream->Write(&Y,sizeof(X));
	}

	//!Load
	void GPoint::Load(IReadFile* pStream)
	{
		pStream->Read(&X,sizeof(X),NULL);
		pStream->Read(&Y,sizeof(Y),NULL);
	}
	//---------------------------------------------------
	GPoint* GPoint::Create(f64 x,f64 y)
	{
		return new GPoint(x,y);
	}

	GPoint* GPoint::Create(const GCoordinate& coordinate)
	{
		return GPoint::Create(coordinate.X,coordinate.Y);
	}

	
	//!对几何对象进行坐标变换
	void GPoint::Transform(IGeoTransform* pTransformer)
	{
		if(pTransformer == NULL)return;
		pTransformer->Transform(X,Y,&X,&Y);

	}
}