#ifndef __G_RT_HELPER_H__
#define __G_RT_HELPER_H__

#include "GTypes.h"
#include "GCoreBase.h"
#include "GPosition2d.h"
#include "GDimension2d.h"
#include "GRect.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace gmap{
	namespace rtree
	{
	template<class boundT>
	inline rect<f32> combineBounds(vector<boundT>& bounds)
	{	
		rect<f32> retBound;

		u32 size = bounds.size();
		if(size>0)
		{
			retBound = bounds[0].getBound();
			for(u32 i =1;i<size;i++)
			{
				rect<f32> bound = bounds[i].getBound();
				retBound.addInternalPoint(bound.LowerRightCorner);
				retBound.addInternalPoint(bound.UpperLeftCorner);
			}
			retBound.repair();
		}

		return retBound;
	}

	//template<class boundT>
	//inline void peekSplitSeeds(const vector<boundT>& bounds,u32& indexA,u32& indexB)
	//{
	//	u32 size = bounds.size();

	//	f32 minx = bounds[0].Bound.UpperLeftCorner.X;
	//	f32 maxx = bounds[0].Bound.LowerRightCorner.X;

	//	f32 miny = bounds[0].Bound.UpperLeftCorner.Y;
	//	f32 maxy = bounds[0].Bound.LowerRightCorner.Y;
	//	indexA = indexB =0;

	//	u32 xA = 0,xB = 0,yA = 0,yB =0;

	//	for(u32 i=1;i<size;i++)
	//	{
	//		const rect<f32>& bound = bounds[i].Bound;


	//		minx = min( bound.UpperLeftCorner.X, minx);
	//		maxx = max( bound.LowerRightCorner.X,maxx);

	//		miny = min( bound.UpperLeftCorner.Y, miny);
	//		maxy = max( bound.LowerRightCorner.Y,maxy);

	//		xA = (bound.UpperLeftCorner.X >bounds[xA].Bound.UpperLeftCorner.X?i:xA);
	//		xB = (bound.LowerRightCorner.X <bounds[xB].Bound.LowerRightCorner.X?i:xB);

	//		yA = (bound.UpperLeftCorner.Y >bounds[yA].Bound.UpperLeftCorner.Y?i:yA);
	//		yB = (bound.LowerRightCorner.Y <bounds[yB].Bound.LowerRightCorner.Y?i:yB);
	//	}

	//	f32 fx = maxx - minx;
	//	if(fx<0) fx = 1.0;
	//	fx = (bounds[xA].Bound.LowerRightCorner.X - bounds[xB].Bound.UpperLeftCorner.X)/fx;

	//	f32 fy = maxy - miny;
	//	if(fy<0) fy = 1.0;
	//	fy = (bounds[yA].Bound.LowerRightCorner.Y - bounds[yB].Bound.UpperLeftCorner.Y)/fy;

	//	if(fx>fy)
	//	{
	//		indexA = xA;indexB = xB;
	//	}
	//	else
	//	{
	//		indexA = yA;indexB = yB;
	//	}

	//	if(indexA == indexB)
	//	{
	//		if(indexB ==0 )
	//		{
	//			indexB ++;
	//		}
	//		else
	//		{
	//			indexB --;
	//		}
	//	}
	//}

	template<class boundT>
	inline void peekSplitSeeds(vector<boundT>& bounds,u32& indexA,u32& indexB)
	{
		u32 size = bounds.size();

		f32 minx = bounds[0].getBound().UpperLeftCorner.X;
		f32 maxx = bounds[0].getBound().LowerRightCorner.X;

		f32 miny = bounds[0].getBound().UpperLeftCorner.Y;
		f32 maxy = bounds[0].getBound().LowerRightCorner.Y;
		indexA = indexB =0;

		u32 xA = 0,xB = 0,yA = 0,yB =0;

		for(u32 i=1;i<size;i++)
		{
			rect<f32> bound = bounds[i].getBound();


			minx = min( bound.UpperLeftCorner.X, minx);
			maxx = max( bound.LowerRightCorner.X,maxx);

			miny = min( bound.UpperLeftCorner.Y, miny);
			maxy = max( bound.LowerRightCorner.Y,maxy);

			xA = (bound.UpperLeftCorner.X >bounds[xA].getBound().UpperLeftCorner.X?i:xA);
			xB = (bound.LowerRightCorner.X <bounds[xB].getBound().LowerRightCorner.X?i:xB);

			yA = (bound.UpperLeftCorner.Y >bounds[yA].getBound().UpperLeftCorner.Y?i:yA);
			yB = (bound.LowerRightCorner.Y <bounds[yB].getBound().LowerRightCorner.Y?i:yB);
		}


		f32 fx = (bounds[xA].getBound().UpperLeftCorner.X - bounds[xB].getBound().LowerRightCorner.X);
		f32 fy = (bounds[yA].getBound().UpperLeftCorner.Y - bounds[yB].getBound().LowerRightCorner.Y);

		if(fx>fy)
		{
			indexA = xA;indexB = xB;
		}
		else
		{
			indexA = yA;indexB = yB;
		}

		if(indexA == indexB)
		{
			if(indexB ==0 )
			{
				indexB ++;
			}
			else
			{
				indexB --;
			}
		}
	}

	template<class T>
	inline const rect<T>  combineBounds(const rect<T>& ra,const rect<T>& rb)
	{
		rect<T> r = ra;
		r.addInternalPoint(rb.LowerRightCorner);
		r.addInternalPoint(rb.UpperLeftCorner);
		r.repair();
		return r;
	}

	template<class boundT>
	inline void sortBounds(vector<boundT>& bounds)
	{

	}

	//!判断边界是否包含
	inline bool contains(const rect<f32>& boundA,const rect<f32>& boundB)
	{
		return boundA.isPointInside(boundB.LowerRightCorner) 
			&&
			boundA.isPointInside(boundB.UpperLeftCorner);
	}
	//!计算判定因子
	template<class rectT>
	inline f32  computeDeltaValue(const rectT& nodeBound,const rectT& itemBound)
	{
			rectT newBound=nodeBound;
			newBound.addInternalPoint(itemBound.LowerRightCorner);
			newBound.addInternalPoint(itemBound.UpperLeftCorner);

			return newBound.getArea()-nodeBound.getArea();
	}

	template<class rectT>
	inline f32  computeDeltaValue(const rectT& nodeBound,const rectT& itemBound,u32 nChild)
	{
		if(!contains(nodeBound,itemBound))
		{
			rectT newBound=nodeBound;
			newBound.addInternalPoint(itemBound.LowerRightCorner);
			newBound.addInternalPoint(itemBound.UpperLeftCorner);

			return newBound.getArea()-nodeBound.getArea();
		}
		else
		{
			return -1.0f/nChild;
		}
	}

	template<class boundT>
	inline void splitBounds( vector<boundT>& bounds,rect<f32>& boundA,vector<u32>& groupA,rect<f32>& boundB,vector<u32>& groupB,u32 minCount)
	{
		u32 indexA=0,indexB=0;
		peekSplitSeeds<boundT>(bounds,indexA,indexB);

		groupA.push_back(indexA);
		groupB.push_back(indexB);

		boundA = bounds[indexA].getBound();
		boundB = bounds[indexB].getBound();

		u32 size = bounds.size();
		s32 leftCount = size -2;

		vector<bool> flags(bounds.size());
		//flags.set_used(bounds.size());
		for(u32 i=0;i<flags.size();i++)flags[i]=false;
		flags[indexA]=flags[indexB]=true;


		while(leftCount>0)
		{
			if(leftCount + groupA.size() == minCount)
			{
				for(u32 i=0;i<size;i++)
				{
					if(!flags[i]) 
					{
						groupA.push_back(i);
						boundA =combineBounds(boundA,bounds[i].getBound());
						flags[i]=true;
					}
					leftCount --;

				}
			}
			else if(leftCount + groupB.size() == minCount)
			{				
				for(u32 i=0;i<size;i++)
				{
					if(i == indexA || i == indexB) 
						continue;

					if(!flags[i]) 
					{
						groupB.push_back(i);
						boundB =combineBounds(boundB,bounds[i].getBound());
						flags[i]=true;
					}
					leftCount --;			
				}
			}
			else
			{
				f32 aA = boundA.getArea(),aB = boundB.getArea();
				f32 deltaA=0,deltaB=0;
				f32 delta=-1e20;
				u32 index=-1;

				for(u32 i =0;i<size;i++)
				{
					if(!flags[i])
					{
						const rect<f32>& itemBound=bounds[i].getBound();
						f32 dA=computeDeltaValue(boundA,itemBound);
						f32 dB=computeDeltaValue(boundB,itemBound);
						f32 d=fabs(dA-dB);
						if(d>delta)
						{
							delta=d;
							deltaA=dA;
							deltaB=dB;
							index = i;
							//break;
						}
					}
				}
				flags[index]=true;
				const rect<f32>& itemBound=bounds[index].getBound();
				if(deltaA < deltaB)
				{
					groupA.push_back(index);
					boundA = combineBounds(boundA,bounds[index].getBound());
				}
				else if(deltaB < deltaA)
				{
					groupB.push_back(index);
					boundB = combineBounds(boundB,bounds[index].getBound());
				}
				else if(aA < aB)
				{
					groupA.push_back(index);
					boundA = combineBounds(boundA,bounds[index].getBound());
				}
				else if(aB < aA)
				{
					groupB.push_back(index);
					boundB = combineBounds(boundB,bounds[index].getBound());
				}
				else if(groupA.size()<groupB.size())
				{
					groupA.push_back(index);
					boundA = combineBounds(boundA,bounds[index].getBound());
				}
				else if(groupB.size()<groupA.size())
				{
					groupB.push_back(index);
					boundB = combineBounds(boundB,bounds[index].getBound());
				}
				else 
				{				
					groupA.push_back(index);
					boundA = combineBounds(boundA,bounds[index].getBound());
				}

				leftCount--;
			}
		}

	}

}}
#endif