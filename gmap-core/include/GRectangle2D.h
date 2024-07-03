#ifndef __G_RECTANGLE2D_H__
#define __G_RECTANGLE2D_H__

#include "GTypes.h"
#include "GVector2d.h"

namespace gmap
{

	//!2DµÄ°üÎ§ºÐ
	template<class T>
	struct GRectangle2d
	{
	public:
		typedef GVector2d<T>	vector2dType;
		typedef GRectangle2d<T> selfType;
	public:
		GRectangle2d()
		{
			MaxEdge = vector2dType(-1e20,-1e20);
			MinEdge = vector2dType( 1e20, 1e20);		
		}

		GRectangle2d(T x1,T y1,T x2,T y2)
		{
			MinEdge = vector2dType(x1,y1);
			MaxEdge = vector2dType(x2,y2);
		}

		GRectangle2d(const vector2dType& minEdge,const vector2dType& maxEdge)
		{
			MinEdge = minEdge;
			MaxEdge = maxEdge;
		}

		GRectangle2d(const selfType& other)
		{
			MinEdge = other.MinEdge;
			MaxEdge = other.MaxEdge;
		}

		void Reset(T x1,T y1,T x2,T y2)
		{
			MinEdge = vector2dType(x1,y1);
			MaxEdge = vector2dType(x2,y2);
		}

		void Reset(const vector2dType& minEdge,const vector2dType& maxEdge)
		{
			MinEdge = minEdge;
			MaxEdge = maxEdge;
		}

		void Reset(const selfType& other)
		{
			MinEdge = other.MinEdge;
			MaxEdge = other.MaxEdge;
		}

		T GetWidth()const
		{
			return MaxEdge.X - MinEdge.X;
		}

		T GetHeight() const
		{
			return MaxEdge.Y - MinEdge.Y;
		}

		void AddPoint(T x,T y)
		{
			if(MinEdge.X>x)MinEdge.X = x;
			if(MinEdge.Y>y)MinEdge.Y = y;
			if(MaxEdge.X<x)MaxEdge.X = x;
			if(MaxEdge.Y<y)MaxEdge.Y = y;
		}

		void AddPoint(const vector2dType& position)
		{
			AddPoint(position.X,position.Y);
		}

		void AddRectangle(const selfType& other)
		{
			AddPoint(other.MinEdge);
			AddPoint(other.MaxEdge);
		}

		void Repair()
		{
			if(MinEdge.X>MaxEdge.X)
			{
				T t = MaxEdge.X;
				MaxEdge.X = MinEdge.X;
				MinEdge.X = t;
			}
			if(MinEdge.Y>MaxEdge.Y)
			{
				T t = MaxEdge.Y;
				MaxEdge.Y = MinEdge.Y;
				MinEdge.Y = t;
			}
		}

		bool IsPointIn(const vector2dType& point)
		{
			return  
				(point.X <= MaxEdge.X && point.X >= MinEdge.X)
				&&
				(point.Y <= MaxEdge.Y && point.Y >= MinEdge.Y);
		}

		bool IsRectCollided(const GRectangle2d<T>& other) const
		{
			return (MaxEdge.Y > other.MinEdge.Y &&
				MinEdge.Y < other.MaxEdge.Y &&
				MaxEdge.X > other.MinEdge.X &&
				MinEdge.X < other.MaxEdge.X);
		}

	public:
		GVector2d<T> MinEdge;
		GVector2d<T> MaxEdge;

	};

	typedef GRectangle2d<f32> GRectangle2df;
	typedef GRectangle2d<f64> GRectangle2dd;


}

#endif