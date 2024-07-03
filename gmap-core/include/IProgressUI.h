#ifndef __I_PROGRESS_UI__H__
#define __I_PROGRESS_UI__H__

#include "GCoreBase.h"
namespace gmap{

	//!进度条记录
	class IProgressUI
	{
		public:

			virtual void OnSetTotal(const wchar_t* id,u32 total)=0;

			virtual void OnProgress(const wchar_t* id,u32 progress)=0; 
			//!进度
			virtual void OnProgress(const wchar_t* id,u32 progress,u32 total)=0; 
	};

}

#endif