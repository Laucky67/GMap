#ifndef __G_LAYER_H__
#define __G_LAYER_H__

#include "GUnknown.h"
#include "GGeometry.h"
#include "IFile.h"
namespace gmap{

	class GMap;
	class GMapDrawing;
	class GMapEditObject;

	//!图层
	class GMAPCORE_API GMapLayer:public GUnknown
	{
	public:
		enum EnumLayerType
		{
			LT_UNKNOWN,
			LT_GRAPHICS,
			LT_VECTOR,
			LT_IMAGE,
			LT_RASTER,
			LT_TIN
		};
	public:
		GMapLayer();

		virtual ~GMapLayer(void);

		//!绘制
		virtual void Draw(GMapDrawing* pDrawing);

		//!得到包围盒
		virtual GRectangle2dd GetBoundingBox();

		//!设置默认包围盒
		void SetDefaultBoundingBox(GRectangle2dd& bbox);

		//!获取名字
		const wstring& GetName();

		//!设置名字
		void SetName(const wstring& name);

		//!可见性
		bool GetVisible();

		//!设置可见性
		void SetVisible(bool visible);
		
		virtual GMapLayer::EnumLayerType GetLayerType();

		//!保存
		virtual void Save(IWriteFile* pStream){}
		
		//!加载
		virtual void Load(IReadFile* pStream){}

		GMap*	GetMap();

		virtual void	AttachMap(GMap* map);

		virtual void UpdateCache() {}

		virtual GMapEditObject* HitTest(const GCoordinate& hitPoint,f64 hitTest){return NULL;}	
	protected:
		GMap*		  Map;
		wstring		  Name;
		GRectangle2dd DefaultBoundingBox;
		bool		  Visible;
	};

}
#endif