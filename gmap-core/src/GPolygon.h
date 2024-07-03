#ifndef __G_POLYGON_H__
#define __G_POLYGON_H__
#include "GLinearRing.h"

namespace gmap{

	class GMAPCORE_API GPolygon:public GGeometry
	{
	protected:
		GPolygon();

		GPolygon(GLinearRing* externalRing);

		GPolygon(GLinearRing* externalRing,const vector<GLinearRing*>& internalRings);
	
	public:		
		~GPolygon(void);

		 //!得到外环
		 GLinearRing* GetExternalRing();

		 //!得到内环的个数
		 u32		  GetInternalRingCount();

		 //!得到内环
		 GLinearRing* GetInternalRingAt(u32 index);

		 //!添加内环
		 void AddInternalRing(GLinearRing* ring,bool newCopy);

		 //!移除内环
		 void RemoveInternalRing(u32 index);

		 //!清除内环
		 void ClearAllInternalRings();

		 //!得到包围盒
		 virtual GRectangle2dd GetBoundingBox();

		 //!Clone
		 virtual GGeometry* Clone();

		 //!得到几何类型
		 virtual GGeometry::EnumGeometryType GetGeometryType();
		 
		 //!Save
		 virtual void Save(IWriteFile* pStream);

		 //!Load
		 virtual void Load(IReadFile* pStream);

		void  Transform(IGeoTransform* pTransformer);

	public:
		static GPolygon* Create();

		static GPolygon* Create(GLinearRing* externalRing);

		static GPolygon* Create(GLinearRing* externalRing,const vector<GLinearRing*>& internalRing);

		static GPolygon* Create(u32 count,GCoordinate* pCoordinates);

		static GPolygon* Create(const vector<GCoordinate>& pCoordinates);

		static GPolygon* Create(f64 x0,f64 y0,f64 x1,f64 y1,f64 x2,f64 y2);

		static GPolygon* Create(const GCoordinate& p0,const GCoordinate& p1,const GCoordinate& p2);

		static GPolygon* Create(const GRectangle2dd& rect);
	
	public:
		GLinearRing*		ExternalRing;
		vector<GLinearRing*> InternalRings;
	};

}

#endif