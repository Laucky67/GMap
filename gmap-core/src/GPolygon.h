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

		 //!�õ��⻷
		 GLinearRing* GetExternalRing();

		 //!�õ��ڻ��ĸ���
		 u32		  GetInternalRingCount();

		 //!�õ��ڻ�
		 GLinearRing* GetInternalRingAt(u32 index);

		 //!����ڻ�
		 void AddInternalRing(GLinearRing* ring,bool newCopy);

		 //!�Ƴ��ڻ�
		 void RemoveInternalRing(u32 index);

		 //!����ڻ�
		 void ClearAllInternalRings();

		 //!�õ���Χ��
		 virtual GRectangle2dd GetBoundingBox();

		 //!Clone
		 virtual GGeometry* Clone();

		 //!�õ���������
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