
// GMapAppView.cpp : GMapAppView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "GMapApp.h"
#endif

#include "GMapAppDoc.h"
#include "GMapAppView.h"
#include "MainFrm.h"

#include "StyleManagerDlg.h"
#include "LayerManagerDlg.h"
#include "CoordSysDlg.h"
#include "VtDataSourceMDBDlg.h"
#include "CValueClassRenderDialog.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// GMapAppView

IMPLEMENT_DYNCREATE(GMapAppView, CView)

BEGIN_MESSAGE_MAP(GMapAppView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &GMapAppView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CREATE()
	ON_COMMAND(ID_SYMBOL_MANAGER, &GMapAppView::OnSymbolManager)
	ON_COMMAND(ID_LAYER_MANAGE, &GMapAppView::OnLayerManage)
	ON_COMMAND(ID_PROJ_MANAGER, &GMapAppView::OnProjManager)
	ON_COMMAND(ID_FULL_MAP_EXTENT, &GMapAppView::OnFullMapExtent)
	ON_COMMAND(ID_MAP_PAN_ZOOM, &GMapAppView::OnMapPanZoom)
	ON_COMMAND(ID_ADD_POINT_ELEMENT, &GMapAppView::OnAddPointElement)
	ON_COMMAND(ID_ADD_LINE_ELEMENT, &GMapAppView::OnAddLineElement)
	ON_COMMAND(ID_ADD_POLYGON, &GMapAppView::OnAddPolygon)
	ON_COMMAND(ID_EDIT_GEOMETRY, &GMapAppView::OnEditGeometry)
	ON_COMMAND(ID_OPEN_VT_DATASOURCE, &GMapAppView::OnOpenVtDataSource)
	ON_COMMAND(ID_CREATE_VT_DATASOURCE, &GMapAppView::OnCreateVtDatasource)
	ON_COMMAND(ID_VT_DATA_SOURCE_MANAGER, &GMapAppView::OnVtDataSourceManager)
	ON_COMMAND(ID_TEST_DLG, &GMapAppView::OnTestDlg)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// GMapAppView 构造/析构x

GMapAppView::GMapAppView()
{
	// TODO: 在此处添加构造代码
	m_offscreen_bitmap = NULL;
	m_cache_bitmap = NULL;
	m_map_drawing = new GMapDrawingImpl();
	m_client_dc = NULL;
	m_client_graphics = NULL;
	m_editing_object = NULL;

	m_tools_pool.push_back(new GMapPanZoomTool(this));
	m_tools_pool.push_back(new GAddPointElementTool(this));
	m_tools_pool.push_back(new GAddLineElementTool(this));
	m_tools_pool.push_back(new GAddPolygonElementTool(this));
	m_tools_pool.push_back(new GSimpleEditTool(this));
	

	m_panzoom_tool = m_tools_pool[0];
	m_active_tool = NULL;
}

GMapAppView::~GMapAppView()
{
	delete m_map_drawing;
	delete m_cache_bitmap;
	if (m_editing_object)
		m_editing_object->Release();

	for each (auto it in m_tools_pool)
	{
		it->Release();
	}
}

BOOL GMapAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// GMapAppView 绘制

void GMapAppView::OnDraw(CDC* pDC)
{
	GMapAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码
	PrepareMap();

	Graphics g(pDC->GetSafeHdc());
	CachedBitmap cbitmap(m_cache_bitmap, &g);
	g.DrawCachedBitmap(&cbitmap, 0, 0);

	if (m_active_tool)
	{
		m_active_tool->Draw(&g);
	}
}

// GMapAppView 打印


void GMapAppView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL GMapAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void GMapAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void GMapAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}



void GMapAppView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// GMapAppView 诊断

#ifdef _DEBUG
void GMapAppView::AssertValid() const
{
	CView::AssertValid();
}

void GMapAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}


GMapAppDoc* GMapAppView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(GMapAppDoc)));
	return (GMapAppDoc*)m_pDocument;
}
#endif //_DEBUG


Graphics * GMapAppView::GetEditGraphics()
{
	if (!m_client_dc)
	{
		m_client_dc = new CClientDC(this);
		m_client_graphics = new Graphics(m_client_dc->GetSafeHdc());
	}
	return m_client_graphics;
}

void GMapAppView::ReleaseEditGraphics()
{
	if (!m_client_dc)
	{
		//delete m_client_graphics;
		//delete m_client_dc;
	}
}

GMapDrawing * GMapAppView::GetDrawing()
{
	return m_map_drawing;
}

GMap * GMapAppView::GetMap()
{
	return GetDocument()->GetMap();
}

void GMapAppView::Redraw()
{
	this->Invalidate(FALSE);
}

void GMapAppView::SetStatusText(u32 panel, const wstring & text)
{
	static_cast<CMainFrame*>(AfxGetMainWnd())->SetStatusText(panel, text.c_str());
}

void GMapAppView::DrawCache(int offsetX, int offsetY)
{
	if (!m_client_dc)
	{
		m_client_dc = new CClientDC(this);
		m_client_graphics = new Graphics(m_client_dc->GetSafeHdc());
	}
	if (offsetX == offsetY && offsetX == 0)
	{
		m_client_graphics->DrawImage(m_cache_bitmap, offsetX, offsetY);
	}
	else
	{
		{
			Graphics g2(m_offscreen_bitmap);
			g2.Clear(Color::White);
			g2.DrawImage(m_cache_bitmap, offsetX, offsetY);
		}
		m_client_graphics->DrawImage(m_offscreen_bitmap, 0, 0);
	}
}

void GMapAppView::PrepareMap()
{
	if (!m_cache_bitmap)
	{
		CRect rc;
		GetWindowRect(&rc);
		m_cache_bitmap = new Bitmap(rc.Width(), rc.Height());
		m_offscreen_bitmap = new Bitmap(rc.Width(), rc.Height());
	}

	Graphics g(m_cache_bitmap);
	g.Clear(Color::White);
	m_map_drawing->BeginDraw(&g);
	GetMap()->Draw(m_map_drawing);
	m_map_drawing->EndDraw();
}

// GMapAppView 消息处理程序
void GMapAppView::OnSize(UINT nType, int cx, int cy)
{
	if (m_cache_bitmap)
	{
		delete m_cache_bitmap;
		m_cache_bitmap = NULL;

		delete m_offscreen_bitmap;
		m_offscreen_bitmap = NULL;
	}
	// TODO: 在此处添加消息处理程序代码
	if (m_map_drawing->GetScale() <= 0)
	{
		OnFullMapExtent();
	}
	else
	{
		m_map_drawing->Resize(RectF(0, 0, cx, cy));
	}
	__super::OnSize(nType, cx, cy);
}


void GMapAppView::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_client_dc)
	{
		delete m_client_graphics;
		delete m_client_dc;
	}
	__super::OnClose();
}


void GMapAppView::OnSymbolManager()
{
	// TODO: 在此添加命令处理程序代码
	CStyleManagerDlg dlg(this,GetDocument()->GetStyles());
	dlg.DoModal();
}


void GMapAppView::OnLayerManage()
{
	// TODO: 在此添加命令处理程序代码
	CLayerManagerDlg dlg(this, GetDocument());
	dlg.DoModal();
}

void GMapAppView::OnProjManager()
{
	// TODO: 在此添加命令处理程序代码
	CCoordSysDlg dlg(this);
	dlg.DoModal();
}


void GMapAppView::OnFullMapExtent()
{
	// TODO: 在此添加命令处理程序代码

	CRect wRect;
	GetWindowRect(&wRect);
	m_map_drawing->Resize(RectF(0, 0, wRect.Width(), wRect.Height()));

	GMap* map = GetDocument()->GetMap();
	GRectangle2dd rc = map->GetMapExtent();
	m_map_drawing->ToExtent(rc);
	this->Redraw();
}

void GMapAppView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_active_tool == NULL ||
		!m_active_tool->OnMouseMove(point.x, point.y, nFlags))
	{
		m_panzoom_tool->OnMouseMove(point.x, point.y, nFlags);
	}

	__super::OnMouseMove(nFlags, point);
}

BOOL GMapAppView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_active_tool == NULL ||
		!m_active_tool->OnMouseWheel(pt.x, pt.y, nFlags, zDelta))
	{
		m_panzoom_tool->OnMouseWheel(pt.x, pt.y, nFlags, zDelta);
	}

	return __super::OnMouseWheel(nFlags, zDelta, pt);
}

void GMapAppView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_active_tool == NULL ||
		!m_active_tool->OnMouseUp(point.x, point.y, MK_LBUTTON))
	{
		m_panzoom_tool->OnMouseUp(point.x, point.y, MK_LBUTTON);
	}
	__super::OnLButtonUp(nFlags, point);
}

void GMapAppView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_active_tool == NULL ||
		!m_active_tool->OnMouseDown(point.x, point.y, MK_MBUTTON))
	{
		m_panzoom_tool->OnMouseDown(point.x, point.y, MK_MBUTTON);
	}
	__super::OnMButtonDown(nFlags, point);
}

void GMapAppView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_active_tool == NULL ||
		!m_active_tool->OnMouseUp(point.x, point.y, MK_MBUTTON))
	{
		m_panzoom_tool->OnMouseUp(point.x, point.y, MK_MBUTTON);
	}
	__super::OnMButtonUp(nFlags, point);
}

void GMapAppView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_active_tool == NULL ||
		!m_active_tool->OnMouseUp(point.x, point.y, MK_RBUTTON))
	{
		m_panzoom_tool->OnMouseUp(point.x, point.y, MK_RBUTTON);
	}
	__super::OnRButtonUp(nFlags, point);
}

void GMapAppView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_active_tool == NULL ||
		!m_active_tool->OnMouseDown(point.x, point.y, MK_RBUTTON))
	{
		m_panzoom_tool->OnMouseDown(point.x, point.y, MK_RBUTTON);
	}
	__super::OnRButtonDown(nFlags, point);
}

void GMapAppView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_active_tool == NULL ||
		!m_active_tool->OnMouseDown(point.x, point.y, MK_LBUTTON))
	{
		m_panzoom_tool->OnMouseDown(point.x, point.y, MK_LBUTTON);
	}
	__super::OnLButtonDown(nFlags, point);
}

int GMapAppView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//OnFullMapExtent();
	return 0;
}


void GMapAppView::OnMapPanZoom()
{
	// TODO: 在此添加命令处理程序代码
	m_active_tool = NULL;
}


void GMapAppView::OnAddPointElement()
{
	// TODO: 在此添加命令处理程序代码
	m_active_tool = m_tools_pool[1];
}


void GMapAppView::OnAddLineElement()
{
	// TODO: 在此添加命令处理程序代码
	m_active_tool = m_tools_pool[2];
}


void GMapAppView::OnAddPolygon()
{
	// TODO: 在此添加命令处理程序代码
	m_active_tool = m_tools_pool[3];
}

void GMapAppView::OnEditGeometry()
{
	// TODO: 在此添加命令处理程序代码
	m_active_tool = m_tools_pool[4];
}



void GMapAppView::OnOpenVtDataSource()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE, _T("*.mdb"),NULL,6UL,_T("access file 2007-(*.mdb)|*.mdb|access 2007+(*.accdb)|*.accdb||"));
	if (dlg.DoModal() == IDOK)
	{
		CString fileName = dlg.GetPathName();
		wstring wfn = fileName.GetBuffer();
		GetDocument()->OpenVtDataSourceMDB(wfn);
	}
}

void GMapAppView::OnCreateVtDatasource()
{
	// TODO: 在此添加命令处理程序代码
}


void GMapAppView::OnVtDataSourceManager()
{
	// TODO: 在此添加命令处理程序代码
	CVtDataSourceMDBDlg dlg(GetDocument(),this);
	dlg.DoModal();
}

GValueClassRender* classRender = NULL;
void GMapAppView::OnTestDlg()
{
	// TODO: 在此添加命令处理程序代码
	GMap* map = GetDocument()->GetMap();
	GVectorDataSource* pDS = GetDocument()->GetVectorDataSources()[0];
	GVectorDataLayer*  pLayer = pDS->GetLayer(0);
	if(classRender==NULL)
		classRender = new GValueClassRender();
	CValueClassRenderDialog dlg(map, pLayer, classRender, this);
	dlg.DoModal();
}


void GMapAppView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	__super::OnLButtonDblClk(nFlags, point);
}
