#ifndef __GRASTER_COLOR_CONVERT_H__
#define __GRASTER_COLOR_CONVERT_H__
#include "GCoreBase.h"
#include "GUnknown.h"
namespace gmap {
	class GMAPCORE_API GRasterColorConvert:public GUnknown
	{
	public:
		enum Type {
			cvtRGB,
			cvtGray,
			cvtRainbow
		};
		virtual	const string&	GetName()
		{
			return Name;
		}
		virtual void SetName(const string& name)
		{
			Name = name;
		}
		GRasterColorConvert() { Range.a = 0; Range.b = 1; }
		virtual			Gdiplus::Color	convert(double* px) = 0;
		virtual void	SetValueRange(GRange& range);
		const GRange&	GetValueRange();
	protected:
		string			Name;
		GRange			Range;
	};

	class GMAPCORE_API GRasterRGBConvert:public GRasterColorConvert
	{
	public:
		GRasterRGBConvert();
		~GRasterRGBConvert();
		virtual			Gdiplus::Color convert(double* px); 
	};

	class GMAPCORE_API GRasterGrayConvert :public GRasterColorConvert
	{
	public:
		GRasterGrayConvert();
		~GRasterGrayConvert();
		virtual	Gdiplus::Color convert(double* px); 
	};


}

#endif