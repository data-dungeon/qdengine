#include "Root.h"

#include	INCL_KCORE(Template/QDT_Vector)

template <>
bool QDT_VECTOR<UInt8>::Reserve(UInt32 nReserveSize)
{
	if (nReserveSize > _nAllocatedSize)
	{
		UInt8 * pTempArray = new UInt8 [nReserveSize];
		
		if (_pArray)
		{
			memcpy(pTempArray, _pArray, _nSize * sizeof(UInt8));

			delete [] _pArray;
		}

		_pArray = pTempArray;
		_nAllocatedSize = nReserveSize;
		return (true);
	}

	return (false);
}
template <>
bool QDT_VECTOR<Int8>::Reserve(UInt32 nReserveSize)
{
	if (nReserveSize > _nAllocatedSize)
	{
		Int8 * pTempArray = new Int8 [nReserveSize];
		
		if (_pArray)
		{
			memcpy(pTempArray, _pArray, _nSize * sizeof(Int8));

			delete [] _pArray;
		}

		_pArray = pTempArray;
		_nAllocatedSize = nReserveSize;
		return (true);
	}

	return (false);
}
template <>
bool QDT_VECTOR<UInt16>::Reserve(UInt32 nReserveSize)
{
	if (nReserveSize > _nAllocatedSize)
	{
		UInt16 * pTempArray = new UInt16 [nReserveSize];
		
		if (_pArray)
		{
			memcpy(pTempArray, _pArray, _nSize * sizeof(UInt16));

			delete [] _pArray;
		}

		_pArray = pTempArray;
		_nAllocatedSize = nReserveSize;
		return (true);
	}

	return (false);
}
template <>
bool QDT_VECTOR<Int16>::Reserve(UInt32 nReserveSize)
{
	if (nReserveSize > _nAllocatedSize)
	{
		Int16 * pTempArray = new Int16 [nReserveSize];
		
		if (_pArray)
		{
			memcpy(pTempArray, _pArray, _nSize * sizeof(Int16));

			delete [] _pArray;
		}

		_pArray = pTempArray;
		_nAllocatedSize = nReserveSize;
		return (true);
	}

	return (false);
}
template <>
bool QDT_VECTOR<UInt32>::Reserve(UInt32 nReserveSize)
{
	if (nReserveSize > _nAllocatedSize)
	{
		UInt32 * pTempArray = new UInt32 [nReserveSize];
		
		if (_pArray)
		{
			memcpy(pTempArray, _pArray, _nSize * sizeof(UInt32));

			delete [] _pArray;
		}

		_pArray = pTempArray;
		_nAllocatedSize = nReserveSize;
		return (true);
	}

	return (false);
}
template <>
bool QDT_VECTOR<Int32>::Reserve(UInt32 nReserveSize)
{
	if (nReserveSize > _nAllocatedSize)
	{
		Int32 * pTempArray = new Int32 [nReserveSize];
		
		if (_pArray)
		{
			memcpy(pTempArray, _pArray, _nSize * sizeof(Int32));

			delete [] _pArray;
		}

		_pArray = pTempArray;
		_nAllocatedSize = nReserveSize;
		return (true);
	}

	return (false);
}
