
// GMapAppDoc.cpp : GMapAppDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "GMapApp.h"
#endif

#include "GMapAppDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// GMapAppDoc

IMPLEMENT_DYNCREATE(GMapAppDoc, CDocument)

BEGIN_MESSAGE_MAP(GMapAppDoc, CDocument)
END_MESSAGE_MAP()


// GMapAppDoc ����/����

GMapAppDoc::GMapAppDoc()
{
	// TODO: �ڴ����һ���Թ������
	m_styles = new GStyleGroup(L"styles");
	m_map = new GMap(L"gmap",m_styles,NULL);
}

GMapAppDoc::~GMapAppDoc()
{
	m_map->ClearLayers();
	m_map->Release();
	m_styles->Release();
	for each(auto it in GetVectorDataSources())
	{
		it->Close();
		it->Release();
	}
}

BOOL GMapAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// GMapAppDoc ���л�

void GMapAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void GMapAppDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void GMapAppDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void GMapAppDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// GMapAppDoc ���

#ifdef _DEBUG
void GMapAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void GMapAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

GVectorDataSource * GMapAppDoc::GetVectorDataSource(wstring & name)
{
	for (vector<GVectorDataSource*>::iterator it = m_vt_source_list.begin();
		it != m_vt_source_list.end(); it++)
	{
		GVectorDataSource* ds = *it;
		if (ds->GetName() == name)
			return ds;
	}
	return NULL;
}

bool GMapAppDoc::OpenVtDataSourceMDB(const wstring&  dbfile)
{
	GVectorDataSource* pVectorDataSource = gmap::OpenVectorDataSourceMDB(dbfile,dbfile);
	if (pVectorDataSource)
	{
		m_vt_source_list.push_back(pVectorDataSource);
		return true;
	}
	return false;
}

bool GMapAppDoc::CreateVtDataSourceMDB(const wstring&  dbfile)
{
	GVectorDataSource* pVectorDataSource = gmap::CreateVectorDataSourceMDB(dbfile, dbfile);
	if (pVectorDataSource)
	{
		m_vt_source_list.push_back(pVectorDataSource);
		return true;
	}
	return false;
}