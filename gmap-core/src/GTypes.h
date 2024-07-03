#ifndef _G_TYPES_H__
#define _G_TYPES_H__


#include <vector>
#include <map>
#include <string>
using namespace std;
namespace gmap{
	
	typedef unsigned char	u8;
	typedef unsigned short	u16;
	typedef int				s32;
	typedef unsigned int	u32;
	typedef float			f32;
	typedef double			f64;

	const f64 PI64		= 3.1415926535897932384626433832795028841971693993751;

	const f32 PI		 = 3.141592653589793238;

	const f64 RECIPROCAL_PI64 = 1.0/PI64;

	const f32 DEGTORAD = PI / 180.0f;

	const f32 RADTODEG   = 180.0f / PI;

	const f64 DEGTORAD64 = PI64 / 180.0;

	const f64 RADTODEG64 = 180.0 / PI64;

	template<class T>
	bool Equals(T a,T b,T tolerance=1e-10)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

}
#endif