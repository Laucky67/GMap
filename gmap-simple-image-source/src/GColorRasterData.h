#ifndef __G_COLOR_RASTER_DATA_H__
#define __G_COLOR_RASTER_DATA_H__
#include "GRasterData.h"


#ifdef GMAPSIMPLEIMAGESOURCE_EXPORTS
#define GMAPSIMPLEIMAGESOURCE_API	__declspec(dllexport)
#else
#define GMAPSIMPLEIMAGESOURCE_API	__declspec(dllimport)
#endif

namespace gmap {
	class GMAPSIMPLEIMAGESOURCE_API GColorRasterData :public GRasterData
	{
	public:
		GColorRasterData(const wstring& name,Bitmap* bitmap,GCoordinateSystem* cs,const Transform& t);
		~GColorRasterData();
		virtual const wstring&			GetName();
		virtual GCoordinateSystem*		GetCoordinateSystem();
		virtual const GRectangle2dd&	GetBoundingBox();
		virtual int						GetWidth();
		virtual int						GetHeight();
		virtual int						GetPixelElementCount();
		virtual bool					GetPixelData(int ix, int iy, double* pData);
		virtual const Transform&		GetTransform();
	public:
		static GRasterData* CreateFromConfigure(const wstring& fileName);
	private:
		wstring Name;
		GCoordinateSystem* CoordinateSystem;
		GRectangle2dd BoundingBox;
		int			Width;
		int			Height;
		double*     PixelData;
		Transform	Matrix;
	};
}

#endif