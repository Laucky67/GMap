#ifndef __GVectorDataQueryMDB_H__
#define __GVectorDataQueryMDB_H__

#include "GVectorDataCommand.h"
#include "adoInclude.h"

namespace gmap{

	class GVectorDataLayerMDB;

	class GVectorDataQueryMDB:public GVectorDataQuery
	{
	public:
		GVectorDataQueryMDB(GVectorDataLayerMDB* pDataLayer,bool geomertyField);

		~GVectorDataQueryMDB(void);
		
		virtual GVectorDataQueryResult* Execute();	

		virtual void DrawDebug(GMapDrawing* pDrawing,EnumSpatialFilterType filterType,GGeometry* filter);
	
	protected:
		//!
		virtual GVectorDataLayer* GetDataLayer();

		friend class GVectorDataQueryResultMDB;

	private:
		GVectorDataLayerMDB*	  DataLayerMDB;
		int a,b,c;


	};

}

#endif
