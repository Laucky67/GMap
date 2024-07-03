#include "GRasterColorConvert.h"
namespace gmap {

	void GRasterColorConvert::SetValueRange(GRange & range)
	{
		Range = range;
	}

	const GRange&	GRasterColorConvert::GetValueRange()
	{
		return Range;
	}

	GRasterRGBConvert::GRasterRGBConvert()
	{
		Name = "RGB";
	}

	GRasterRGBConvert::~GRasterRGBConvert()
	{
		
	}
	
	Gdiplus::Color GRasterRGBConvert::convert(double * px)
	{
		double d = Range.b - Range.a;
		return Gdiplus::Color(255*(px[0]-Range.a)/d, 255 * (px[1] - Range.a) / d, 255 * (px[2] - Range.a) / d);
	}

	GRasterGrayConvert::GRasterGrayConvert()
	{
		Name = "GRAY";
	}
	
	GRasterGrayConvert::~GRasterGrayConvert()
	{

	}

	Gdiplus::Color GRasterGrayConvert::convert(double * px)
	{
		BYTE a = 255*(px[0] - Range.a) / (Range.b - Range.a);
		BYTE b = 255 * (px[1]-Range.a)/(Range.b - Range.a);
	
		return Gdiplus::Color(b,a,a,a);
	}
}