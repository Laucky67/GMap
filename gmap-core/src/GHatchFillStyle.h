#ifndef __G_HATCH_FILL_STYLE_H__
#define __G_HATCH_FILL_STYLE_H__

#include "GFillStyle.h"

namespace gmap{

	class GMAPCORE_API GHatchFillStyle:public GFillStyle
	{
	public:
		GHatchFillStyle(GStyleGroup* group,u32 id,const wstring& name);

		~GHatchFillStyle(void);

		virtual GFillStyle::EnumFillStyleType GetFillStyleType();

		Color GetForeColor();

		void SetForeColor(const Color& color);

		Color GetBackColor();

		void SetBackColor(const Color& color);

		HatchStyle	GetHatchStyle();

		void		SetHatchStyle(HatchStyle hatchStyle);

		//!保存到流中
		void Save(IWriteFile* pStream);

		//!从流中读取
		void Load(IReadFile* pStream);

	protected:
		virtual void CheckDirty();
	private:
		Color		ForeColor;
		Color		BackColor;
		HatchStyle  FillHatchStyle;
	};

}

#endif