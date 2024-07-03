#include "GColorRasterData.h"
namespace gmap {
	GColorRasterData::GColorRasterData(const wstring & name, Bitmap * bitmap,GCoordinateSystem* cs, const Transform & t)
	{
		Name = name;
		Width = bitmap->GetWidth();
		Height = bitmap->GetHeight();
		Matrix = t;
		CoordinateSystem = cs;
		PixelData = new double[Width*Height*3];
		Color color;
		for (int ix = 0; ix < Width; ix++)
		{
			for (int iy = 0; iy < Height; iy++)
			{
				bitmap->GetPixel(ix, iy,&color);
				int p = (iy*Width + ix) * 3;
				PixelData[p  ] =   color.GetR()   / 255.0;
				PixelData[p+1] = color.GetG() / 255.0;
				PixelData[p+2] = color.GetB() / 255.0;
			}
		}

		double x, y;
		x = t.c, y = t.f;
		BoundingBox = GRectangle2dd(x, y,x,y);
		x = t.a * Width   + t.c;
		y = t.d * Width   + t.f;
		BoundingBox.AddPoint(x, y);


		x = t.a * Width + t.b * Height + t.c;
		y = t.d * Width + t.e * Height + t.f;
		BoundingBox.AddPoint(x, y);


		x =  t.b * Height + t.c;
		y =  t.e * Height + t.f;
		BoundingBox.AddPoint(x, y);

	}
	GColorRasterData::~GColorRasterData()
	{
		delete PixelData;
	}
	const wstring & GColorRasterData::GetName()
	{
		return Name;
	}
	GCoordinateSystem * GColorRasterData::GetCoordinateSystem()
	{
		return CoordinateSystem;
	}
	const GRectangle2dd & GColorRasterData::GetBoundingBox()
	{
		return BoundingBox;
	}
	
	int GColorRasterData::GetWidth()
	{
		return Width;
	}
	
	int GColorRasterData::GetHeight()
	{
		return Height;
	}
	
	int GColorRasterData::GetPixelElementCount()
	{
		return 3;
	}

	bool GColorRasterData::GetPixelData(int ix, int iy, double * pData)
	{
		if (ix<0 || ix>Width - 1 || iy<0 || iy>Height - 1)
			return false;

		double* pixel = PixelData+(ix + iy * Width) * 3;
		pData[0] = pixel[0];
		pData[1] = pixel[1];
		pData[2] = pixel[2];

		return true;
	}
	const GColorRasterData::Transform & GColorRasterData::GetTransform()
	{
		return Matrix;
	}
	GRasterData * GColorRasterData::CreateFromConfigure(const wstring & file)
	{
		IReadFile* pStream = FileUtil::CreateReadFile(file.c_str());

		wstring fileName;
		string  srid;
		float x0, y0, x1, y1;

		while (!pStream->Eof())
		{
			string line = FileUtil::ReadLine(pStream);
			string k, v;
			if (GetKV(line, k, v))
			{
				if (k == string("srid"))
				{
					srid = v;
				}
				if (k == string("bitmap"))
				{
					wchar_t buff[1024];

					size_t len = v.size();
					mbstowcs_s(&len, buff, v.c_str(), 1023);
					fileName = buff;
				}
				if (k == string("bbox"))
				{
					sscanf_s(v.c_str(), "%f,%f,%f,%f", &x0, &y0, &x1, &y1);
				}
			}
		}

		wstring path = file;
		int pos = file.find_last_of(L'\\');
		path = path.substr(0, pos+1)+fileName;
		Bitmap* bitmap = Bitmap::FromFile(path.c_str());
		if (!bitmap)
			return NULL;

		int width =  bitmap->GetWidth();
		int height = bitmap->GetHeight();

		double a = (x1 - x0) / width;
		double d = (y0 - y1) / height;

		Transform mtx = {a,0,x0,0,d,y1};
		GRasterData* rasterData = new GColorRasterData(path, bitmap, GCoordinateSystem::GetEPSGCoordinateSystem(srid), mtx);
		delete bitmap;

		return rasterData;
	}
}