#ifndef __G_PLG_EDIT_OBJECT_H__
#define __G_PLG_EDIT_OBJECT_H__

#include "GMapEditObject.h"

namespace gmap{

	//!��༭����
	class GMAPCORE_API GPolygonEditObject:public GMapEditObject
	{
	public:
		GPolygonEditObject(const wstring& name,GPolygon* polygon);

		~GPolygonEditObject(void);

		//!ִ�в���
		virtual bool HitTest(GVector2dd& hitPoint,f64 tolerence);

		//!��ѯ��ǰλ����֧�ֱ༭����
		virtual EnumEditHandleType QueryEditHandle(GVector2dd& hitPoint,u32* pHandleCookie,f64 tolerence);


		virtual void DrawEditEffect(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie);
	
		//!��ʼ�༭
		virtual void BeginEdit(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie);
		
		//!�������
		virtual void MouseDown(GVector2dd& point,u32 flags);

		//!����ƶ�
		virtual void MouseMove(GVector2dd& point,u32 flags);

		//!���̧��
		virtual void MouseUp(GVector2dd& point,u32 flags);

		//!�����༭
		virtual void EndEdit();

		//!��������
		virtual void Draw(Graphics* graphics);

		//!�޸�֮��
		virtual void OnEdited();
	protected:
		GPolygon*					Polygon;
		EnumEditHandleType			EditHandleType;
		u32							HandleCookie;
		Graphics*					EditGraphics;
	};

}


#endif
