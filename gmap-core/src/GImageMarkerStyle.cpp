#include "GImageMarkerStyle.h"
namespace gmap{

	//!构造函数
	GImageMarkerStyle::GImageMarkerStyle(GStyleGroup* group,u32 id,const wstring& name):
		GMarkerStyle(group,id,name)
	{
		Dirty = false;
		CachedImage = NULL;
		LayoutRect = Rect(-8,-8,16,16);
	}

	GImageMarkerStyle::~GImageMarkerStyle(void)
	{	
		if(CachedImage)
		{
			delete CachedImage;
		}
	}

	const wstring& GImageMarkerStyle::GetImageFile()
	{
		return ImageFile;
	}

	void GImageMarkerStyle::SetImageFile(const wstring& imageFile)
	{
		if(ImageFile != imageFile)
		{
			ImageFile = imageFile;
			Dirty = true;
		}
	}

	void GImageMarkerStyle::SetLayoutRect(const Rect& layout)
	{
		LayoutRect = layout;
	}

	const Rect& GImageMarkerStyle::GetLayoutRect()
	{
		return LayoutRect;
	}

	GMarkerStyle::EnumMarkerStyleType GImageMarkerStyle::GetMarkerStyleType()
	{
		return GMarkerStyle::MST_IMAGE_STYLE;
	}

	void GImageMarkerStyle::Draw(Graphics* graphics,const PointF& point)
	{
		if(Dirty)
		{
			if(CachedImage){
				delete CachedImage;
				CachedImage=NULL;
			}
			CachedImage = Image::FromFile(ImageFile.c_str());
			Dirty =false;
		}

		if(CachedImage!=NULL)
		{
			RectF rect( LayoutRect.X,LayoutRect.Y,LayoutRect.Width,LayoutRect.Height);
			rect.Offset(point);
			graphics->DrawImage(CachedImage,rect);
		}
	}
	//!保存到流中
	void GImageMarkerStyle::Save(IWriteFile* pStream)
	{
		pStream->Write(&LayoutRect,sizeof(LayoutRect));
		FileUtil::WriteString(ImageFile,pStream);
	}

	//!从流中读取
	void GImageMarkerStyle::Load(IReadFile* pStream)
	{
		pStream->Read(&LayoutRect,sizeof(LayoutRect));
		ImageFile = FileUtil::ReadString(pStream);
		Dirty = true;
	}

}