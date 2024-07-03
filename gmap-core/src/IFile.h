#ifndef __I_FILE_H__
#define __I_FILE_H__

#include "GUnknown.h"
#include <vector>
#include <string>
#include <windows.h>
using namespace std;

namespace gmap
{
	//!文件
	class GMAPCORE_API IReadFile: public GUnknown
	{
	public:
		
		virtual const wstring& GetFileName()=0;

		//!文件是否结束
		virtual bool Eof()=0;

		//!移动文件指针
		virtual void Read(void* pBuffer,u32 bufferSize,u32* pReadSize=NULL)=0;

		template<class T>
		void Read(T& val)
		{
			Read(&val,sizeof(T));
		}
	};

	//!写文件接口
	class GMAPCORE_API IWriteFile:public GUnknown
	{
	public:
		virtual const wstring& GetFileName()=0;
		
		//!写缓冲区
		virtual void Write(void* pBuffer,u32 bufferSize)=0;	

		template<class T>
		void Write(T& val)
		{
			Write(&val,sizeof(T));
		}
	};

	class GMAPCORE_API FileUtil{
	public:

		static inline  void WriteString(const wstring& str,IWriteFile* pStream)
		{
			u32 size = str.size();

			if(size>1024)
				size=1024;

			pStream->Write(&size,sizeof(size));
			pStream->Write((void*)str.c_str(),sizeof(wchar_t)*size);
		}

		static inline  wstring ReadString(IReadFile* pStream)
		{
			static wchar_t buffer[1025];//不要超过1024;	
			memset(buffer,0,sizeof(wchar_t)*1025);

			u32 size = 0;
			pStream->Read(&size,sizeof(size));
			pStream->Read(buffer,sizeof(wchar_t)*size);
			return wstring(buffer);
		}

		static inline wstring ReadLineW(IReadFile* pStream)
		{
			wchar_t c;
			wchar_t buffer[4096];
			memset(buffer,0,4096*2);
			int n=0;

			c='\n';
			while((c==L'\n'||c==L'\r')&& !pStream->Eof())
			{
				pStream->Read(&c,sizeof(c),0);
			}
			
			if(c!=L'\n'||c!=L'\r')
			{
				while(c!=L'\n'&&c!=L'\r'&& !pStream->Eof())
				{
					buffer[n++]=c;
					pStream->Read(&c,sizeof(c),0);					
				}
			}
			return wstring(buffer);
		
		}

		static inline string ReadLine(IReadFile* pStream)
		{
			char c;
			char buffer[4096];
			memset(buffer,0,4096);
			int n=0;

			c='\n';
			while((c=='\n'||c=='\r')&& !pStream->Eof())
			{
				pStream->Read(&c,sizeof(c),0);
			}
			
			if(c!='\n'||c!='\r')
			{
				while(c!='\n'&&c!='\r'&& !pStream->Eof())
				{
					buffer[n++]=c;
					pStream->Read(&c,sizeof(c),0);					
				}
			}
			return string(buffer);
		}


		static IReadFile* CreateReadFile(const wchar_t* fn);

		static IWriteFile* CreateWriteFile(const wchar_t* fn);
	};

	class GMAPCORE_API GMemReadFile:public IReadFile
	{
		
		GMemReadFile(void* buffer,u32 size,const wstring& name);

	public:
		~GMemReadFile();

		virtual const wstring& GetFileName();

		//!文件是否结束
		virtual bool Eof();

		//!移动文件指针
		virtual void Read(void* pBuffer,u32 bufferSize,u32* pReadSize=NULL);
	public:
		static GMemReadFile* Create(void* buffer,u32 size,const wstring& name=wstring(L""));
	private:
		BYTE* Buffer;
		u32	  BufferSize;
		BYTE* Pointer;
		wstring Name;
	};

	class GMAPCORE_API GMemWriteFile:public IWriteFile
	{
		GMemWriteFile(const wstring& name);
	public:
		~GMemWriteFile();
		//!
		virtual const wstring& GetFileName();
		//!写缓冲区
		virtual void Write(void* pBuffer,u32 bufferSize);	
		//!得到缓冲区
		BYTE* GetBuffer();
		//!尺寸
		u32	  GetDataSize();
		//Rest
		void  Reset();
	public:
		//GMAPCORE_API
		static GMemWriteFile* Create(const wstring& name=wstring(L""));
	private:
		BYTE* Buffer;
		u32	  BufferSize;
		u32	  DataSize;
		wstring Name;
	};

	inline bool GetKV(const string& line, string& k, string& v)
	{
		int pos = line.find_first_of('=');

		if (pos != -1)
		{
			k = line;
			k = k.substr(0, pos);
			v = line;
			v = v.substr(pos + 1, line.size() - pos - 1);
			return true;
		}
		return false;
	}
	 
}

#endif