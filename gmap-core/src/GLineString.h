#ifndef __C_LINE_STRING_H__
#define __C_LINE_STRING_H__
#include "ggeometry.h"

namespace gmap{

	class GMAPCORE_API GLineString :public GGeometry
	{
	protected:
		GLineString(u32 count,GCoordinate* pCoordinates);

		GLineString(const vector<GCoordinate>& coordinates);
	public:

		~GLineString(void);

		//!得到包围盒
		virtual	GRectangle2dd GetBoundingBox();

		//!Clone
		virtual GGeometry* Clone();

		//!得到几何类型
		virtual GGeometry::EnumGeometryType GetGeometryType();

		//!得到点的个数
		u32 GetCoordinateCount();

		//!得到坐标
		void GetCoordinateAt(u32 index,GCoordinate* pCoordinate);
		
		const GCoordinate& GetCoordinateAt(u32 index);

		//!得到坐标
		void GetCoordinates(GCoordinate* pCoordinates);
		
		//!得到坐标
		void SetCoordinates(u32 count,GCoordinate* pCoordinates);

		//!设置坐标
		void SetCoordinate(u32 index,f64 x,f64 y);

		//!设置坐标
		void SetCoordinate(u32 index,const GCoordinate& coordinate);

		//!清除坐标
		void Clear();

		//!插入坐标
		void InsertCoordinate(u32 index,const GCoordinate& coordinate);
		
		//!插入坐标
		void InsertCoordinate(u32 index,f64 x,f64 y);

		//!插入坐标
		void AppendCoordinate(const GCoordinate& coordinate);
		
		//!插入坐标
		void AppendCoordinate(f64 x,f64 y);

		//!移除坐标
		void RemoveCoordinate(u32 index);

		//!Save
		virtual void Save(IWriteFile* pStream);

		//!Load
		virtual void Load(IReadFile* pStream);


		//!对几何对象进行坐标变换
		void Transform(IGeoTransform* pTransformer);

	public:
		
		static GLineString* Create();

		static GLineString* Create(u32 count,GCoordinate* pCoordinates);

		static GLineString* Create(const vector<GCoordinate>& pCoordinates);

		static GLineString* Create(f64 x1,f64 y1,f64 x2,f64 y2);

		static GLineString* Create(const GCoordinate& p0,const GCoordinate& p1);

	protected:
		vector<GCoordinate> Coordinates;	
		bool				Dirty;
		GRectangle2dd		BoundingBox;
	};

}


#endif
