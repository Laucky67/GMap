#include "GPoint.h"
namespace gmap{
	//!���캯��
	GPoint::GPoint(f64 x,f64 y)
	{
		X = x;
		Y = y;
	}

	//!��������
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

	//!�õ�����
	void GPoint::GetCoordinate(GCoordinate* pCoordinate)
	{
		if(pCoordinate)
		{
			pCoordinate->X = X;
			pCoordinate->Y = Y;
		}
	}

	//!���õ�
	void GPoint::SetCoordinate(f64 x,f64 y)
	{
		X = x;
		Y = y;
	}

	//!��������
	void GPoint::SetCoordinate(const GCoordinate& coordinate)
	{
		X = coordinate.X;
		Y = coordinate.Y;
	}

	//!�õ���Χ��
	GRectangle2dd GPoint::GetBoundingBox()
	{
		return GRectangle2dd(X,Y,X,Y);
	}

	//!Clone
	GGeometry* GPoint::Clone()
	{
		return GPoint::Create(X,Y);
	}

	//!�õ���������
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

	
	//!�Լ��ζ����������任
	void GPoint::Transform(IGeoTransform* pTransformer)
	{
		if(pTransformer == NULL)return;
		pTransformer->Transform(X,Y,&X,&Y);

	}
}