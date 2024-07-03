#include "GPolygon.h"
namespace gmap{
	
	GPolygon::GPolygon()
	{
		ExternalRing  = NULL;
	}

	GPolygon::GPolygon(GLinearRing* externalRing)
	{
		ExternalRing = dynamic_cast<GLinearRing*>(externalRing->Clone());
	}

	GPolygon::GPolygon(GLinearRing* externalRing,const vector<GLinearRing*>& internalRings)
	{
		ExternalRing = dynamic_cast<GLinearRing*>(externalRing->Clone());

		for(int i=0;i<internalRings.size();i++)
		{
			GLinearRing* lr = dynamic_cast<GLinearRing*>(internalRings[i]->Clone());
			InternalRings.push_back(lr);
		}
	}

	GPolygon::~GPolygon(void)
	{
		if(ExternalRing)
		{
			ExternalRing->Release();
		}
		ClearAllInternalRings();
	}

	//!得到外环
	GLinearRing* GPolygon::GetExternalRing()
	{
		return 	ExternalRing;
	}

	//!得到内环的个数
	u32		  GPolygon::GetInternalRingCount()
	{
		return 	InternalRings.size();
	}

	//!得到内环
	GLinearRing* GPolygon::GetInternalRingAt(u32 index)
	{
		return InternalRings[index];
	}

	//!添加内环
	void GPolygon::AddInternalRing(GLinearRing* ring,bool newCopy)
	{
		if(newCopy)
		{
		GLinearRing* lr = dynamic_cast<GLinearRing*>(ring->Clone());
		
		InternalRings.push_back(lr);
		}
		else
		{
			
			InternalRings.push_back(ring);
			ring->AddRef();
		}
	}

	//!移除内环
	void GPolygon::RemoveInternalRing(u32 index)
	{
		if(index>=0 && index<InternalRings.size())
		{
			GLinearRing* lr = InternalRings[index];
			InternalRings.erase(InternalRings.begin()+index);
			lr->Release();
		}
	}

	//!清除内环
	void GPolygon::ClearAllInternalRings()
	{			
		//ExternalRing->Release();
		for(int i=0;i<InternalRings.size();i++)
		{
			InternalRings[i]->Release();
		}
		InternalRings.clear();
	}

	//!得到包围盒
	GRectangle2dd GPolygon::GetBoundingBox()
	{
		return ExternalRing->GetBoundingBox();
	}

	//!Clone
	GGeometry* GPolygon::Clone()
	{
		return GPolygon::Create(ExternalRing,InternalRings);
	}

	//!得到几何类型
	GGeometry::EnumGeometryType GPolygon::GetGeometryType()
	{
		return GGeometry::GT_POLYGON;
	}


	//!对几何对象进行坐标变换
	void  GPolygon::Transform(IGeoTransform* pTransformer)
	{
		if(pTransformer == NULL)return;
		
		u32 count = InternalRings.size();
		ExternalRing->Transform(pTransformer);

		for(int i=0;i<InternalRings.size();i++)
		{
			InternalRings[i]->Transform(pTransformer);
		}
	}

	//-------------------------------------------------------
	//!Save
	void GPolygon::Save(IWriteFile* pStream)
	{
		if(this->ExternalRing!=NULL)
		{
			u32 flag=1;
			pStream->Write(&flag,sizeof(flag));
			ExternalRing->Save(pStream);
		}
		else
		{
			u32 flag=0;
			pStream->Write(&flag,sizeof(flag));
		}
		
		int count = InternalRings.size();
		pStream->Write(&count,sizeof(count));
		
		for(int i=0;i<count;i++)
		{
			InternalRings[i]->Save(pStream);
		}
	}

	//!Load
	void GPolygon::Load(IReadFile* pStream)
	{
		u32 flag =0;
		pStream->Read(&flag,sizeof(u32));

		if(flag)
		{
			if(ExternalRing)
			{
				ExternalRing->Release();
			}
			ExternalRing = GLinearRing::Create(); 
			ExternalRing->Load(pStream);

			ClearAllInternalRings();

			u32 count=0;
			pStream->Read(&count,sizeof(count));
			for(int i=0;i<count;i++)
			{
				GLinearRing* lr = GLinearRing::Create();
				lr->Load(pStream);
				InternalRings.push_back(lr);
			}
		}
	}

	//-------------------------------------------------------
	GPolygon* GPolygon::Create()
	{
		return new GPolygon();
	}

	GPolygon* GPolygon::Create(GLinearRing* externalRing)
	{
		GPolygon* polygon= new 	GPolygon(externalRing);
		return polygon;
	}

	GPolygon* GPolygon::Create(GLinearRing* externalRing,const vector<GLinearRing*>& internalRing)
	{
		GPolygon* polygon = new GPolygon(externalRing,internalRing);
		return polygon;
	}

	GPolygon* GPolygon::Create(u32 count,GCoordinate* pCoordinates)
	{
		GLinearRing* lr = GLinearRing::Create(count,pCoordinates);
		GPolygon* polygon = GPolygon::Create(lr);
		lr->Release();
		return polygon;
	}

	GPolygon* GPolygon::Create(const vector<GCoordinate>& pCoordinates)
	{
		GLinearRing* lr = GLinearRing::Create(pCoordinates);
		GPolygon* polygon = GPolygon::Create(lr);
		lr->Release();
		return polygon;
	}

	GPolygon* GPolygon::Create(f64 x0,f64 y0,f64 x1,f64 y1,f64 x2,f64 y2)
	{
		vector<GCoordinate> coordinates;
		coordinates.push_back(GCoordinate(x0,y0));
		coordinates.push_back(GCoordinate(x1,y1));
		coordinates.push_back(GCoordinate(x2,y2));

		return GPolygon::Create(coordinates);
	}

	GPolygon* GPolygon::Create(const GCoordinate& p0,const GCoordinate& p1,const GCoordinate& p2)
	{
		vector<GCoordinate> coordinates;
		coordinates.push_back(p0);
		coordinates.push_back(p1);
		coordinates.push_back(p2);

		return GPolygon::Create(coordinates);
	}

	GPolygon* GPolygon::Create(const GRectangle2dd& rect)
	{
		vector<GCoordinate> coordinates;
		coordinates.push_back(rect.MinEdge);
		coordinates.push_back(GCoordinate(rect.MaxEdge.X,rect.MinEdge.Y));
		coordinates.push_back(rect.MaxEdge);
		coordinates.push_back(GCoordinate(rect.MinEdge.X,rect.MaxEdge.Y));
		return GPolygon::Create(coordinates);
	}
}
