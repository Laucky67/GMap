
// GMapAppView.h : GMapAppView ��Ľӿ�
//

#pragma once

#include "GMapDrawingImpl.h"
#include "GMapPanZoomTool.h"

class GMapAppView : public CView, public IMapUIContext
{
protected: // �������л�����
	GMapAppView();
	DECLARE_DYNCREATE(GMapAppView)

// ����
public:
	GMapAppDoc* GetDocument() const;
// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~GMapAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//!�õ��߽��ͼ����
	virtual Graphics*		GetEditGraphics();
	//!�ͷű༭��ͼ����
	virtual void			ReleaseEditGraphics();
	//!�õ���ͼ�ӿ�
	virtual GMapDrawing*	GetDrawing();
	//!�õ�������ͼ����
	virtual GMap*			GetMap();
	//!ˢ�µ�ͼ
	virtual void			Redraw();
	//!����״̬�ı�
	virtual void			SetStatusText(u32 panel, const wstring& text);
	//!���ƻ���ĵ�ͼ
	virtual void			DrawCache(int offsetX = 0, int offsetY = 0);

	//!ע��༭����
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
// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // GMapAppView.cpp �еĵ��԰汾
inline GMapAppDoc* GMapAppView::GetDocument() const
   { return reinterpret_cast<GMapAppDoc*>(m_pDocument); }
#endif

