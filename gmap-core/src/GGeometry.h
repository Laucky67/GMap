#ifndef __G_GEOMETRY_H__
#define __G_GEOMETRY_H__
#include "GCoreBase.h"
#include "IFile.h"
namespace gmap{

	typedef GVector2dd  GCoordinate;

	class GMAPCORE_API IGeoTransform:public GUnknown
	{
	public:
		virtual void Transform(double inX,double inY,double* outX,double* outY)=0;
	};

	//!几何对象的基类
	class GMAPCORE_API GGeometry:public GUnknown
	{
	public:
	 //!几何类型
	 enum EnumGeometryType 
	 {
		GT_UNKNOWN,
		GT_POINT,
		GT_LINE_STRING,
		GT_LINEAR_RING,
		GT_POLYGON,
		GT_COLLECTION=0x00000010
	 };
	public:
		//!构造函数
		GGeometry(void):GUnknown(){}
		
		//!析构函数
		virtual	~GGeometry(void){}

		//!得到包围盒
		virtual	GRectangle2dd GetBoundingBox()=0;

		//!Clone
		virtual GGeometry* Clone()=0;

		//!得到几何类型
		virtual GGeometry::EnumGeometryType GetGeometryType(){return GT_UNKNOWN;}

		//!Save
		virtual void Save(IWriteFile* pStream){}

		//!Load
		virtual void Load(IReadFile* pStream){}

		//!对几何对象进行坐标变换
		virtual void Transform(IGeoTransform* pTransformer)=0;

		////!
		////返回几何对象的中心点，可以是所有点的平均值代替。
		//virtual GCoordinate GetCentriod() = 0;
		////!返回几何对象的长度，点是0
		//virtual double		GetLength() = 0;
		////!返回几何对象的面积
		//virtual double		GetArea() = 0;
	public:
		//!从流中创建
		static GGeometry* CreateFromStream(IReadFile* pStream);
		
		//!保存到流中
		static void SaveToStream(GGeometry* pGeometry,IWriteFile* pStream);
	protected:
	};


}

#endif
