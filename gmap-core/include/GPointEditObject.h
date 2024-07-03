#ifndef __G_POINT_EDIT_OBJECT_H__
#define __G_POINT_EDIT_OBJECT_H__

#include "GMapEditObject.h"

namespace gmap{

	//!点编辑对象
	class GMAPCORE_API GPointEditObject:public GMapEditObject
	{
	public:
		GPointEditObject(const wstring& name,GPoint* point);

		~GPointEditObject(void);

		//!执行测试
		virtual bool HitTest(GVector2dd& hitPoint,f64 tolerence);

		//!查询当前位置上支持编辑类型
		virtual EnumEditHandleType QueryEditHandle(GVector2dd& hitPoint,u32* pHandleCookie,f64 tolerence);


		virtual void DrawEditEffect(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie);
	

		//!开始编辑
		virtual void BeginEdit(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie);
		
		//!鼠标落下
		virtual void MouseDown(GVector2dd& point,u32 flags);

		//!鼠标移动
		virtual void MouseMove(GVector2dd& point,u32 flags);

		//!鼠标抬起
		virtual void MouseUp(GVector2dd& point,u32 flags);

		//!结束编辑
		virtual void EndEdit();

		//!绘制自身
		virtual void Draw(Graphics* graphics);

		//!修改之后
		virtual void OnEdited();
	protected:
		GPoint*			Point;
		bool			Editing;
		Graphics*		EditGraphics;
	};
}


#endif
