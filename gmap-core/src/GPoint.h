#ifndef __G_POINT_H__
#define __G_POINT_H__

#include "GGeometry.h"

namespace gmap{

	//!点
	class GMAPCORE_API GPoint:public GGeometry
	{
	protected:	
		//!构造函数
		GPoint(f64 x,f64 y);
	public:
		//!析构函数
		~GPoint(void);

		//!X
		f64 GetX();

		//!Y
		f64 GetY();

		//!得到坐标
		void GetCoordinate(GCoordinate* pCoordinate);

		//!设置点
		void SetCoordinate(f64 x,f64 y);

		//!设置坐标
		void SetCoordinate(const GCoordinate& coordinate);

		//!得到包围盒
		virtual	GRectangle2dd GetBoundingBox();

		//!Clone
		virtual GGeometry* Clone();

		//!得到几何类型
		virtual GGeometry::EnumGeometryType GetGeometryType();

		//!Save
		virtual void Save(IWriteFile* pStream);

		//!Load
		virtual void Load(IReadFile* pStream);
		
		//!偏移
		virtual void Move(const GCoordinate& offsetXY)
		{
			X+=offsetXY.X;
			Y+=offsetXY.Y;
		}
		
		//!对几何对象进行坐标变换
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