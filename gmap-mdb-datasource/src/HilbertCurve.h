#ifndef __HILBERT_CURVE_H__
#define __HILBERT_CURVE_H__

//希尔伯特曲线生成算法
class HilbertCurve
{
public:

	//!将二维的行列编码转换成希尔伯特编码
	template<typename PointType,typename IndexType>
	static int HbCodeFromPoint(const PointType& point,int nLevel,int initMapCodeIndex)
	{
		IndexType index = 0;
		int x = point.X;
		int y = point.Y;
		int lastMapCodeIndex = initMapCodeIndex;

		for (int iLevel = 0; iLevel < nLevel; iLevel++)
		{
			int offset = nLevel - iLevel - 1;
			int mask = 0x1 << offset;
			int x0 = (x & mask) >> offset;
			int y0 = (y & mask) >> offset;

			int index1d = (y0 << 1) + x0;

			int* positions = mInvertMapCodes[lastMapCodeIndex];

			int levelIndex = positions[index1d];
			index = (index << 2) + levelIndex;

			lastMapCodeIndex = mGenCodes[lastMapCodeIndex][levelIndex];
		}

		return index;
	}

	//!将希尔伯特编码转成行列编码
	template<typename PointType,typename IndexType >
	static  PointType HbCodeToPoint(IndexType hbCode,int nLevel,int initMapCodeIndex)
	{
		PointType point;
		point.X =0;
		point.Y =0;

		int lastMapCodeIndex = initMapCodeIndex;

		for (int iLevel = 0; iLevel < nLevel; iLevel++)
		{
			int offset = (nLevel - iLevel - 1) * 2;
			int mask = ((IndexType)0x3) << offset;
			int levelIndex = (hbCode & mask) >> offset;

			int* positions = mMapCodes[lastMapCodeIndex];
			int offsetxy = positions[levelIndex];

			int x1 = point.X * 2 + offsetxy % 2;
			int y1 = point.Y * 2 + offsetxy / 2;

			point.X = x1;
			point.Y = y1;

			lastMapCodeIndex = mGenCodes[lastMapCodeIndex][levelIndex];
		}
		return point;
	}

	static int mMapCodes[4][4];
	static int mInvertMapCodes[4][4];
	static int mGenCodes[4][4];
};


#endif