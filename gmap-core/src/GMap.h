#ifndef __G_MAP_H__
#define __G_MAP_H__
#include "GCoreBase.h"
#include "GStyleGroup.h"
#include "GCoordinateSystem.h"
namespace gmap{

	class GMapDrawing;
	class GMapLayer;
	class GGraphicsLayer;

	//!地图
	class GMAPCORE_API GMap:public GUnknown
	{
	public:
		 GMap(const wstring& name,GStyleGroup* styleGroup,GCoordinateSystem* CS=NULL);

		~GMap(void);

		//!地图名称
		const wstring&	GetName();

		//!地图名称
		void SetName(const wstring& name);

		//!得到StyleGroup
		GStyleGroup*	GetStyleGroup();

		//!绘制
		void Draw(GMapDrawing* pDrawing);

		//!得到图层的个数
		u32			GetLayerCount();

		//!得到图层
		GMapLayer*	GetLayer(u32 index);

		//!得到图层
		GMapLayer*	GetLayer(const wstring& name);

		GMapLayer*	GetCurrentLayer();

		GGraphicsLayer*	GetDefaultGraphicsLayer();

		void SetCurrentLayer(GMapLayer* layer);

		//!添加图层
		void AddLayer(GMapLayer* pLayer);

		//!移除图层
		void RemoveLayer(u32 iLayer);

		//!上移图层
		bool MoveLayer(u32 fromLayer,u32 toLayer);

		//!移除所有图层
		void ClearLayers();

		//!得到包围盒
		GRectangle2dd GetMapExtent();

		//!设置地图坐标系
		void SetCoordinateSystem(GCoordinateSystem* pCS);
		
		//!坐标系
		GCoordinateSystem*		GetCoordinateSystem();
	private:
		wstring Name;
		vector<GMapLayer*>	Layers;
		GStyleGroup*		StyleGroup;
		GMapLayer*			CurrentLayer;
		GCoordinateSystem*	CoordinateSystem;
	};

}
#endif
