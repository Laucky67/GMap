#ifndef __G_HIT_TEST_HELPER_H__
#define __G_HIT_TEST_HELPER_H__

#include "GGeometry.h"
#include "GPoint.h"
#include "GLineString.h"
#include "GPolygon.h"

namespace gmap
{
	class GHitTestHelper
	{
	public:

		static inline f64 GetDistance(GPoint* point,const GCoordinate& hitPoint)
		{
			GCoordinate coord(point->GetX(),point->GetY());

			return coord.GetDistanceFrom(hitPoint);
		}

		static inline f64 GetNearestPoint(GLineString* lineString,const GCoordinate& hitPoint,u32* pIndex,f64 tolerence=1e10)
		{
			f64 D = tolerence;
			*pIndex=-1;
			u32 count = lineString->GetCoordinateCount();
			for(int i=0;i<count;i++)
			{
				const GCoordinate& coord = lineString->GetCoordinateAt(i);
				f64 d = coord.GetDistanceFrom(hitPoint);
				if(d<D)
				{
					D = d;
					*pIndex = i;
				}
			}
			return D;
		}

		static inline f64 GetNearestPoint(GPolygon* polygon,const GCoordinate& hitPoint,u32* pIndex,f64 tolerence=1e10)
		{	
			GLinearRing* lr= polygon->GetExternalRing();
			u32 hitIndex;
			f64 d = GetNearestPoint(lr,hitPoint,&hitIndex,tolerence);
			u32 preCount = lr->GetCoordinateCount();

			for(int i=0;i<polygon->GetInternalRingCount();i++)
			{					
				GLinearRing* lr= polygon->GetInternalRingAt(i);
				u32 hitIndex2;
				f64 d2 = GetNearestPoint(lr,hitPoint,&hitIndex2,tolerence);
				if(d2<d)
				{
					d=d2;
					hitIndex = hitIndex2+preCount;
				}
				preCount+= lr->GetCoordinateCount();
			}

			*pIndex = hitIndex;
			return d;
		}
		

		static inline f64 GetNearestEdge(GLineString* lineString,const GCoordinate& hitPoint,u32* pIndex,bool closed,f64 tolerence=1e10)
		{
			f64 D = tolerence;
			*pIndex=-1;
			u32 count = lineString->GetCoordinateCount();
			int count2 = count+(closed?0:-1);
			for(int i=0;i<count2;i++)
			{
				GCoordinate p1 = lineString->GetCoordinateAt((i+1)%count);
				GCoordinate p0=lineString->GetCoordinateAt(i);
				
				GCoordinate c = hitPoint - p0;
				GCoordinate v = p1 -p0;
				f64 d = v.GetLength();
				v /= d;
				f64 t = v.DotProduct(c);

				if (t < 0.0||t > d)
					continue;

				v *= t;
				GCoordinate p =p0 + v;

				f64 dis = p.GetDistanceFrom(hitPoint);
				if(dis<D)
				{
					D=dis;
					*pIndex = i;
				}
			}
			return D;
		}

		static inline f64 GetNearestEdge(GPolygon* polygon,const GCoordinate& hitPoint,u32* pIndex,f64 tolerence=1e10)
		{	
			GLinearRing* lr= polygon->GetExternalRing();
			u32 hitIndex;
			f64 d = GetNearestEdge(lr,hitPoint,&hitIndex,true,tolerence);
			u32 preCount = lr->GetCoordinateCount();

			for(int i=0;i<polygon->GetInternalRingCount();i++)
			{					
				GLinearRing* lr= polygon->GetInternalRingAt(i);
				u32 hitIndex2;
				f64 d2 = GetNearestEdge(lr,hitPoint,&hitIndex2,true,tolerence);
				if(d2<d)
				{
					d=d2;
					hitIndex = hitIndex2+preCount;
				}
				preCount+= lr->GetCoordinateCount();
			}

			*pIndex = hitIndex;
			return d;
		}
		
		static inline u32 GetPointInRingCounter(GLinearRing* ring,const GCoordinate& p)
		{
			u32 counter = 0;
			double xinters;
			GCoordinate p1,p2;

			u32 PointCount = ring->GetCoordinateCount();

			p1 = ring->GetCoordinateAt(0);
			for (int i=1;i<=PointCount;i++) {
				p2 = ring->GetCoordinateAt( i% PointCount);
				if (p.Y > min(p1.Y,p2.Y)) {
					if (p.Y <= max(p1.Y,p2.Y)) {
						if (p.X <= max(p1.X,p2.X)) {
							if (p1.Y != p2.Y) {
								xinters = (p.Y-p1.Y)*(p2.X-p1.X)/(p2.Y-p1.Y)+p1.X;
								if (p1.X == p2.X || p.X <= xinters)
									counter++;
							}
						}
					}
				}
				p1 = p2;
			}
			return counter;
		}

		static inline bool PointInPolygon(GPolygon* polygon,const GCoordinate& hitPoint)
		{
			u32 n = GetPointInRingCounter(polygon->GetExternalRing(),hitPoint);

			for(int i=0;i<polygon->GetInternalRingCount();i++)
			{
				n+=GetPointInRingCounter(polygon->GetInternalRingAt(i),hitPoint);
			}

			return n%2==1;
		}
		
	};
}

#endif