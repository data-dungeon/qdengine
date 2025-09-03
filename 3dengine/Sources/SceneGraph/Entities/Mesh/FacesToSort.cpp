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
//	CLASS:	FACES_TO_SORT
//
//	06-02-02:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Root.h"
#include	"FacesToSort.h"
#include	"RadixSort.h"
#include	INCL_KCORE(Stream/IOStream)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(Renderer/IIndexBuffer)
#include	INCL_3DENGINE(Display/DisplayManager)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FACES_TO_SORT constructor
//	Object:		
//	06-02-02:	RMA - Created
//-----------------------------------------------------------------------------
FACES_TO_SORT::FACES_TO_SORT()
:
_uiSize(0),
_bSkinning(false),
_pDistance(NULL),
_prX(NULL),
_prY(NULL),
_prZ(NULL),
_puiBI0(NULL),
_puiBI1(NULL),
_puiBI2(NULL),
_puiBI3(NULL),
_puiBW0(NULL),
_puiBW1(NULL),
_puiBW2(NULL),
_puiBW3(NULL),
_puiTriangleIndices(NULL),
_uiCurrentIndexBuffer(0)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		FACES_TO_SORT destructor
//	Object:		
//	06-02-02:	RMA - Created
//-----------------------------------------------------------------------------
FACES_TO_SORT::~FACES_TO_SORT()
{
	delete [] _pDistance;
	delete [] _prX;
	delete [] _prY;
	delete [] _prZ;
	delete [] _puiTriangleIndices;

	if (_bSkinning)
	{
		delete [] _puiBI0;
		delete [] _puiBI1;
		delete [] _puiBI2;
		delete [] _puiBI3;
		delete [] _puiBW0;
		delete [] _puiBW1;
		delete [] _puiBW2;
		delete [] _puiBW3;
	}	

	for (UInt32 i = 0 ; i < MAX_INDEX_BUFFER_COUNT ; ++i)
	{
		 RENDER_CONTEXT::ReleaseIndexBuffer(_apIndexBuffer[i]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Allocate
//	Object:		
//	06-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FACES_TO_SORT::Allocate(UInt32	uiSize,
								bool	bSkinning,
								bool	bCreateIndexBuffer)
{
	_uiSize = uiSize;
	_bSkinning = bSkinning;
	
	_pDistance = new Float32[uiSize];
	_prX = new Float32[uiSize];
	_prY = new Float32[uiSize];
	_prZ = new Float32[uiSize];

	_puiTriangleIndices = new UInt16[uiSize * 3];

	if (_bSkinning)
	{
		_puiBI0 = new UInt8[uiSize];
		_puiBI1 = new UInt8[uiSize];
		_puiBI2 = new UInt8[uiSize];
		_puiBI3 = new UInt8[uiSize];

		_puiBW0 = new Float32[uiSize];
		_puiBW1 = new Float32[uiSize];
		_puiBW2 = new Float32[uiSize];
		_puiBW3 = new Float32[uiSize];
	}

	if (bCreateIndexBuffer)
	{
		for (UInt32 i = 0 ; i < MAX_INDEX_BUFFER_COUNT ; ++i)
		{
			_apIndexBuffer[i] = RENDER_CONTEXT::CreateIndexBuffer(uiSize * 3);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Allocate
//	Object:		This copy is used during the export process, so it doesn't
//				copy the distance and create the index buffers
//	06-03-16:	RMA - Created
//-----------------------------------------------------------------------------
void	FACES_TO_SORT::Copy(const FACES_TO_SORT & Faces)
{
	_uiSize = Faces._uiSize;
	_bSkinning = Faces._bSkinning;

	Allocate(_uiSize, _bSkinning, false);

	memcpy(_prX, Faces._prX, _uiSize * sizeof(Float32));
	memcpy(_prY, Faces._prY, _uiSize * sizeof(Float32));
	memcpy(_prZ, Faces._prZ, _uiSize * sizeof(Float32));
	memcpy(_puiTriangleIndices, Faces._puiTriangleIndices, _uiSize * sizeof(UInt16) * 3);
	
	if (_bSkinning)
	{
		memcpy(_puiBI0, Faces._puiBI0, _uiSize);
		memcpy(_puiBI1, Faces._puiBI1, _uiSize);
		memcpy(_puiBI2, Faces._puiBI2, _uiSize);
		memcpy(_puiBI3, Faces._puiBI3, _uiSize);

		memcpy(_puiBW0, Faces._puiBW0, _uiSize * sizeof(Float32));
		memcpy(_puiBW1, Faces._puiBW1, _uiSize * sizeof(Float32));
		memcpy(_puiBW2, Faces._puiBW2, _uiSize * sizeof(Float32));
		memcpy(_puiBW3, Faces._puiBW3, _uiSize * sizeof(Float32));
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComputeDistance
//	Object:		This method support skinning and could be a bottleneck for our
//				application. It's highly SIMDable. It's a perfect case study
//				of the subject.
//				I use SAO instead of AOF. This can seems strange but it's
//				far more optimale for cache misses.
//				Other optimisation possible : sort the points only if the 
//				camera plane has changed enough since last frame.
//	06-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FACES_TO_SORT::ComputeDistance(const PLANE & vEyePlane)
{
	UInt32 i;
	Float32 f1, f2, f3, f4;
	
	DISPLAY_MANAGER * pDM = DISPLAY_MANAGER::Instance();
	
	vEyePlane.GetCoeff(f1, f2, f3, f4);

	if (pDM->IsSkinningEnabled())
	{
		QDT_ASSERT(_puiBI0 && _puiBI1 && _puiBI2 && _puiBI3);
		QDT_ASSERT(_puiBW0 && _puiBW1 && _puiBW2 && _puiBW3);

		for (i = 0 ; i < _uiSize ; ++i)
		{
			const MATRIX &M0 = pDM->GetSkinningMatrix(_puiBI0[i]);
			const MATRIX &M1 = pDM->GetSkinningMatrix(_puiBI1[i]);
			const MATRIX &M2 = pDM->GetSkinningMatrix(_puiBI2[i]);
			const MATRIX &M3 = pDM->GetSkinningMatrix(_puiBI3[i]);

			Float32 blendWeight0 = _puiBW0[i];
			Float32 blendWeight1 = _puiBW1[i];
			Float32 blendWeight2 = _puiBW2[i];
			Float32 blendWeight3 = _puiBW3[i];

			Float32 x = _prX[i];
			Float32 y = _prY[i];
			Float32 z = _prZ[i];

			Float32 skinnedX, skinnedY, skinnedZ;

			skinnedX = (x * M0(0, 0) + y * M0(1, 0) + z * M0(2, 0) + M0(3, 0)) * blendWeight0;
			skinnedY = (x * M0(0, 1) + y * M0(1, 1) + z * M0(2, 1) + M0(3, 1)) * blendWeight0;
			skinnedZ = (x * M0(0, 2) + y * M0(1, 2) + z * M0(2, 2) + M0(3, 2)) * blendWeight0;

			skinnedX += (x * M1(0, 0) + y * M1(1, 0) + z * M1(2, 0) + M1(3, 0)) * blendWeight1;
			skinnedY += (x * M1(0, 1) + y * M1(1, 1) + z * M1(2, 1) + M1(3, 1)) * blendWeight1;
			skinnedZ += (x * M1(0, 2) + y * M1(1, 2) + z * M1(2, 2) + M1(3, 2)) * blendWeight1;
																							 
			skinnedX += (x * M2(0, 0) + y * M2(1, 0) + z * M2(2, 0) + M2(3, 0)) * blendWeight2;
			skinnedY += (x * M2(0, 1) + y * M2(1, 1) + z * M2(2, 1) + M2(3, 1)) * blendWeight2;
			skinnedZ += (x * M2(0, 2) + y * M2(1, 2) + z * M2(2, 2) + M2(3, 2)) * blendWeight2;
																							 
			skinnedX += (x * M3(0, 0) + y * M3(1, 0) + z * M3(2, 0) + M3(3, 0)) * blendWeight3;
			skinnedY += (x * M3(0, 1) + y * M3(1, 1) + z * M3(2, 1) + M3(3, 1)) * blendWeight3;
			skinnedZ += (x * M3(0, 2) + y * M3(1, 2) + z * M3(2, 2) + M3(3, 2)) * blendWeight3;
		
			_pDistance[i] = skinnedX * f1 + skinnedY * f2 + skinnedZ  * f3;
		}
	}
	else
	{	
		for (i = 0 ; i < _uiSize ; ++i)
		{
			_pDistance[i] = _prX[i] * f1 + _prY[i] * f2 + _prZ[i] * f3;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Sort
//	Object:		I'm using radix sort to sort transparent faces.
//				The speed result are awesome. I suppose this can be optimized
//				a little with cache prediction.
//				I could also change the sort algorithm according to the number
//				of entries. (radix sort is not very interesting for a few
//				floats, and other forms of radix sort performs better if the
//				number of entries is > 2000)
//	06-02-02:	RMA - Created
//-----------------------------------------------------------------------------
I_INDEX_BUFFER *	FACES_TO_SORT::Sort(const PLANE & vEyePlane)
{
	ComputeDistance(vEyePlane);
	
	RADIX_SORT RS;
	RS.Sort(_pDistance, _uiSize);
	const UInt32 * pSortedIndices = RS.GetIndices();

	++_uiCurrentIndexBuffer;
	if (_uiCurrentIndexBuffer == MAX_INDEX_BUFFER_COUNT)
	{
		_uiCurrentIndexBuffer = 0;
	}

	UInt16 * pBuffer = (UInt16 *)_apIndexBuffer[_uiCurrentIndexBuffer]->Lock();
	
	UInt32 i;
	for (i = 0 ; i < _uiSize ; ++i)
	{
		UInt32 uiIndex = pSortedIndices[_uiSize - i - 1];
		*pBuffer++ = _puiTriangleIndices[uiIndex * 3];
		*pBuffer++ = _puiTriangleIndices[uiIndex * 3 + 1];
		*pBuffer++ = _puiTriangleIndices[uiIndex * 3 + 2];
	}

	_apIndexBuffer[_uiCurrentIndexBuffer]->Unlock();

	return (_apIndexBuffer[_uiCurrentIndexBuffer]);
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrTriangles
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	FACES_TO_SORT::SetNbrTriangles(UInt32	uiNbrTriangles)
{
	_uiSize = uiNbrTriangles;
}

//-----------------------------------------------------------------------------
//	Name:		SetPosition
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	FACES_TO_SORT::SetPositions(Float32 *	prX,
								    Float32 *	prY,
								    Float32 *	prZ)
{
	_prX = prX;
	_prY = prY;
	_prZ = prZ;
}

//-----------------------------------------------------------------------------
//	Name:		SetIndices
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	FACES_TO_SORT::SetIndices(UInt16 *	pi)
{
	_puiTriangleIndices = pi;
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	FACES_TO_SORT::Read(IO_STREAM &	Stream)
{
	Stream >> _uiSize;
	Stream >> _bSkinning;

	Allocate(_uiSize, _bSkinning, true);

	Stream.GetBufferFast((void*)_prX, _uiSize * 4);
	Stream.GetBufferFast((void*)_prY, _uiSize * 4);
	Stream.GetBufferFast((void*)_prZ, _uiSize * 4);

	Stream.GetBufferFast((void*)_puiTriangleIndices, _uiSize * 2 * 3);

	if (_bSkinning)
	{
		Stream.GetBufferFast((void*)_puiBI0, _uiSize);
		Stream.GetBufferFast((void*)_puiBI1, _uiSize);
		Stream.GetBufferFast((void*)_puiBI2, _uiSize);
		Stream.GetBufferFast((void*)_puiBI3, _uiSize);

		Stream.GetBufferFast((void*)_puiBW0, _uiSize * 4);
		Stream.GetBufferFast((void*)_puiBW1, _uiSize * 4);
		Stream.GetBufferFast((void*)_puiBW2, _uiSize * 4);
		Stream.GetBufferFast((void*)_puiBW3, _uiSize * 4);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	FACES_TO_SORT::Write(IO_STREAM &	Stream) const
{
	Stream << _uiSize;
	Stream << _bSkinning;

	Stream.PutBufferFast((void*)_prX, _uiSize * 4);
	Stream.PutBufferFast((void*)_prY, _uiSize * 4);
	Stream.PutBufferFast((void*)_prZ, _uiSize * 4);

	Stream.PutBufferFast((void*)_puiTriangleIndices, _uiSize * 2 * 3);

	if (_bSkinning)
	{
		Stream.PutBufferFast((void*)_puiBI0, _uiSize);
		Stream.PutBufferFast((void*)_puiBI1, _uiSize);
		Stream.PutBufferFast((void*)_puiBI2, _uiSize);
		Stream.PutBufferFast((void*)_puiBI3, _uiSize);

		Stream.PutBufferFast((void*)_puiBW0, _uiSize * 4);
		Stream.PutBufferFast((void*)_puiBW1, _uiSize * 4);
		Stream.PutBufferFast((void*)_puiBW2, _uiSize * 4);
		Stream.PutBufferFast((void*)_puiBW3, _uiSize * 4);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetWeightsIndices
//	Object:		
//	06-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	FACES_TO_SORT::SetWeightsIndices(UInt8 *	p0,
										 UInt8 *	p1,
										 UInt8 *	p2,
										 UInt8 *	p3)
{
	_puiBI0 = p0;
	_puiBI1 = p1;
	_puiBI2 = p2;
	_puiBI3 = p3;
}

//-----------------------------------------------------------------------------
//	Name:		SetWeights
//	Object:		
//	06-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	FACES_TO_SORT::SetWeights(Float32 *	p0,
								  Float32 *	p1,
								  Float32 *	p2,
								  Float32 *	p3)
{
	_puiBW0 = p0;
	_puiBW1 = p1;
	_puiBW2 = p2;
	_puiBW3 = p3;
}

//-----------------------------------------------------------------------------
//	Name:		SetSkinning
//	Object:		
//	06-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	FACES_TO_SORT::SetSkinning(Bool	b)
{
	_bSkinning = b;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
