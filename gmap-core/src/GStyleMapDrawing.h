#ifndef __STYLE_DRAWING_GEOMETRY_H__
#define __STYLE_DRAWING_GEOMETRY_H__

#include "GStyleGroup.h"
#include "GPoint.h"
#include "GLineString.h"
#include "GPolygon.h"
#include "GMultiGeometry.h"
#include "GMapDrawing.h"

namespace gmap
{
	class GStyleMapDrawing
	{
	public:

		 static  inline void DrawPoint(GMapDrawing* pDrawing, GPoint* pGeometry, GMarkerStyle* pMakerStyle)
		{
			GPoint* Point = pGeometry;
			GCoordinate coordinate;
			Point->GetCoordinate(&coordinate);
			PointF point;
			pDrawing->MapToView(coordinate,point);

			if(pMakerStyle)
			{
				pMakerStyle->Draw(pDrawing->GetGraphics(),point);
			}
		}

		static inline void DrawLineString(GMapDrawing* pDrawing,GLineString* pGeometry,GLineStyle* pLineStyle)
		{	
			GLineString* LineString = pGeometry;

			int count = LineString->GetCoordinateCount();
			//GCoordinate* coordinates=new GCoordinate[count];
			static GCoordinate coordinates[10000];
			LineString->GetCoordinates(coordinates);
			//PointF* points=new PointF[count];
			static PointF points[10000];
			pDrawing->MapToView(coordinates,points,count,&count);

			if(pLineStyle)
			{
				pLineStyle->Draw(pDrawing->GetGraphics(),points,count,false);
			}

			//delete[] coordinates;
			//delete[] points;
		}

		static inline void DrawPolygon(GMapDrawing* pDrawing,GPolygon* pGeometry,GLineStyle* pLineStyle,GFillStyle* pFillStyle)
		{
			GPolygon* Polygon = pGeometry;
			GLinearRing* externalRing = Polygon->GetExternalRing();

			vector<GraphicsPath*> paths;
			GraphicsPath path0;
			{
				int count = externalRing->GetCoordinateCount();
				GCoordinate* coordinates=new GCoordinate[count];

				externalRing->GetCoordinates(coordinates);
				PointF* points=new PointF[count];
				pDrawing->MapToView(coordinates,points,count,&count);

				path0.AddPolygon(points,count);
				delete[] coordinates;
				delete[] points;
				paths.push_back(path0.Clone());
			}

			Region region(&path0);

			for(int i=0;i<Polygon->GetInternalRingCount();i++)
			{
				GLinearRing* ring = Polygon->GetInternalRingAt(i);
				int count = ring->GetCoordinateCount();
				GCoordinate* coordinates=new GCoordinate[count];

				ring->GetCoordinates(coordinates);
				PointF* points=new PointF[count];
				pDrawing->MapToView(coordinates,points,count,&count);
				GraphicsPath path1;
				path1.AddPolygon(points,count);

				paths.push_back(path1.Clone());

				path1.CloseFigure();

				region.Exclude(path1.Clone());

				delete[] coordinates;
				delete[] points;

			}
			if(pFillStyle)
			{
				pFillStyle->Draw(pDrawing->GetGraphics(),&region);
			}

			for(int i=0;i<paths.size();i++)
			{
				if(pLineStyle)
				{
					pLineStyle->Draw(pDrawing->GetGraphics(),paths[i]);
				}
				delete paths[i];
			}
		}

	};

}

#endif