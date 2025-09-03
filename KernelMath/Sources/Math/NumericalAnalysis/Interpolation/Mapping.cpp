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
//	CLASS:	MAPPING
//
//	04-08-26:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/NumericalAnalysis/Interpolation/Mapping)
#include	INCL_KMATH(Math/Math)
#include	INCL_KCORE(Stream/iostream)

const float	MAPPING::EPSILON = 0.0001f;

//-----------------------------------------------------------------------------
//	Name:		MAPPING constructor
//	Object:		
//	04-08-17:	GGO - Created
//-----------------------------------------------------------------------------
MAPPING::MAPPING()
:
_rStartValue	( 0.0f ),
_rEndValue		( 0.0f ),
_bHasMin		( false ),
_bHasMax		( false ),
_rMin			( 0.0f ),
_rMax			( 0.0f )
{

}

//-----------------------------------------------------------------------------
//	Name:		MAPPING constructor
//	Object:		
//	04-08-26:	GGO - Created
//-----------------------------------------------------------------------------
MAPPING::MAPPING(const MAPPING &	Mapping)
{
	Copy( Mapping );
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	04-08-26:	GGO - Created
//-----------------------------------------------------------------------------
void	MAPPING::Copy(const MAPPING &	Mapping)
{
	_InterpolationPoints	= Mapping._InterpolationPoints;
	_rStartValue			= Mapping._rStartValue;
	_rEndValue				= Mapping._rEndValue;
	_bHasMin 				= Mapping._bHasMin;
	_bHasMax 				= Mapping._bHasMax;
	_rMin 					= Mapping._rMin;
	_rMax 					= Mapping._rMax;
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	04-08-26:	GGO - Created
//-----------------------------------------------------------------------------
void	MAPPING::operator=(const MAPPING &	Mapping)
{
	Copy( Mapping );	
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	04-08-26:	GGO - Created
//-----------------------------------------------------------------------------
void	MAPPING::Write(IO_STREAM &	Stream) const
{
	int	nPointsCount, nPointIndex;

	Stream << _rStartValue << _rEndValue;
	Stream << _bHasMin << _bHasMax << _rMin << _rMax;

	nPointsCount = _InterpolationPoints.GetSize();
	Stream << nPointsCount;
	for ( nPointIndex = 0; nPointIndex < nPointsCount; ++nPointIndex )
	{
		Stream << _InterpolationPoints[ nPointIndex ]._rX;
		Stream << _InterpolationPoints[ nPointIndex ]._rY;
	}
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	04-08-26:	GGO - Created
//-----------------------------------------------------------------------------
void	MAPPING::Read(IO_STREAM &	Stream)
{
	int					nPointsCount, nPointIndex;
	INTERPOLATION_POINT	Point;

	Stream >> _rStartValue >> _rEndValue;
	Stream >> _bHasMin >> _bHasMax >> _rMin >> _rMax;

	Stream >> nPointsCount;
	_InterpolationPoints.Clear();
	for ( nPointIndex = 0; nPointIndex < nPointsCount; ++nPointIndex )
	{
		Stream >>Point._rX;
		Stream >>Point._rY;

		_InterpolationPoints.PushTail( Point );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetInterpolationPointsCount
//	Object:		
//	04-08-17:	GGO - Created
//-----------------------------------------------------------------------------
int	MAPPING::GetInterpolationPointsCount() const
{
	return ( _InterpolationPoints.GetSize() + 2 );
}

//-----------------------------------------------------------------------------
//	Name:		SetInpolationPoint
//	Object:		
//	04-08-17:	GGO - Created
//-----------------------------------------------------------------------------
void	MAPPING::SetInterpolationPoint(int		nPointIndex,
									   float	rY)
{
	// Min/max?
	if ( ( _bHasMax == true ) && ( rY > _rMax ) )
	{
		rY = _rMax;
	}
	if ( ( _bHasMin == true ) && ( rY < _rMin ) )
	{
		rY = _rMin;
	}

	if ( nPointIndex == 0 )
	{
		_rStartValue = rY;
	}
	else
	if ( nPointIndex == static_cast<int>( _InterpolationPoints.GetSize() + 1 ) )
	{
		_rEndValue = rY;
	}
	else
	if ( ( nPointIndex >= 0 ) && ( nPointIndex < (Int32)_InterpolationPoints.GetSize() ) )
	{
		_InterpolationPoints[ nPointIndex - 1 ]._rY = rY;
	}
	else
	{
		QDT_FAIL();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetInpolationPoint
//	Object:		
//	04-08-17:	GGO - Created
//-----------------------------------------------------------------------------
void	MAPPING::SetInterpolationPoint(int		nPointIndex,
									   float	rX,
									   float	rY)
{
	float	rCropPrev, rCropNext;

	// Min/max?
	if ( ( _bHasMax == true ) && ( rY > _rMax ) )
	{
		rY = _rMax;
	}
	if ( ( _bHasMin == true ) && ( rY < _rMin ) )
	{
		rY = _rMin;
	}


	if ( nPointIndex == 0 )
	{
		_rStartValue = rY;
	}
	else
	if ( nPointIndex == static_cast<int>( _InterpolationPoints.GetSize() + 1 ) )
	{
		_rEndValue = rY;
	}
	else
	if ( ( nPointIndex > 0 ) && ( nPointIndex < static_cast<int>( _InterpolationPoints.GetSize() + 1 ) ) )
	{
		// Crop.
		rCropPrev = GetInterpolationPointXCoord( nPointIndex - 1 );
		rCropNext = GetInterpolationPointXCoord( nPointIndex + 1 );

		if ( rX < rCropPrev )
		{
			rX = rCropPrev;
		}
		else
		if ( rX > rCropNext )
		{
			rX = rCropNext;
		}

		// Set point new coordinates.
		_InterpolationPoints[ nPointIndex - 1 ]._rX = rX;
		_InterpolationPoints[ nPointIndex - 1 ]._rY = rY;
	}
	else
	{
		QDT_FAIL();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetInterpolationPointXCoord
//	Object:		
//	04-08-17:	GGO - Created
//-----------------------------------------------------------------------------
float	MAPPING::GetInterpolationPointXCoord(int	nPointIndex) const
{
	if ( nPointIndex == 0 )
	{
		return ( 0.0f );
	}
	else
	if ( nPointIndex == static_cast<int>( _InterpolationPoints.GetSize() + 1 ) )
	{
		return ( 1.0f );
	}
	else
	if ( ( nPointIndex > 0 ) && ( nPointIndex < static_cast<int>( _InterpolationPoints.GetSize() + 1 ) ) )
	{
		return ( _InterpolationPoints[ nPointIndex - 1 ]._rX );
	}
	else
	{
		QDT_FAIL();
		return ( 0.0f );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetInterpolationPointYCoord
//	Object:		
//	04-08-17:	GGO - Created
//-----------------------------------------------------------------------------
float	MAPPING::GetInterpolationPointYCoord(int	nPointIndex) const
{
	if ( nPointIndex == 0 )
	{
		return ( _rStartValue );
	}
	else
	if ( nPointIndex == static_cast<int>( _InterpolationPoints.GetSize() + 1 ) )
	{
		return ( _rEndValue );
	}
	else
	if ( ( nPointIndex > 0 ) && ( nPointIndex < static_cast<int>( _InterpolationPoints.GetSize() + 1 ) ) )
	{
		return ( _InterpolationPoints[ nPointIndex - 1 ]._rY );
	}
	else
	{
		QDT_FAIL();
		return ( 0.0f );
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddInterpolationPoint
//	Object:		
//	04-08-17:	GGO - Created
//-----------------------------------------------------------------------------
int	MAPPING::AddInterpolationPoint(float	rX,
								   float	rY)
{
	MAP_IT				ItCur;
	MAP_IT				ItEnd;
	INTERPOLATION_POINT	NewPoint;
	int					nInsertPos;

	// Min/max?
	if ( ( _bHasMax == true ) && ( rY > _rMax ) )
	{
		rY = _rMax;
	}
	if ( ( _bHasMin == true ) && ( rY < _rMin ) )
	{
		rY = _rMin;
	}

	if ( rX < EPSILON )
	{
		_rStartValue = rY;

		return ( 0 );
	}
	else
	if ( rX > 1.0f - EPSILON )
	{
		_rEndValue = rY;

		return ( _InterpolationPoints.GetSize() + 2 );
	}
	else
	{
	nInsertPos	= 1;
	ItCur		= _InterpolationPoints.GetHead();
	ItEnd		= _InterpolationPoints.GetTail();
	while ( ItCur != ItEnd )
	{
		if ( (*ItCur)._rX > rX )
		{
			break;
		}

		++ItCur;
		++nInsertPos;
	}

		if ( ( ItCur != _InterpolationPoints.GetTail() ) &&
			 ( MATH::Abs( (*ItCur)._rX - rX ) < EPSILON ) )
		{
			(*ItCur)._rY = rY;
		}
		else
		{
	NewPoint._rX = rX;
	NewPoint._rY = rY;

	_InterpolationPoints.Insert( ItCur, NewPoint );
		}

	return ( nInsertPos );
}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveInterpolationPoint
//	Object:		
//	04-08-17:	GGO - Created
//-----------------------------------------------------------------------------
void	MAPPING::RemoveInterpolationPoint(int	nPointIndex)
{
	--nPointIndex;

	if ( ( nPointIndex >= 0 ) && ( nPointIndex < (Int32)_InterpolationPoints.GetSize() ) )
	{
		_InterpolationPoints.Remove( _InterpolationPoints.GetHead() + nPointIndex );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetValue
//	Object:		
//	04-08-17:	GGO - Created
//-----------------------------------------------------------------------------
float	MAPPING::GetValue(float	rX,
						  int	nAlgo) const
{
	switch ( nAlgo )
	{
	case MIA_LINEAR:
		return ( GetValueLinear( rX ) );

//	case MIA_CUBIC:
//		return ( GetValueCubic( rX ) );

	default:
		QDT_FAIL();
		return ( 0.0f );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetValueLinear
//	Object:		
//	04-08-18:	GGO - Created
//-----------------------------------------------------------------------------
float	MAPPING::GetValueLinear(float	rX) const
{
	int		nPointIndex, nPointsCount;
	float	rP0x, rP0y, rP1x, rP1y, rDx, rA, rB;

	nPointsCount = GetInterpolationPointsCount();
	for ( nPointIndex = 0; nPointIndex < nPointsCount; ++nPointIndex )
	{
		if ( GetInterpolationPointXCoord( nPointIndex ) >= rX )
		{
			break;
		}
	}

	if ( nPointIndex == 0 )
	{
		nPointIndex = 1;
	}

	rP0x = GetInterpolationPointXCoord( nPointIndex - 1 );
	rP0y = GetInterpolationPointYCoord( nPointIndex - 1 );
	rP1x = GetInterpolationPointXCoord( nPointIndex );
	rP1y = GetInterpolationPointYCoord( nPointIndex );

	rDx  = rP0x - rP1x;
	if ( MATH::Abs( rDx ) < EPSILON )
	{
		return ( 0.5f * ( rP0y + rP1y ) );
	}
	else
	{
		rA = ( rP0y - rP1y ) / rDx;
		rB = rP0y - rA * rP0x;

		return ( rA * rX + rB );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetValueHermite
//	Object:		
//	04-08-18:	GGO - Created
//-----------------------------------------------------------------------------
float	MAPPING::GetValueCubic(float	rX) const
{
	/*	int		nPointIndex, nPointsCount;
	float	rP0x, rP0y, rP1x, rP1y, rT0x, rT0y, rT1x, rT1y;

	nPointsCount = GetInterpolationPointsCount();
	for ( nPointIndex = 0; nPointIndex < nPointsCount; ++nPointIndex )
	{
	if ( GetInterpolationPointXCoord( nPointIndex ) >= rX )
	{
	break;
	}
	}

	rP0x = GetCircularExtension( nPointIndex - 2, rP0x, rP0y );
	rP1x = GetCircularExtension( nPointIndex - 1, rP1x, rP1y );
	rP2x = GetCircularExtension( nPointIndex,     rP2x, rP2y );
	rP3x = GetCircularExtension( nPointIndex + 1, rP3x, rP3y );*/

	return ( 0.0f );
}

//-----------------------------------------------------------------------------
//	Name:		SetMinMax
//	Object:		
//	04-08-17:	GGO - Created
//-----------------------------------------------------------------------------
void	MAPPING::SetMinMax(bool		bHasMin,
						   bool		bHasMax,
						   float	rMin,
						   float	rMax)
{
	_bHasMin	= bHasMin;
	_bHasMax	= bHasMax;

	_rMin		= rMin;
	_rMax		= rMax;
}

//-----------------------------------------------------------------------------
//	Name:		CircularExtension
//	Object:		
//	04-08-18:	GGO - Created
//-----------------------------------------------------------------------------
int	MAPPING::CircularExtension(int	nPointIndex) const
{
	int	nPointsCount;

	nPointsCount = GetInterpolationPointsCount();

	if ( nPointIndex < 0 )
	{
		nPointIndex = -nPointIndex - 1;
	}

	nPointIndex = nPointIndex % ( 2 * nPointsCount );

	if ( nPointIndex >= nPointsCount )
	{
		nPointIndex = -nPointIndex + 2 * nPointsCount - 1;
	}

	return ( nPointIndex );
}

//-----------------------------------------------------------------------------
//	Name:		GetCircularExtension
//	Object:		
//	04-08-18:	GGO - Created
//-----------------------------------------------------------------------------
void	MAPPING::GetCircularExtension(int		nPointIndex,
									  float &	rCoordX,
									  float &	rCoordY) const
{
	nPointIndex = CircularExtension( nPointIndex );

	if ( nPointIndex == 0 )
	{
		rCoordX	= 0.0f;
		rCoordY = _rStartValue;
	}
	else
	if ( nPointIndex == static_cast<int>( _InterpolationPoints.GetSize() + 1 ) )
	{
		rCoordX	= 1.0f;
		rCoordY = _rEndValue;
	}
	else
	if ( ( nPointIndex > 0 ) && ( nPointIndex < static_cast<int>( _InterpolationPoints.GetSize() + 1 ) ) )
	{
		rCoordX = _InterpolationPoints[ nPointIndex - 1 ]._rX;
		rCoordY = _InterpolationPoints[ nPointIndex - 1 ]._rY;
	}
	else
	{
		QDT_FAIL();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	04-08-26:	GGO - Created
//-----------------------------------------------------------------------------
void	MAPPING::Clear(float	rStartValue,
					   float	rEndValue)
{
	_InterpolationPoints.Clear();

	SetInterpolationPoint( 0 , rStartValue );
	SetInterpolationPoint( 1 , rEndValue );
}

#ifndef _MASTER
//-----------------------------------------------------------------------------
//	Name:		WriteBinary
//	Object:		
//	04-08-30:	GGO - Created
//-----------------------------------------------------------------------------
void	MAPPING::WriteDependenciesBinary(int nThisOffset,
										 IO_STREAM &	Stream)
{
	if (_InterpolationPoints.GetSize() > 0)
	{
		Stream.AddPadding(16);
		int nInterpolationPointsOffset = Stream.GetPos();
		Stream.PutBufferFast( &(_InterpolationPoints[0]), _InterpolationPoints.GetSize() * sizeof( INTERPOLATION_POINT ) );

		// QDT_VECTOR relocation
		Stream.AddRelocationLink( nThisOffset + ((char*)&_InterpolationPoints - (char*)this) + VECTOR_START	, nInterpolationPointsOffset );
		Stream.AddRelocationLink( nThisOffset + ((char*)&_InterpolationPoints - (char*)this) + VECTOR_LAST	, nInterpolationPointsOffset + _InterpolationPoints.GetSize() * sizeof( INTERPOLATION_POINT ));
		Stream.AddRelocationLink( nThisOffset + ((char*)&_InterpolationPoints - (char*)this) + VECTOR_END	, nInterpolationPointsOffset + _InterpolationPoints.GetSize() * sizeof( INTERPOLATION_POINT ));
	}
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
