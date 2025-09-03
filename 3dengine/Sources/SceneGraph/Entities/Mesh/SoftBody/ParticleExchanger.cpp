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
//	CLASS:	PARTICLE_EXCHANGER
//
//	02-03-13:	JLT - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include "Include.h"

#include	INC_KRNCOM(OldCom/Chunk)
#include	INC_KRNCORE(Stream)
	
#ifndef _DEBUG
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point3D)

#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PARTICLE_EXCHANGER constructor
//	Object:		
//	02-03-13:	JLT - Created
//-----------------------------------------------------------------------------
PARTICLE_EXCHANGER::PARTICLE_EXCHANGER()
:_NbrPoints(0)
,_pPoints(NULL)
,_pGeometryIndex(NULL)
,_pVertexIndex(NULL)
,_pTargetCoeff(NULL)
{

}

//-----------------------------------------------------------------------------
//	Name:		PARTICLE_EXCHANGER destructor
//	Object:		
//	02-03-13:	JLT - Created
//-----------------------------------------------------------------------------
PARTICLE_EXCHANGER::~PARTICLE_EXCHANGER()
{
	Clean();
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPoints
//	Object:		
//	02-03-14:	JLT - Created
//-----------------------------------------------------------------------------
unsigned int PARTICLE_EXCHANGER::GetNbrPoints() const
{
	return _NbrPoints;
}

//-----------------------------------------------------------------------------
//	Name:		Clean
//	Object:		
//	02-03-14:	JLT - Created
//-----------------------------------------------------------------------------
void PARTICLE_EXCHANGER::Clean()
{
	_NbrPoints = 0;
	delete [] _pPoints; _pPoints = NULL;
	delete [] _pGeometryIndex; _pGeometryIndex = NULL;
	delete [] _pVertexIndex; _pVertexIndex = NULL;
	delete [] _pTargetCoeff; _pTargetCoeff = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		SetIndices
//	Object:		
//	02-03-14:	JLT - Created
//-----------------------------------------------------------------------------
void PARTICLE_EXCHANGER::SetIndices(const unsigned int NbrPts, const POINT3D *pPts, const QDT_DLIST < int > *pGI, const QDT_DLIST < int > *pVI)
{
	Clean();

	if (NbrPts == 0)
		return;

	_NbrPoints = NbrPts;

	_pPoints = new POINT3D[NbrPts];
	_pGeometryIndex = new QDT_DLIST < int > [NbrPts];
	_pVertexIndex = new QDT_DLIST < int > [NbrPts];

	unsigned int i;
	for (i=0; i<NbrPts; ++i)
	{
		_pGeometryIndex[i] = pGI[i];
		_pVertexIndex[i] = pVI[i];
		_pPoints[i] = pPts[i] * 0.01f;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-03-14:	JLT - Created
//-----------------------------------------------------------------------------
bool PARTICLE_EXCHANGER::Read(IO_STREAM &s)
{
	unsigned int i;
	int j, Value, Size;
	
	Clean();

	s >> Value;
	_NbrPoints = (unsigned int) Value;

	_pPoints = new POINT3D[_NbrPoints];
	_pGeometryIndex = new QDT_DLIST < int > [_NbrPoints];
	_pVertexIndex = new QDT_DLIST < int > [_NbrPoints];
	_pTargetCoeff = new float [_NbrPoints];
	memset(_pTargetCoeff, 0, sizeof(float)*_NbrPoints);
	
	for (i=0; i<_NbrPoints; ++i)
	{
		_pPoints[i].Read(s);

		s >> Size;
		for (j=0; j<Size; ++j)
		{
			s >> Value;
			_pGeometryIndex[i].PushTail(Value);
		}

	
		s >> Size;
		for (j=0; j<Size; ++j)
		{
			s >> Value;
			_pVertexIndex[i].PushTail(Value);
		}
	}
	
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-03-14:	JLT - Created
//-----------------------------------------------------------------------------
bool PARTICLE_EXCHANGER::Write(IO_STREAM &s) const
{
	QDT_DLIST < int >::ITERATOR it;
	unsigned int i;
	int Value;

	s << _NbrPoints;
	
	for (i=0; i<_NbrPoints; ++i)
	{
		_pPoints[i].Write(s);

		Value = _pGeometryIndex[i].GetSize();
		s << Value;
		for (it=_pGeometryIndex[i].GetHead(); it!=_pGeometryIndex[i].GetTail(); it++)
		{
			Value = (*it);
			s << Value;
		}

		Value = _pVertexIndex[i].GetSize();
		s << Value;
		for (it=_pVertexIndex[i].GetHead(); it!=_pVertexIndex[i].GetTail(); it++)
		{
			Value = (*it);
			s << Value;
		}
		
	}

	return true;
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint
//	Object:		
//	02-03-14:	JLT - Created
//-----------------------------------------------------------------------------
POINT3D* PARTICLE_EXCHANGER::GetPoint(unsigned int i)
{
	POINT3D *P = NULL;

	if (i<_NbrPoints)
		P = _pPoints + i;
	
	return P;
}

//-----------------------------------------------------------------------------
//	Name:		GetPoints
//	Object:		
//	02-03-15:	JLT - Created
//-----------------------------------------------------------------------------
POINT3D* PARTICLE_EXCHANGER::GetPoints()
{
	return _pPoints;
}

//-----------------------------------------------------------------------------
//	Name:		GetGeometryIndex
//	Object:		
//	02-03-15:	JLT - Created
//-----------------------------------------------------------------------------
QDT_DLIST< int >* PARTICLE_EXCHANGER::GetGeometryIndex()
{
	return _pGeometryIndex;
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexIndex
//	Object:		
//	02-03-15:	JLT - Created
//-----------------------------------------------------------------------------
QDT_DLIST< int >* PARTICLE_EXCHANGER::GetVertexIndex()
{
	return _pVertexIndex;
}
 
//-----------------------------------------------------------------------------
//	Name:		SetTargetCoeff
//	Object:		
//	02-03-26:	JLT - Created
//-----------------------------------------------------------------------------
void PARTICLE_EXCHANGER::SetTargetCoeff(const unsigned int i, const float V)
{
	if (!_pTargetCoeff || i>_NbrPoints)
		return;

	_pTargetCoeff[i] = V;
}

//-----------------------------------------------------------------------------
//	Name:		GetTargetCoeff
//	Object:		
//	02-03-26:	JLT - Created
//-----------------------------------------------------------------------------
float PARTICLE_EXCHANGER::GetTargetCoeff(const unsigned int i) const
{
	return ((i<_NbrPoints) ? _pTargetCoeff[i] : -1.f);
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
