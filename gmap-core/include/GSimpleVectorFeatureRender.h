#ifndef __G_SIMPLE_VECTOR_FEATURE_RENDER_H__
#define __G_SIMPLE_VECTOR_FEATURE_RENDER_H__

#include "GVectorFeatureRender.h"
#include "GStyleMapDrawing.h"
namespace gmap{

	class GMAPCORE_API GSimpleVectorFeatureRender:public GVectorFeatureRender
	{
	public:
		GSimpleVectorFeatureRender(GMarkerStyle* markStyle,GLineStyle* lineStyle,GFillStyle* fillStyle);
		~GSimpleVectorFeatureRender(void);

		virtual Type GetType() { return RT_SIMPLE; }
		void DrawFeature(GMapDrawing*  pDrawing,GVectorDataQueryResult* pCurrent);
		void InitQuery(GVectorDataQuery* pQuery);
		GMarkerStyle* GetMarkerStyle();
		void SetMarkerStyle(GMarkerStyle* pStyle);
		GLineStyle* GetLineStyle();
		void SetLineStyle(GLineStyle* pStyle);
		GFillStyle* GetFillStyle();
		void SetFillStyle(GFillStyle* pStyle);
		virtual void begin();
	private:
		void DrawGeometry(GMapDrawing*  pDrawing,GGeometry* pGeometry);
	private:
		GMarkerStyle* MarkerStyle;
		GLineStyle*   LineStyle;
		GFillStyle*	  FillStyle;
	};
}


#endif