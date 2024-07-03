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

		//!�õ���Χ��
		virtual	GRectangle2dd GetBoundingBox();

		//!Clone
		virtual GGeometry* Clone();

		//!�õ���������
		virtual GGeometry::EnumGeometryType GetGeometryType();

		//!�õ���ĸ���
		u32 GetCoordinateCount();

		//!�õ�����
		void GetCoordinateAt(u32 index,GCoordinate* pCoordinate);
		
		const GCoordinate& GetCoordinateAt(u32 index);

		//!�õ�����
		void GetCoordinates(GCoordinate* pCoordinates);
		
		//!�õ�����
		void SetCoordinates(u32 count,GCoordinate* pCoordinates);

		//!��������
		void SetCoordinate(u32 index,f64 x,f64 y);

		//!��������
		void SetCoordinate(u32 index,const GCoordinate& coordinate);

		//!�������
		void Clear();

		//!��������
		void InsertCoordinate(u32 index,const GCoordinate& coordinate);
		
		//!��������
		void InsertCoordinate(u32 index,f64 x,f64 y);

		//!��������
		void AppendCoordinate(const GCoordinate& coordinate);
		
		//!��������
		void AppendCoordinate(f64 x,f64 y);

		//!�Ƴ�����
		void RemoveCoordinate(u32 index);

		//!Save
		virtual void Save(IWriteFile* pStream);

		//!Load
		virtual void Load(IReadFile* pStream);


		//!�Լ��ζ����������任
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
