#include "GRasterLayer.h"
#include "GMapDrawing.h"
#include "GMap.h"
#define REAL_TIME_REPROJECTION
namespace gmap {
	GRasterLayer::GRasterLayer()
	{
		RasterData = NULL;
		ColorConvert = NULL;
		CacheBitmap = NULL;
	}
	GRasterLayer::~GRasterLayer()
	{
		if (RasterData)
			RasterData->Release();
		if (ColorConvert)
			ColorConvert->Release();
	}

	GMapLayer::EnumLayerType GRasterLayer::GetLayerType() {
		return LT_RASTER;
	}

	void GRasterLayer::Draw(GMapDrawing * pDrawing)
	{
		if (!RasterData)
			return;
		if (!ColorConvert)
			return;
#ifdef REAL_TIME_REPROJECTION
		if (!CacheBitmap)
			updateCacheBitmap();

		float x0, y0, x1, y1;
		pDrawing->MapToView(DefaultBoundingBox.MinEdge.X, DefaultBoundingBox.MinEdge.Y, x0, y0);
		pDrawing->MapToView(DefaultBoundingBox.MaxEdge.X, DefaultBoundingBox.MaxEdge.Y, x1, y1);

		if (y0 > y1)
			swap(y0, y1);



		pDrawing->GetGraphics()->DrawImage(CacheBitmap, x0, y0, x1 - x0, y1 - y0);
		Pen pen(Color::Red);
		pDrawing->GetGraphics()->DrawRectangle(&pen, Rect(x0, y0, x1 - x0, y1 - y0));

#else
		GCoordinateSystem* mapCS = GetMap()->GetCoordinateSystem();
		GCoordinateSystem* rasCS = RasterData->GetCoordinateSystem();
		IGeoTransform* trans = GCoordinateSystem::CreateTransform(mapCS,rasCS);

		const GRectangle2dd& vb = pDrawing->GetViewBounds();
		const GRectangle2dd& rb = DefaultBoundingBox;
		
		double x0 = max(vb.MinEdge.X, rb.MinEdge.X);
		double y0 = max(vb.MinEdge.Y, rb.MinEdge.Y);
		double x1 = min(vb.MaxEdge.X, rb.MaxEdge.X);
		double y1 = min(vb.MaxEdge.Y, rb.MaxEdge.Y);

		Pen pen(Color::Red);

		if (x0 > x1 || y0 > y1)
			return;
		
		float sx0, sy0, sx1, sy1;
		pDrawing->MapToView(x0,y0,sx0,sy0);
		pDrawing->MapToView(x1,y1,sx1,sy1);

		int ix0 = sx0, iy0 = sy0,
			ix1 = sx1, iy1 = sy1;
		if (ix0 > ix1)
			swap(ix0, ix1);
		if (iy0 > iy1)
			swap(iy0, iy1);

		GRasterData::Transform t = RasterData->GetTransform();
		double dc = t.d*t.c;
		double af = t.a*t.f;
		double ce = t.c*t.e;
		double bf = t.b*t.f;
		double db = t.d*t.b;
		double ae = t.a*t.e;
		double dcaf = dc - af;
		double cebf = ce - bf;
		double dbae = db - ae;

		double pData[16];
		GRasterData* ras = RasterData;
		GRasterColorConvert* cvt = ColorConvert;
		Bitmap  bitmap(ix1-ix0,iy1-iy0);
		BitmapData bmpData;
		Rect rc(0, 0, ix1 - ix0, iy1 - iy0);
		bitmap.LockBits(&rc, ImageLockModeWrite, PixelFormat32bppARGB, &bmpData);

		for (int iy = iy0; iy < iy1; iy++)
		{
			for (int ix = ix0; ix < ix1; ix++)
			{
				double x0, y0,mx,my;
				pDrawing->ViewToMap(ix, iy, mx, my);
				trans->Transform(mx, my, &x0, &y0);
				 
				double px = 0; 
				double py = 0; 

				{
					px = (t.e*x0 - t.b*y0 - cebf) / -dbae;
					py = (t.d*x0 - t.a*y0 - dcaf) /  dbae;
				}

				int ipx = round(px);
				int ipy = round(py);

				if (ras->GetPixelData(ipx, ipy, pData))
				{
					BYTE* pixel = (BYTE*)bmpData.Scan0 + (iy-iy0) * bmpData.Stride + sizeof(INT32)*(ix-ix0);
					*((ARGB*)pixel) = cvt->convert(pData).GetValue();
				}
			}
		}
		bitmap.UnlockBits(&bmpData);
		pDrawing->GetGraphics()->DrawImage(&bitmap, ix0, iy0,ix1-ix0,iy1-iy0);
#endif
	}

	void GRasterLayer::UpdateValueRangeFromData()
	{
		if (!RasterData)
			return;
		double pData[16];
		GRange range;
		range.a =  1e10;
		range.b = -1e10;
		int ec = RasterData->GetPixelElementCount();
		for (int iy = 0, height = RasterData->GetHeight(),width = RasterData->GetWidth(), ec = RasterData->GetPixelElementCount(); iy < height; iy++)
		{
			for (int ix = 0; ix < width; ix++)
			{
				if (RasterData->GetPixelData(ix, iy, pData))
				{
					for (int ei = 0;ei<ec;ei++)
					{
						range.a = min(range.a, pData[ei]);
						range.b = max(range.b, pData[ei]);
					}
				}
			}
		}
		SetValueRange(range);
	}

	void GRasterLayer::SetValueRange(const GRange & range)
	{
		Range = range;
	}

	void GRasterLayer::AttachMap(GMap * map)
	{
		GMapLayer::AttachMap(map);
		updateBoundingBox();
	}

	void GRasterLayer::UpdateCache()
	{
		updateBoundingBox();
		if (CacheBitmap)
		{
			delete CacheBitmap;
			CacheBitmap = NULL;
		}
	}

	void GRasterLayer::updateBoundingBox()
	{
		if (RasterData && GetMap())
		{
			UpdateValueRangeFromData();
			GCoordinateSystem* mapCS = GetMap()->GetCoordinateSystem();
			GCoordinateSystem* rasCS = RasterData->GetCoordinateSystem();
			IGeoTransform* trans = GCoordinateSystem::CreateTransform(rasCS, mapCS);

			GRectangle2dd bx = RasterData->GetBoundingBox();
			double x, y;
			trans->Transform(bx.MinEdge.X, bx.MinEdge.Y, &x, &y);
			GRectangle2dd bx0(x, y, x, y);
			trans->Transform(bx.MaxEdge.X, bx.MinEdge.Y, &x, &y);
			bx0.AddPoint(x, y);
			trans->Transform(bx.MaxEdge.X, bx.MaxEdge.Y, &x, &y);
			bx0.AddPoint(x, y);
			trans->Transform(bx.MinEdge.X, bx.MaxEdge.Y, &x, &y);
			bx0.AddPoint(x, y);

			DefaultBoundingBox = bx0;
		}
	}

	void GRasterLayer::updateCacheBitmap()
	{
		if (!RasterData)
			return;
		if (!ColorConvert)
			return;

		GCoordinateSystem* mapCS = GetMap()->GetCoordinateSystem();
		GCoordinateSystem* rasCS = RasterData->GetCoordinateSystem();
		IGeoTransform* trans = GCoordinateSystem::CreateTransform(mapCS, rasCS);

		const GRectangle2dd& box = DefaultBoundingBox;

		int width  = RasterData->GetWidth()*2;
		int height = box.GetHeight() / box.GetWidth()*width;

		CacheBitmap = new Bitmap(width, height);
		double dx = DefaultBoundingBox.GetWidth() / width;
		double dy = DefaultBoundingBox.GetHeight() / height;
		double x0 = box.MinEdge.X, y1 = box.MaxEdge.Y;

		 
		GRasterData::Transform t = RasterData->GetTransform();
		double dc = t.d*t.c;
		double af = t.a*t.f;
		double ce = t.c*t.e;
		double bf = t.b*t.f;
		double db = t.d*t.b;
		double ae = t.a*t.e;
		double dcaf = dc - af;
		double cebf = ce - bf;
		double dbae = db - ae;

		double pData[16];
		GRasterData* ras = RasterData;
		GRasterColorConvert* cvt = ColorConvert;
	
		BitmapData bmpData;
		Rect rc(0, 0, width,height);
		CacheBitmap->LockBits(&rc, ImageLockModeWrite, PixelFormat32bppARGB, &bmpData);

		for (int iy = 0; iy < height; iy++)
		{
			for (int ix = 0; ix < width; ix++)
			{
				double mx = x0 + ix * dx;
				double my = y1 - iy * dy;
				double xp, yp;
				trans->Transform(mx, my, &xp, &yp);

				double px = 0;
				double py = 0;
				px = (t.e*xp - t.b*yp - cebf) / -dbae;
				py = (t.d*xp - t.a*yp - dcaf) /  dbae;

				int ipx = round(px);
				int ipy = round(py);

				if (ras->GetPixelData(ipx, ipy, pData))
				{
					BYTE* pixel = (BYTE*)bmpData.Scan0 + iy * bmpData.Stride + sizeof(INT32)*ix;
					*((ARGB*)pixel) = cvt->convert(pData).GetValue();
				}
			}
		}
		CacheBitmap->UnlockBits(&bmpData);
	}

	void GRasterLayer::SetRasterData(GRasterData * ras)
	{
		if (RasterData)
			RasterData->Release();
		RasterData = ras;
		if (RasterData)
			RasterData->AddRef();
		updateBoundingBox();
	}

	GRasterData * GRasterLayer::GetRasterData()
	{
		return RasterData;
	}

	void GRasterLayer::SetColorConvert(GRasterColorConvert * cvt)
	{
		if (ColorConvert)
			ColorConvert->Release();
		ColorConvert = cvt;
		if (ColorConvert)
			ColorConvert->AddRef();
	}

	GRasterColorConvert * GRasterLayer::GetColorConvert()
	{
		return ColorConvert;
	}

}