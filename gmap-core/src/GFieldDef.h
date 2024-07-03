#ifndef __G_FIELD_DEF_H__
#define __G_FIELD_DEF_H__

#include "GCoreBase.h"
#include "GUnknown.h"


namespace gmap{

	class GMAPCORE_API GFieldDef:public GUnknown
	{
	public:
		//!
		enum EnumFieldType
		{
			ftEmpty = 0,
			ftTinyInt = 16,
			ftSmallInt = 2,
			ftInteger = 3,
			ftBigInt = 20,
			ftUnsignedTinyInt = 17,
			ftUnsignedSmallInt = 18,
			ftUnsignedInt = 19,
			ftUnsignedBigInt = 21,
			ftSingle = 4,
			ftDouble = 5,
			ftCurrency = 6,
			ftDecimal = 14,
			ftNumeric = 131,
			ftBoolean = 11,
			ftError = 10,
			ftUserDefined = 132,
			ftVariant = 12,
			ftIDispatch = 9,
			ftIUnknown = 13,
			ftGUID = 72,
			ftDate = 7,
			ftDBDate = 133,
			ftDBTime = 134,
			ftDBTimeStamp = 135,
			ftBSTR = 8,
			ftChar = 129,
			ftVarChar = 200,
			ftLongVarChar = 201,
			ftWChar = 130,
			ftVarWChar = 202,
			ftLongVarWChar = 203,
			ftBinary = 128,
			ftVarBinary = 204,
			ftLongVarBinary = 205,
			ftChapter = 136,
			ftFileTime = 64,
			ftPropVariant = 138,
			ftVarNumeric = 139,
			ftArray = 8192
		};
	protected:

		//!
		GFieldDef(const wstring& name,EnumFieldType fldType,u32 fieldSize);
	public:

		~GFieldDef(void);

		//!获取名字
		const wstring& GetName();

		//!获取字段类型
		EnumFieldType  GetFieldType();

		//!得到字段尺寸
		u32			   GetFieldSize();
	private:
		wstring			Name;
		EnumFieldType	FieldType;
		u32				FieldSize;

		friend class GVectorDataLayerDef;
		friend class GVectorDataLayer;
		friend class GVectorDataQueryResultMDB;
	};

}

#endif