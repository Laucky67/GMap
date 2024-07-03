#ifndef __G_VECTOR_2D_H__
#define __G_VECTOR_2D_H__

#include "GTypes.h"

namespace gmap{

	//!¶þÎ¬ÏòÁ¿
	template <typename T>
	struct GVector2d 
	{
	public:

		GVector2d()
		{
			X = Y =0;
		}

		GVector2d(T x,T y)
		{
			X = x;
			Y = y;
		}

		GVector2d(const GVector2d& other)
		{
			X = other.X;
			Y = other.Y;
		}

		GVector2d<T> operator-() const 
		{
			return GVector2d<T>(-X, -Y);
		}


		GVector2d<T> operator+(const GVector2d<T>& other) const 
		{
			return GVector2d<T>(X + other.X, Y + other.Y); 
		}
		
		GVector2d<T>& operator+=(const GVector2d<T>& other) 
		{
			X+=other.X; 
			Y+=other.Y; 
			return *this; 
		}

		GVector2d<T> operator-(const GVector2d<T>& other) const 
		{
			return GVector2d<T>(X - other.X, Y - other.Y);
		}
			
		GVector2d<T>& operator-=(const GVector2d<T>& other) 
		{
			X-=other.X; Y-=other.Y;
			return *this; 
		}

		GVector2d<T> operator*(const GVector2d<T>& other) const 
		{
			return GVector2d<T>(X * other.X, Y * other.Y); 
		}

		GVector2d<T> operator*(const T v) const 
		{
			return GVector2d<T>(X * v, Y * v);
		}
			
		GVector2d<T>& operator*=(const GVector2d<T>& other)
		{ 
			X*=other.X; 
			Y*=other.Y; 
			return *this; 
		}


		GVector2d<T>& operator*=(const T v) 
		{
			X*=v; Y*=v; 
			return *this; 
		}
	
		GVector2d<T> operator/(const GVector2d<T>& other) const 
		{
			return GVector2d<T>(X / other.X, Y / other.Y); 
		}
		
		GVector2d<T>& operator/=(const GVector2d<T>& other)
		{
			X/=other.X; Y/=other.Y; 
			return *this;
		}
		GVector2d<T> operator/(const T v) const 
		{
			return GVector2d<T>(X / v, Y / v); 
		}

		GVector2d<T>& operator/=(const T v) 
		{
			X/=v; Y/=v; 
			return *this;
		}


		void Normalize()
		{
			T d = sqrt(X*X+Y*Y);
			X/=d;
			Y/=d;
		}

		T  GetLength()
		{
			return sqrt(X*X+Y*Y);
		}

		T GetLengthSQ()
		{
			return X*X+Y*Y;
		}

		void SetLength(T d)
		{
			Normalize();
			X*=d;
			Y*=d;
		}

		T GetDistanceFrom(const GVector2d<T>& other)const
		{
			T dx = other.X - X;
			T dy = other.Y - Y;
			return sqrt(dx*dx+dy*dy);
		}

		T DotProduct(const GVector2d<T>& other)
		{
			return X*other.X + Y*other.Y;
		}

		GVector2d<T>  CrossProduct(const GVector2d<T>& other)
		{
			return GVector2d<T>(X*other.X-Y*other.Y, X*other.Y+other.X*Y); 
		}

		GVector2d<T>& RotateBy(f64 degrees, const GVector2d<T>& center=GVector2d<T>())
		{
			degrees *= DEGTORAD64;
			const f64 cs = cos(degrees);
			const f64 sn = sin(degrees);

			X -= center.X;
			Y -= center.Y;

			T x= X*cs - Y*sn;
			T y= X*sn + Y*cs;

			X = center.X+x;
			Y = center.Y+y;
			return *this;
		}

		bool IsBetweenPoints(const GVector2d<T>& begin, const GVector2d<T>& end) const
		{
			const T f = (end - begin).GetLengthSQ();
			return GetLengthSQ(begin) <= f &&
				GetLengthSQ(end) <= f;
		}

		f64 GetAngle() const
		{
			if (Y == 0)
				return X < 0 ? 180 : 0;
			else
				if (X == 0)
					return Y < 0 ? 270 : 90;

			return atan2(Y,X)*RADTODEG64;
		}

		GVector2d<T> GetInterpolated(const GVector2d<T>& other, f64 d) const
		{
			f64 inv = 1.0f - d;
			return GVector2d<T>((T)(other.X*inv + X*d), (T)(other.Y*inv + Y*d));
		}


	public:
		T X;
		T Y;
	};

	typedef GVector2d<f32> GVector2df;

	typedef GVector2d<f64> GVector2dd;
}


#endif