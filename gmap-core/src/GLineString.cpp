#include "GLineString.h"
namespace gmap{

		GLineString::GLineString(u32 count,GCoordinate* pCoordinates)
		{
			for(int i=0;i<count;i++)
			{
				Coordinates.push_back(pCoordinates[i]);
			}
			Dirty = true;
		}

		
		GLineString::GLineString(const vector<GCoordinate>& coordinates)
		{
			int count = coordinates.size();
			for(int i=0;i<count;i++)
			{
				Coordinates.push_back(coordinates[i]);
			}
			Dirty = true;
		}

		GLineString::~GLineString(void)
		{
					
		}

		//!�õ���Χ��
		GRectangle2dd GLineString::GetBoundingBox()
		{
			if(Dirty)
			{
				if(Coordinates.size()>0)
				{
					BoundingBox = GRectangle2dd(Coordinates[0],Coordinates[0]);
				}

				int count = Coordinates.size();
				for(int i=0;i<count;i++)
				{
					BoundingBox.AddPoint(Coordinates[i]);
				}
				Dirty = false;
			}
			return BoundingBox;
		}

		//!Clone
		GGeometry* GLineString::Clone()
		{
			GLineString* lineString = GLineString::Create(Coordinates);
			return lineString;
		}

		//!�õ���������
		GGeometry::EnumGeometryType GLineString::GetGeometryType()
		{
			return GGeometry::GT_LINE_STRING;
		}

		//!�õ���ĸ���
		u32 GLineString::GetCoordinateCount()
		{
			return Coordinates.size();
		}

		//!�õ�����
		void GLineString::GetCoordinateAt(u32 index,GCoordinate* pCoordinate)
		{
			*pCoordinate = Coordinates[index];
		}

				//!�õ�����
		const GCoordinate& GLineString::GetCoordinateAt(u32 index)
		{
			return Coordinates[index];
		}

		//!�õ�����
		void GLineString::GetCoordinates(GCoordinate* pCoordinates)
		{
			int count = Coordinates.size();
			for(int i=0;i<count;i++)
			{
				pCoordinates[i]=Coordinates[i];
			}
			//memcpy(pCoordinates, Coordinates.data(), sizeof(GCoordinate) * Coordinates.size());
		}
		///////////////////////////////////////////////////////////////////////////////
		
		//!�õ�����
		void GLineString::SetCoordinates(u32 count,GCoordinate* pCoordinates)
		{
			Dirty  = true;
			Coordinates.clear();
			for(int i=0;i<count;i++)
			{
				Coordinates.push_back(pCoordinates[i]);
			}			

		}

		//!��������
		void GLineString::SetCoordinate(u32 index,f64 x,f64 y)
		{
			SetCoordinate(index,GCoordinate(x,y));
		}

		//!��������
		void GLineString::SetCoordinate(u32 index,const GCoordinate& coordinate)
		{
			
			Dirty =true;
			if(index>=0&& index<Coordinates.size())
			{
				Coordinates[index]=coordinate;
			}
		}

		//!��������
		void GLineString::InsertCoordinate(u32 index,const GCoordinate& coordinate)
		{
			Dirty =true;
			if(index>=00&& index<Coordinates.size())
			{
				Coordinates.insert(Coordinates.begin()+index,coordinate);
			}
			//else
			//{
			//	AppendCoordinate(coordinate);
			//}
		}
		
		//!��������
		void GLineString::InsertCoordinate(u32 index,f64 x,f64 y)
		{
			InsertCoordinate(index,GCoordinate(x,y));
		}

		//!��������
		void GLineString::AppendCoordinate(const GCoordinate& coordinate)
		{
			Dirty = true;
			Coordinates.push_back(coordinate);
		}
		
		//!��������
		void GLineString::AppendCoordinate(f64 x,f64 y)
		{
			AppendCoordinate(GCoordinate(x,y));
		}

		//!�Ƴ�����
		void GLineString::RemoveCoordinate(u32 index)
		{			
			Dirty =true;
			if(index>=0&& index<Coordinates.size())
			{
				Coordinates.erase(Coordinates.begin()+index);
			}
		}
		//!�������
		void GLineString::Clear()
		{
			Coordinates.clear();
		}

		

		//!�Լ��ζ����������任
		void  GLineString::Transform(IGeoTransform* pTransformer)
		{
			if(pTransformer == NULL)return;
			u32 count = Coordinates.size();
			for(int i=0;i<count;i++)
			{
				GCoordinate& coordinate = Coordinates[i];
				pTransformer->Transform(coordinate.X,coordinate.Y,&coordinate.X,&coordinate.Y);
			}
			
			Dirty = true;
		}
		//--------------------------------------------------------------------
		//!Save
		void GLineString::Save(IWriteFile* pStream)
		{
			u32 count = Coordinates.size();
			pStream->Write(&count,sizeof(count));

			for(int i=0;i<count;i++)
			{
				GCoordinate& coordinate = Coordinates[i];
				pStream->Write(&coordinate,sizeof(GCoordinate));
			}
		}

		//!Load
		void GLineString::Load(IReadFile* pStream)
		{
			if(Coordinates.size()>0)
			{
				Coordinates.clear();
			}

			u32 count;
			pStream->Read(&count,sizeof(u32),NULL);

			GCoordinate* coordinates = new GCoordinate[count];
			pStream->Read(coordinates,sizeof(GCoordinate)*count);
			for(int i=0;i<count;i++)
			{
				Coordinates.push_back(coordinates[i]);
			}
			delete[] coordinates;
			Dirty = true;
		}

		//////////////////////////////////////////////////////////////////////////
		
		GLineString* GLineString::Create()
		{
			GLineString* lineString = new 	GLineString(0,	NULL);
			return lineString;
		}

		GLineString* GLineString::Create(u32 count,GCoordinate* pCoordinates)
		{
			GLineString* lineString = new 	GLineString(count,	pCoordinates);
			return lineString;
		}

		GLineString* GLineString::Create(const vector<GCoordinate>& pCoordinates)
		{
			GLineString* lineString = new 	GLineString(pCoordinates);
			return lineString;
		}

		GLineString* GLineString::Create(f64 x1,f64 y1,f64 x2,f64 y2)
		{		
			return GLineString::Create(GCoordinate(x1,y1),GCoordinate(x2,y2));
		}

		GLineString* GLineString::Create(const GCoordinate& p0,const GCoordinate& p1)
		{
			vector<GCoordinate> coordinates;
			coordinates.push_back(p0);
			coordinates.push_back(p1);
			return GLineString::Create(coordinates);
		}

}