#ifndef __G_FIELD_VALUE_H__
#define __G_FIELD_VALUE_H__

#include "GCoreBase.h"
#include "GUnknown.h"
#include "GFieldDef.h"
#include <comdef.h>
namespace gmap
{
	//!字段值
	class GMAPCORE_API GFieldValue:public GUnknown
	{
	public:
		GFieldValue()
		{
		
		}

		//!得到值
		_variant_t GetValue();

		//!得到值的定义
		const _variant_t& GetValueRef();

		//!设置值
		void SetValue(_variant_t value)
		{
			Value = value;
		}

	protected:
		_variant_t Value;

	};

}

#endif