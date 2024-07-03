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
		//!�Ƿ��Ѿ�����
		virtual bool IsEOF()=0;
		//!����
		virtual void Reset() = 0;
		//!�ƶ�
		virtual bool MoveNext()=0;

		//!�õ�Id
		virtual u32			GetId()=0;

		//!�õ�����
		virtual GGeometry*	GetGeometry()=0;

		//!�õ��ֶ�ֵ
		virtual void		GetFieldValue(u32 index,_variant_t& val)=0;

		//!�õ��ֶε�ֵ
		virtual void		GetFieldValue(const wstring& fieldName,_variant_t& val)=0;

		//!�õ��ֶθ���
		virtual  u32		GetFieldCount()=0;

		//!�õ��ֶζ���
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
		
		//!����ֶ�
		void AddField(const wstring& name);

		//!�õ��ֶθ���
		u32 GetFieldCount();

		//!����SQL
		void SetSqlWhere(const wstring& sql);

		//!���ð�Χ��
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

	//!��������
	class GMAPCORE_API GVectorDataInsert:public GUnknown
	{
	public:		
		GVectorDataInsert(){}

		virtual ~GVectorDataInsert(){}

		//!���ü����ֶ�
		virtual void SetGeometry(GGeometry* pGeometry)=0;

		//!�����ֶε�ֵ
		virtual void SetFieldValue(u32 index,_variant_t& val)=0;

		//!�����ֶε�ֵ
		virtual void SetFieldValue(const wstring& fieldName,_variant_t& val)=0;

		//!ִ��
		virtual void Execute()=0;
	protected:

	};
}

#endif