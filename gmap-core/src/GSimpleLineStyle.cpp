#include "GSimpleLineStyle.h"

namespace gmap{

	GSimpleLineStyle::GSimpleLineStyle(GStyleGroup* group,u32 id,const wstring& name)
		:GLineStyle(group,id,name)
	{
		LineColor = Color(255,128,128,128);
		LineDashStyle = DashStyleSolid;
		LineWidth = 1;
		Scale0 = 1.0;
		LineDashCap	  =	DashCapFlat;
		LineStartCap  = LineCapFlat;
		LineEndCap	  = LineCapFlat;
		_LineJoin	  = LineJoinMiter;
		CachedPen	  = NULL;
		Dirty		  = true;
	}

	GSimpleLineStyle::~GSimpleLineStyle(void)
	{
		if(CachedPen)
		{
			delete CachedPen;
		}
	}

	f32 GSimpleLineStyle::GetLineWidth()
	{
		return LineWidth;
	}

	void GSimpleLineStyle::SetLineWidth(f32 lineWidth)
	{
		LineWidth = lineWidth;
		Dirty = true;
	}

	const Color& GSimpleLineStyle::GetColor()
	{
		return LineColor;
	}

	void GSimpleLineStyle::SetColor(const Color& color)
	{
		LineColor = color;
		Dirty = true;
	}

	DashStyle GSimpleLineStyle::GetDashStyle()
	{
		return LineDashStyle; 
	}

	void GSimpleLineStyle::SetDashStyle(DashStyle dashStyle)
	{
		LineDashStyle = dashStyle;
		Dirty = true;
	}

	LineCap	GSimpleLineStyle::GetStartCap()
	{
		return LineStartCap;
	}

	void GSimpleLineStyle::SetStartCap(LineCap cap)
	{
		LineStartCap = cap;
		Dirty = true;
	}

	LineCap GSimpleLineStyle::GetEndCap()
	{
		return LineEndCap;
	}

	void GSimpleLineStyle::SetEndCap(LineCap cap)
	{
		LineEndCap = cap;
		Dirty = true;
	}

	LineJoin GSimpleLineStyle::GetLineJoin()
	{
		return _LineJoin;
	}

	void GSimpleLineStyle::SetLineJoin(LineJoin lineJion)
	{
		_LineJoin = lineJion;
		Dirty = true;
	}

	DashCap	GSimpleLineStyle::GetDashCap()
	{
		return LineDashCap;
	}

	void GSimpleLineStyle::SetDashCap(DashCap dashCap)
	{
		LineDashCap = dashCap;
	}

	//!线的风格类型
	GLineStyle::EnumLineStyleType	GSimpleLineStyle::GetLineStyleType()
	{
		return GLineStyle::LST_SIMPLE_LINE_STYLE;
	}

	bool GSimpleLineStyle::IsSupportRenderScale() 
	{
		return true; 
	}

	void GSimpleLineStyle::SetRenderScale(double size)
	{
		Scale0 = size;
	}

	void GSimpleLineStyle::CheckDirty()
	{
		if(Dirty)
		{
			if(CachedPen)
			{
				delete CachedPen;
				CachedPen = NULL;
			}
			CachedPen = new Pen(LineColor,LineWidth);
			CachedPen->SetDashStyle(LineDashStyle);
			CachedPen->SetDashCap(LineDashCap);
			CachedPen->SetStartCap(LineStartCap);
			CachedPen->SetEndCap(LineEndCap);
			CachedPen->SetLineJoin(_LineJoin);
			Dirty =false;
		}
	}

	//!绘制一条线
	void GSimpleLineStyle::Draw(Graphics* graphics,PointF* points,UINT count,bool closed)
	{
		CheckDirty();

		CachedPen->SetWidth(float(LineWidth*Scale0));
		if(!closed)
		{
			graphics->DrawLines(CachedPen,points,count);
		}
		else
		{
			graphics->DrawPolygon(CachedPen,points,count);
		}
	}

	//!绘制一条线
	void GSimpleLineStyle::Draw(Graphics* graphics,GraphicsPath* path)
	{
		CheckDirty();
		graphics->DrawPath(CachedPen,path);
	}

	//!保存到流中
	void GSimpleLineStyle::Save(IWriteFile* pStream)
	{
		pStream->Write(LineColor);
		pStream->Write(LineDashStyle);
		pStream->Write(LineWidth);
		pStream->Write(LineDashCap);
		pStream->Write(LineStartCap);
		pStream->Write(LineEndCap);
		pStream->Write(_LineJoin);
	}

	//!从流中读取
	void GSimpleLineStyle::Load(IReadFile* pStream)
	{
		pStream->Read(LineColor);
		pStream->Read(LineDashStyle);
		pStream->Read(LineWidth);
		pStream->Read(LineDashCap);
		pStream->Read(LineStartCap);
		pStream->Read(LineEndCap);
		pStream->Read(_LineJoin);
		Dirty = true;
	}
}