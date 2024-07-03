#include "IFile.h"
#include <windows.h>
namespace gmap{

	class CReadFile:public IReadFile
	{
	public:
		CReadFile(const wstring& fn,HANDLE file)
		{
			FileName = fn;
			File = file;
			NotEOF= TRUE;
		}
		~CReadFile()
		{
			CloseHandle(File);
		}


		virtual const wstring& GetFileName()
		{
			return FileName;
		}

		//!文件是否结束
		virtual bool Eof()
		{
			return !NotEOF;
		}
		//!移动文件指针
		virtual void Read(void* pBuffer,u32 bufferSize,u32* pReadSize)
		{
			DWORD dwSize;
			NotEOF= ReadFile(File,pBuffer,bufferSize,&dwSize,NULL);
			NotEOF = dwSize>0;

			if(pReadSize)*pReadSize = dwSize;
		}
	private:
		wstring FileName;
		HANDLE	File;		
		BOOL    NotEOF;
	};

	//-----------------------------------------------------
	class CWriteFile:public IWriteFile
	{
	public:
		CWriteFile(const wstring& fn,FILE* file)
		{
			FileName = fn;
			File = file;
		}
		~CWriteFile()
		{
			fclose(File);
		}

		virtual const wstring& GetFileName()
		{
			return FileName;
		}

		//!移动文件指针
		virtual void Write(void* pBuffer,u32 bufferSize)
		{
			int rs = fwrite(pBuffer,bufferSize,1,File);
		}
	private:
		wstring FileName;
		FILE*	File;		
	};
	//-----------------------------------------------------

	IReadFile* FileUtil::CreateReadFile(const wchar_t* fn)
	{	
		HANDLE file = CreateFile(fn,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(file!=INVALID_HANDLE_VALUE)
		{
			return new CReadFile(wstring(fn),file);
		}
		else
		{
			return NULL;
		}
	}

	IWriteFile* FileUtil::CreateWriteFile(const wchar_t* fn)
	{
		FILE* file = _wfopen(fn, L"w+");
		if(file!=0)
		{
			return new CWriteFile(wstring(fn),file);
		}
		else
		{
			return NULL;
		}	
	}
	//---------------------------------------------------------------			
	GMemReadFile::GMemReadFile(void* buffer,u32 size,const wstring& name)
	{
		Buffer = (BYTE*)buffer;
		Pointer = Buffer;
		BufferSize = size;
	}

	GMemReadFile::~GMemReadFile()
	{

	}

	const wstring& GMemReadFile::GetFileName()
	{
		return Name;
	}

	//!文件是否结束
	bool GMemReadFile::Eof()
	{
		return (Pointer>=Buffer);
	}

	//!移动文件指针
	void GMemReadFile::Read(void* pBuffer,u32 bufferSize,u32* pReadSize)
	{
		u32 readSize =0;
		BYTE* pBuffer2 = (BYTE*)pBuffer;
		while(Pointer<(Buffer+BufferSize)&&readSize<bufferSize)
		{
			*pBuffer2=*Pointer;
			pBuffer2++;
			Pointer++;
			readSize++;
		}
		if(pReadSize)*pReadSize=readSize;
	}

	GMemReadFile* GMemReadFile::Create(void* buffer,u32 size,const wstring& name)
	{
		return new GMemReadFile((BYTE*)buffer,size,name);
	}
	//----------------------------------------------------------------------
	const int STACK_SIZE=1024;

	GMemWriteFile::GMemWriteFile(const wstring& name)
	{
		Buffer = (BYTE*)malloc(STACK_SIZE);
		Name = name;
		BufferSize = STACK_SIZE;
		DataSize	= 0;
		memset(Buffer,0,BufferSize);
	}

	GMemWriteFile::~GMemWriteFile()
	{
		free(Buffer);
	}


	//!
	const wstring& GMemWriteFile::GetFileName()
	{
		return Name;
	}

	//!写缓冲区
	void GMemWriteFile::Write(void* pBuffer,u32 bufferSize)
	{
		if(DataSize+bufferSize>BufferSize)
		{
			u32 newSize = BufferSize+STACK_SIZE;
			byte* newBuffer =(BYTE*) malloc(newSize);
			memset(newBuffer,0,newSize);
			memcpy(newBuffer,Buffer,BufferSize);

			BufferSize = newSize;
			free(Buffer);
			Buffer = newBuffer;
		}

		int i=0;
		BYTE* pointer = (BYTE*)pBuffer;
		while(i<bufferSize)
		{
			Buffer[DataSize]=pointer[i];
			DataSize++;
			i++;
		}
	}

	//!得到缓冲区
	BYTE* GMemWriteFile::GetBuffer()
	{
		return Buffer;
	}

	u32	  GMemWriteFile::GetDataSize()
	{
		return DataSize;
	}
	
	void  GMemWriteFile::Reset()
	{
		DataSize=0;
	}

	GMemWriteFile* GMemWriteFile::Create(const wstring& name)
	{
		return new GMemWriteFile(name);
	}

}