#ifndef __G_RASTER_H__
#define __G_RASTER_H__
#include "GCoordinateSystem.h"
namespace gmap {
	class GMAPCORE_API GRasterData:public GUnknown{
	public:
		union Transform{
			struct {
				double a;
				double b;
				double c;
				double d;
				double e;
				double f;
			};
			double m[6];
		};
	public:
		virtual const wstring&			GetName()=0;
		virtual GCoordinateSystem*		GetCoordinateSystem()=0;
		virtual const GRectangle2dd&	GetBoundingBox() = 0;
		virtual int						GetWidth() = 0;
		virtual int						GetHeight() = 0;
		virtual int						GetPixelElementCount() = 0;
		virtual bool					GetPixelData(int ix,int iy,double* pData) = 0;
		virtual const Transform&		GetTransform() = 0;
	};
}
#endif
