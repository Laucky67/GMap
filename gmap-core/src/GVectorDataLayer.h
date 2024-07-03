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

	//!矢量数据图层
	class GMAPCORE_API GVectorDataLayer:public GUnknown
	{
	public:
		//!矢量数据图层的类型
		enum EnumVectorLayerType
		{
			VLT_UNKNOWN,
			VLT_POINT,
			VLT_LINE_STRING,
			VLT_POLYGON,
			VLT_GEOMETRY_COLLECTION=0x10
		};

		//!空间索引的类型
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

		//!得到名字
		virtual const wstring& GetName();

		//!图层类型
		virtual EnumVectorLayerType GetLayerType()=0;

		//!得到字段的个数
		virtual u32 GetFieldCount();

		//!得到字段的定义
		virtual GFieldDef* GetFieldDef(u32 index);

		//!得到字段定义
		virtual GFieldDef*  GetFieldDef(const wstring& name);

		//!得到包围盒
		virtual const GRectangle2dd& GetBoundingBox()=0;

		//!删除
		virtual bool DeleteFeature(u32 id){return false;}

		//!构建索引
		virtual void UpdateSpatialIndex(IProgressUI* pUI=NULL){}

		//!是否有空间索引
		EnumSpatialIndexType GetSpatialIndexType(){return SIT_NONE;}
		
		//!创建新的数据查询
		virtual GVectorDataQuery*  CreateQueryCommand(bool bGeometry){return NULL;}

		//!创建数据制图查询
		virtual GVectorDataQuery*  CreateMapDrawingQueryCommand(){return NULL;}

		//!得到数据插入
		virtual GVectorDataInsert* CreateInsertCommand(){return NULL;}

		//!获得坐标系
		GCoordinateSystem*	GetCoordinateSystem();

		//!设置坐标系
		virtual void		SetCoordinateSystem(GCoordinateSystem* pCS);
		
	protected:
		wstring Name;
		vector<GFieldDef*>	FieldDefList;
		EnumVectorLayerType LayerType;
		GCoordinateSystem*	CoordinateSystem;

	};

}

#endif