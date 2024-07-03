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

		//!与上下文进行关联
		void AttachUIContext(IMapUIContext* pUIContext);

		//!编辑对象的名字
		const wstring& GetName();

		//!执行测试
		virtual bool HitTest(GVector2dd& hitPoint,f64 tolerence)=0;

		//!查询当前位置上支持编辑类型
		virtual EnumEditHandleType QueryEditHandle(GVector2dd& hitPoint,u32* pHandleCookie,f64 tolerence)=0;

		//!开始编辑
		virtual void DrawEditEffect(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie)=0;
		
		//!开始编辑
		virtual void BeginEdit(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie)=0;
		
		//!鼠标落下
		virtual void MouseDown(GVector2dd& point,u32 flags)=0;

		//!鼠标移动
		virtual void MouseMove(GVector2dd& point,u32 flags)=0;

		//!鼠标抬起
		virtual void MouseUp(GVector2dd& point,u32 flags)=0;

		//!结束编辑
		virtual void EndEdit()=0;

		//!绘制自身
		virtual void Draw(Graphics* graphics)=0;

		//!修改之后
		virtual void OnEdited()=0;
	protected:
		wstring			Name;
		IMapUIContext*  MapUIContext;
	};

}
#endif
