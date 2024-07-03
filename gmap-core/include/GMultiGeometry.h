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

		//!�õ����ζ���ĸ���
		u32 GetGeometryCount();

		//!�õ����ζ���
		GGeometry* GetGeometryAt(u32 index);
	
		//!�õ���Χ��
		virtual	GRectangle2dd GetBoundingBox();

		//!Clone
		virtual GGeometry* Clone();

		//!�õ���������
		virtual GGeometry::EnumGeometryType GetGeometryType();

		//!���
		void AppendGeometry(GGeometry* pGeometry,bool newCopy);

		//!�Ƴ�
		void RemoveGeometry(u32 index);

		//!���
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
