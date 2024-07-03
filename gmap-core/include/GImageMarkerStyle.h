#ifndef __G_IMAGE_MARKER_STYLE_H__
#define __G_IMAGE_MARKER_STYLE_H__

#include "GMarkerStyle.h"

namespace gmap{

	//!图像Marker
	class GMAPCORE_API GImageMarkerStyle:public GMarkerStyle
	{
	public:
		//!构造函数
		GImageMarkerStyle(GStyleGroup* group,u32 id,const wstring& name);

		~GImageMarkerStyle(void);

		const wstring& GetImageFile();

		void SetImageFile(const wstring& imageFile);
		
		void SetLayoutRect(const Rect& layout);

		const Rect& GetLayoutRect();
		
		virtual GMarkerStyle::EnumMarkerStyleType GetMarkerStyleType();

		virtual void Draw(Graphics* graphics,const PointF& point);

		//!保存到流中
		virtual void Save(IWriteFile* pStream);

		//!从流中读取
		virtual void Load(IReadFile* pStream);
	private:
		wstring ImageFile;
		Image*	CachedImage;
		Rect	LayoutRect;
		bool	Dirty;
	};

}
#endif