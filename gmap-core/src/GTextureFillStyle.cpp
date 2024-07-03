#include "GTextureFillStyle.h"
namespace gmap{

	GTextureFillStyle::GTextureFillStyle(GStyleGroup* group,u32 id,const wstring& name)
		:GFillStyle(group,id,name)
	{
		CachedImage = NULL;
		TextureWrapMode = WrapModeTile;
	}

	GTextureFillStyle::~GTextureFillStyle(void)
	{
		if(CachedImage)
		{
			delete CachedImage;
		}
	}

	GFillStyle::EnumFillStyleType GTextureFillStyle::GetFillStyleType()
	{
		return GFillStyle::FTS_TEXTURE_FILL_STYLE;
	}

	const wstring& GTextureFillStyle::GetImageFile()
	{
		return ImageFile;
	}

	void GTextureFillStyle::SetImageFile(const wstring& imageFile)
	{
		ImageFile = imageFile;
		Dirty = true;
	}

	WrapMode GTextureFillStyle::GetWrapMode()
	{
		return TextureWrapMode;
	}

	void GTextureFillStyle::SetWrapMode(WrapMode wm)
	{
		TextureWrapMode = wm;
		Dirty = true;
	}

	void GTextureFillStyle::CheckDirty()
	{
		if(Dirty)
		{
			if(CachedImage)
			{
				delete CachedImage;
				CachedImage = NULL;
			}
			if(CachedBrush)
			{
				delete CachedBrush;
				CachedBrush = NULL;
			}

			CachedImage = Image::FromFile(ImageFile.c_str());
			if(CachedImage)
			{
				TextureBrush* tb = new TextureBrush(CachedImage,TextureWrapMode);
				CachedBrush = tb;
			}
			Dirty = false;
		}
	}
	
	//!保存到流中
	void GTextureFillStyle::Save(IWriteFile* pStream)
	{
		FileUtil::WriteString(ImageFile,pStream);
		pStream->Write(TextureWrapMode);
	}

	//!从流中读取
	void GTextureFillStyle::Load(IReadFile* pStream)
	{
		ImageFile = FileUtil::ReadString(pStream);
		pStream->Read(TextureWrapMode);
	}
}