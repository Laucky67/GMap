#ifndef __G_VECTOR_FEATURE_RENDER_H__
#define __G_VECTOR_FEATURE_RENDER_H__

#include "GCoreBase.h"
#include "GUnknown.h"
#include "IFile.h"
namespace gmap{

	class GVectorDataQueryResult;
	class GVectorDataQuery;
	class GMapDrawing;

	class GVectorFeatureRender:public GUnknown
	{
	public:
		enum Type
		{
			RT_UNKNOWN,
			RT_SIMPLE,
			RT_VALUE_CLASS,
			RT_SIZE,
			RT_XXXX
		};
	public:
		virtual Type GetType() { return RT_UNKNOWN; }
		virtual void begin(){}
		virtual void InitQuery(GVectorDataQuery* pQuery)=0;	
		virtual void DrawFeature(GMapDrawing*   pDrawing,GVectorDataQueryResult* pCurrent)=0;
		virtual void Load(IReadFile*	stream) {}
		virtual void Save(IWriteFile*	stream) {}
		virtual void end() {}
	};
}


#endif