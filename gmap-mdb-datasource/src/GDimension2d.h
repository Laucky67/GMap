#ifndef __GP_DIMENSION2D_H_INCLUDED__
#define __GP_DIMENSION2D_H_INCLUDED__

#include "GTypes.h"

namespace gmap
{
	namespace rtree{
	//! Specifies a 2 dimensional size.
	template <class T>
	class dimension2d
	{
		public:
			dimension2d()
				: Width(0), Height(0) {};

			dimension2d(const T& width, const T& height)
				: Width(width), Height(height) {};

			bool operator == (const dimension2d<T>& other) const
			{
				return Width == other.Width && Height == other.Height;
			}

			bool operator != (const dimension2d<T>& other) const
			{
				return ! (*this == other);
			}

			dimension2d<T>& set(const T& width, const T& height) 
			{
				Width = width;
				Height = height;
				return *this;
			}

			dimension2d<T> operator/(const T& scale) 
			{
				return dimension2d<T>(Width/scale, Height/scale);
			}

			dimension2d<T> operator*(const T& scale) 
			{
				return dimension2d<T>(Width*scale, Height*scale);
			}

			T Width, Height;
	};

	//! Typedef for a f32 dimension.
	typedef dimension2d<f32> dimension2df;
	//! Typedef for an integer dimension.
	typedef dimension2d<s32> dimension2di;

	} 
}

#endif

