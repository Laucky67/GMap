#pragma once
#include "gmap-include.h"
namespace gmap {
	class GMapPanZoomTool :public GMapUITool
	{
	public:
		GMapPanZoomTool(IMapUIContext* context);
		~GMapPanZoomTool(void);
		virtual bool OnMouseMove(s32 x, s32 y, s32 flags);
		virtual bool OnMouseDown(u32 x, s32 y, s32 button);
		virtual bool OnMouseUp(s32 x, s32 y, s32 button);
		virtual bool OnMouseWheel(s32 x, s32 y, u32 button, s32 delta);
	protected:
		s32 LastDownX;
		s32 LastDownY;
		s32 MouseDownX;
		s32 MouseDownY;
		bool LeftDown;
	};
}


