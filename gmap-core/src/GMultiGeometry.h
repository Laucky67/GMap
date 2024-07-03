#ifndef __G_MULTI_GEOMETRY_H__
#define __G_MULTI_GEOMETRY_H__

#include "GGeometry.h"
namespace gmap{

	class GMAPCORE_API GMultiGeometry:public GGeometry
	{
	protected:
		GMultiGeometry(GGeometry::EnumGeometryType primitiveType);
	public:
		~GMultiGeometry(void);

		//!得到几何对象的个数
		u32 GetGeometryCount();

		//!得到几何对象
		GGeometry* GetGeometryAt(u32 index);
	
		//!得到包围盒
		virtual	GRectangle2dd GetBoundingBox();

		//!Clone
		virtual GGeometry* Clone();

		//!得到几何类型
		virtual GGeometry::EnumGeometryType GetGeometryType();

		//!添加
		void AppendGeometry(GGeometry* pGeometry,bool newCopy);

		//!移除
		void RemoveGeometry(u32 index);

		//!清除
		void Clear();
		
		//!Save
		virtual void Save(IWriteFile* pStream);

		//!Load
		virtual void Load(IReadFile* pStream);


		void  Transform(IGeoTransform* pTransformer);

	public:
		static GMultiGeometry* Create(const vector<GGeometry*>& geometries,GGeometry::EnumGeometryType	primitiveType=GGeometry::GT_UNKNOWN);

		static GMultiGeometry* Create(GGeometry* pGeometry,GGeometry::EnumGeometryType	primitiveType=GGeometry::GT_UNKNOWN);

		static GMultiGeometry* Create(GGeometry::EnumGeometryType	primitiveType=GGeometry::GT_UNKNOWN);
	protected:
		vector<GGeometry*> Geometries;
		GGeometry::EnumGeometryType PrimitiveType;
	};

}
#endif
