#ifndef __G_SIMPLE_EDIT_TOOL_H__
#define __G_SIMPLE_EDIT_TOOL_H__

#include "GMapUITool.h"
#include "GMapEditObject.h"
#include "GMapLayer.h"
#include "GMapDrawing.h"
#include "GMap.h"

namespace gmap{

	class GMAPCORE_API GSimpleEditTool:public GMapUITool
	{
	public:
		GSimpleEditTool(IMapUIContext* context);
		
		~GSimpleEditTool();
		virtual bool OnMouseMove(s32 x, s32 y, s32 flags);

		virtual bool OnMouseDown(u32 x, s32 y, s32 button);

		virtual bool OnMouseUp(s32 x, s32 y, s32 button);

		virtual bool OnKeyDown(u32 key);

		virtual bool OnKeyUp(u32 key);

		virtual bool OnMouseWheel(s32 x, s32 y, u32 button, s32 delta);

		virtual void Draw(Graphics* graphics);

	private:
		GMapEditObject* CurrentEditObject;
		bool			Editing;
	
	};
}
#endif