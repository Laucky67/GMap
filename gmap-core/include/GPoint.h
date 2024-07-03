#ifndef __G_POINT_H__
#define __G_POINT_H__

#include "GGeometry.h"

namespace gmap{

	//!��
	class GMAPCORE_API GPoint:public GGeometry
	{
	protected:	
		//!���캯��
		GPoint(f64 x,f64 y);
	public:
		//!��������
		~GPoint(void);

		//!X
		f64 GetX();

		//!Y
		f64 GetY();

		//!�õ�����
		void GetCoordinate(GCoordinate* pCoordinate);

		//!���õ�
		void SetCoordinate(f64 x,f64 y);

		//!��������
		void SetCoordinate(const GCoordinate& coordinate);

		//!�õ���Χ��
		virtual	GRectangle2dd GetBoundingBox();

		//!Clone
		virtual GGeometry* Clone();

		//!�õ���������
		virtual GGeometry::EnumGeometryType GetGeometryType();

		//!Save
		virtual void Save(IWriteFile* pStream);

		//!Load
		virtual void Load(IReadFile* pStream);
		
		//!ƫ��
		virtual void Move(const GCoordinate& offsetXY)
		{
			X+=offsetXY.X;
			Y+=offsetXY.Y;
		}
		
		//!�Լ��ζ����������任
		virtual void Transform(IGeoTransform* pTransformer);

	public:
		static GPoint* Create(f64 x=0,f64 y=0);

		static GPoint* Create(const GCoordinate& coordinate);

	protected:
		f64 X;
		f64 Y;
	};

}

#endif