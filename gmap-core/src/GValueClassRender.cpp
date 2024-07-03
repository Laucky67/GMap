#include "GValueClassRender.h"
#include "GVectorDataCommand.h"
#include "GStyleMapDrawing.h"
namespace gmap {

	GValueClassRender::GValueClassRender()
	{
		DefaultStyle = NULL;
	}

	GValueClassRender::~GValueClassRender()
	{
		for (auto it = Styles.begin(), end = Styles.end(); it != end; it++)
		{
			delete* it;
		}
	}

	void GValueClassRender::InitQuery(GVectorDataQuery * pQuery)
	{
		pQuery->AddField(ClassField);
	}

	void GValueClassRender::DrawFeature(GMapDrawing * pDrawing, GVectorDataQueryResult * pCurrent)
	{
		_variant_t val;
		pCurrent->GetFieldValue(0,val);
		int ival = (int)val;
		StyleClassItem* style = DefaultStyle;
		auto it = StyleMap.find(ival);
		if (it != StyleMap.end())
		{
			style = it->second;
			DrawGeometry(pDrawing, pCurrent->GetGeometry(), style);
		}
		else if(DefaultStyle)
		{
			DrawGeometry(pDrawing, pCurrent->GetGeometry(), DefaultStyle);
		}
		else
		{
			auto it = StyleMap.find(-1);
			if (it != StyleMap.end())
			{
				style = it->second;
				DefaultStyle = style;
			}
			DrawGeometry(pDrawing, pCurrent->GetGeometry(), DefaultStyle);
		}
	}

	int GValueClassRender::GetCount()
	{
		return Styles.size();
	}

	GValueClassRender::StyleClassItem * GValueClassRender::AddItem(int clsid)
	{
		if (StyleMap.find(clsid) == StyleMap.end())
		{
			StyleClassItem* item = new StyleClassItem();
			Styles.push_back(item);
			item->ClassValue = clsid;
			StyleMap[clsid] = item;
			return item;
		}
		else
		{
			return StyleMap[clsid];
		}
	}

	GValueClassRender::StyleClassItem * GValueClassRender::GetItem(int ix)
	{
		return Styles[ix];
	}

	void gmap::GValueClassRender::RemoveItem(int ix)
	{
		StyleClassItem* it = Styles[ix];
		StyleMap.erase(it->ClassValue);
		delete it;
		Styles.erase(Styles.begin()+ix);
	}

	void GValueClassRender::SetClassField(const wstring & classField)
	{
		ClassField = classField;
	}

	const wstring & GValueClassRender::GetClassField()
	{
		return ClassField;
	}

	GValueClassRender::StyleClassItem * GValueClassRender::GetItemByClass(int clsid)
	{
		for (auto it = Styles.begin(), end = Styles.end(); it != end; it++)
		{
			if ((*it)->ClassValue == clsid)
				return *it;
		}
		return NULL;
	}

	void GValueClassRender::RemoveByClass(int clsid)
	{
		if (StyleMap.find(clsid) != StyleMap.end())
		{
			for (auto it = Styles.begin(), end = Styles.end(); it != end; it++)
			{
				if ((*it)->ClassValue == clsid)
				{
					delete *it;
					Styles.erase(it);
					break;
				}
			}
			StyleMap.erase(clsid);
		}
	}

	void GValueClassRender::DrawGeometry(GMapDrawing * pDrawing, GGeometry * pGeometry, GValueClassRender::StyleClassItem * style)
	{
		GGeometry::EnumGeometryType gtype = pGeometry->GetGeometryType();

		if (gtype == GGeometry::GT_POINT)
		{
			GStyleMapDrawing::DrawPoint(pDrawing, (GPoint*)pGeometry, style->MStyle);
		}
		else if (gtype == GGeometry::GT_LINE_STRING)
		{
			GStyleMapDrawing::DrawLineString(pDrawing, (GLineString*)pGeometry, style->LStyle);
		}
		else if (gtype == GGeometry::GT_POLYGON)
		{
			GStyleMapDrawing::DrawPolygon(pDrawing, (GPolygon*)pGeometry, style->LStyle, style->FStyle);
		}
		else if ((gtype&GGeometry::GT_COLLECTION) != NULL)
		{
			GMultiGeometry* pGeometries = (GMultiGeometry*)pGeometry;
			for (int i = 0; i < pGeometries->GetGeometryCount(); i++)
			{
				DrawGeometry(pDrawing, pGeometries->GetGeometryAt(i),style);
			}
		}
	}
}
