#ifndef __G_COORDINATE_SYSTEM_H__
#define __G_COORDINATE_SYSTEM_H__

#include "GCoreBase.h"
#include "GGeometry.h"


namespace gmap
{
	//!坐标系
	class GMAPCORE_API GCoordinateSystem :public GUnknown
	{
	public:
		enum CoordinateSystemType
		{
			CST_UNKNOWN,	//未知坐标系
			CST_LONLAT,		//地理坐标系,经纬度
			CST_PROJECTION,	//投影坐标系
			CST_GEOCENTER
		};
	public:
		//!构造函数
		GCoordinateSystem(const string& wkName);

		virtual ~GCoordinateSystem();

		//!类型
		const string&	GetName();

		//!得到坐标系的类型
		virtual CoordinateSystemType GetType();

		//!得到参数字符串
		const string& GetParameters();

		//!得到WkName
		const string& GetWkName();
		//!
		bool Forward(double lon, double lat, double* x, double* y) { *x = lon; *y = lat; return true; }
		//!
		bool Inverse(double x, double y, double* lon, double* lat) { *lon = x; *lat = y; return true; }

		static GCoordinateSystem*	CreateUnknownCoordinateSystem();

		static void					LoadEPSG();

		static void					FreeEPSG();

		static int					GetEPGSCoordSysCount();

		static GCoordinateSystem*	GetEPSGCoordinateSystem(int index);

		static GCoordinateSystem*	GetEPSGCoordinateSystem(const string& name);

		static IGeoTransform*		CreateTransform(GCoordinateSystem* pInCS, GCoordinateSystem* pOutCS);
	protected:
		string							mWkName;
		string							mName;
		string							mParameters;
		static vector<GCoordinateSystem*> EPSGList;
	};

	class GCoordinateSystemPROJ :public GCoordinateSystem
	{
	public:
		GCoordinateSystemPROJ(const string& name, const string& wkName, const string& projs, void* proj);
		~GCoordinateSystemPROJ();

		//!得到坐标系的类型
		virtual CoordinateSystemType GetType();
		//!
		bool Forward(double lon, double lat, double* x, double* y);
		//!
		bool Inverse(double x, double y, double* lon, double* lat);
	protected:
		void*			mPROJ;
		friend class	GTransformPROJ;
	};


	class GTransformNull :public IGeoTransform
	{
	public:
		virtual void Transform(double inX, double inY, double* outX, double* outY)
		{
			*outX = inX;
			*outY = inY;
		}
	};

	class GTransformPROJ :public IGeoTransform
	{
	public:
		GTransformPROJ(GCoordinateSystemPROJ* inCS, GCoordinateSystemPROJ* outCS);
		~GTransformPROJ();
		virtual void Transform(double inX, double inY, double* outX, double* outY);
	private:
		void* mInPROJ;
		void* mOutPROJ;
	};

}

#endif