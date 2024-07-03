#include "GGraphicsLayer.h"
#include "GGraphicsElement.h"
#include "GMap.h"
#include "GStyleMapDrawing.h"

namespace gmap{

	GGraphicsLayer::GGraphicsLayer(const wstring& name,const GRectangle2dd& defaultBox)
	{
		Name = name;
		NextId=0;
		DefaultBoundingBox=defaultBox;

		DefaultMarkerStyle=NULL;
		DefaultLineStyle=NULL;
		DefaultFillStyle=NULL;
	}

	GGraphicsLayer::~GGraphicsLayer(void)
	{
		ClearElements();
		if(DefaultMarkerStyle)DefaultMarkerStyle->Release();
		if(DefaultLineStyle)DefaultLineStyle->Release();
		if(DefaultFillStyle)DefaultFillStyle->Release();
	}

	//!图元个数
	u32 	GGraphicsLayer::GetElementCount()
	{
		return Elements.size();
	}

	//!得到图元
	GGraphicsElement* 	GGraphicsLayer::GetElementAt(u32 index)
	{
		return Elements[index];
	}

	//!移除图元
	void 	GGraphicsLayer::RemoveElement(u32 index)
	{
		if(index>0&&index<GetElementCount())
		{
			Elements[index]->Release();
			Elements.erase(Elements.begin()+index);
		}
	}

	//!添加图元
	void 	GGraphicsLayer::AppendElement(GGraphicsElement* pElement)
	{
		Elements.push_back(pElement);
		pElement->AttachLayer(NextId++,this);
		pElement->AddRef();
	}

	//!清除图元
	void 	GGraphicsLayer::ClearElements()
	{
		int count = Elements.size();
		for(int i=0;i<count;i++)
		{
			Elements[i]->AttachLayer(-1,NULL);
			Elements[i]->Release();
		}
		Elements.clear();
	}

	//!得到包围盒
	GRectangle2dd 	GGraphicsLayer::GetBoundingBox()
	{
		GRectangle2dd box = DefaultBoundingBox;
		int count = Elements.size();
		for(int i=0;i<count;i++)
		{
			GRectangle2dd box0  = Elements[i]->GetBoundingBox();
			box.AddRectangle(box0);
		}
		return box;
	}

	//!绘制
	void GGraphicsLayer::Draw(GMapDrawing* pDrawing)
	{
		//TODO:1加入对包围盒的判断
		//TODO:2加入空间索引
		int count = Elements.size();
		for(int i=0;i<count;i++)
		{
			Elements[i]->DrawElement(pDrawing);
		}
	}

	//!设置默认的点图元
	void  GGraphicsLayer::SetDefaultMarkerStyle(GMarkerStyle* style)
	{
		if(DefaultMarkerStyle!=NULL)DefaultMarkerStyle->Release();
		DefaultMarkerStyle = style;
		if(DefaultMarkerStyle)DefaultMarkerStyle->AddRef();
	}

	//!设置默认的点图元
	GMarkerStyle*  GGraphicsLayer::GetDefaultMarkerStyle()
	{
		return DefaultMarkerStyle;

	}

	//!设置默认的点图元
	void  GGraphicsLayer::SetDefaultLineStyle(GLineStyle* style)
	{
		if(DefaultLineStyle!=NULL)DefaultLineStyle->Release();
		DefaultLineStyle = style;
		if(DefaultLineStyle)DefaultLineStyle->AddRef();

	}

	//!设置默认的点图元
	GLineStyle*  GGraphicsLayer::GetDefaultLineStyle()
	{
		return DefaultLineStyle;
	}

	//!设置默认的点图元
	void  GGraphicsLayer::SetDefaultFillStyle(GFillStyle* style)
	{
		if(DefaultFillStyle!=NULL)DefaultFillStyle->Release();
		DefaultFillStyle = style;
		if(DefaultFillStyle)DefaultFillStyle->AddRef();
	}

	//!设置默认的点图元
	GFillStyle*  GGraphicsLayer::GetDefaultFillStyle()
	{
		return DefaultFillStyle;
	}

	//!保存
	void GGraphicsLayer::Save(IWriteFile* pStream)
	{
		FileUtil::WriteString(Name,pStream);
		pStream->Write(NextId);
		pStream->Write(DefaultBoundingBox);
		u32 styleId;
		styleId=DefaultMarkerStyle!=NULL?DefaultMarkerStyle->GetId():-1;
		pStream->Write(styleId);

		styleId=DefaultLineStyle!=NULL?DefaultLineStyle->GetId():-1;
		pStream->Write(styleId);

		styleId=DefaultFillStyle!=NULL?DefaultFillStyle->GetId():-1;
		pStream->Write(styleId);

		u32 count = Elements.size();
		pStream->Write(count);
		for(int i=0;i<count;i++)
		{
			GGraphicsElement* pElement = Elements.at(i);
			u32 id = pElement->GetId();
			pStream->Write(id);
			GGraphicsElement::EnumGraphicsElmementType type = pElement->GetElementType();
			pStream->Write(type);
			pElement->Save(pStream);
		}		
	}

	//!加载
	void GGraphicsLayer::Load(IReadFile* pStream)
	{
		Name = FileUtil::ReadString(pStream);
		u32 nextid;
		pStream->Read(nextid);
		pStream->Read(DefaultBoundingBox);
		u32 styleId;
		pStream->Read(styleId);
		if(styleId!=-1)
		{
			GStyle* pStyle = GetMap()->GetStyleGroup()->GetStyleById(styleId);
			SetDefaultMarkerStyle((GMarkerStyle*)pStyle);
		}		
		pStream->Read(styleId);
		if(styleId!=-1)
		{
			GStyle* pStyle = GetMap()->GetStyleGroup()->GetStyleById(styleId);
			SetDefaultLineStyle((GLineStyle*)pStyle);
		}		
		pStream->Read(styleId);
		if(styleId!=-1)
		{
			GStyle* pStyle = GetMap()->GetStyleGroup()->GetStyleById(styleId);
			SetDefaultFillStyle((GFillStyle*)pStyle);
		}

		u32 count;
		pStream->Read(count);

		for(int i=0;i<count;i++)
		{
			u32 id;
			pStream->Read(id);
			GGraphicsElement::EnumGraphicsElmementType type;
			pStream->Read(type);

			if(type==GGraphicsElement::GE_POINT)
			{
				GPointElement* pElement = new GPointElement();
				AppendElement(pElement);
				pElement->Load(pStream);
				pElement->Release();
			}		
			else if(type==GGraphicsElement::GE_LINE)
			{
				GLineStringElement* pElement = new GLineStringElement();
				AppendElement(pElement);
				pElement->Load(pStream);
				pElement->Release();
			}			
			else if(type==GGraphicsElement::GE_POLYGON)
			{
				GPolygonElement* pElement = new GPolygonElement();
				AppendElement(pElement);
				pElement->Load(pStream);
				pElement->Release();
			}
		}

		NextId = nextid;
	}

	//!点击选中测试
	GMapEditObject* GGraphicsLayer::HitTest(const GCoordinate& hitPoint,f64 hitTest)
	{
		u32 count=Elements.size();
		f64 D=hitTest;
		GGraphicsElement* pHitElement=NULL;

		for(int i=count-1;i>=0;i--)
		{
			GGraphicsElement* pElement = Elements.at(i);
			f64 d = pElement->HitTest(hitPoint);
			if(D>d)
			{
				pHitElement=pElement;
				D =d;
			}
		}

		if(pHitElement)
		{
			return pHitElement->CreateEditObject();
		}
		else
		{
			return NULL;
		}
	}
}