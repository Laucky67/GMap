#ifndef __G_STYLE_H__
#define __G_STYLE_H__

#include "GUnknown.h"
#include "IFile.h"
namespace gmap{

	class GStyleGroup;
	class GMAPCORE_API GStyle:public GUnknown
	{
	public:
		enum EnumStyleType
		{
			ST_UNKNOWN_STYLE,
			ST_MARKER_STYLE,
			ST_LINE_STYLE,
			ST_FILL_STYLE
		};

	public:
		GStyle(GStyleGroup* group,u32 id,const wstring& name);
		
		~GStyle(void);

		GStyleGroup* GetGroup();

		u32	GetId();
		//!名字
		const wstring& GetName();

		//!名字
		void SetName(const wstring& name);

		//!Style Type
		virtual GStyle::EnumStyleType GetStyleType();

		void ResetDefault(){}
		//
		virtual bool IsSupportRenderScale() { return false; }
		//
		virtual void SetRenderScale(double size) {}
		//
		virtual bool IsSupportRenderColor() { return false; }
		//
		virtual void SetRenderColor(const Gdiplus::Color& clr) {}

		//!保存到流中
		virtual void Save(IWriteFile* pStream){}

		//!从流中读取
		virtual void Load(IReadFile* pStream){}

	private:
		void AttachGroup(GStyleGroup* group,u32 id){Group= group;Id = id;}
		friend class GStyleGroup;
	protected:
		GStyleGroup* Group;
		u32			 Id;
		wstring		 Name;
	};


}

#endif