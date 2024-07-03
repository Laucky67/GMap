#ifndef __G_UI_TOOL_H__
#define __G_UI_TOOL_H__

#include "GUnknown.h"
namespace gmap{

	class GMapEditObject;
	class GMapDrawing;
	class GMap;

	class IMapUIContext
	{
	public:
		//!�õ��߽��ͼ����
		virtual Graphics*		GetEditGraphics()=0;

		//!�ͷű༭��ͼ����
		virtual void			ReleaseEditGraphics()=0;

		//!�õ���ͼ�ӿ�
		virtual GMapDrawing*	GetDrawing()=0;

		//!�õ�������ͼ����
		virtual GMap*			GetMap() = 0;

		//!ˢ�µ�ͼ
		virtual void			Redraw()=0;

		//!����״̬�ı�
		virtual void			SetStatusText(u32 panel,const wstring& text)=0;

		//!ע��༭����
		virtual void			RegisterEditObject(GMapEditObject* pEditObject)=0;	
		
		//!���ƻ���ĵ�ͼ
		virtual void			DrawCache(int offsetX=0,int offsetY=0)=0;
	};

	class GMAPCORE_API GMapUITool:public GUnknown
	{
	public:
		GMapUITool(IMapUIContext* context)
		{
			MapUIContext =context;
		}
		virtual bool OnMouseMove(s32 x,s32 y,s32 flags){return false;}

		virtual bool OnMouseDown(u32 x,s32 y,s32 button){return false;}

		virtual bool OnMouseUp(s32 x,s32 y,s32 button){return false;}

		virtual bool OnKeyDown(u32 key){return false;}

		virtual bool OnKeyUp(u32 key){return false;}

		virtual bool OnMouseWheel(s32 x,s32 y,u32 button,s32 delta){return false;}

		virtual void Draw(Graphics* graphics){}
	protected:
		IMapUIContext* MapUIContext;
	};

}



#endif