//*****************************************************************************
//
// Copyright (C) 2000-2005 Quantic Dream SA
//
// These coded instructions, statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are 
// protected by French and EEC copyright laws. They may not be 
// disclosed to third parties or copied or duplicated, in whole or in part, 
// without prior written consent of Quantic Dream SA
//
// Unpublished-rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
//	CLASS:	NGONE
//
//	06-06-30:	BFA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/nGone/nGone)

//-----------------------------------------------------------------------------
//	Name:		Ctor
//	Object:		
//	06-06-30:	BFA - Created
//-----------------------------------------------------------------------------
NGONE::NGONE()
: _nVertexCount(0)
{
}

//-----------------------------------------------------------------------------
//	Name:		Ctor (copy)
//	Object:		
//	06-06-30:	BFA - Created
//-----------------------------------------------------------------------------
NGONE::NGONE(NGONE& NGone)
{
	UInt32 iPoint;
	_nVertexCount = NGone._nVertexCount;
	for (iPoint=0;iPoint<_nVertexCount;++iPoint)
	{
		_aVertices[iPoint] = NGone._aVertices[iPoint];
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddVertex
//	Object:		
//	06-06-30:	BFA - Created
//-----------------------------------------------------------------------------
void NGONE::AddVertex(const VECTOR& vPosition)
{
	_aVertices[_nVertexCount] =vPosition;
	++_nVertexCount;
}

//-----------------------------------------------------------------------------
//	Name:		ClipOnXPlane
//	Object:		
//	06-06-30:	BFA - Created
//-----------------------------------------------------------------------------
UInt32 NGONE::ClipOnXPlane(Float32 rPlanePositionX, Bool bClipSide)
{
	VECTOR	Buffer[MAX_NGONE];
	UInt32	i, iGone;

	//QDT_ASSERT(_nVertexCount >= 3)					// ici on fait pas juste des lignes :)))
	if(_nVertexCount < 3)
	{
		return _nVertexCount;
	}

	iGone=0;
	for(i=0; i<_nVertexCount-1; ++i)
	{
		Buffer[iGone] = _aVertices[i];
		++iGone;
		if(((_aVertices[i].GetX() < rPlanePositionX) && (_aVertices[i+1].GetX() > rPlanePositionX)) || ((_aVertices[i].GetX() > rPlanePositionX) && (_aVertices[i+1].GetX() < rPlanePositionX)))
		{
			Float32	Weight1	= (_aVertices[i+1].GetX()-rPlanePositionX) / (_aVertices[i+1].GetX() - _aVertices[i].GetX());
			Float32	Weight2	= 1 - Weight1;
			Buffer[iGone].SetXYZ(rPlanePositionX,_aVertices[i].GetY()*Weight1+_aVertices[i+1].GetY()*Weight2,_aVertices[i].GetZ()*Weight1+_aVertices[i+1].GetZ()*Weight2);
			++iGone;
		}
	}
	Buffer[iGone] = _aVertices[i];
	++iGone;
	if(((_aVertices[i].GetX() < rPlanePositionX) && (_aVertices[0].GetX() > rPlanePositionX)) || ((_aVertices[i].GetX() > rPlanePositionX) && (_aVertices[0].GetX() < rPlanePositionX)))
	{
		Float32	Weight1	= (_aVertices[0].GetX()-rPlanePositionX) / (_aVertices[0].GetX() - _aVertices[i].GetX());
		Float32	Weight2	= 1 - Weight1;
		Buffer[iGone].SetXYZ(rPlanePositionX,_aVertices[i].GetY()*Weight1+_aVertices[0].GetY()*Weight2,_aVertices[i].GetZ()*Weight1+_aVertices[0].GetZ()*Weight2);
		++iGone;
	}

	_nVertexCount=0;
	if(!bClipSide)
	{
		for(i=0; i<iGone; ++i)
		{
			if(Buffer[i].GetX() >= rPlanePositionX)
			{
				_aVertices[_nVertexCount] = Buffer[i];
				++_nVertexCount;
			}
		}
	}
	else
	{
		for(i=0; i<iGone; ++i)
		{
			if(Buffer[i].GetX() <= rPlanePositionX)
			{
				_aVertices[_nVertexCount] = Buffer[i];
				++_nVertexCount;
			}
		}
	}
	
	return _nVertexCount;
}

//-----------------------------------------------------------------------------
//	Name:		ClipOnYPlane
//	Object:		
//	06-06-30:	BFA - Created
//-----------------------------------------------------------------------------
UInt32 NGONE::ClipOnYPlane(Float32 rPlanePositionY, Bool bClipSide)
{
	VECTOR	Buffer[MAX_NGONE];
	UInt32	i, iGone;

	//QDT_ASSERT(_nVertexCount >= 3)					// ici on fait pas juste des lignes :)))
	if(_nVertexCount < 3)
	{
		return _nVertexCount;
	}

	iGone=0;
	for(i=0; i<_nVertexCount-1; ++i)
	{
		Buffer[iGone] = _aVertices[i];
		++iGone;
		if(((_aVertices[i].GetY() < rPlanePositionY) && (_aVertices[i+1].GetY() > rPlanePositionY)) || ((_aVertices[i].GetY() > rPlanePositionY) && (_aVertices[i+1].GetY() < rPlanePositionY)))
		{
			Float32	Weight1	= (_aVertices[i+1].GetY()-rPlanePositionY) / (_aVertices[i+1].GetY() - _aVertices[i].GetY());
			Float32	Weight2	= 1 - Weight1;
			Buffer[iGone].SetXYZ(_aVertices[i].GetX()*Weight1+_aVertices[i+1].GetX()*Weight2,rPlanePositionY,_aVertices[i].GetZ()*Weight1+_aVertices[i+1].GetZ()*Weight2);
			++iGone;
		}
	}
	Buffer[iGone] = _aVertices[i];
	++iGone;
	if(((_aVertices[i].GetY() < rPlanePositionY) && (_aVertices[0].GetY() > rPlanePositionY)) || ((_aVertices[i].GetY() > rPlanePositionY) && (_aVertices[0].GetY() < rPlanePositionY)))
	{
		Float32	Weight1	= (_aVertices[0].GetY()-rPlanePositionY) / (_aVertices[0].GetY() - _aVertices[i].GetY());
		Float32	Weight2	= 1 - Weight1;
		Buffer[iGone].SetXYZ(_aVertices[i].GetX()*Weight1+_aVertices[0].GetX()*Weight2,rPlanePositionY,_aVertices[i].GetZ()*Weight1+_aVertices[0].GetZ()*Weight2);
		++iGone;
	}

	_nVertexCount=0;
	if(!bClipSide)
	{
		for(i=0; i<iGone; ++i)
		{
			if(Buffer[i].GetY() >= rPlanePositionY)
			{
				_aVertices[_nVertexCount] = Buffer[i];
				++_nVertexCount;
			}
		}
	}
	else
	{
		for(i=0; i<iGone; ++i)
		{
			if(Buffer[i].GetY() <= rPlanePositionY)
			{
				_aVertices[_nVertexCount] = Buffer[i];
				++_nVertexCount;
			}
		}
	}
	
	return _nVertexCount;
}

//-----------------------------------------------------------------------------
//	Name:		ClipOnZPlane
//	Object:		
//	06-06-30:	BFA - Created
//-----------------------------------------------------------------------------
UInt32 NGONE::ClipOnZPlane(Float32 rPlanePositionZ, Bool bClipSide)
{
	VECTOR	Buffer[MAX_NGONE];
	UInt32	i, iGone;

	//QDT_ASSERT(_nVertexCount >= 3)					// ici on fait pas juste des lignes :)))
	if(_nVertexCount < 3)
	{
		return _nVertexCount;
	}

	iGone=0;
	for(i=0; i<_nVertexCount-1; ++i)
	{
		Buffer[iGone] = _aVertices[i];
		++iGone;
		if(((_aVertices[i].GetZ() < rPlanePositionZ) && (_aVertices[i+1].GetZ() > rPlanePositionZ)) || ((_aVertices[i].GetZ() > rPlanePositionZ) && (_aVertices[i+1].GetZ() < rPlanePositionZ)))
		{
			Float32	Weight1	= (_aVertices[i+1].GetZ()-rPlanePositionZ) / (_aVertices[i+1].GetZ() - _aVertices[i].GetZ());
			Float32	Weight2	= 1 - Weight1;
			Buffer[iGone].SetXYZ(_aVertices[i].GetX()*Weight1+_aVertices[i+1].GetX()*Weight2,_aVertices[i].GetY()*Weight1+_aVertices[i+1].GetY()*Weight2,rPlanePositionZ);
			++iGone;
		}
	}
	Buffer[iGone] = _aVertices[i];
	++iGone;
	if(((_aVertices[i].GetZ() < rPlanePositionZ) && (_aVertices[0].GetZ() > rPlanePositionZ)) || ((_aVertices[i].GetZ() > rPlanePositionZ) && (_aVertices[0].GetZ() < rPlanePositionZ)))
	{
		Float32	Weight1	= (_aVertices[0].GetZ()-rPlanePositionZ) / (_aVertices[0].GetZ() - _aVertices[i].GetZ());
		Float32	Weight2	= 1 - Weight1;
		Buffer[iGone].SetXYZ(_aVertices[i].GetX()*Weight1+_aVertices[0].GetX()*Weight2,_aVertices[i].GetY()*Weight1+_aVertices[0].GetY()*Weight2,rPlanePositionZ);
		++iGone;
	}

	_nVertexCount=0;
	if(!bClipSide)
	{
		for(i=0; i<iGone; ++i)
		{
			if(Buffer[i].GetZ() >= rPlanePositionZ)
			{
				_aVertices[_nVertexCount] = Buffer[i];
				++_nVertexCount;
			}
		}
	}
	else
	{
		for(i=0; i<iGone; ++i)
		{
			if(Buffer[i].GetZ() <= rPlanePositionZ)
			{
				_aVertices[_nVertexCount] = Buffer[i];
				++_nVertexCount;
			}
		}
	}
	
	return _nVertexCount;
}

//-----------------------------------------------------------------------------
//	Name:		ComputeSurface
//	Object:		
//	06-06-30:	BFA - Created
//-----------------------------------------------------------------------------
Float32 NGONE::ComputeSurface(const VECTOR& a_Normal) const
{
	VECTOR	csum;

	for (UInt32 i=0,j=1;i<=_nVertexCount-2;i++,j++)
	{
		csum.SetX(csum.GetX()+_aVertices[i].GetY()*_aVertices[j].GetZ()-_aVertices[i].GetZ()*_aVertices[j].GetY());
		csum.SetY(csum.GetY()+_aVertices[i].GetZ()*_aVertices[j].GetX()-_aVertices[i].GetX()*_aVertices[j].GetZ());
		csum.SetZ(csum.GetZ()+_aVertices[i].GetX()*_aVertices[j].GetY()-_aVertices[i].GetY()*_aVertices[j].GetX());
	}

	csum.SetX(csum.GetX()+_aVertices[_nVertexCount-1].GetY()*_aVertices[0].GetZ()-_aVertices[_nVertexCount-1].GetZ()*_aVertices[0].GetY());
	csum.SetY(csum.GetY()+_aVertices[_nVertexCount-1].GetZ()*_aVertices[0].GetX()-_aVertices[_nVertexCount-1].GetX()*_aVertices[0].GetZ());
	csum.SetZ(csum.GetZ()+_aVertices[_nVertexCount-1].GetX()*_aVertices[0].GetY()-_aVertices[_nVertexCount-1].GetY()*_aVertices[0].GetX());

	return 0.5f*MATH::Abs(a_Normal.GetX()*csum.GetX()+a_Normal.GetY()*csum.GetY()+a_Normal.GetZ()*csum.GetZ());
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
