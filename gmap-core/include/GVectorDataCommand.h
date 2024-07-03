#ifndef __G_VECTOR_DATA_COMMAND_H__
#define __G_VECTOR_DATA_COMMAND_H__

#include "GCoreBase.h"
#include "GFieldDef.h"
#include "GGeometry.h"
#include "GFieldValue.h"
#include "GMapDrawing.h"
namespace gmap{

	class GVectorDataLayer;

	class GMAPCORE_API GVectorDataQueryResult:public GUnknown
	{
	public:
		//!是否已经结束
		virtual bool IsEOF()=0;
		//!重置
		virtual void Reset() = 0;
		//!移动
		virtual bool MoveNext()=0;

		//!得到Id
		virtual u32			GetId()=0;

		//!得到几何
		virtual GGeometry*	GetGeometry()=0;

		//!得到字段值
		virtual void		GetFieldValue(u32 index,_variant_t& val)=0;

		//!得到字段的值
		virtual void		GetFieldValue(const wstring& fieldName,_variant_t& val)=0;

		//!得到字段个数
		virtual  u32		GetFieldCount()=0;

		//!得到字段定义
		virtual  GFieldDef* GetFieldDef(u32 index)=0;
	};

	class GMAPCORE_API GVectorDataQuery:public GUnknown
	{
	public:
		enum EnumSpatialFilterType
		{
			SFT_NULL,
			SFT_BOUNDING_BOX,
			SFT_QCODE
		};
	public:
		GVectorDataQuery(bool geometryField);

		~GVectorDataQuery();
		
		//!添加字段
		void AddField(const wstring& name);

		//!得到字段个数
		u32 GetFieldCount();

		//!设置SQL
		void SetSqlWhere(const wstring& sql);

		//!设置包围盒
		void SetSpatialFilter(EnumSpatialFilterType filterType,GGeometry* filter);

		virtual GVectorDataQueryResult* Execute()=0;

		virtual void DrawDebug(GMapDrawing* pDrawing,EnumSpatialFilterType filterType,GGeometry* filter){}

	protected:
		//!
		virtual GVectorDataLayer* GetDataLayer()=0;
	protected:
		bool				   Dirty;
		wstring				   SqlWhere;
		GRectangle2dd		   BoundingBoxFilter;
		
		EnumSpatialFilterType SpatialFilterType;
		GGeometry*			  SpatialFilter;
		bool				  QueryGeometryField;
		vector<wstring>		  QueryFieldList;
	};

	//!插入命令
	class GMAPCORE_API GVectorDataInsert:public GUnknown
	{
	public:		
		GVectorDataInsert(){}

		virtual ~GVectorDataInsert(){}

		//!设置几何字段
		virtual void SetGeometry(GGeometry* pGeometry)=0;

		//!设置字段的值
		virtual void SetFieldValue(u32 index,_variant_t& val)=0;

		//!设置字段的值
		virtual void SetFieldValue(const wstring& fieldName,_variant_t& val)=0;

		//!执行
		virtual void Execute()=0;
	protected:

	};
}

#endif