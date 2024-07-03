#ifndef __G_OGR_IMPORTER_H__
#define __G_OGR_IMPORTER_H__

#include "GVectorDataLayer.h"
#include "GVectorDataSource.h"

class OGRLayer;

namespace gmap{
//!ogrµº»Î
class GOGRImporter
{
public:
	static void RegisterAll();
	static GVectorDataLayer* Import(const wstring& name,OGRLayer* pLayer,GVectorDataSource* pDataSource,IProgressUI* pUI=NULL,bool utf8=false);
	static GVectorDataLayer* ExportMiTab(GVectorDataLayer* pSrcLayer,char* file,char* name);
};
}
#endif