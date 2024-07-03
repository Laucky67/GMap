#ifndef __G_LINE_H__
#define __G_LINE_H__
#include "GVector2D.h"
namespace gmap{


	template<class T>
	struct GLine2d
	{
	public:
		typedef	GVector2d<T> vector2dType;
		typedef	GLine2d<T>  selfType;
	public:
		GLine2d(){}

		GLine2d(T x1,T y1, T x2, T y2)
			:Start(x1,y1),End(x2,y2)
		{

		}

		GLine2d(const vector2dType&  start,const vector2dType& end)
			:Start(start),End(end)
		{
		
		}

		f64 GetLength() const 
		{
			return Start.GetDistanceFrom(End); 
		}
		

		GVector2d<T> GetMiddle() const
		{
			return (Start + End) * (T)0.5;
		}

		
		GVector2d<T> GetVector() const 
		{
			return GVector2d<T>(End.X - Start.X, End.Y - Start.Y); 
		}

		bool IntersectWith(const GLine2d<T>& l, GVector2d<T>& out) const
		{

			const f64 commonDenominator = (l.End.Y - l.Start.Y)*(End.X - Start.X) -
				(l.End.X - l.Start.X)*(End.Y - Start.Y);

			const f64 numeratorA = (l.End.X - l.Start.X)*(Start.Y - l.Start.Y) -
				(l.End.Y - l.Start.Y)*(Start.X -l.Start.X);

			const f64 numeratorB = (End.X - Start.X)*(Start.Y - l.Start.Y) -
				(End.Y - Start.Y)*(Start.X -l.Start.X); 

			if(Equals(commonDenominator,(f64) 0.0,1e-10))
			{ 
				// The lines are either coincident or parallel
				//加强了判断相交的条件 by wine。
				if(Equals(numeratorA, (f64)0.0,(f64)1e-10) && Equals(numeratorB, (f64)0.0,(f64)1e-10))
				{
					// Try and find a common Endpoint
					if(l.Start == Start || l.End == Start)
						out = Start;
					else if(l.End == End || l.Start == End)
						out = End;
					else
						// one line is contained in the other, so for lack of a better
						// answer, pick the average of both lines
						out = ((Start + End + l.Start + l.End) * 0.25f);

					return true; // coincident
				}

				return false; // parallel
			}

			// Get the point of intersection on this line, checking that
			// it is within the line segment.
			const f64 uA = numeratorA / commonDenominator;
			if(uA < 0.f || uA > 1.f)
				return false; // Outside the line segment

			const f64 uB = numeratorB / commonDenominator;
			if(uB < 0.f || uB > 1.f)
				return false; // Outside the line segment

			// Calculate the intersection point.
			out.X = Start.X + uA * (End.X - Start.X);
			out.Y = Start.Y + uA * (End.Y - Start.Y);
			return true; 
		}

		T GetPointOrientation(const GVector2d<T>& point) const
		{
			return ( (End.X - Start.X) * (point.Y - Start.Y) -
					(point.X - Start.X) * (End.Y - Start.Y) );
		}

		bool IsPointOnLine(const GVector2d<T>& point) const
		{
			T d = GetPointOrientation(point);
			return (d == 0 && point.IsBetweenPoints(Start, End));
		}

		GVector2d<T> GetClosestPoint(const GVector2d<T>& point) const
		{
			GVector2d<T> c = point - Start;
			GVector2d<T> v = End -Start;
			T d = (T)v.GetLength();
			v /= d;
			T t = v.DotProduct(c);

			if (t < (T)0.0) return Start;
			if (t > d) return End;

			v *= t;
			return Start + v;
		}

	public:
		vector2dType Start;
		vector2dType End;

	};
	typedef GLine2d<f32> GLine2df;
	typedef GLine2d<f64> GLine2dd;

}

#endif