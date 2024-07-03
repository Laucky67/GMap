#ifndef __G_MAP_H__
#define __G_MAP_H__
#include "GCoreBase.h"
#include "GStyleGroup.h"
#include "GCoordinateSystem.h"
namespace gmap{

	class GMapDrawing;
	class GMapLayer;
	class GGraphicsLayer;

	//!��ͼ
	class GMAPCORE_API GMap:public GUnknown
	{
	public:
		 GMap(const wstring& name,GStyleGroup* styleGroup,GCoordinateSystem* CS=NULL);

		~GMap(void);

		//!��ͼ����
		const wstring&	GetName();

		//!��ͼ����
		void SetName(const wstring& name);

		//!�õ�StyleGroup
		GStyleGroup*	GetStyleGroup();

		//!����
		void Draw(GMapDrawing* pDrawing);

		//!�õ�ͼ��ĸ���
		u32			GetLayerCount();

		//!�õ�ͼ��
		GMapLayer*	GetLayer(u32 index);

		//!�õ�ͼ��
		GMapLayer*	GetLayer(const wstring& name);

		GMapLayer*	GetCurrentLayer();

		GGraphicsLayer*	GetDefaultGraphicsLayer();

		void SetCurrentLayer(GMapLayer* layer);

		//!���ͼ��
		void AddLayer(GMapLayer* pLayer);

		//!�Ƴ�ͼ��
		void RemoveLayer(u32 iLayer);

		//!����ͼ��
		bool MoveLayer(u32 fromLayer,u32 toLayer);

		//!�Ƴ�����ͼ��
		void ClearLayers();

		//!�õ���Χ��
		GRectangle2dd GetMapExtent();

		//!���õ�ͼ����ϵ
		void SetCoordinateSystem(GCoordinateSystem* pCS);
		
		//!����ϵ
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
