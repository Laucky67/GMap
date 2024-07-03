#ifndef __G_SIMPLE_LINE_STYLE_H__
#define __G_SIMPLE_LINE_STYLE_H__

#include "GLineStyle.h"

namespace gmap{

	class GMAPCORE_API GSimpleLineStyle:public GLineStyle
	{
	public:
		GSimpleLineStyle(GStyleGroup* group,u32 id,const wstring& name);

		~GSimpleLineStyle(void);

		virtual bool IsSupportRenderScale();
		//
		virtual void SetRenderScale(double size);

		void ResetDefault() 
		{
			Scale0 = 1.0;
		}

		f32 GetLineWidth();

		void SetLineWidth(f32 lineWidth);

		const Color& GetColor();

		void SetColor(const Color& color);

		DashStyle GetDashStyle();

		void SetDashStyle(DashStyle dashStyle);

		LineCap	GetStartCap();

		void SetStartCap(LineCap cap);

		LineCap GetEndCap();

		void SetEndCap(LineCap cap);

		LineJoin GetLineJoin();

		void SetLineJoin(LineJoin lineJion);

		DashCap	GetDashCap();

		void SetDashCap(DashCap dashCap);

		//!线的风格类型
		virtual GLineStyle::EnumLineStyleType	GetLineStyleType();

		//!绘制一条线
		virtual void Draw(Graphics* graphics,PointF* points,UINT count,bool closed);

		//!绘制一条线
		virtual void Draw(Graphics* graphics,GraphicsPath* path);

		//!保存到流中
		virtual void Save(IWriteFile* pStream);

		//!从流中读取
		virtual void Load(IReadFile* pStream);
		
	private:
		void CheckDirty();
	private:
		f32			LineWidth;
		f64		    Scale0;
		Color		LineColor;
		DashStyle	LineDashStyle;
		DashCap		LineDashCap;
		LineCap     LineStartCap;
		LineCap		LineEndCap;
		LineJoin	_LineJoin;
		Pen*		CachedPen;			
		bool		Dirty;
	};

}

#endif