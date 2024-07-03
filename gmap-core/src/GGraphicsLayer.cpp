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

	//!ͼԪ����
	u32 	GGraphicsLayer::GetElementCount()
	{
		return Elements.size();
	}

	//!�õ�ͼԪ
	GGraphicsElement* 	GGraphicsLayer::GetElementAt(u32 index)
	{
		return Elements[index];
	}

	//!�Ƴ�ͼԪ
	void 	GGraphicsLayer::RemoveElement(u32 index)
	{
		if(index>0&&index<GetElementCount())
		{
			Elements[index]->Release();
			Elements.erase(Elements.begin()+index);
		}
	}

	//!���ͼԪ
	void 	GGraphicsLayer::AppendElement(GGraphicsElement* pElement)
	{
		Elements.push_back(pElement);
		pElement->AttachLayer(NextId++,this);
		pElement->AddRef();
	}

	//!���ͼԪ
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

	//!�õ���Χ��
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

	//!����
	void GGraphicsLayer::Draw(GMapDrawing* pDrawing)
	{
		//TODO:1����԰�Χ�е��ж�
		//TODO:2����ռ�����
		int count = Elements.size();
		for(int i=0;i<count;i++)
		{
			Elements[i]->DrawElement(pDrawing);
		}
	}

	//!����Ĭ�ϵĵ�ͼԪ
	void  GGraphicsLayer::SetDefaultMarkerStyle(GMarkerStyle* style)
	{
		if(DefaultMarkerStyle!=NULL)DefaultMarkerStyle->Release();
		DefaultMarkerStyle = style;
		if(DefaultMarkerStyle)DefaultMarkerStyle->AddRef();
	}

	//!����Ĭ�ϵĵ�ͼԪ
	GMarkerStyle*  GGraphicsLayer::GetDefaultMarkerStyle()
	{
		return DefaultMarkerStyle;

	}

	//!����Ĭ�ϵĵ�ͼԪ
	void  GGraphicsLayer::SetDefaultLineStyle(GLineStyle* style)
	{
		if(DefaultLineStyle!=NULL)DefaultLineStyle->Release();
		DefaultLineStyle = style;
		if(DefaultLineStyle)DefaultLineStyle->AddRef();

	}

	//!����Ĭ�ϵĵ�ͼԪ
	GLineStyle*  GGraphicsLayer::GetDefaultLineStyle()
	{
		return DefaultLineStyle;
	}

	//!����Ĭ�ϵĵ�ͼԪ
	void  GGraphicsLayer::SetDefaultFillStyle(GFillStyle* style)
	{
		if(DefaultFillStyle!=NULL)DefaultFillStyle->Release();
		DefaultFillStyle = style;
		if(DefaultFillStyle)DefaultFillStyle->AddRef();
	}

	//!����Ĭ�ϵĵ�ͼԪ
	GFillStyle*  GGraphicsLayer::GetDefaultFillStyle()
	{
		return DefaultFillStyle;
	}

	//!����
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

	//!����
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

	//!���ѡ�в���
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