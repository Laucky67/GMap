
// GMapAppDoc.h : GMapAppDoc ��Ľӿ�
//


#pragma once
#include "gmap-include.h"

class GMapAppDoc : public CDocument
{
protected: // �������л�����
	GMapAppDoc();
	DECLARE_DYNCREATE(GMapAppDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~GMapAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	GMap*							m_map;
	GStyleGroup*					m_styles;
	vector<GVectorDataSource*>		m_vt_source_list;
public:
	GMap*									GetMap()	{ return m_map; }
	GStyleGroup*							GetStyles() { return m_styles; }
	const vector<GVectorDataSource*>&		GetVectorDataSources() { return m_vt_source_list;}
	GVectorDataSource*						GetVectorDataSource(wstring& name);
	bool									OpenVtDataSourceMDB(const wstring&  dbfile);
	bool									CreateVtDataSourceMDB(const wstring&  dbfile);

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:

};
