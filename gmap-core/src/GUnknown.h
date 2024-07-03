#ifndef __G_UNKNOWN_H__
#define __G_UNKNOWN_H__

#include "GCoreBase.h"

namespace gmap{

	//!�ṩ�������ü������ڴ����
	//!����COMʵ��
	class GMAPCORE_API GUnknown
	{
	public:
		//!���캯��
		GUnknown()
		{
			ReferenceCounter =1;
		}

		//!��������
		virtual ~GUnknown()
		{	
		}

		//!������ü���
		u32 AddRef()
		{
			ReferenceCounter++;
			return ReferenceCounter;
		}

		//!�ͷ�
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

		//!�õ�
		u32 GetReferenceCounter()
		{
			return ReferenceCounter;
		}
	protected:
		u32 ReferenceCounter;

	};

}

#endif