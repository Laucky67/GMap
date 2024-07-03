#include "GSimpleImageSource.h"
#include "IFile.h"

namespace gmap{
	//!构造函数
	GSimpleImageSource::GSimpleImageSource(const wstring& imgSrc,GCoordinateSystem* pCS,const GRectangle2dd& bbox)
		:GImageSource(imgSrc,pCS,bbox)
	{
		Bitmap = Bitmap::FromFile(imgSrc.c_str());
	}

	//!构造函数
	GSimpleImageSource::GSimpleImageSource(const wstring& name,Gdiplus::Bitmap* pBitmap,GCoordinateSystem* pCS,const GRectangle2dd& bbox)
				:GImageSource(name,pCS,bbox)
	{
		Bitmap= pBitmap;
	}


	GSimpleImageSource::~GSimpleImageSource(void)
	{
		if(Bitmap)
			delete Bitmap;
	}

	inline double interpolate(double a,double b,double factor)
	{
		return (b-a)*factor+a;
	}

	inline double factor(double a,double b,double x)
	{
		return (x-a)/(b-a);
	}

	//!锁定图形区域
	void GSimpleImageSource::LockRegion(const GRectangle2dd& lockRegion,Gdiplus::Bitmap* pLockBitmap,IGeoTransform* pTransformer)
	{
		int width=pLockBitmap->GetWidth();
		int height = pLockBitmap->GetHeight();

		int s_width = Bitmap->GetWidth();
		int s_height = Bitmap->GetHeight();

		double t_min_x=lockRegion.MinEdge.X;
		double t_min_y=lockRegion.MinEdge.Y;
		double t_max_x=lockRegion.MaxEdge.X;
		double t_max_y=lockRegion.MaxEdge.Y;

		double s_min_x=BoundingBox.MinEdge.X;
		double s_min_y=BoundingBox.MinEdge.X;
		double s_max_x=BoundingBox.MaxEdge.X;
		double s_max_y=BoundingBox.MaxEdge.X;

		for(int iy=0;iy<height;iy++)
		{
			
			for(int ix=0;ix<width;ix++)
			{
				double y = interpolate(t_min_y,t_max_y,factor(0,height,iy));
				double x = interpolate(t_min_x,t_max_x,factor(0,width, ix));

				if(pTransformer)
				{
					pTransformer->Transform(x,y,&x,&y);
				}

				int s_x = s_width* factor(s_min_x,s_max_x,x);
				int s_y = s_height* factor(s_min_y,s_max_y,y);

				if(s_x>=0&&s_x<s_width && s_y>=0 && s_y< s_height)
				{
					Color color;
					Bitmap->GetPixel(s_x,s_height-s_y-1,&color);
					pLockBitmap->SetPixel(ix,height-iy-1,color);
				}
				else 
				{
					pLockBitmap->SetPixel(ix,height-iy-1,Color(123,123,123,123));
				}
			}
		}
	}
	GImageSource * GSimpleImageSource::CreateFromConfigure(const wstring & file)
	{
		IReadFile* pStream = FileUtil::CreateReadFile(file.c_str());

		wstring imageSrc;
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
					mbstowcs_s(&len,buff, v.c_str(), 1023);
					imageSrc = buff;
				}
				if (k == string("bbox"))
				{
					sscanf_s(v.c_str(), "%f,%f,%f,%f", &x0, &y0, &x1, &y1);
				}
			}
		}

		wstring path = file;
		int pos = file.find_last_of(L'\\');
		path = path.substr(0, pos);

		GSimpleImageSource* pImageSource = new GSimpleImageSource(path + wstring(L"\\") + imageSrc, GCoordinateSystem::GetEPSGCoordinateSystem(srid), GRectangle2dd(x0, y0, x1, y1));
		return pImageSource;
	}
	GImageSource * GSimpleImageSource::Create(const wstring & imgSrc, GCoordinateSystem * pCS, const GRectangle2dd & bbox)
	{
		return new GSimpleImageSource(imgSrc,pCS,bbox);
	}
}
