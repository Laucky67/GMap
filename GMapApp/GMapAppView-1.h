
// GMapAppView.h : GMapAppView 类的接口
//

#pragma once

#include "GMapDrawingImpl.h"
#include "GMapPanZoomTool.h"

class GMapAppView : public CView, public IMapUIContext
{
protected: // 仅从序列化创建
	GMapAppView();
	DECLARE_DYNCREATE(GMapAppView)

// 特性
public:
	GMapAppDoc* GetDocument() const;
// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~GMapAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//!得到边界绘图对象
	virtual Graphics*		GetEditGraphics();
	//!释放编辑绘图对象
	virtual void			ReleaseEditGraphics();
	//!得到地图视口
	virtual GMapDrawing*	GetDrawing();
	//!得到关联地图对象
	virtual GMap*			GetMap();
	//!刷新地图
	virtual void			Redraw();
	//!设置状态文本
	virtual void			SetStatusText(u32 panel, const wstring& text);
	//!绘制缓存的地图
	virtual void			DrawCache(int offsetX = 0, int offsetY = 0);

	//!注册编辑对象
	virtual void			RegisterEditObject(GMapEditObject* pEditObject) {}
protected:
	void				    PrepareMap();
protected:
	Gdiplus::Bitmap*		m_offscreen_bitmap;
	Gdiplus::Bitmap*		m_cache_bitmap;
	GMapDrawingImpl*		m_map_drawing;
	CClientDC*				m_client_dc;
	Graphics*				m_client_graphics;
	GMapEditObject*			m_editing_object;

	vector<GMapUITool*>		m_tools_pool;
	GMapUITool*				m_active_tool;
	GMapUITool*				m_panzoom_tool;
// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose(); 
	afx_msg void OnSymbolManager();
	afx_msg void OnLayerManage();
	afx_msg void OnProjManager();
	afx_msg void OnFullMapExtent();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMapPanZoom();
	afx_msg void OnAddPointElement();
	afx_msg void OnAddLineElement();
	afx_msg void OnAddPolygon();
	afx_msg void OnEditGeometry();
	afx_msg void OnOpenVtDataSource();
	afx_msg void OnCreateVtDatasource();
	afx_msg void OnVtDataSourceManager();
	afx_msg void OnTestDlg();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // GMapAppView.cpp 中的调试版本
inline GMapAppDoc* GMapAppView::GetDocument() const
   { return reinterpret_cast<GMapAppDoc*>(m_pDocument); }
#endif

