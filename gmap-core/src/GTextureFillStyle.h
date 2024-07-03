#ifndef __G_TEXTURE_FILL_STYLE_H__
#define __G_TEXTURE_FILL_STYLE_H__

#include "GFillStyle.h"

namespace gmap{

	class GMAPCORE_API GTextureFillStyle:public GFillStyle
	{
	public:
		GTextureFillStyle(GStyleGroup* group,u32 id,const wstring& name);

		~GTextureFillStyle(void);

		
		virtual GFillStyle::EnumFillStyleType GetFillStyleType();

		const wstring& GetImageFile();

		void SetImageFile(const wstring& imageFile);

		WrapMode GetWrapMode();

		void SetWrapMode(WrapMode wm);

		//!���浽����
		virtual void Save(IWriteFile* pStream);

		//!�����ж�ȡ
		virtual void Load(IReadFile* pStream);

	protected:
		void CheckDirty();

	private:
		wstring		ImageFile;
		WrapMode	TextureWrapMode;
		Image*		CachedImage;
	};

}

#endif
