#ifndef __G_UNKNOWN_H__
#define __G_UNKNOWN_H__

#include "GCoreBase.h"

namespace gmap{

	//!提供基于引用计数的内存管理
	//!参照COM实现
	class GMAPCORE_API GUnknown
	{
	public:
		//!构造函数
		GUnknown()
		{
			ReferenceCounter =1;
		}

		//!析构函数
		virtual ~GUnknown()
		{	
		}

		//!添加引用计数
		u32 AddRef()
		{
			ReferenceCounter++;
			return ReferenceCounter;
		}

		//!释放
		u32 Release()
		{
			ReferenceCounter--;
			if(ReferenceCounter==0)
			{
				delete this;
				return 0L;
			}
			return ReferenceCounter;			
		}

		//!得到
		u32 GetReferenceCounter()
		{
			return ReferenceCounter;
		}
	protected:
		u32 ReferenceCounter;

	};

}

#endif