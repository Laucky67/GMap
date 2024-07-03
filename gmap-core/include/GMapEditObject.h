#ifndef __G_MAP_EDIT_OBJECT_H__
#define __G_MAP_EDIT_OBJECT_H__

#include "GMapUITool.h"
#include "GPoint.h"
#include "GLineString.h"
#include "GPolygon.h"
#include "GStyleGroup.h"
namespace gmap{

	class GMAPCORE_API GMapEditObject:public GUnknown
	{
	public:
		enum EnumEditHandleType
		{
			EHT_NONE			=0,
			EHT_MOVE_HANDLE		=1,
			EHT_SCALE_HANDLE	=2,
			EHT_ROTATE_HANDLE	=3,
			EHT_NODE_HANDLE		=4,
			EHT_EDGE_HANDLE		=5
		};
	public:
		GMapEditObject(const wstring& name);


		virtual ~GMapEditObject(void);

		//!�������Ľ��й���
		void AttachUIContext(IMapUIContext* pUIContext);

		//!�༭���������
		const wstring& GetName();

		//!ִ�в���
		virtual bool HitTest(GVector2dd& hitPoint,f64 tolerence)=0;

		//!��ѯ��ǰλ����֧�ֱ༭����
		virtual EnumEditHandleType QueryEditHandle(GVector2dd& hitPoint,u32* pHandleCookie,f64 tolerence)=0;

		//!��ʼ�༭
		virtual void DrawEditEffect(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie)=0;
		
		//!��ʼ�༭
		virtual void BeginEdit(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie)=0;
		
		//!�������
		virtual void MouseDown(GVector2dd& point,u32 flags)=0;

		//!����ƶ�
		virtual void MouseMove(GVector2dd& point,u32 flags)=0;

		//!���̧��
		virtual void MouseUp(GVector2dd& point,u32 flags)=0;

		//!�����༭
		virtual void EndEdit()=0;

		//!��������
		virtual void Draw(Graphics* graphics)=0;

		//!�޸�֮��
		virtual void OnEdited()=0;
	protected:
		wstring			Name;
		IMapUIContext*  MapUIContext;
	};

}
#endif
