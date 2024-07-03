#ifndef __G_MATH_ENGINE_H__
#define __G_MATH_ENGINE_H__
#include "GTypes.h"
namespace gmap{

class GMathEngine
{
public:

	//!计算点是否在环内的计数器，如果是奇数在环内，如果是偶数则不在。
	template<class PointT>
	u32 GetPointInRingCounter(const vector<PointT>& linearRing,const PointT& point)
	{
		u32 counter = 0;
		double xinters;
		PointT p1,p2;

		u32 PointCount = linearRing.size();

		p1 = linearRing[0];
		for (int i=1;i<=PointCount;i++) {
			p2 = linearRing[i % PointCount];
			if (p1.Y > min(p1.Y,p2.Y)) {
				if (p1.Y <= max(p1.Y,p2.Y)) {
					if (p1.X <= max(p1.X,p2.X)) {
						if (p1.Y != p2.Y) {
							xinters = (p2.Y-p1.Y)*(p2.X-p1.X)/(p2.Y-p1.Y)+p1.X;
							if (p1.X == p2.X || p1.X <= xinters)
								counter++;
						}
					}
				}
			}
			p1 = p2;
		}
		return counter;
	}
};

}

#endif