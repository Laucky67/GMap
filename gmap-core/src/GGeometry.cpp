#include "GGeometry.h"
#include "GPoint.h"
#include "GLineString.h"
#include "GLinearRing.h"
#include "GPolygon.h"
#include "GMultiGeometry.h"
namespace gmap{

	//!从流中创建
	GGeometry* GGeometry::CreateFromStream(IReadFile* pStream)
	{
		GGeometry::EnumGeometryType gtype;
		pStream->Read(&gtype,sizeof(gtype));
		
		if(gtype == GGeometry::GT_POINT)
		{
			GPoint* point=GPoint::Create(0);
			point->Load(pStream);
			return point;
		}
		else if(gtype==GGeometry::GT_LINE_STRING)
		{
			GLineString* lineString = GLineString::Create();
			lineString->Load(pStream);
			return lineString;
		}
		else if(gtype==GGeometry::GT_LINEAR_RING)
		{
			GLinearRing* linearRing = GLinearRing::Create();
			linearRing->Load(pStream);
			return linearRing;
		}
		else if(gtype==GGeometry::GT_POLYGON)
		{
			GPolygon* polygon = GPolygon::Create();
			polygon->Load(pStream);
			return polygon;
		}
		else if((gtype&GGeometry::GT_COLLECTION)==GGeometry::GT_COLLECTION)
		{
			GMultiGeometry* pMultiGeometry = GMultiGeometry::Create(gtype);
			pMultiGeometry->Load(pStream);
			return pMultiGeometry;
		}
		else
		{
			return NULL;
		}
	}
	//!保存到流中
	void GGeometry::SaveToStream(GGeometry* pGeometry,IWriteFile* pStream)
	{	
		GGeometry::EnumGeometryType gtype;
		gtype = pGeometry->GetGeometryType();
		pStream->Write(&gtype,sizeof(gtype));

		pGeometry->Save(pStream);
	}

}