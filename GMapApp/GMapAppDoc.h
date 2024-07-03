
// GMapAppDoc.h : GMapAppDoc 类的接口
//


#pragma once
#include "gmap-include.h"

class GMapAppDoc : public CDocument
{
protected: // 仅从序列化创建
	GMapAppDoc();
	DECLARE_DYNCREATE(GMapAppDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
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

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:

};
