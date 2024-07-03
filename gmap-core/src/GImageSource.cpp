#include "GImageSource.h"
#include "IFile.h"
namespace gmap{

	//!构造函数
	GImageSource::GImageSource(const wstring& name,GCoordinateSystem* pCS,const GRectangle2dd& bbox)
	{
		Name = name;
		CoordinateSystem = pCS;
		if(CoordinateSystem)
		{
			CoordinateSystem->AddRef();
		}
		BoundingBox = bbox;
	}
	
	//!析构函数
	GImageSource::~GImageSource(void)
	{
		if(CoordinateSystem)
		{
			CoordinateSystem->Release();
		}
	}

	//!获取名字
	const wstring& GImageSource::GetName()
	{
		return Name;
	}

	//!得到坐标系
	GCoordinateSystem* GImageSource::GetCoordinateSystem()
	{
		return CoordinateSystem;
	}

	//!得到范围
	const GRectangle2dd& GImageSource::GetBoundingBox()
	{
		return BoundingBox;
	}

	//bool GetKV(const string& line,string& k,string& v)
	//{
	//	int pos = line.find_first_of('=');

	//	if(pos!=-1)
	//	{
	//		k =line;
	//		k=k.substr(0,pos);
	//		v = line;
	//		v=v.substr(pos+1,line.size()-pos-1);
	//		return true;
	//	}
	//	return false;
	//}

	//GImageSource* GImageSource::CreateFromFile(const wstring& file)
	//{
	//	IReadFile* pStream = FileUtil::CreateReadFile(file.c_str());

	//	wstring imageSrc;
	//	string  srid;
	//	float x0,y0,x1,y1;

	//	while(!pStream->Eof())
	//	{
	//		string line = FileUtil::ReadLine(pStream);
	//		string k,v;
	//		if(GetKV(line,k,v))
	//		{
	//			if(k==string("srid"))
	//			{
	//				srid = v;
	//			}				
	//			if(k==string("bitmap"))
	//			{
	//				USES_CONVERSION;
	//				imageSrc = wstring(A2W(v.c_str()));
	//			}				
	//			if(k==string("bbox"))
	//			{
	//				sscanf(v.c_str(),"%f,%f,%f,%f",&x0,&y0,&x1,&y1);
	//			}
	//		}
	//	}

	//	wstring path = file;
	//	int pos = file.find_last_of(L'\\');
	//	path = path.substr(0,pos);

	//	GSimpleImageSource* pImageSource =  new GSimpleImageSource(path+wstring(L"\\")+imageSrc,GCoordinateSystem::GetEPSGCoordinateSystem(srid),GRectangle2dd(x0,y0,x1,y1));
	//	return pImageSource;
	//}
}
