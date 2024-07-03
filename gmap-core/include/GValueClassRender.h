#ifndef __G_VALUE_CLASS_RENDER_H__
#define __G_VALUE_CLASS_RENDER_H__
#include "GVectorFeatureRender.h"
#include "GStyle.h"
#include "GMarkerStyle.h"
#include "GLineStyle.h"
#include "GFillStyle.h"
#include "GGeometry.h"

namespace gmap {
	class GMAPCORE_API GValueClassRender :
		public GVectorFeatureRender
	{	
	public:
		struct StyleClassItem {
			StyleClassItem() { ClassValue = -1; MStyle = NULL; LStyle = NULL; FStyle = NULL; }
			~StyleClassItem() { if (MStyle)MStyle->Release(); if (LStyle)LStyle->Release();if(FStyle)FStyle->Release(); }
			int					ClassValue;
			GMarkerStyle*		MStyle;
			GLineStyle*			LStyle;
			GFillStyle*			FStyle;
			void				setMStyle(GMarkerStyle* style) { if (MStyle)MStyle->Release(); MStyle = style; MStyle->AddRef();}
			void				setLStyle(GLineStyle* style) { if (LStyle)LStyle->Release(); LStyle = style;LStyle->AddRef(); }
			void				setFStyle(GFillStyle* style) { if (MStyle)FStyle->Release(); FStyle = style; FStyle->AddRef(); }
		};
	public:
		GValueClassRender();
		~GValueClassRender();
		virtual Type GetType() { return RT_VALUE_CLASS; }
		virtual void InitQuery(GVectorDataQuery* pQuery);
		virtual void DrawFeature(GMapDrawing*   pDrawing, GVectorDataQueryResult* pCurrent);
		
		int				GetCount();
		StyleClassItem* AddItem(int clsid);
		StyleClassItem* GetItem(int ix);
		void			RemoveItem(int ix);
		void			SetClassField(const wstring& classField);
		const wstring&	GetClassField();
		StyleClassItem* GetItemByClass(int clsid);
		void			RemoveByClass(int clsid);

	private:
		void DrawGeometry(GMapDrawing*  pDrawing, GGeometry* pGeometry,StyleClassItem* style);
	private:
		wstring						ClassField;
		vector<StyleClassItem*>		Styles;
		map<int, StyleClassItem*>	StyleMap;
		StyleClassItem*				DefaultStyle;
	};
}
#endif
