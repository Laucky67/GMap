#include "GVectorMapLayer.h"
#include "GVectorFeatureRender.h"
#include "GMapDrawing.h"
#include "GVectorDataCommand.h"
#include "GVectorDataLayer.h"
#include "GMap.h"
#include "GGraphicsLayer.h"
#include "GStyleMapDrawing.h"

namespace gmap{

		static wstring _default_map_drawing(L"_default_map_drawing");

		GVectorMapLayer::GVectorMapLayer()
		{
			DataLayer = NULL;
			FeatureRender = NULL;
			CacheQuery = NULL;
		}

		GVectorMapLayer::~GVectorMapLayer(void)
		{
			if(DataLayer)DataLayer->Release();
			if(FeatureRender)FeatureRender->Release();
			if (CacheQuery)CacheQuery->Release();
		}

		
		GVectorFeatureRender* GVectorMapLayer::GetRender()
		{
			return FeatureRender;
		}

		void GVectorMapLayer::ResetRender(GVectorFeatureRender * render)
		{
			if (render)
			{
				if (FeatureRender)
					FeatureRender->Release();
				FeatureRender = render;
				FeatureRender->AddRef();
			}
			if (CacheQuery)
			{
				CacheQuery->Release();
				CacheQuery = NULL;
			}
		}

		//!绘制
		void GVectorMapLayer::Draw(GMapDrawing* pDrawing)
		{
			if (!DataLayer)
				return;
			if (!FeatureRender)
				return;

			GRectangle2dd vb = pDrawing->GetViewBounds();

			GCoordinateSystem* pOut = Map->GetCoordinateSystem();
			GCoordinateSystem* pIn =  DataLayer->GetCoordinateSystem();

			IGeoTransform* pTransformer1= GCoordinateSystem::CreateTransform(pOut,pIn);
			if(pTransformer1)
			{
				double x0,y0,x1,y1;
				pTransformer1->Transform(vb.MinEdge.X,vb.MinEdge.Y,&x0,&y0);
				pTransformer1->Transform(vb.MaxEdge.X,vb.MaxEdge.Y,&x1,&y1);

				GRectangle2dd rect0(x0,y0,x1,y1);
				rect0.Repair();
				vb=rect0;

				pTransformer1->Release();
			}

			GPolygon* pViewPolygon = GPolygon::Create(vb);

			if (!CacheQuery)
			{
				CacheQuery = DataLayer->CreateMapDrawingQueryCommand();
				FeatureRender->InitQuery(CacheQuery);
			}

			CacheQuery->SetSpatialFilter(GVectorDataQuery::SFT_BOUNDING_BOX,pViewPolygon);
			GVectorDataQueryResult* pResult = CacheQuery->Execute();
			
			IGeoTransform* pTransformer2= GCoordinateSystem::CreateTransform(pIn,pOut);
			pResult->Reset();
			FeatureRender->begin();
			while(!pResult->IsEOF())
			{	
				if(pTransformer2)
				{
					pResult->GetGeometry()->Transform(pTransformer2);			
				}
				FeatureRender->DrawFeature(pDrawing,pResult);
				pResult->MoveNext();
			}

			if(pTransformer2)
			{
				pTransformer2->Release();
			}
			pViewPolygon->Release();
			pResult->Release();
			FeatureRender->end();
		}

		//!得到包围盒
		GRectangle2dd GVectorMapLayer::GetBoundingBox()
		{
			GCoordinateSystem* pOut = Map->GetCoordinateSystem();
			GCoordinateSystem* pIn =  DataLayer->GetCoordinateSystem();

			
			GRectangle2dd rect = DataLayer->GetBoundingBox();

			IGeoTransform* pTransformer= GCoordinateSystem::CreateTransform(pIn,pOut);
			if(pTransformer)
			{
				double x0,y0,x1,y1;
				pTransformer->Transform(rect.MinEdge.X,rect.MinEdge.Y,&x0,&y0);
				pTransformer->Transform(rect.MaxEdge.X,rect.MaxEdge.Y,&x1,&y1);

				GRectangle2dd rect0(x0,y0,x1,y1);
				rect0.Repair();
				rect=rect0;

				pTransformer->Release();
			}

			return rect;
		}

		//!得到图层的类型
		GMapLayer::EnumLayerType GVectorMapLayer::GetLayerType()
		{
			return GMapLayer::LT_VECTOR;
		}

		//!得到对应的数据图层
		GVectorDataLayer*	GVectorMapLayer::GetDataLayer()
		{
			return DataLayer;
		}
		void GVectorMapLayer::SetDataLayer(GVectorDataLayer * dataLayer)
		{
			DataLayer = dataLayer;
			DataLayer->AddRef();
			if (CacheQuery)
				CacheQuery->Release();
		}
}