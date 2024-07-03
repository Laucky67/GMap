#ifndef __G_LINEAR_RING__
#define __G_LINEAR_RING__
#include "GLineString.h"
namespace gmap{

	class GMAPCORE_API GLinearRing:public GLineString
	{	
	protected:
		GLinearRing(u32 count,GCoordinate* pCoordinates);

		GLinearRing(const vector<GCoordinate>& coordinates);
	public:
		virtual ~GLinearRing(void);
		
		//!Clone
		virtual GGeometry* Clone();


		//!得到几何类型
		virtual GGeometry::EnumGeometryType GetGeometryType(){return GGeometry::GT_LINEAR_RING;}

	public:
		static GLinearRing* Create();

		static GLinearRing* Create(u32 count,GCoordinate* pCoordinates);

		static GLinearRing* Create(const vector<GCoordinate>& pCoordinates);

		static GLinearRing* Create(f64 x0,f64 y0,f64 x1,f64 y1,f64 x2,f64 y2);

		static GLinearRing* Create(const GCoordinate& p0,const GCoordinate& p1,const GCoordinate& p2);

		static GLinearRing* Create(const GRectangle2dd& rect);
	};

}

#endif
