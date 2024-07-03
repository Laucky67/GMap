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

	//!���ζ���Ļ���
	class GMAPCORE_API GGeometry:public GUnknown
	{
	public:
	 //!��������
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
		//!���캯��
		GGeometry(void):GUnknown(){}
		
		//!��������
		virtual	~GGeometry(void){}

		//!�õ���Χ��
		virtual	GRectangle2dd GetBoundingBox()=0;

		//!Clone
		virtual GGeometry* Clone()=0;

		//!�õ���������
		virtual GGeometry::EnumGeometryType GetGeometryType(){return GT_UNKNOWN;}

		//!Save
		virtual void Save(IWriteFile* pStream){}

		//!Load
		virtual void Load(IReadFile* pStream){}

		//!�Լ��ζ����������任
		virtual void Transform(IGeoTransform* pTransformer)=0;

		////!
		////���ؼ��ζ�������ĵ㣬���������е��ƽ��ֵ���档
		//virtual GCoordinate GetCentriod() = 0;
		////!���ؼ��ζ���ĳ��ȣ�����0
		//virtual double		GetLength() = 0;
		////!���ؼ��ζ�������
		//virtual double		GetArea() = 0;
	public:
		//!�����д���
		static GGeometry* CreateFromStream(IReadFile* pStream);
		
		//!���浽����
		static void SaveToStream(GGeometry* pGeometry,IWriteFile* pStream);
	protected:
	};


}

#endif
