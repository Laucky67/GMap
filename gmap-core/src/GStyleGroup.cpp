#include "GStyleGroup.h"
namespace gmap{


	GStyleGroup::GStyleGroup(const wstring& name)
	{
		Name = name;
		NextId = 0;

	}

	GStyleGroup::~GStyleGroup(void)
	{
		for(int i=0;i<Styles.size();i++)
		{
			Styles[i]->Release();
		}
	}

	const wstring& GStyleGroup::GetName()
	{
		return Name;
	}

	void GStyleGroup::AddStyle(GStyle* pStyle,u32 id)
	{
		if(pStyle->GetGroup() == NULL)
		{
			if(id!=-1)
			{
				if(id>NextId)
				{
					NextId= id+1;
				}
			}
			else 
			{
				id = NextId++;
			}
			pStyle->AttachGroup(this,id);
			//NextId++;
			Styles.push_back(pStyle);
			pStyle->AddRef();
		}
	}

	void GStyleGroup::RemoveStyleAt(s32 index)
	{
		if(index>=0&&index<Styles.size())
		{
			Styles[index]->Release();
			Styles.erase(Styles.begin()+index);
		}
	}

	void GStyleGroup::RemoveStyleById(u32 id)
	{
		for(int i=0;i<Styles.size();i++)
		{
			if(Styles[i]->GetId() == id)
			{
				RemoveStyleAt(i);
				break;
			}
		}
	}

	GStyle* GStyleGroup::GetStyleAt(s32 index)
	{
		if(index>=0&&index<Styles.size())
		{
			return Styles[index];
		}
		else
		{
			return NULL;
		}
	}

	GStyle* GStyleGroup::GetStyleById(u32 id)
	{
		for(int i=0;i<Styles.size();i++)
		{
			if(Styles[i]->GetId() == id)
			{
				return Styles[i];
			}
		}
		return NULL;
	}

	u32 GStyleGroup::GetStyleCount()
	{
		return Styles.size();
	}
	
		
	//!保存到流中
	void GStyleGroup::SaveToStream(IWriteFile* pStream)
	{
		FileUtil::WriteString(Name,pStream);
		pStream->Write(NextId);

		u32 count = Styles.size();
		pStream->Write(count);

		for(int i=0;i<count;i++)
		{
			GStyle* pStyle = Styles.at(i);
			u32 id = pStyle->GetId();
			const wstring name = pStyle->GetName();
			GStyle::EnumStyleType styleType = pStyle->GetStyleType();

			pStream->Write(id);
			FileUtil::WriteString(name,pStream);
			pStream->Write(styleType);

			if(styleType==GStyle::ST_MARKER_STYLE)
			{
				GMarkerStyle* pMarkerStyle = static_cast<GMarkerStyle*>(pStyle);
				GMarkerStyle::EnumMarkerStyleType markerStyle = pMarkerStyle->GetMarkerStyleType();
				pStream->Write(markerStyle);

				pMarkerStyle->Save(pStream);

			}
			else if(styleType==GStyle::ST_LINE_STYLE)
			{
				GLineStyle* pLineStyle = static_cast<GLineStyle*>(pStyle);
				GLineStyle::EnumLineStyleType lineStyle = pLineStyle->GetLineStyleType();
				pStream->Write(lineStyle);

				pLineStyle->Save(pStream);
			}
			else if(styleType==GStyle::ST_FILL_STYLE)
			{
				GFillStyle* pFillStyle = static_cast<GFillStyle*>(pStyle);
				GFillStyle::EnumFillStyleType fillStyle = pFillStyle->GetFillStyleType();
				pStream->Write(fillStyle);

				pFillStyle->Save(pStream);
			}

		}
	}

	//!从流中加载
	GStyleGroup* GStyleGroup::LoadFromStream(IReadFile* pStream)
	{
		wstring name= FileUtil::ReadString(pStream);
		u32		nextId;
		pStream->Read(nextId);
		
		GStyleGroup* pStyleGroup = new GStyleGroup(name);
		u32 count;
		pStream->Read(count);
		for(int i=0;i<count;i++)
		{
			u32 id;
			wstring styleName;
			GStyle::EnumStyleType styleType;
			pStream->Read(id);
			styleName = FileUtil::ReadString(pStream);
			pStream->Read(styleType);

			if(styleType==GStyle::ST_MARKER_STYLE)
			{
				GMarkerStyle::EnumMarkerStyleType markerStyle;
				pStream->Read(markerStyle);

				if(markerStyle==GMarkerStyle::MST_IMAGE_STYLE)
				{
					GImageMarkerStyle* pStyle = new GImageMarkerStyle(NULL,0,styleName);
					pStyleGroup->AddStyle(pStyle,id);
					pStyle->Load(pStream);
					pStyle->Release();
				}
			}
			else if(styleType==GStyle::ST_LINE_STYLE)
			{
				GLineStyle::EnumLineStyleType lineStyle;
				pStream->Read(lineStyle);
				if(lineStyle==GLineStyle::LST_SIMPLE_LINE_STYLE)
				{
					GSimpleLineStyle* pLineStyle = new GSimpleLineStyle(NULL,0,styleName);
					pStyleGroup->AddStyle(pLineStyle,id);
					pLineStyle->Load(pStream);
					pLineStyle->Release();
				}
			}
			else if(styleType==GStyle::ST_FILL_STYLE)
			{
				GFillStyle::EnumFillStyleType fillStyle;
				pStream->Read(fillStyle);
				if(fillStyle==GFillStyle::FTS_COLOR_FILL_STYLE)
				{
					GColorFillStyle* pStyle = new GColorFillStyle(NULL,0,styleName);
					pStyleGroup->AddStyle(pStyle,id);
					
					pStyle->Load(pStream);
					pStyle->Release();
				}
				else if(fillStyle==GFillStyle::FTS_HATCH_FILL_STYLE)
				{
					GHatchFillStyle* pStyle = new GHatchFillStyle(NULL,0,styleName);
					pStyleGroup->AddStyle(pStyle,id);
					pStyle->Load(pStream);
					pStyle->Release();
				}
				else if(fillStyle==GFillStyle::FTS_TEXTURE_FILL_STYLE)
				{
					GTextureFillStyle* pStyle = new GTextureFillStyle(NULL,0,styleName);
					pStyleGroup->AddStyle(pStyle,id);
					pStyle->Load(pStream);
					pStyle->Release();
				}
			}
		}
		return pStyleGroup;

	}

	//-----------------------------------------------------------------------------
	void DrawStyle(Graphics* graphics,gmap::GStyle* pStyle,RectF rc)
	{

		if(pStyle->GetStyleType()==gmap::GStyle::ST_MARKER_STYLE)
		{
			gmap::GMarkerStyle* pMarkerStyle = (gmap::GMarkerStyle* )pStyle;
			pMarkerStyle->Draw(graphics,PointF((rc.GetLeft()+rc.GetRight())/2,(rc.GetTop()+rc.GetBottom())/2));

		}
		else if(pStyle->GetStyleType()==gmap::GStyle::ST_LINE_STYLE)
		{
			gmap::GLineStyle* pLineStyle = (gmap::GLineStyle* )pStyle;
			PointF points[2];
			points[0]=PointF(rc.GetLeft(),(rc.GetTop()+rc.GetBottom())/2);
			points[1]=PointF(rc.GetRight(),(rc.GetTop()+rc.GetBottom())/2);
			pLineStyle->Draw(graphics,points,2,false);
		}
		else if(pStyle->GetStyleType()==gmap::GStyle::ST_FILL_STYLE)
		{
			gmap::GFillStyle* pFillStyle = (gmap::GFillStyle* )pStyle;
			PointF points[4];
			points[0]=PointF(rc.GetLeft(),rc.GetTop());
			points[1]=PointF(rc.GetLeft(),rc.GetBottom());
			points[2]=PointF(rc.GetRight(),rc.GetBottom());
			points[3]=PointF(rc.GetRight(),rc.GetTop());
			pFillStyle->Draw(graphics,points,4);
		}

	}
}
