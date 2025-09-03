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
//	CLASS:	IRRADIANCE_VOLUME
//
//	03-03-10:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/SphericalHarmonics/IrradianceVolume)

#include	INCL_KMATH(Math/Geometry/Vector2/Vector2)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Math)
#include	INCL_KCORE(Stream/iostream)
#include	INCL_KMATH(Math/SphericalHarmonics/SHRotateMatrix)
#include	INCL_KMATH(Math/SphericalHarmonics/SphericalHarmonicsField)

#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)



//-----------------------------------------------------------------------------
//	Name:		IRRADIANCE_VOLUME constructor
//	Object:		
//	06-03-07:	PDE - Created
//-----------------------------------------------------------------------------
IRRADIANCE_VOLUME::IRRADIANCE_VOLUME()
{
	_pNodes = NULL;
	_uiNodesCount = 0;

	_pShFields = NULL;
	_uiNumFields = 0;

	_fMinX = -1.0f;
	_fMaxX = 1.0f;
	_fMinY = -1.0f;
	_fMaxY = 1.0f;
	_fMinZ = -1.0f;
	_fMaxZ = 1.0f;
}

//-----------------------------------------------------------------------------
//	Name:		IRRADIANCE_VOLUME constructor
//	Object:		
//	06-03-10:	PDE - Created
//-----------------------------------------------------------------------------
IRRADIANCE_VOLUME::IRRADIANCE_VOLUME(IO_STREAM &	s)
{
	s >> *this;
}


//-----------------------------------------------------------------------------
//	Name:		~IRRADIANCE_VOLUME destructor
//	Object:		
//	06-03-07:	PDE - Created
//-----------------------------------------------------------------------------
IRRADIANCE_VOLUME::~IRRADIANCE_VOLUME()
{
	delete[] _pNodes;
	delete[] _pShFields;
}

#define _MIN_X_ 0
#define _MIN_Y_ 1
#define _MIN_Z_ 2
#define _MAX_X_ 3
#define _MAX_Y_ 4
#define _MAX_Z_ 5

//-----------------------------------------------------------------------------
//	Name:		CellLookup
//	Object:		
//	06-03-02:	PDE - Created
//-----------------------------------------------------------------------------
IRRADIANCE_VOLUME::NODE*	IRRADIANCE_VOLUME::CellLookup(Float32	x,
  														  Float32	y,
														  Float32	z,
														  Float32*  bound)
{
	const Float32 delta = 0.0001f;


	if (x < _fMinX)
		x = _fMinX + delta;
	else if (x > _fMaxX)
		x = _fMaxX - delta;

	if (y < _fMinY)
		y = _fMinY + delta;
	else if (y > _fMaxY)
		y = _fMaxY - delta;

	if (z < _fMinZ)
		z = _fMinY + delta;
	else if (z > _fMaxZ)
		z = _fMaxZ - delta;

	static Float32 staticBound[6];
	
	if (bound == NULL)
		bound =  staticBound;

	bound[_MIN_X_] = _fMinX;
	bound[_MIN_Y_] = _fMinY;
	bound[_MIN_Z_] = _fMinZ;
	bound[_MAX_X_] = _fMaxX;
	bound[_MAX_Y_] = _fMaxY;
	bound[_MAX_Z_] = _fMaxZ;

	return CellLookup_rec(_pNodes, x, y, z, bound);
}



//-----------------------------------------------------------------------------
//	Name:		CellLookup_rec
//	Object:		
//	06-03-02:	PDE - Created
//-----------------------------------------------------------------------------
IRRADIANCE_VOLUME::NODE* 	IRRADIANCE_VOLUME::CellLookup_rec(IRRADIANCE_VOLUME::NODE* rootnode,
																Float32	x,
																Float32	y,
																Float32	z,
																Float32*  bound)
{
		Float32 halfX = (bound[_MIN_X_] + bound[_MAX_X_]) * 0.5f;
		Float32 halfY = (bound[_MIN_Y_] + bound[_MAX_Y_]) * 0.5f;
		Float32 halfZ = (bound[_MIN_Z_] + bound[_MAX_Z_]) * 0.5f;


		if (!rootnode->Children[0]) return rootnode;
		

		if (z > halfZ)
		{
			bound[_MIN_Z_] = halfZ;
			if (x > halfX)
			{
				bound[_MIN_X_] = halfX;

				if (y > halfY)
				{
					bound[_MIN_Y_] = halfY;
					return CellLookup_rec(&_pNodes[rootnode->Children[1]], x, y, z, bound);
				}
				else
				{
					bound[_MAX_Y_] = halfY;
					return CellLookup_rec(&_pNodes[rootnode->Children[2]], x, y, z, bound);
				}
			}
			else
			{
				bound[_MAX_X_] = halfX;

				if (y > halfY)
				{
					bound[_MIN_Y_] = halfY;
					return CellLookup_rec(&_pNodes[rootnode->Children[0]], x, y, z, bound);
				}
				else
				{
					bound[_MAX_Y_] = halfY;
					return CellLookup_rec(&_pNodes[rootnode->Children[3]], x, y, z, bound);
				}
			}
		}
		else
		{
			bound[_MAX_Z_] = halfZ;

			if (x > halfX)
			{
				bound[_MIN_X_] = halfX;
			
				if (y > halfY)
				{
					bound[_MIN_Y_] = halfY;
					return CellLookup_rec(&_pNodes[rootnode->Children[5]], x, y, z, bound);
				}
				else
				{
					bound[_MAX_Y_] = halfY;
					return CellLookup_rec(&_pNodes[rootnode->Children[6]], x, y, z, bound);
				}
			}
			else
			{
				bound[_MAX_X_] = halfX;

				if (y > halfY)
				{
					bound[_MIN_Y_] = halfY;
					return CellLookup_rec(&_pNodes[rootnode->Children[4]], x, y, z, bound);
				}
				else
				{
					bound[_MAX_Y_] = halfY;
					return CellLookup_rec(&_pNodes[rootnode->Children[7]], x, y, z, bound);
				}
			}
		}

		return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		SamplePositiveX
//	Object:		
//	06-03-09:	PDE - Created
//-----------------------------------------------------------------------------
void	IRRADIANCE_VOLUME::SamplePositiveX(SPHERICAL_HARMONICS *	sh,
										   NODE *					root,
										   Float32 *				bound,
										   Float32					x,
										   Float32					y,
										   Float32					z)
{

	if (root->positiveXAdjacent != -1)
	{
		Float32 w = bound[_MAX_X_] - bound[_MIN_X_];
		bound[_MAX_X_] += w;
		bound[_MIN_X_] += w;
		SampleNegativeX(sh, &_pNodes[root->positiveXAdjacent], bound, x, y, z);
	}
	else if (root->Children[0])
	{
		Float32 halfY = (bound[_MIN_Y_] + bound[_MAX_Y_]) * 0.5f;
		Float32 halfZ = (bound[_MIN_Z_] + bound[_MAX_Z_]) * 0.5f;

		if (y > halfY)
		{
			bound[_MIN_Y_] = halfY;

			if (z > halfZ)
			{
				bound[_MIN_Z_] = halfZ;
				SamplePositiveX(sh, &_pNodes[root->Children[1]], bound, x, y, z);
			}
			else
			{
				bound[_MAX_Z_] = halfZ;
				SamplePositiveX(sh, &_pNodes[root->Children[5]], bound, x, y, z);
			}
		}
		else
		{
			bound[_MAX_Y_] = halfY;

			if (z > halfZ)
			{
				bound[_MIN_Z_] = halfZ;
				SamplePositiveX(sh, &_pNodes[root->Children[2]], bound, x, y, z);
			}
			else
			{
				bound[_MAX_Z_] = halfZ;
				SamplePositiveX(sh, &_pNodes[root->Children[6]], bound, x, y, z);
			}
		}
	}
	else
	{
		SPHERICAL_HARMONICS p1, p2, p5, p6;

		_pShFields[root->ShId[1]].TaylorExpansion(p1, VECTOR(0.5f * (x - bound[_MAX_X_]), 0.5f * (y - bound[_MAX_Y_]), 0.5f * (z - bound[_MAX_Z_])));
		_pShFields[root->ShId[2]].TaylorExpansion(p2, VECTOR(0.5f * (x - bound[_MAX_X_]), 0.5f * (y - bound[_MIN_Y_]), 0.5f * (z - bound[_MAX_Z_])));
		_pShFields[root->ShId[5]].TaylorExpansion(p5, VECTOR(0.5f * (x - bound[_MAX_X_]), 0.5f * (y - bound[_MAX_Y_]), 0.5f * (z - bound[_MIN_Z_])));
		_pShFields[root->ShId[6]].TaylorExpansion(p6, VECTOR(0.5f * (x - bound[_MAX_X_]), 0.5f * (y - bound[_MIN_Y_]), 0.5f * (z - bound[_MIN_Z_])));

		SPHERICAL_HARMONICS up, down;
		Float32 ratioY = (y - bound[_MIN_Y_]) / (bound[_MAX_Y_] - bound[_MIN_Y_]);
		Float32 ratioZ = (z - bound[_MIN_Z_]) / (bound[_MAX_Z_] - bound[_MIN_Z_]);

		
		up.Lerp(p2, p1,   ratioY);
		down.Lerp(p6, p5, ratioY);
		sh->Lerp(down, up, ratioZ);
	}
}


//-----------------------------------------------------------------------------
//	Name:		SampleNegativeX
//	Object:		
//	06-03-09:	PDE - Created
//-----------------------------------------------------------------------------
void	IRRADIANCE_VOLUME::SampleNegativeX(SPHERICAL_HARMONICS *	sh,
										   NODE *					root,
										   Float32 *				bound,
										   Float32					x,
										   Float32					y,
										   Float32					z)
{

	if (root->negativeXAdjacent != -1)
	{
		Float32 w = bound[_MAX_X_] - bound[_MIN_X_];
		bound[_MAX_X_] -= w;
		bound[_MIN_X_] -= w;
		SamplePositiveX(sh, &_pNodes[root->negativeXAdjacent], bound, x, y, z);
	}
	else if (root->Children[0])
	{
		Float32 halfY = (bound[_MIN_Y_] + bound[_MAX_Y_]) * 0.5f;
		Float32 halfZ = (bound[_MIN_Z_] + bound[_MAX_Z_]) * 0.5f;

		if (y > halfY)
		{
			bound[_MIN_Y_] = halfY;

			if (z > halfZ)
			{
				bound[_MIN_Z_] = halfZ;
				SampleNegativeX(sh, &_pNodes[root->Children[0]], bound, x, y, z);
			}
			else
			{
				bound[_MAX_Z_] = halfZ;
				SampleNegativeX(sh, &_pNodes[root->Children[4]], bound, x, y, z);
			}
		}
		else
		{
			bound[_MAX_Y_] = halfY;

			if (z > halfZ)
			{
				bound[_MIN_Z_] = halfZ;
				SampleNegativeX(sh, &_pNodes[root->Children[3]], bound, x, y, z);
			}
			else
			{
				bound[_MAX_Z_] = halfZ;
				SampleNegativeX(sh, &_pNodes[root->Children[7]], bound, x, y, z);
			}
		}
	}
	else
	{
		SPHERICAL_HARMONICS p0, p3, p4, p7;

		const Float32 factor = 1.0f;
		_pShFields[root->ShId[0]].TaylorExpansion(p0, VECTOR(factor * (x - bound[_MIN_X_]), factor * (y - bound[_MAX_Y_]), factor * (z - bound[_MAX_Z_])));
		_pShFields[root->ShId[3]].TaylorExpansion(p3, VECTOR(factor * (x - bound[_MIN_X_]), factor * (y - bound[_MIN_Y_]), factor * (z - bound[_MAX_Z_])));
		_pShFields[root->ShId[4]].TaylorExpansion(p4, VECTOR(factor * (x - bound[_MIN_X_]), factor * (y - bound[_MAX_Y_]), factor * (z - bound[_MIN_Z_])));
		_pShFields[root->ShId[7]].TaylorExpansion(p7, VECTOR(factor * (x - bound[_MIN_X_]), factor * (y - bound[_MIN_Y_]), factor * (z - bound[_MIN_Z_])));
													  
		SPHERICAL_HARMONICS up, down;
		Float32 ratioY = (y - bound[_MIN_Y_]) / (bound[_MAX_Y_] - bound[_MIN_Y_]);
		Float32 ratioZ = (z - bound[_MIN_Z_]) / (bound[_MAX_Z_] - bound[_MIN_Z_]);

		up.Lerp(p3, p0, ratioY);
		down.Lerp(p7, p4, ratioY);
		sh->Lerp(down, up, ratioZ);
	}
}



//-----------------------------------------------------------------------------
//	Name:		SampleIrradiance
//	Object:		
//	06-03-08:	PDE - Created
//-----------------------------------------------------------------------------
void	IRRADIANCE_VOLUME::SampleIrradiance(SPHERICAL_HARMONICS *	sh,
											Float32					x,
											Float32					y,
											Float32					z)
{
	if (!_pNodes)
	{
		sh->Init();
		sh->AddConstant(0, 3.0, 0);
		return;
	}

	Float32 bound[6];
	NODE* node = CellLookup(x, y, z, bound);

	Float32 ratioX = (x - bound[_MIN_X_]) / (bound[_MAX_X_] - bound[_MIN_X_]);

	Float32 boundCopy[6];
	QDT_MEMCOPY(boundCopy, bound, sizeof(bound));


	SPHERICAL_HARMONICS positiveX, negativeX;

	SamplePositiveX(&positiveX, node, bound, x, y, z);
	SampleNegativeX(&negativeX, node, boundCopy, x, y, z);

	sh->Lerp(negativeX, positiveX, ratioX);
}


//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	06-03-10:	PDE - Created
//-----------------------------------------------------------------------------
void	IRRADIANCE_VOLUME::Read(IO_STREAM &	Stream)
{
	Stream >> _fMinX;
	Stream >> _fMaxX;
	Stream >> _fMinY;
	Stream >> _fMaxY;
	Stream >> _fMinZ;
	Stream >> _fMaxZ;

	Stream >> _uiNodesCount;

	if (_uiNodesCount)
	{
		_pNodes = new NODE[_uiNodesCount];
		UInt32 numRead = Stream.GetBuffer(_pNodes, _uiNodesCount * sizeof(NODE));
		QDT_ASSERT(numRead == _uiNodesCount * sizeof(NODE));
	}
	
	Stream >> _uiNumFields;

	if (_uiNumFields)
	{
		_pShFields = new SPHERICAL_HARMONICS_FIELD[_uiNumFields];
		UInt32 numRead = Stream.GetBuffer(_pShFields, _uiNumFields * sizeof(SPHERICAL_HARMONICS_FIELD));
		QDT_ASSERT(numRead == _uiNumFields * sizeof(SPHERICAL_HARMONICS_FIELD));
	}
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	06-03-10:	PDE - Created
//-----------------------------------------------------------------------------
void	IRRADIANCE_VOLUME::Write(IO_STREAM &	Stream) const
{
	Stream << _fMinX;
	Stream << _fMaxX;
	Stream << _fMinY;
	Stream << _fMaxY;
	Stream << _fMinZ;
	Stream << _fMaxZ;
	Stream << _uiNodesCount;

	for (UInt32 i = 0; i < _uiNodesCount; i++)
	{
			Stream << _pNodes[i];
	}
	Stream << _uiNumFields;

	for (UInt32 j = 0; j < _uiNumFields; j++)
	{
			Stream << _pShFields[j];
	}
}





//=============================================================================
//	CODE ENDS HERE
//=============================================================================
