#ifndef __G_SQL_HELPER_H__
#define __G_SQL_HELPER_H__
#include "GCoreBase.h"
#include "GFieldDef.h"

namespace gmap
{
	class GSqlHelper
	{
	public:
		static wstring GetlSqStatementOfField(const wstring& fieldName,GFieldDef::EnumFieldType fieldType,u32 fieldSize)
		{

			wchar_t wcSize[16];
			memset(wcSize,0,sizeof(wchar_t)*16);
			_itow_s(fieldSize,wcSize,10);
			wstring wstrSize(wcSize);
			wstring fnName=wstring(L"[")+fieldName+wstring(L"]");

			switch(fieldType)
			{
			case	GFieldDef::ftEmpty				:return wstring(L"");
			case	GFieldDef::ftTinyInt			:return fnName+wstring(L" int");
			case	GFieldDef::ftSmallInt			:return fnName+wstring(L" int");
			case	GFieldDef::ftInteger			:return fnName+wstring(L" int");
			case	GFieldDef::ftBigInt			:return fnName+wstring(L" int");
			case	GFieldDef::ftUnsignedTinyInt	:return fnName+wstring(L" int");
			case	GFieldDef::ftUnsignedSmallInt	:return fnName+wstring(L" int");
			case	GFieldDef::ftUnsignedInt		:return fnName+wstring(L" int");
			case	GFieldDef::ftUnsignedBigInt	:return fnName+wstring(L" int");
			case	GFieldDef::ftSingle			:return fnName+wstring(L" float");
			case	GFieldDef::ftDouble			:return fnName+wstring(L" float");
			case	GFieldDef::ftCurrency			:return fnName+wstring(L" float");
			case	GFieldDef::ftDecimal			:return fnName+wstring(L" float");
			case	GFieldDef::ftNumeric			:return fnName+wstring(L" float");
			case	GFieldDef::ftBoolean			:return fnName+wstring(L" boolean");
			case	GFieldDef::ftError				:return wstring(L"");
			case	GFieldDef::ftUserDefined		:return wstring(L"");
			case	GFieldDef::ftVariant			:return wstring(L"");
			case	GFieldDef::ftIDispatch			:return wstring(L"");
			case	GFieldDef::ftIUnknown			:return wstring(L"");
			case	GFieldDef::ftGUID				:return wstring(L"");
			case	GFieldDef::ftDate				:return wstring(L"");
			case	GFieldDef::ftDBDate			:return fnName+wstring(L" datetime");
			case	GFieldDef::ftDBTime			:return fnName+wstring(L" datetime");
			case	GFieldDef::ftDBTimeStamp		:return fnName+wstring(L" datetime");
			case	GFieldDef::ftBSTR				:return fnName+wstring(L" ntext");
			case	GFieldDef::ftChar				:return fnName+wstring(L" varchar(")+wstrSize+wstring(L")");
			case	GFieldDef::ftVarChar			:return fnName+wstring(L" varchar(")+wstrSize+wstring(L")");
			case	GFieldDef::ftLongVarChar		:return fnName+wstring(L" varchar(")+wstrSize+wstring(L")");
			case	GFieldDef::ftWChar				:return fnName+wstring(L" nvarchar(")+wstrSize+wstring(L")");
			case	GFieldDef::ftVarWChar			:return fnName+wstring(L" nvarchar(")+wstrSize+wstring(L")");
			case	GFieldDef::ftLongVarWChar		:return fnName+wstring(L" nvarchar(")+wstrSize+wstring(L")");
			case	GFieldDef::ftBinary			:return fnName+wstring(L" image");
			case	GFieldDef::ftVarBinary			:return fnName+wstring(L" image");
			case	GFieldDef::ftLongVarBinary		:return fnName+wstring(L" image");
			case	GFieldDef::ftChapter			:return wstring(L"");
			case	GFieldDef::ftFileTime			:return wstring(L"");
			case	GFieldDef::ftPropVariant		:return wstring(L"");
			case	GFieldDef::ftVarNumeric		:return wstring(L"");
			case	GFieldDef::ftArray				:return wstring(L"");
			default:
				return wstring(L"");

			}
		}	

		static _ParameterPtr AddSqlCommandParamter(_CommandPtr comm,const wstring& name,GFieldDef::EnumFieldType type,long size,const _variant_t& val=vtMissing,long attributes=adParamSigned|adParamNullable|adParamLong)
		{
			_ParameterPtr param=comm->CreateParameter(_bstr_t(name.c_str()),(DataTypeEnum)type,adParamInput,size);
			param->Size = size;
			param->Attributes=attributes;
			HRESULT hr = comm->Parameters->Append(param);
			if(val!=vtMissing)
			{
				param->Value = val;
			}
			return param;
		}
	};
}
#endif