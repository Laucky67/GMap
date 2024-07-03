#ifndef __G_VECTOR_DATA_LAYER_H__
#define __G_VECTOR_DATA_LAYER_H__


#include "GCoreBase.h"
#include "GUnknown.h"
#include "GMapLayer.h"
#include "GCoordinateSystem.h"

namespace gmap{

	class GVectorDataSource;
	class GFieldDef;
	class GVectorDataCommand;
	class GVectorDataQuery;
	class GVectorDataUpdate;
	class GVectorDataInsert;
	class IProgressUI;

	//!ʸ������ͼ��
	class GMAPCORE_API GVectorDataLayer:public GUnknown
	{
	public:
		//!ʸ������ͼ�������
		enum EnumVectorLayerType
		{
			VLT_UNKNOWN,
			VLT_POINT,
			VLT_LINE_STRING,
			VLT_POLYGON,
			VLT_GEOMETRY_COLLECTION=0x10
		};

		//!�ռ�����������
		enum EnumSpatialIndexType
		{
			SIT_NONE,
			SIT_RTREE,
			SIT_GRID,
			SIT_TILED
		};
	public:
		GVectorDataLayer(const wstring& name);

		~GVectorDataLayer(void);

		virtual GVectorDataSource* GetDataSource()=0;

		//!�õ�����
		virtual const wstring& GetName();

		//!ͼ������
		virtual EnumVectorLayerType GetLayerType()=0;

		//!�õ��ֶεĸ���
		virtual u32 GetFieldCount();

		//!�õ��ֶεĶ���
		virtual GFieldDef* GetFieldDef(u32 index);

		//!�õ��ֶζ���
		virtual GFieldDef*  GetFieldDef(const wstring& name);

		//!�õ���Χ��
		virtual const GRectangle2dd& GetBoundingBox()=0;

		//!ɾ��
		virtual bool DeleteFeature(u32 id){return false;}

		//!��������
		virtual void UpdateSpatialIndex(IProgressUI* pUI=NULL){}

		//!�Ƿ��пռ�����
		EnumSpatialIndexType GetSpatialIndexType(){return SIT_NONE;}
		
		//!�����µ����ݲ�ѯ
		virtual GVectorDataQuery*  CreateQueryCommand(bool bGeometry){return NULL;}

		//!����������ͼ��ѯ
		virtual GVectorDataQuery*  CreateMapDrawingQueryCommand(){return NULL;}

		//!�õ����ݲ���
		virtual GVectorDataInsert* CreateInsertCommand(){return NULL;}

		//!�������ϵ
		GCoordinateSystem*	GetCoordinateSystem();

		//!��������ϵ
		virtual void		SetCoordinateSystem(GCoordinateSystem* pCS);
		
	protected:
		wstring Name;
		vector<GFieldDef*>	FieldDefList;
		EnumVectorLayerType LayerType;
		GCoordinateSystem*	CoordinateSystem;

	};

}

#endif