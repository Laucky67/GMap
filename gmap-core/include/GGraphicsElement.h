#ifndef __G_GRAPHICS_ELEMENT_H__
#define __G_GRAPHICS_ELEMENT_H__
#include "GMapDrawing.h"
#include "GStyleGroup.h"
namespace gmap{

	class GMapDrawing;
	class GGraphicsLayer;
	class GMapEditObject;

	class GMAPCORE_API GGraphicsElement:public GUnknown
	{
	public:
		enum EnumGraphicsElmementType
		{
			GE_UNKNOWN,
			GE_POINT,
			GE_LINE,
			GE_POLYGON
		};
	public:
		GGraphicsElement(void);

		~GGraphicsElement(void);

		//!得到Id
		u32 GetId();

		//!得到图层
		GGraphicsLayer* GetLayer();

		virtual EnumGraphicsElmementType GetElementType()=0;

		//!绘制
		virtual void DrawElement(GMapDrawing* pDrawing)=0;

		//!得到包围盒
		virtual GRectangle2dd GetBoundingBox()=0;

		virtual void Save(IWriteFile* pStream)=0;

		virtual void Load(IReadFile* pStream)=0;

		virtual f64	HitTest(const GCoordinate& point)=0;

		virtual GMapEditObject* CreateEditObject()=0;
	private:
		friend class GGraphicsLayer;
		void AttachLayer(u32 id,GGraphicsLayer* layer);

	protected:
		u32				Id;
		GGraphicsLayer* Layer;
	};

}

#endif
