#ifndef __G_LAYER_H__
#define __G_LAYER_H__

#include "GUnknown.h"
#include "GGeometry.h"
#include "IFile.h"
namespace gmap{

	class GMap;
	class GMapDrawing;
	class GMapEditObject;

	//!ͼ��
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

		//!����
		virtual void Draw(GMapDrawing* pDrawing);

		//!�õ���Χ��
		virtual GRectangle2dd GetBoundingBox();

		//!����Ĭ�ϰ�Χ��
		void SetDefaultBoundingBox(GRectangle2dd& bbox);

		//!��ȡ����
		const wstring& GetName();

		//!��������
		void SetName(const wstring& name);

		//!�ɼ���
		bool GetVisible();

		//!���ÿɼ���
		void SetVisible(bool visible);
		
		virtual GMapLayer::EnumLayerType GetLayerType();

		//!����
		virtual void Save(IWriteFile* pStream){}
		
		//!����
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