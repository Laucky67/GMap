#ifndef __G_FIELD_VALUE_H__
#define __G_FIELD_VALUE_H__

#include "GCoreBase.h"
#include "GUnknown.h"
#include "GFieldDef.h"
#include <comdef.h>
namespace gmap
{
	//!�ֶ�ֵ
	class GMAPCORE_API GFieldValue:public GUnknown
	{
	public:
		GFieldValue()
		{
		
		}

		//!�õ�ֵ
		_variant_t GetValue();

		//!�õ�ֵ�Ķ���
		const _variant_t& GetValueRef();

		//!����ֵ
		void SetValue(_variant_t value)
		{
			Value = value;
		}

	protected:
		_variant_t Value;

	};

}

#endif