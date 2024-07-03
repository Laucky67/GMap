#include "GMultiGeometry.h"
namespace gmap{

	GMultiGeometry::GMultiGeometry(GGeometry::EnumGeometryType primitiveType)
	{
		PrimitiveType = primitiveType;
	}

	GMultiGeometry::~GMultiGeometry(void)
	{
		Clear();
	}

	//!得到几何对象的个数
	u32 GMultiGeometry::GetGeometryCount()
	{
		return Geometries.size();	
	}

	//!得到几何对象
	GGeometry* GMultiGeometry::GetGeometryAt(u32 index)
	{
		return 	Geometries[index];
	}

	//!得到包围盒
	GRectangle2dd GMultiGeometry::GetBoundingBox()
	{
		GRectangle2dd rect;
		int count = Geometries.size();
		for(int i=0;i<count;i++)
		{
			rect.AddRectangle(Geometries[i]->GetBoundingBox());
		}
		return rect;
	}

	//!Clone
	GGeometry* GMultiGeometry::Clone()
	{
		GMultiGeometry* pClone = GMultiGeometry::Create(PrimitiveType);
		int count = Geometries.size();
		for(int i=0;i<count;i++)
		{
			pClone->AppendGeometry(Geometries[i],true);
		}
		return pClone;
	}


	void  GMultiGeometry::Transform(IGeoTransform* pTransformer)
	{
		if(pTransformer == NULL)return;

		int count = Geometries.size();
		for(int i=0;i<count;i++)
		{
			Geometries[i]->Transform(pTransformer);
		}
	}

	//!得到几何类型
	GGeometry::EnumGeometryType GMultiGeometry::GetGeometryType()
	{
		return (GGeometry::EnumGeometryType)(PrimitiveType|GGeometry::GT_COLLECTION);
	}

	//!添加
	void GMultiGeometry::AppendGeometry(GGeometry* pGeometry,bool newCopy)
	{
		if(NULL!=(pGeometry->GetGeometryType() & GGeometry::GT_COLLECTION))
		{
			GMultiGeometry* pMultiGeometry = static_cast<GMultiGeometry*>(pGeometry);
			for(u32 i=0;i<pMultiGeometry->GetGeometryCount();i++)
			{
				AppendGeometry(pMultiGeometry->GetGeometryAt(i), newCopy);
			}
		}
		else
		{
			if(PrimitiveType==GGeometry::GT_UNKNOWN 
				|| PrimitiveType==pGeometry->GetGeometryType())
			{
				if(newCopy)
				{
					GGeometry* pClone = pGeometry->Clone();
					Geometries.push_back(pClone);
				}
				else
				{
					Geometries.push_back(pGeometry);
					pGeometry->AddRef();
				}
			}
		}
	}

	//!移除
	void GMultiGeometry::RemoveGeometry(u32 index)
	{
		if(index>=0 && index<Geometries.size())
		{
			Geometries[index]->Release();
			Geometries.erase(Geometries.begin()+index);
		}
	}

	
	//!Save
	void GMultiGeometry::Save(IWriteFile* pStream)
	{
		int count = Geometries.size();
		pStream->Write(&count,sizeof(count));

		for(int i=0;i<count;i++)
		{
			GGeometry::SaveToStream(Geometries[i],pStream);
		}
	}

	//!Load
	void GMultiGeometry::Load(IReadFile* pStream)
	{
		Clear();

		int count = 0;
		pStream->Read(&count,sizeof(count));

		for(int i=0;i<count;i++)
		{
			GGeometry* pSubGeometry = GGeometry::CreateFromStream(pStream);
			Geometries.push_back(pSubGeometry);	
		}
	}

	//!清除
	void GMultiGeometry::Clear()
	{
		int count= Geometries.size();
		for(int i=0;i<count;i++)
		{
			Geometries[i]->Release();
		}
		Geometries.clear();
	}

	GMultiGeometry* GMultiGeometry::Create(const vector<GGeometry*>& geometries,GGeometry::EnumGeometryType	primitiveType)
	{
			GMultiGeometry* pObject = new GMultiGeometry(primitiveType);
			for(int i=0;i<geometries.size();i++)
			{
				pObject->AppendGeometry(geometries[i],true);
			}
			return pObject;
	}

	GMultiGeometry* GMultiGeometry::Create(GGeometry* pGeometry,GGeometry::EnumGeometryType	primitiveType)
	{
			GMultiGeometry* pObject = new GMultiGeometry(primitiveType);
			pObject->AppendGeometry(pGeometry,true);
			return pObject;
	}

	GMultiGeometry* GMultiGeometry::Create(GGeometry::EnumGeometryType	primitiveType)
	{
		GMultiGeometry* pObject = new GMultiGeometry(primitiveType);
		return pObject;	
	}
}
