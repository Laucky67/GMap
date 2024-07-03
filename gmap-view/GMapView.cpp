
// GMapView.cpp: GMapView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "gmap-view.h"
#endif

#include "GMapViewDoc.h"
#include "GMapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// GMapView

IMPLEMENT_DYNCREATE(GMapView, CView)

BEGIN_MESSAGE_MAP(GMapView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &GMapView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// GMapView 构造/析构

GMapView::GMapView() noexcept
{
	// TODO: 在此处添加构造代码

}

GMapView::~GMapView()
{
}

BOOL GMapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// GMapView 绘图

void GMapView::OnDraw(CDC* /*pDC*/)
{
	GMapViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// GMapView 打印


void GMapView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL GMapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void GMapView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void GMapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void GMapView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void GMapView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// GMapView 诊断

#ifdef _DEBUG
void GMapView::AssertValid() const
{
	CView::AssertValid();
}

void GMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

GMapViewDoc* GMapView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(GMapViewDoc)));
	return (GMapViewDoc*)m_pDocument;
}
#endif //_DEBUG


// GMapView 消息处理程序
