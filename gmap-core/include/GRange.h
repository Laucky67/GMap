#ifndef __G_RANGE_H__
#define __G_RANGE_H__
#include "GTypes.h"
namespace gmap{

	struct GRange
	{
		u32 a;
		u32 b;
		bool operator <(const GRange& other)const
		{
			return a<other.a;
		}
	};

}

#endif