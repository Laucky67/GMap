#include "GCoordinateSystem.h"
#include "proj_api.h"
#include "IFile.h"

namespace gmap
{
	vector<GCoordinateSystem*> GCoordinateSystem::EPSGList;

	vector<string> GetParamsFromString(const string& parameters)
	{
		string::const_iterator pointer = parameters.begin();
		char*  param=0;
		char buffer[1024];
		int n=0;
		int count=0;
		vector<string> paramlist;

		while(pointer!=parameters.end())
		{
			if(*pointer=='+')
			{
				if(count>0)
				{
					paramlist.push_back(string(buffer));
				}
				memset(buffer,0,1024);
				n=0;
				count++;
			}

			else if(*pointer!=' ')
			{
				buffer[n++]=*pointer;
			}				
			pointer++;
		}
		paramlist.push_back(string(buffer));
		return paramlist;
	}

	map<string,string> GetNamedParamsFromString(const string& parameters)
	{
		vector<string> paramlist=GetParamsFromString(parameters);
		map<string,string> nameparams;
		for(int i=0;i<paramlist.size();i++)
		{
			string param = paramlist.at(i);
			int pos = param.find_first_of('=');

			if(pos!=-1)
			{

				string k =param;
				k=k.substr(0,pos);
				string v = param;
				v=v.substr(pos+1,param.size()-pos-1);
				nameparams[k]=v;
			}
		}

		return nameparams;
	}

	//---------------------------------------------------------------
	//!构造函数
	GCoordinateSystem::GCoordinateSystem(const string& wkName)
	{
		mName = "UNKNOWN";
		mParameters = "";
		mWkName = wkName;
	}

	GCoordinateSystem::~GCoordinateSystem()
	{

	}
	//!得到WkName
	const string& GCoordinateSystem::GetWkName()
	{
		return mWkName;
	}


	//!类型
	const string&	GCoordinateSystem::GetName()
	{
		return mName;
	}

	//!得到坐标系的类型
	GCoordinateSystem::CoordinateSystemType GCoordinateSystem::GetType()
	{
		return GCoordinateSystem::CST_UNKNOWN;
	}

	//!得到参数字符串
	const string& GCoordinateSystem::GetParameters()
	{
		return mParameters;
	}

	GCoordinateSystem* GCoordinateSystem::CreateUnknownCoordinateSystem()
	{
		return new GCoordinateSystem("Unknown");
	}

	GCoordinateSystemPROJ::GCoordinateSystemPROJ(const string& name, const string& wkName, const string& projs, void* proj)
		:GCoordinateSystem(wkName)
	{
		mName = name;
		mParameters = projs;
		mPROJ = proj;
	}
	GCoordinateSystemPROJ::~GCoordinateSystemPROJ()
	{
		pj_free(mPROJ);
	}

	GCoordinateSystem::CoordinateSystemType GCoordinateSystemPROJ::GetType()
	{
		if (pj_is_latlong(mPROJ))
			return CST_LONLAT;
		if (pj_is_geocent(mPROJ))
			return CST_GEOCENTER;
		return CST_PROJECTION;
	}

	//!
	bool GCoordinateSystemPROJ::Forward(double lon, double lat, double* x, double* y)
	{
		if (pj_is_latlong(mPROJ))
		{
			*x = lon; *y = lat;
			return true;
		}
		else
		{
			projUV p={lon,lat};

			p.u*=DEGTORAD64;
			p.v*=DEGTORAD64;

			p = pj_fwd(p, mPROJ);

			*x= p.u;
			*y =p.v;
			return true;
		}
	}

	//!
	bool GCoordinateSystemPROJ::Inverse(double x, double y, double* lon, double* lat)
	{
		if (pj_is_latlong(mPROJ))
		{
			*lon = x; *lat = y;
			return true;
		}
		else
		{
			projUV p = { x,y };

			p.u *= DEGTORAD64;
			p.v *= DEGTORAD64;

			p = pj_inv(p, mPROJ);

			*lon = p.u*RADTODEG64;
			*lat = p.v;
			return true;
		}
	}

	GTransformPROJ::GTransformPROJ(GCoordinateSystemPROJ* inCS, GCoordinateSystemPROJ* outCS)
	{
		mInPROJ = inCS->mPROJ;
		mOutPROJ = outCS->mPROJ;
	}
	
	GTransformPROJ::~GTransformPROJ()
	{

	}
	
	void GTransformPROJ::Transform(double inX, double inY, double* outX, double* outY)
	{
		if (pj_is_latlong(mInPROJ))
		{
			inX *= DEGTORAD64;
			inY *= DEGTORAD64;
		}
		pj_transform(mInPROJ, mOutPROJ,1,1,&inX, &inY, NULL);
		if (pj_is_latlong(mOutPROJ))
		{
			inX *= RADTODEG64;
			inY *= RADTODEG64;
		}
		*outX = inX;
		*outY = inY;
	}

	//GCoordinateSystem* GCoordinateSystem::CreateGeographicsCoordinateSystem(const string& name,const string& ellps,const string& wkname)
	//{
	//	return new GGeographicsCoordinateSystem(name,ellps,wkname);
	//}

	//GCoordinateSystem* GCoordinateSystem::CreateProjectionCoordinateSystem(const string& name,const string& parameters,const string& wkname)
	//{
	//	return new GProjectionCoordinateSystem(name,parameters,wkname);
	//}

	string ParseEPSG(const string& line,int* id)
	{
		string s0,s1;
		int n=0;
		int n0=0;

		char c;
		while((c=line[n++])!='<');
		n0=n;

		while((c=line[n++]!='>'));

		s0 = line;
		s0=s0.substr(n0,n-n0-1);

		*id = atoi(s0.c_str());

		//while((c==line[n++]!='+'));

		s1=line.substr(n);

		return s1;
	}
	//-------------------------------------------------------------
	void GCoordinateSystem::LoadEPSG()
	{
		const wchar_t* epsg = L".\\dat\\epsg";
		IReadFile* pStream = FileUtil::CreateReadFile(epsg);
		string preline;

		while(!pStream->Eof())
		{
			string line = FileUtil::ReadLine(pStream);

			if(line.size()>0)
			{
				if(line[0]!='#')
				{
					int id;							 
					string parameters = ParseEPSG(line,&id);
					map<string,string> params2 = GetNamedParamsFromString(parameters);

					char buffer[256];
					memset(buffer,0,256);
					sprintf(buffer,"epsg:%d",id);

					string proj = params2["proj"];


					void* pj = pj_init_plus(parameters.c_str());
					if (pj!=NULL)
					{
						GCoordinateSystemPROJ* PROJ = new GCoordinateSystemPROJ(string(buffer), preline, parameters, pj);
						EPSGList.push_back(PROJ);
					}
				}
				preline = line;
			}
		}

		pStream->Release();
	}

	void GCoordinateSystem::FreeEPSG()
	{
		for(int i=0;i<EPSGList.size();i++)
		{
			EPSGList.at(i)->Release();
		}

		EPSGList.clear();

	}

	int GCoordinateSystem::GetEPGSCoordSysCount()
	{
		return EPSGList.size();
	}

	GCoordinateSystem* GCoordinateSystem::GetEPSGCoordinateSystem(int index)
	{
		return EPSGList.at(index);
	}

	GCoordinateSystem* GCoordinateSystem::GetEPSGCoordinateSystem(const string& name)
	{
		int count = EPSGList.size();
		for(int i=0;i<count;i++)
		{
			if(GCoordinateSystem::EPSGList.at(i)->GetName()==name)
			{
				return GCoordinateSystem::EPSGList.at(i);
			}
		}

		return NULL;
	}
	IGeoTransform * GCoordinateSystem::CreateTransform(GCoordinateSystem * pInCS, GCoordinateSystem * pOutCS)
	{
		if (pInCS == NULL || pOutCS == NULL)
			return new GTransformNull();
		if (pInCS->GetType() == CST_UNKNOWN || pOutCS->GetType() == CST_UNKNOWN ||
			pInCS->GetType() == CST_GEOCENTER || pOutCS->GetType() == CST_GEOCENTER)
			return new GTransformNull();
		return new GTransformPROJ(static_cast<GCoordinateSystemPROJ*>(pInCS), 
								  static_cast<GCoordinateSystemPROJ*>(pOutCS));
	}
}