#include "GLinearRing.h"
namespace gmap{

	GLinearRing::GLinearRing(u32 count,GCoordinate* pCoordinates):
		GLineString(count,pCoordinates)
	{
	
	}

	GLinearRing::GLinearRing(const vector<GCoordinate>& coordinates):
		GLineString(coordinates)
	{
	
	}

	GLinearRing::~GLinearRing(void)
	{
		
	}

	GGeometry* GLinearRing::Clone()
	{
		return GLinearRing::Create(Coordinates);
	}

	//---------------------------------------------------
	
	GLinearRing* GLinearRing::Create()
	{	
		GLinearRing* linearRing = new 	GLinearRing(0,	NULL);
		return linearRing;
	}

	//!得到几何类型
	GLinearRing* GLinearRing::Create(u32 count,GCoordinate* pCoordinates)
	{
		GLinearRing* linearRing = new 	GLinearRing(count,	pCoordinates);
		return linearRing;
	}

	GLinearRing* GLinearRing::Create(const vector<GCoordinate>& pCoordinates)
	{
		GLinearRing* linearRing = new 	GLinearRing(pCoordinates);
		return linearRing;
	}

	GLinearRing* GLinearRing::Create(f64 x0,f64 y0,f64 x1,f64 y1,f64 x2,f64 y2)
	{
		return Create(GCoordinate(x0,y0),GCoordinate(x1,y1),GCoordinate(x2,y2));
	}

	GLinearRing* GLinearRing::Create(const GCoordinate& p0,const GCoordinate& p1,const GCoordinate& p2)
	{
		vector<GCoordinate> coordinates;

		coordinates.push_back(p0);
		coordinates.push_back(p1);
		coordinates.push_back(p2);

		GLinearRing* linearRing = new 	GLinearRing(coordinates);
		return linearRing;
	}

	GLinearRing* GLinearRing::Create(const GRectangle2dd& rect)
	{
		vector<GCoordinate> coordinates;

		coordinates.push_back(rect.MinEdge);
		coordinates.push_back(GCoordinate(rect.MaxEdge.X,rect.MinEdge.Y));
		coordinates.push_back(rect.MaxEdge);
		coordinates.push_back(GCoordinate(rect.MinEdge.X,rect.MaxEdge.Y));

		GLinearRing* linearRing = new 	GLinearRing(coordinates);
		return linearRing;
	}

}