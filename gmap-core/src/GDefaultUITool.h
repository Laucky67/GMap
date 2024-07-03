#ifndef __G_DEFAULT_UI_TOOL_H__
#define __G_DEFAULT_UI_TOOL_H__

#include "GMapUITool.h"

namespace gmap{

	class GMAPCORE_API GDefaultUITool:public GMapUITool
	{
	public:
		GDefaultUITool(IMapUIContext* context);
		
		~GDefaultUITool(void);
		
		virtual bool OnMouseMove(s32 x,s32 y,s32 flags);

		virtual bool OnMouseDown(u32 x,s32 y,s32 button);

		virtual bool OnMouseUp(s32 x,s32 y,s32 button);

		virtual bool OnMouseWheel(s32 x,s32 y,u32 button,s32 delta);
	protected:
		s32 LastDownX;
		s32 LastDownY;
		s32 MouseDownX;
		s32 MouseDownY;
		bool LeftDown;
		
	};

}
#endif