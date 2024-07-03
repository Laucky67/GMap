#ifndef __GSimpleAddElementTool_H__
#define __GSimpleAddElementTool_H__

#include "GMapUITool.h"
#include "GMap.h"
#include "GGraphicsLayer.h"
#include "GMapDrawing.h"
namespace gmap{

	//!
	class GMAPCORE_API GAddPointElementTool:public GMapUITool
	{
	public:
		GAddPointElementTool(IMapUIContext* context);

		~GAddPointElementTool(void);

		virtual bool OnMouseDown(u32 x, s32 y, s32 button);

		virtual void Draw(Graphics* graphics);
	};

	//////////////////////////////////////////////////////////////////////////////////
	class GMAPCORE_API GAddLineElementTool:public GMapUITool
	{
	public:
		GAddLineElementTool(IMapUIContext* context);

		~GAddLineElementTool(void);

		virtual bool OnMouseDown(u32 x, s32 y, s32 button);

		virtual void Draw(Graphics* graphics);
	private:
		vector<GCoordinate> Points;
	};

		//////////////////////////////////////////////////////////////////////////////////
	class GMAPCORE_API GAddPolygonElementTool:public GMapUITool
	{
	public:
		GAddPolygonElementTool(IMapUIContext* context);

		~GAddPolygonElementTool(void);

		virtual bool OnMouseDown(u32 x, s32 y, s32 button);

		virtual void Draw(Graphics* graphics);
	private:
		vector<GCoordinate> Points;
	};

}
#endif