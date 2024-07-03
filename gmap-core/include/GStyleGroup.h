#ifndef __G_STYLE_GROUP_H__
#define __G_STYLE_GROUP_H__

#include "GUnknown.h"
#include "GStyle.h"
#include "GImageMarkerStyle.h"
#include "GSimpleLineStyle.h"
#include "GColorFillStyle.h"
#include "GHatchFillStyle.h"
#include "GTextureFillStyle.h"

namespace gmap{

	class GMAPCORE_API GStyleGroup:public GUnknown
	{
	public:
		GStyleGroup(const wstring& name);

		~GStyleGroup(void);

		const wstring& GetName();

		void AddStyle(GStyle* pStyle,u32 id=-1);

		void RemoveStyleAt(s32 index);

		void RemoveStyleById(u32 id);

		GStyle* GetStyleAt(s32 index);

		GStyle* GetStyleById(u32 id);

		u32 GetStyleCount();

		//!保存到流中
		void SaveToStream(IWriteFile* pStream);

		//!从流中加载
		static GStyleGroup* LoadFromStream(IReadFile* pStream);
	private:
		wstring			Name;
		vector<GStyle*>	Styles;
		u32				NextId;
	};

	void GMAPCORE_API DrawStyle(Graphics* graphics,gmap::GStyle* pStyle,RectF rc);
}

#endif
