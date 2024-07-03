#ifndef __G_IMAGE_MARKER_STYLE_H__
#define __G_IMAGE_MARKER_STYLE_H__

#include "GMarkerStyle.h"

namespace gmap{

	//!ͼ��Marker
	class GMAPCORE_API GImageMarkerStyle:public GMarkerStyle
	{
	public:
		//!���캯��
		GImageMarkerStyle(GStyleGroup* group,u32 id,const wstring& name);

		~GImageMarkerStyle(void);

		const wstring& GetImageFile();

		void SetImageFile(const wstring& imageFile);
		
		void SetLayoutRect(const Rect& layout);

		const Rect& GetLayoutRect();
		
		virtual GMarkerStyle::EnumMarkerStyleType GetMarkerStyleType();

		virtual void Draw(Graphics* graphics,const PointF& point);

		//!���浽����
		virtual void Save(IWriteFile* pStream);

		//!�����ж�ȡ
		virtual void Load(IReadFile* pStream);
	private:
		wstring ImageFile;
		Image*	CachedImage;
		Rect	LayoutRect;
		bool	Dirty;
	};

}
#endif