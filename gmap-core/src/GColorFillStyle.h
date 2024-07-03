#ifndef __G_COLOR_FILL_STYLE_H__
#define __G_COLOR_FILL_STYLE_H__

#include "GFillStyle.h"
#include "IFile.h"

namespace gmap{

	class GMAPCORE_API GColorFillStyle:public GFillStyle
	{
	public:
		GColorFillStyle(GStyleGroup* group,u32 id,const wstring& name);

		~GColorFillStyle(void);

		virtual GFillStyle::EnumFillStyleType GetFillStyleType();

		Color GetFillColor();

		void SetFillColor(const Color& color);

		//!保存到流中
		virtual void Save(IWriteFile* pStream);

		//!从流中读取
		virtual void Load(IReadFile* pStream);

	protected:
		virtual void CheckDirty();
	private:
		Color FillColor;

	};

}

#endif