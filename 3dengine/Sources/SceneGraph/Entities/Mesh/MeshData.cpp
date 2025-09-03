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
//	CLASS:	MESH_DATA
//
//	05-11-03:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	"RadixSort.h"
#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include	INCL_KRENDERER(Renderer/SVertexElement)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(Renderer/IVertexBuffer)
#include	INCL_KRENDERER(Renderer/IIndexBuffer)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/MeshData)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/FacesToSort)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Primitive)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Mesh)

#ifdef _DEBUG
	#include	"MeshData.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MESH_DATA constructor
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
MESH_DATA::MESH_DATA()
:
COMMUNICATOR			(GetGenericCommunicatorDescriptor()),
_uiNbrVertexBuffers		(0),
_puiNbrVertices			(NULL),
_puiVertexSizes			(NULL),
_pVertices				(NULL),
_puiNbrVertexElement	(NULL),
_pVertexElement			(NULL),
_uiNbrIndexBuffers		(0),
_puiNbrIndices			(NULL),
_pIndices				(NULL),
_uiNbrPrimitives		(0),
_uiNbrFacesToSort		(0),
_pPrimitives			(NULL),
_pLocalBoundingBoxes	(NULL),
_pLocalBoundingSpheres	(NULL),
_pVertexDecl			(NULL),
_pVertexBuffer			(NULL),
_pIndexBuffer			(NULL),
_bInit					(false),
_pFacesToSort			(NULL), 
_iFlowDataNb			(0), 
_pFlowDataArray			(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		MESH_DATA destructor
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
MESH_DATA::~MESH_DATA()
{
	UInt32	i;

	Clear();

	// For vertex buffers
	if	(_uiNbrVertexBuffers)
	{
		for	(i = 0 ; i < _uiNbrVertexBuffers ; ++i)
		{
			delete [] (UInt8 *)(_pVertices[i]);
		}
		delete [] _puiNbrVertices;
		delete [] _puiVertexSizes;
		delete [] _pVertices;
		
		_puiNbrVertices = NULL;
		_puiVertexSizes = NULL;
		_pVertices = NULL;
		
		// For vertex declaration
		for	(i = 0 ; i < _uiNbrVertexBuffers ; ++i)
		{
			delete [] _pVertexElement[i];
		}
		delete [] _puiNbrVertexElement;
		delete [] _pVertexElement;
		
		_puiNbrVertexElement = NULL;
		_pVertexElement = NULL;
	}
	_uiNbrVertexBuffers = 0;

	// For index buffers
	if	(_uiNbrIndexBuffers)
	{
		for	(i = 0 ; i < _uiNbrIndexBuffers ; ++i)
		{
			delete [] _pIndices[i];
		}
		delete [] _puiNbrIndices;
		delete [] _pIndices;
		
		_puiNbrIndices = NULL;
		_pIndices = NULL;
	}
	_uiNbrIndexBuffers = 0;

	// For primitives
	if	(_uiNbrPrimitives)
	{
		delete [] _pPrimitives;
		delete [] _pLocalBoundingBoxes;
		delete [] _pLocalBoundingSpheres;

		_pPrimitives = NULL;
		_pLocalBoundingBoxes = NULL;
		_pLocalBoundingSpheres = NULL;
	}
	_uiNbrPrimitives = 0;
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
MESH_DATA &	MESH_DATA::operator=(const MESH_DATA & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::Copy(const MESH_DATA & C)
{
	UInt32	i, VertexSize;

	// For vertex buffers
	_uiNbrVertexBuffers = C._uiNbrVertexBuffers;
	if	(_uiNbrVertexBuffers)
	{
		// For vertex declaration
		_puiNbrVertexElement = new UInt32[_uiNbrVertexBuffers];
		memcpy(_puiNbrVertexElement, C._puiNbrVertexElement, _uiNbrVertexBuffers * sizeof(UInt32));

		_pVertexElement = new S_VERTEX_ELEMENT*[_uiNbrVertexBuffers];
		for	(i = 0 ; i < _uiNbrVertexBuffers ; ++i)
		{
			_pVertexElement[i] = new S_VERTEX_ELEMENT[_puiNbrVertexElement[i]];
			memcpy(_pVertexElement[i], C._pVertexElement[i], _puiNbrVertexElement[i] * sizeof(S_VERTEX_ELEMENT));
		}

		// For vertex buffers
		UInt8 **	pVertexBuffers;

		_puiNbrVertices = new UInt32[_uiNbrVertexBuffers];
		memcpy(_puiNbrVertices, C._puiNbrVertices, _uiNbrVertexBuffers * sizeof(UInt32));

		_puiVertexSizes = new UInt32[_uiNbrVertexBuffers];
		memcpy(_puiVertexSizes, C._puiVertexSizes, _uiNbrVertexBuffers * sizeof(UInt32));

		pVertexBuffers = new UInt8*[_uiNbrVertexBuffers];
		for	(i = 0 ; i < _uiNbrVertexBuffers ; ++i)
		{
			VertexSize = GetVertexSize(i);

			pVertexBuffers[i] = new UInt8[VertexSize * _puiNbrVertices[i]];
			memcpy(pVertexBuffers[i], C._pVertices[i], _puiNbrVertices[i] * VertexSize);
		}

		_pVertices = (void **)(pVertexBuffers);
	}

	// For index buffers
	_uiNbrIndexBuffers = C._uiNbrIndexBuffers;
	if	(_uiNbrIndexBuffers)
	{
		_puiNbrIndices = new UInt32[_uiNbrIndexBuffers];
		memcpy(_puiNbrIndices, C._puiNbrIndices, _uiNbrIndexBuffers * sizeof(UInt32));

		_pIndices = new UInt16*[_uiNbrIndexBuffers];
		for	(i = 0 ; i < _uiNbrIndexBuffers ; ++i)
		{
			_pIndices[i] = new UInt16[_puiNbrIndices[i]];
			memcpy(_pIndices[i], C._pIndices[i], _puiNbrIndices[i] * sizeof(UInt16));
		}
	}

	// For primitive
	_uiNbrPrimitives = C._uiNbrPrimitives;
	if	(_uiNbrPrimitives)
	{
		_pPrimitives = new PRIMITIVE[_uiNbrPrimitives];
		memcpy(_pPrimitives, C._pPrimitives, _uiNbrPrimitives * sizeof(PRIMITIVE));

		_pLocalBoundingBoxes = new OBB[_uiNbrPrimitives];
		memcpy(_pLocalBoundingBoxes, C._pLocalBoundingBoxes, _uiNbrPrimitives * sizeof(OBB));

		_pLocalBoundingSpheres = new SPHERE[_uiNbrPrimitives];
		memcpy(_pLocalBoundingSpheres, C._pLocalBoundingSpheres, _uiNbrPrimitives * sizeof(SPHERE));
	
		_pfDisplayFlags = new QDT_FLAGS[_uiNbrPrimitives];
		memcpy(_pfDisplayFlags, C._pfDisplayFlags, _uiNbrPrimitives * sizeof(QDT_FLAGS));
	}
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	06-01-25:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::Init()
{
	UInt32	i;

	if	(_bInit == false)
	{
		if	(_uiNbrVertexBuffers > 0)
		{
			_pVertexDecl = new I_VERTEX_DECLARATION*[_uiNbrVertexBuffers];
			_pVertexBuffer = new I_VERTEX_BUFFER*[_uiNbrVertexBuffers];

			for	(i = 0 ; i < _uiNbrVertexBuffers ; ++i)
			{
				_pVertexDecl[i] = RENDER_CONTEXT::CreateVertexDeclaration(_pVertexElement[i], _puiNbrVertexElement[i]);
				_pVertexBuffer[i] = RENDER_CONTEXT::CreateVertexBuffer(_puiNbrVertices[i], GetVertexSize(i));

				void *	pVertices = _pVertexBuffer[i]->Lock();
				memcpy(pVertices, _pVertices[i], _puiNbrVertices[i] * GetVertexSize(i));
				_pVertexBuffer[i]->Unlock();
			}
		}

		if	(_uiNbrIndexBuffers > 0)
		{
			_pIndexBuffer = new I_INDEX_BUFFER*[_uiNbrIndexBuffers];
			for	(i = 0 ; i < _uiNbrIndexBuffers ; ++i)
			{
				_pIndexBuffer[i] = RENDER_CONTEXT::CreateIndexBuffer(_puiNbrIndices[i]);

				UInt16 * pIndices = (UInt16 *)_pIndexBuffer[i]->Lock();
				memcpy(pIndices, _pIndices[i], _puiNbrIndices[i] * sizeof(UInt16));
				_pIndexBuffer[i]->Unlock();
			}
		}

		_bInit = true;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::Clear()
{
	UInt32	i;

	// For vertex buffers
	if	(_pVertexBuffer)
	{
		for	(i = 0 ; i < _uiNbrVertexBuffers ; ++i)
		{
			RENDER_CONTEXT::ReleaseVertexBuffer(_pVertexBuffer[i]);
		}
		delete [] _pVertexBuffer;
		_pVertexBuffer = NULL;
	}

	// For vertex declaration
	if	(_pVertexDecl)
	{
		for	(i = 0 ; i < _uiNbrVertexBuffers ; ++i)
		{
			RENDER_CONTEXT::ReleaseVertexDeclaration(_pVertexDecl[i]);
		}
		delete [] _pVertexDecl;
		_pVertexDecl = NULL;
	}

	// For index buffers
	if	(_pIndexBuffer)
	{
		for	(i = 0 ; i < _uiNbrIndexBuffers ; ++i)
		{
			RENDER_CONTEXT::ReleaseIndexBuffer(_pIndexBuffer[i]);
		}
		delete [] _pIndexBuffer;
		_pIndexBuffer = NULL;
	}

	_bInit = false;
}

//-----------------------------------------------------------------------------
//	Name:		ComputeVertexSize
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	MESH_DATA::ComputeVertexSize(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrVertexBuffers);

	UInt32	i, uiSize = 0;

	for	(i = 0 ; i < _puiNbrVertexElement[nIndex] ; ++i)
	{
		uiSize += __DECL_TYPE_SIZE[_pVertexElement[nIndex][i]._nType];
	}

	return	(uiSize);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MESHDATA", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MESHDATA", 2, CM_ReadChunk2);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MESHDATA", 3, CM_ReadChunk3);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MESHDATA", 4, CM_ReadChunk4);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MESHDATA", 5, CM_ReadChunk5);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MESHDATA", 6, CM_ReadChunk6);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("MESHDATA", 6, CM_WriteChunk6);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	MESH_DATA::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH_DATA) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	MESH_DATA::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("MESH_DATA"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH_DATA,
											QDT_COMMENT("1990A1B8-70DD-3C9A-DDDE-DC9F68B89B58"),
											sizeof(MESH_DATA),
											ComInit,
											QDT_COMMENT("Class for static mesh data exported from maya"),
											COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::EM_Construct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	MESH_DATA	*pMeshData;
	pMeshData = static_cast<MESH_DATA *>(pO);

	QDT_NEW_PLACED(pMeshData, MESH_DATA)();
	QDT_ASSERT(pMeshData);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::EM_Destruct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	MESH_DATA	*pMeshData;
	pMeshData = static_cast<MESH_DATA *>(pO);

	QDT_DELETE_PLACED(pMeshData, MESH_DATA);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::CM_ReadChunk1(IO_STREAM &		Stream,
								 COMMUNICATOR *	pCommunicator)
{
	MESH_DATA	*pMeshData;
	pMeshData = static_cast<MESH_DATA *>(pCommunicator);

	COM_INSTANCE ci;
	UInt32	i, j, n, BufferSize;
	UInt8	*pBuffer;

	// For vertex buffers
	UInt8 **	pVertexBuffers;

	Stream >> pMeshData->_uiNbrVertexBuffers;
	pMeshData->_puiNbrVertices = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_puiVertexSizes = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pVertexBuffers = new UInt8*[pMeshData->_uiNbrVertexBuffers];

	// For vertex declaration
	pMeshData->_puiNbrVertexElement = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_pVertexElement = new S_VERTEX_ELEMENT*[pMeshData->_uiNbrVertexBuffers];

	for	(i = 0 ; i < pMeshData->_uiNbrVertexBuffers ; ++i)
	{
		// For vertex declaration
		Stream >> pMeshData->_puiNbrVertexElement[i];
		pMeshData->_pVertexElement[i] = new S_VERTEX_ELEMENT[pMeshData->_puiNbrVertexElement[i]];
		for	(j = 0 ; j < pMeshData->_puiNbrVertexElement[i] ; ++j)
		{
			Stream >> pMeshData->_pVertexElement[i][j]._nStream;
			Stream >> pMeshData->_pVertexElement[i][j]._nOffset;
			Stream >> pMeshData->_pVertexElement[i][j]._nType;
			Stream >> pMeshData->_pVertexElement[i][j]._nUsage;
		}

		// For vertex buffers
		Stream >> pMeshData->_puiNbrVertices[i];
		Stream >> pMeshData->_puiVertexSizes[i];

		BufferSize = pMeshData->_puiNbrVertices[i] * pMeshData->_puiVertexSizes[i];
		pVertexBuffers[i] = new UInt8[BufferSize];
		pBuffer = pVertexBuffers[i];
		while	(BufferSize > 0)
		{
			Stream >> *pBuffer;
			pBuffer++;
			BufferSize--;
		}
	}
	pMeshData->_pVertices = (void **)(pVertexBuffers);

	// For index buffers
	Stream >> pMeshData->_uiNbrIndexBuffers;
	pMeshData->_puiNbrIndices = new UInt32[pMeshData->_uiNbrIndexBuffers];
	pMeshData->_pIndices = new UInt16*[pMeshData->_uiNbrIndexBuffers];
	for	(i = 0 ; i < pMeshData->_uiNbrIndexBuffers ; ++i)
	{
		Stream >> pMeshData->_puiNbrIndices[i];
		pMeshData->_pIndices[i] = new UInt16[pMeshData->_puiNbrIndices[i]];
		for	(j = 0 ; j < pMeshData->_puiNbrIndices[i] ; ++j)
		{
			Stream >> pMeshData->_pIndices[i][j];
		}
	}

	// For primitives
	Stream >> pMeshData->_uiNbrPrimitives;
	pMeshData->_pPrimitives = new PRIMITIVE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingBoxes = new OBB[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingSpheres = new SPHERE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pfDisplayFlags = new QDT_FLAGS[pMeshData->_uiNbrPrimitives];
	for	(i = 0 ; i < pMeshData->_uiNbrPrimitives ; ++i)
	{
		Stream >> n;
		pMeshData->_pPrimitives[i].SetType((PRIMITIVE_TYPE)n);
		Stream >> n;
		// Cull mode (obsolete -> double sided is managed inside materials)
		Stream >> n;
		pMeshData->_pPrimitives[i].SetVertexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstVertex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrVertices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetIndexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstIndex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrIndices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrJoints(n);

		pMeshData->_pfDisplayFlags[i].Set(MESH::DISPLAY_FLAG_VISIBLE, true);
		pMeshData->_pfDisplayFlags[i].Set(MESH::DISPLAY_FLAG_CAST_SHADOWS, true);
		pMeshData->_pfDisplayFlags[i].Set(MESH::DISPLAY_FLAG_RECEIVE_SHADOWS, true);
		
		UInt32 *	pJointIndices = NULL;
		if	(n > 0)
		{
			pJointIndices = new UInt32[n];
			for	(j = 0 ; j < n ; ++j)
			{
				Stream >> pJointIndices[j];
			}
		}
		pMeshData->_pPrimitives[i].SetJointIndices(pJointIndices);
		
		Stream >> ci;

#if defined(PLATFORM_PC)
		if (RENDER_CONTEXT::SupportShaderModel(SM_D3D_VS_1_1) == false || RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_2_0) == false)
		{
			pMeshData->_pPrimitives[i].SetShader(DISPLAY_MANAGER::Instance()->GetDefaultShader());
		}
		else
#endif
		{
			pMeshData->_pPrimitives[i].SetShader(ci);
		}

		pMeshData->_pLocalBoundingBoxes[i].Read(Stream);
		pMeshData->_pLocalBoundingSpheres[i].Read(Stream);
	}

	// For collision datas
	UInt32		nNbrCollisionVertices;
	UInt32		nNbrCollisionIndices;
	Float32 *	pCollisionVertices;
	UInt32 *	pCollisionIndices;

	Stream >> nNbrCollisionVertices;
	if	(nNbrCollisionVertices)
	{
		pCollisionVertices = new Float32[nNbrCollisionVertices * 3 * sizeof(Float32)];
		for	(i = 0 ; i < nNbrCollisionVertices ; ++i)
		{
			Stream >> pCollisionVertices[3 * i];
			Stream >> pCollisionVertices[(3 * i) + 1];
			Stream >> pCollisionVertices[(3 * i) + 2];
		}
	}

	Stream >> nNbrCollisionIndices;
	if	(nNbrCollisionIndices)
	{
		pCollisionIndices = new UInt32[nNbrCollisionIndices * sizeof(UInt32)];
		for	(i = 0 ; i < nNbrCollisionIndices ; ++i)
		{
			Stream >> pCollisionIndices[i];
		}
	}

	// Load data in video memory
	pMeshData->Init();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk2
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::CM_ReadChunk2(IO_STREAM &		Stream,
								 COMMUNICATOR *	pCommunicator)
{
	MESH_DATA	*pMeshData;
	pMeshData = static_cast<MESH_DATA *>(pCommunicator);

	COM_INSTANCE ci;
	UInt32	i, j, n, BufferSize;
	UInt8	*pBuffer;
	Bool	b;

	// For vertex buffers
	UInt8 **	pVertexBuffers;

	Stream >> pMeshData->_uiNbrVertexBuffers;
	pMeshData->_puiNbrVertices = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_puiVertexSizes = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pVertexBuffers = new UInt8*[pMeshData->_uiNbrVertexBuffers];

	// For vertex declaration
	pMeshData->_puiNbrVertexElement = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_pVertexElement = new S_VERTEX_ELEMENT*[pMeshData->_uiNbrVertexBuffers];

	for	(i = 0 ; i < pMeshData->_uiNbrVertexBuffers ; ++i)
	{
		// For vertex declaration
		Stream >> pMeshData->_puiNbrVertexElement[i];
		pMeshData->_pVertexElement[i] = new S_VERTEX_ELEMENT[pMeshData->_puiNbrVertexElement[i]];
		for	(j = 0 ; j < pMeshData->_puiNbrVertexElement[i] ; ++j)
		{
			Stream >> pMeshData->_pVertexElement[i][j]._nStream;
			Stream >> pMeshData->_pVertexElement[i][j]._nOffset;
			Stream >> pMeshData->_pVertexElement[i][j]._nType;
			Stream >> pMeshData->_pVertexElement[i][j]._nUsage;
		}

		// For vertex buffers
		Stream >> pMeshData->_puiNbrVertices[i];
		Stream >> pMeshData->_puiVertexSizes[i];

		BufferSize = pMeshData->_puiNbrVertices[i] * pMeshData->_puiVertexSizes[i];
		pVertexBuffers[i] = new UInt8[BufferSize];
		pBuffer = pVertexBuffers[i];
		while	(BufferSize > 0)
		{
			Stream >> *pBuffer;
			pBuffer++;
			BufferSize--;
		}
	}
	pMeshData->_pVertices = (void **)(pVertexBuffers);

	// For index buffers
	Stream >> pMeshData->_uiNbrIndexBuffers;
	pMeshData->_puiNbrIndices = new UInt32[pMeshData->_uiNbrIndexBuffers];
	pMeshData->_pIndices = new UInt16*[pMeshData->_uiNbrIndexBuffers];
	for	(i = 0 ; i < pMeshData->_uiNbrIndexBuffers ; ++i)
	{
		Stream >> pMeshData->_puiNbrIndices[i];
		pMeshData->_pIndices[i] = new UInt16[pMeshData->_puiNbrIndices[i]];
		for	(j = 0 ; j < pMeshData->_puiNbrIndices[i] ; ++j)
		{
			Stream >> pMeshData->_pIndices[i][j];
		}
	}

	// For primitives
	Stream >> pMeshData->_uiNbrPrimitives;
	pMeshData->_pPrimitives = new PRIMITIVE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingBoxes = new OBB[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingSpheres = new SPHERE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pfDisplayFlags = new QDT_FLAGS[pMeshData->_uiNbrPrimitives];
	for	(i = 0 ; i < pMeshData->_uiNbrPrimitives ; ++i)
	{
		Stream >> n;
		pMeshData->_pPrimitives[i].SetType((PRIMITIVE_TYPE)n);
		Stream >> n;
		// Cull mode (obsolete -> double sided is managed inside materials)
		Stream >> b;
		pMeshData->_pPrimitives[i].SetPerPolygoneSorting(b);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetVertexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstVertex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrVertices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetIndexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstIndex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrIndices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrJoints(n);

		pMeshData->_pfDisplayFlags[i].Set(MESH::DISPLAY_FLAG_VISIBLE, true);
		pMeshData->_pfDisplayFlags[i].Set(MESH::DISPLAY_FLAG_CAST_SHADOWS, true);
		pMeshData->_pfDisplayFlags[i].Set(MESH::DISPLAY_FLAG_RECEIVE_SHADOWS, true);
		
		UInt32 *	pJointIndices = NULL;
		if	(n > 0)
		{
			pJointIndices = new UInt32[n];
			for	(j = 0 ; j < n ; ++j)
			{
				Stream >> pJointIndices[j];
			}
		}
		pMeshData->_pPrimitives[i].SetJointIndices(pJointIndices);
		
		Stream >> ci;

#if defined(PLATFORM_PC)
		if (RENDER_CONTEXT::SupportShaderModel(SM_D3D_VS_1_1) == false || RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_2_0) == false)
		{
			pMeshData->_pPrimitives[i].SetShader(DISPLAY_MANAGER::Instance()->GetDefaultShader());
		}
		else
#endif
		{
			if (ci.HasHandle() == false)
			{
				pMeshData->_pPrimitives[i].SetShader(DISPLAY_MANAGER::Instance()->GetErrorShader());
			}
			else
			{
				pMeshData->_pPrimitives[i].SetShader(ci);
			}
		}

		pMeshData->_pLocalBoundingBoxes[i].Read(Stream);
		pMeshData->_pLocalBoundingSpheres[i].Read(Stream);
	}

	// For collision datas
	UInt32		nNbrCollisionVertices;
	UInt32		nNbrCollisionIndices;
	Float32 *	pCollisionVertices;
	UInt32 *	pCollisionIndices;

	Stream >> nNbrCollisionVertices;
	if	(nNbrCollisionVertices)
	{
		pCollisionVertices = new Float32[nNbrCollisionVertices * 3 * sizeof(Float32)];
		for	(i = 0 ; i < nNbrCollisionVertices ; ++i)
		{
			Stream >> pCollisionVertices[3 * i];
			Stream >> pCollisionVertices[(3 * i) + 1];
			Stream >> pCollisionVertices[(3 * i) + 2];
		}
	}

	Stream >> nNbrCollisionIndices;
	if	(nNbrCollisionIndices)
	{
		pCollisionIndices = new UInt32[nNbrCollisionIndices * sizeof(UInt32)];
		for	(i = 0 ; i < nNbrCollisionIndices ; ++i)
		{
			Stream >> pCollisionIndices[i];
		}
	}

	Stream >> pMeshData->_uiNbrFacesToSort;
	if (pMeshData->_uiNbrFacesToSort)
	{
		pMeshData->SetNbrFacesToSort(pMeshData->_uiNbrFacesToSort);

		for (i = 0 ; i < pMeshData->_uiNbrFacesToSort ; ++i)
		{
			Stream >> pMeshData->_pFacesToSort[i];
		}
	}

	// Load data in video memory
	pMeshData->Init();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk3
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::CM_ReadChunk3(IO_STREAM &	Stream,
								 COMMUNICATOR *	pCommunicator)
{
	MESH_DATA	*pMeshData;
	pMeshData = static_cast<MESH_DATA *>(pCommunicator);

	COM_INSTANCE ci;
	UInt32	i, j, n, BufferSize;
	UInt8	*pBuffer;
	Bool	b;

	// For vertex buffers
	UInt8 **	pVertexBuffers;

	Stream >> pMeshData->_uiNbrVertexBuffers;
	pMeshData->_puiNbrVertices = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_puiVertexSizes = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pVertexBuffers = new UInt8*[pMeshData->_uiNbrVertexBuffers];

	// For vertex declaration
	pMeshData->_puiNbrVertexElement = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_pVertexElement = new S_VERTEX_ELEMENT*[pMeshData->_uiNbrVertexBuffers];

	for	(i = 0 ; i < pMeshData->_uiNbrVertexBuffers ; ++i)
	{
		// For vertex declaration
		Stream >> pMeshData->_puiNbrVertexElement[i];
		pMeshData->_pVertexElement[i] = new S_VERTEX_ELEMENT[pMeshData->_puiNbrVertexElement[i]];
		for	(j = 0 ; j < pMeshData->_puiNbrVertexElement[i] ; ++j)
		{
			Stream >> pMeshData->_pVertexElement[i][j]._nStream;
			Stream >> pMeshData->_pVertexElement[i][j]._nOffset;
			Stream >> pMeshData->_pVertexElement[i][j]._nType;
			Stream >> pMeshData->_pVertexElement[i][j]._nUsage;
		}

		// For vertex buffers
		Stream >> pMeshData->_puiNbrVertices[i];
		Stream >> pMeshData->_puiVertexSizes[i];

		BufferSize = pMeshData->_puiNbrVertices[i] * pMeshData->_puiVertexSizes[i];
		pVertexBuffers[i] = new UInt8[BufferSize];
		pBuffer = pVertexBuffers[i];
		while	(BufferSize > 0)
		{
			Stream >> *pBuffer;
			pBuffer++;
			BufferSize--;
		}
	}
	pMeshData->_pVertices = (void **)(pVertexBuffers);

	// For index buffers
	Stream >> pMeshData->_uiNbrIndexBuffers;
	pMeshData->_puiNbrIndices = new UInt32[pMeshData->_uiNbrIndexBuffers];
	pMeshData->_pIndices = new UInt16*[pMeshData->_uiNbrIndexBuffers];
	for	(i = 0 ; i < pMeshData->_uiNbrIndexBuffers ; ++i)
	{
		Stream >> pMeshData->_puiNbrIndices[i];
		pMeshData->_pIndices[i] = new UInt16[pMeshData->_puiNbrIndices[i]];
		for	(j = 0 ; j < pMeshData->_puiNbrIndices[i] ; ++j)
		{
			Stream >> pMeshData->_pIndices[i][j];
		}
	}

	// For primitives
	Stream >> pMeshData->_uiNbrPrimitives;
	pMeshData->_pPrimitives = new PRIMITIVE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingBoxes = new OBB[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingSpheres = new SPHERE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pfDisplayFlags = new QDT_FLAGS[pMeshData->_uiNbrPrimitives];
	for	(i = 0 ; i < pMeshData->_uiNbrPrimitives ; ++i)
	{
		Stream >> n;
		pMeshData->_pPrimitives[i].SetType((PRIMITIVE_TYPE)n);
		Stream >> n;
		// Cull mode (obsolete -> double sided is managed inside materials)
		Stream >> b;
		pMeshData->_pPrimitives[i].SetPerPolygoneSorting(b);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetVertexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstVertex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrVertices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetIndexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstIndex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrIndices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrJoints(n);

		pMeshData->_pfDisplayFlags[i].Set(MESH::DISPLAY_FLAG_VISIBLE, true);
		pMeshData->_pfDisplayFlags[i].Set(MESH::DISPLAY_FLAG_CAST_SHADOWS, true);
		pMeshData->_pfDisplayFlags[i].Set(MESH::DISPLAY_FLAG_RECEIVE_SHADOWS, true);
		
		UInt32 *	pJointIndices = NULL;
		if	(n > 0)
		{
			pJointIndices = new UInt32[n];
			for	(j = 0 ; j < n ; ++j)
			{
				Stream >> pJointIndices[j];
			}
		}
		pMeshData->_pPrimitives[i].SetJointIndices(pJointIndices);
		
		Stream >> ci;

#if defined(PLATFORM_PC)
		if (RENDER_CONTEXT::SupportShaderModel(SM_D3D_VS_1_1) == false || RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_2_0) == false)
		{
			pMeshData->_pPrimitives[i].SetShader(DISPLAY_MANAGER::Instance()->GetDefaultShader());
		}
		else
#endif
		{
			if (ci.HasHandle() == false)
			{
				pMeshData->_pPrimitives[i].SetShader(DISPLAY_MANAGER::Instance()->GetErrorShader());
			}
			else
			{
				pMeshData->_pPrimitives[i].SetShader(ci);
			}
		}

		pMeshData->_pLocalBoundingBoxes[i].Read(Stream);
		pMeshData->_pLocalBoundingSpheres[i].Read(Stream);
	}

	Stream >> pMeshData->_uiNbrFacesToSort;
	if (pMeshData->_uiNbrFacesToSort)
	{
		pMeshData->SetNbrFacesToSort(pMeshData->_uiNbrFacesToSort);

		for (i = 0 ; i < pMeshData->_uiNbrFacesToSort ; ++i)
		{
			Stream >> pMeshData->_pFacesToSort[i];
		}
	}

	// Load data in video memory
	pMeshData->Init();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk4
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::CM_ReadChunk4(IO_STREAM &	Stream,
								 COMMUNICATOR *	pCommunicator)
{
	MESH_DATA	*pMeshData;
	pMeshData = static_cast<MESH_DATA *>(pCommunicator);

	COM_INSTANCE ci;
	UInt32	i, j, n, BufferSize;
	UInt8	*pBuffer;
	Bool	b;

	// For vertex buffers
	UInt8 **	pVertexBuffers;

	Stream >> pMeshData->_uiNbrVertexBuffers;
	pMeshData->_puiNbrVertices = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_puiVertexSizes = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pVertexBuffers = new UInt8*[pMeshData->_uiNbrVertexBuffers];

	// For vertex declaration
	pMeshData->_puiNbrVertexElement = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_pVertexElement = new S_VERTEX_ELEMENT*[pMeshData->_uiNbrVertexBuffers];

	for	(i = 0 ; i < pMeshData->_uiNbrVertexBuffers ; ++i)
	{
		// For vertex declaration
		Stream >> pMeshData->_puiNbrVertexElement[i];
		pMeshData->_pVertexElement[i] = new S_VERTEX_ELEMENT[pMeshData->_puiNbrVertexElement[i]];
		for	(j = 0 ; j < pMeshData->_puiNbrVertexElement[i] ; ++j)
		{
			Stream >> pMeshData->_pVertexElement[i][j]._nStream;
			Stream >> pMeshData->_pVertexElement[i][j]._nOffset;
			Stream >> pMeshData->_pVertexElement[i][j]._nType;
			Stream >> pMeshData->_pVertexElement[i][j]._nUsage;
		}

		// For vertex buffers
		Stream >> pMeshData->_puiNbrVertices[i];
		Stream >> pMeshData->_puiVertexSizes[i];

		BufferSize = pMeshData->_puiNbrVertices[i] * pMeshData->_puiVertexSizes[i];
		pVertexBuffers[i] = new UInt8[BufferSize];
		pBuffer = pVertexBuffers[i];
		while	(BufferSize > 0)
		{
			Stream >> *pBuffer;
			pBuffer++;
			BufferSize--;
		}
	}
	pMeshData->_pVertices = (void **)(pVertexBuffers);

	// For index buffers
	Stream >> pMeshData->_uiNbrIndexBuffers;
	pMeshData->_puiNbrIndices = new UInt32[pMeshData->_uiNbrIndexBuffers];
	pMeshData->_pIndices = new UInt16*[pMeshData->_uiNbrIndexBuffers];
	for	(i = 0 ; i < pMeshData->_uiNbrIndexBuffers ; ++i)
	{
		Stream >> pMeshData->_puiNbrIndices[i];
		pMeshData->_pIndices[i] = new UInt16[pMeshData->_puiNbrIndices[i]];
		for	(j = 0 ; j < pMeshData->_puiNbrIndices[i] ; ++j)
		{
			Stream >> pMeshData->_pIndices[i][j];
		}
	}

	// For primitives
	Stream >> pMeshData->_uiNbrPrimitives;
	pMeshData->_pPrimitives = new PRIMITIVE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingBoxes = new OBB[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingSpheres = new SPHERE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pfDisplayFlags = new QDT_FLAGS[pMeshData->_uiNbrPrimitives];
	for	(i = 0 ; i < pMeshData->_uiNbrPrimitives ; ++i)
	{
		Stream >> n;
		pMeshData->_pPrimitives[i].SetType((PRIMITIVE_TYPE)n);
		Stream >> n;
		// Cull mode (obsolete -> double sided is managed inside materials)
		Stream >> b;
		pMeshData->_pPrimitives[i].SetPerPolygoneSorting(b);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetVertexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstVertex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrVertices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetIndexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstIndex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrIndices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrJoints(n);
		Stream >> pMeshData->_pfDisplayFlags[i];
		
		UInt32 *	pJointIndices = NULL;
		if	(n > 0)
		{
			pJointIndices = new UInt32[n];
			for	(j = 0 ; j < n ; ++j)
			{
				Stream >> pJointIndices[j];
			}
		}
		pMeshData->_pPrimitives[i].SetJointIndices(pJointIndices);
		
		Stream >> ci;

#if defined(PLATFORM_PC)
		if (RENDER_CONTEXT::SupportShaderModel(SM_D3D_VS_1_1) == false || RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_2_0) == false)
		{
			pMeshData->_pPrimitives[i].SetShader(DISPLAY_MANAGER::Instance()->GetDefaultShader());
		}
		else
#endif
		{
			if (ci.HasHandle() == false)
			{
				pMeshData->_pPrimitives[i].SetShader(DISPLAY_MANAGER::Instance()->GetErrorShader());
			}
			else
			{
				pMeshData->_pPrimitives[i].SetShader(ci);
			}
		}

		pMeshData->_pLocalBoundingBoxes[i].Read(Stream);
		pMeshData->_pLocalBoundingSpheres[i].Read(Stream);
	}

	Stream >> pMeshData->_uiNbrFacesToSort;
	if (pMeshData->_uiNbrFacesToSort)
	{
		pMeshData->SetNbrFacesToSort(pMeshData->_uiNbrFacesToSort);

		for (i = 0 ; i < pMeshData->_uiNbrFacesToSort ; ++i)
		{
			Stream >> pMeshData->_pFacesToSort[i];
		}
	}

	// Load data in video memory
	pMeshData->Init();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk5
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::CM_ReadChunk5(IO_STREAM &	Stream,
								 COMMUNICATOR *	pCommunicator)
{
	MESH_DATA	*pMeshData;
	pMeshData = static_cast<MESH_DATA *>(pCommunicator);

	COM_INSTANCE ci;
	UInt32	i, j, n, BufferSize;
	UInt8	*pBuffer;
	Bool	b;

	// For vertex buffers
	UInt8 **	pVertexBuffers;

	Stream >> pMeshData->_uiNbrVertexBuffers;
	pMeshData->_puiNbrVertices = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_puiVertexSizes = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pVertexBuffers = new UInt8*[pMeshData->_uiNbrVertexBuffers];

	// For vertex declaration
	pMeshData->_puiNbrVertexElement = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_pVertexElement = new S_VERTEX_ELEMENT*[pMeshData->_uiNbrVertexBuffers];

	for	(i = 0 ; i < pMeshData->_uiNbrVertexBuffers ; ++i)
	{
		// For vertex declaration
		Stream >> pMeshData->_puiNbrVertexElement[i];
		pMeshData->_pVertexElement[i] = new S_VERTEX_ELEMENT[pMeshData->_puiNbrVertexElement[i]];
		for	(j = 0 ; j < pMeshData->_puiNbrVertexElement[i] ; ++j)
		{
			Stream >> pMeshData->_pVertexElement[i][j]._nStream;
			Stream >> pMeshData->_pVertexElement[i][j]._nOffset;
			Stream >> pMeshData->_pVertexElement[i][j]._nType;
			Stream >> pMeshData->_pVertexElement[i][j]._nUsage;
		}

		// For vertex buffers
		Stream >> pMeshData->_puiNbrVertices[i];
		Stream >> pMeshData->_puiVertexSizes[i];

		BufferSize = pMeshData->_puiNbrVertices[i] * pMeshData->_puiVertexSizes[i];
		pVertexBuffers[i] = new UInt8[BufferSize];
		pBuffer = pVertexBuffers[i];
		while	(BufferSize > 0)
		{
			Stream >> *pBuffer;
			pBuffer++;
			BufferSize--;
		}
	}
	pMeshData->_pVertices = (void **)(pVertexBuffers);

	// For index buffers
	Stream >> pMeshData->_uiNbrIndexBuffers;
	if (pMeshData->_uiNbrIndexBuffers)
	{
		pMeshData->_puiNbrIndices = new UInt32[pMeshData->_uiNbrIndexBuffers];
		pMeshData->_pIndices = new UInt16*[pMeshData->_uiNbrIndexBuffers];
		for	(i = 0 ; i < pMeshData->_uiNbrIndexBuffers ; ++i)
		{
			Stream >> pMeshData->_puiNbrIndices[i];
			pMeshData->_pIndices[i] = new UInt16[pMeshData->_puiNbrIndices[i]];
			for	(j = 0 ; j < pMeshData->_puiNbrIndices[i] ; ++j)
			{
				Stream >> pMeshData->_pIndices[i][j];
			}
		}
	}

	// For primitives
	Stream >> pMeshData->_uiNbrPrimitives;
	pMeshData->_pPrimitives = new PRIMITIVE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingBoxes = new OBB[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingSpheres = new SPHERE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pfDisplayFlags = new QDT_FLAGS[pMeshData->_uiNbrPrimitives];
	for	(i = 0 ; i < pMeshData->_uiNbrPrimitives ; ++i)
	{
		Stream >> n;
		pMeshData->_pPrimitives[i].SetType((PRIMITIVE_TYPE)n);
		Stream >> b;
		pMeshData->_pPrimitives[i].SetPerPolygoneSorting(b);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetVertexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstVertex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrVertices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetIndexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstIndex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrIndices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrJoints(n);
		Stream >> pMeshData->_pfDisplayFlags[i];
		
		UInt32 *	pJointIndices = NULL;
		if	(n > 0)
		{
			pJointIndices = new UInt32[n];
			for	(j = 0 ; j < n ; ++j)
			{
				Stream >> pJointIndices[j];
			}
		}
		pMeshData->_pPrimitives[i].SetJointIndices(pJointIndices);
		
		Stream >> ci;

#if defined(PLATFORM_PC)
		if (RENDER_CONTEXT::SupportShaderModel(SM_D3D_VS_1_1) == false || RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_2_0) == false)
		{
			pMeshData->_pPrimitives[i].SetShader(DISPLAY_MANAGER::Instance()->GetDefaultShader());
		}
		else
#endif
		{
			if (ci.HasHandle() == false)
			{
				pMeshData->_pPrimitives[i].SetShader(DISPLAY_MANAGER::Instance()->GetErrorShader());
			}
			else
			{
				pMeshData->_pPrimitives[i].SetShader(ci);
			}
		}

		pMeshData->_pLocalBoundingBoxes[i].Read(Stream);
		pMeshData->_pLocalBoundingSpheres[i].Read(Stream);
	}

	Stream >> pMeshData->_uiNbrFacesToSort;
	if (pMeshData->_uiNbrFacesToSort)
	{
		pMeshData->SetNbrFacesToSort(pMeshData->_uiNbrFacesToSort);

		for (i = 0 ; i < pMeshData->_uiNbrFacesToSort ; ++i)
		{
			Stream >> pMeshData->_pFacesToSort[i];
		}
	}

	// Load data in video memory
	pMeshData->Init();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk6
//	Object:		
//	06-03-29:	FBO - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::CM_ReadChunk6(IO_STREAM &	Stream,
								 COMMUNICATOR *	pCommunicator)
{
	MESH_DATA	*pMeshData;
	pMeshData = static_cast<MESH_DATA *>(pCommunicator);

	COM_INSTANCE ci;
	UInt32	i, j, n, BufferSize;
	UInt8	*pBuffer;
	Bool	b;

	// For vertex buffers
	UInt8 **	pVertexBuffers;

	Stream >> pMeshData->_uiNbrVertexBuffers;
	pMeshData->_puiNbrVertices = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_puiVertexSizes = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pVertexBuffers = new UInt8*[pMeshData->_uiNbrVertexBuffers];

	// For vertex declaration
	pMeshData->_puiNbrVertexElement = new UInt32[pMeshData->_uiNbrVertexBuffers];
	pMeshData->_pVertexElement = new S_VERTEX_ELEMENT*[pMeshData->_uiNbrVertexBuffers];

	for	(i = 0 ; i < pMeshData->_uiNbrVertexBuffers ; ++i)
	{
		// For vertex declaration
		Stream >> pMeshData->_puiNbrVertexElement[i];
		pMeshData->_pVertexElement[i] = new S_VERTEX_ELEMENT[pMeshData->_puiNbrVertexElement[i]];
		for	(j = 0 ; j < pMeshData->_puiNbrVertexElement[i] ; ++j)
		{
			Stream >> pMeshData->_pVertexElement[i][j]._nStream;
			Stream >> pMeshData->_pVertexElement[i][j]._nOffset;
			Stream >> pMeshData->_pVertexElement[i][j]._nType;
			Stream >> pMeshData->_pVertexElement[i][j]._nUsage;
		}

		// For vertex buffers
		Stream >> pMeshData->_puiNbrVertices[i];
		Stream >> pMeshData->_puiVertexSizes[i];

		BufferSize = pMeshData->_puiNbrVertices[i] * pMeshData->_puiVertexSizes[i];
		pVertexBuffers[i] = new UInt8[BufferSize];
		pBuffer = pVertexBuffers[i];
		while	(BufferSize > 0)
		{
			Stream >> *pBuffer;
			pBuffer++;
			BufferSize--;
		}
	}
	pMeshData->_pVertices = (void **)(pVertexBuffers);

	// For index buffers
	Stream >> pMeshData->_uiNbrIndexBuffers;
	if (pMeshData->_uiNbrIndexBuffers)
	{
		pMeshData->_puiNbrIndices = new UInt32[pMeshData->_uiNbrIndexBuffers];
		pMeshData->_pIndices = new UInt16*[pMeshData->_uiNbrIndexBuffers];
		for	(i = 0 ; i < pMeshData->_uiNbrIndexBuffers ; ++i)
		{
			Stream >> pMeshData->_puiNbrIndices[i];
			pMeshData->_pIndices[i] = new UInt16[pMeshData->_puiNbrIndices[i]];
			for	(j = 0 ; j < pMeshData->_puiNbrIndices[i] ; ++j)
			{
				Stream >> pMeshData->_pIndices[i][j];
			}
		}
	}

	// For primitives
	Stream >> pMeshData->_uiNbrPrimitives;
	pMeshData->_pPrimitives = new PRIMITIVE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingBoxes = new OBB[pMeshData->_uiNbrPrimitives];
	pMeshData->_pLocalBoundingSpheres = new SPHERE[pMeshData->_uiNbrPrimitives];
	pMeshData->_pfDisplayFlags = new QDT_FLAGS[pMeshData->_uiNbrPrimitives];
	for	(i = 0 ; i < pMeshData->_uiNbrPrimitives ; ++i)
	{
		Stream >> n;
		pMeshData->_pPrimitives[i].SetType((PRIMITIVE_TYPE)n);
		Stream >> b;
		pMeshData->_pPrimitives[i].SetPerPolygoneSorting(b);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetVertexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstVertex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrVertices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetIndexBufferIdx(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetFirstIndex(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrIndices(n);
		Stream >> n;
		pMeshData->_pPrimitives[i].SetNbrJoints(n);
		Stream >> pMeshData->_pfDisplayFlags[i];
		
		UInt32 *	pJointIndices = NULL;
		if	(n > 0)
		{
			pJointIndices = new UInt32[n];
			for	(j = 0 ; j < n ; ++j)
			{
				Stream >> pJointIndices[j];
			}
		}
		pMeshData->_pPrimitives[i].SetJointIndices(pJointIndices);
		
		Stream >> ci;

#if defined(PLATFORM_PC)
		if (RENDER_CONTEXT::SupportShaderModel(SM_D3D_VS_1_1) == false || RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_2_0) == false)
		{
			pMeshData->_pPrimitives[i].SetShader(DISPLAY_MANAGER::Instance()->GetDefaultShader());
		}
		else
#endif
		{
			if (ci.HasHandle() == false)
			{
				pMeshData->_pPrimitives[i].SetShader(DISPLAY_MANAGER::Instance()->GetErrorShader());
			}
			else
			{
				pMeshData->_pPrimitives[i].SetShader(ci);
			}
		}

		pMeshData->_pLocalBoundingBoxes[i].Read(Stream);
		pMeshData->_pLocalBoundingSpheres[i].Read(Stream);
	}

	Stream >> pMeshData->_uiNbrFacesToSort;
	if (pMeshData->_uiNbrFacesToSort)
	{
		pMeshData->SetNbrFacesToSort(pMeshData->_uiNbrFacesToSort);

		for (i = 0 ; i < pMeshData->_uiNbrFacesToSort ; ++i)
		{
			Stream >> pMeshData->_pFacesToSort[i];
		}
	}

	// FlowData
	UInt32	iFlowDataNb;
	Stream >> iFlowDataNb;
	pMeshData->SetNbrFlowData(iFlowDataNb);

	for (UInt32 iFlowDataP = 0; iFlowDataP < iFlowDataNb; iFlowDataP++)
	{
		UInt16 bIsFlowDataExistsForChannel;
		Stream >> bIsFlowDataExistsForChannel;
		if (bIsFlowDataExistsForChannel != 0)
			pMeshData->_pFlowDataArray[iFlowDataP] = new FLOW_DATA(Stream);
	}

	// Load data in video memory 
	pMeshData->Init();
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk6
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::CM_WriteChunk6(IO_STREAM &	Stream,
								  COMMUNICATOR *	pCommunicator)
{
	MESH_DATA	*pMeshData;
	pMeshData = static_cast<MESH_DATA *>(pCommunicator);

	UInt32	i, j, BufferSize;
	UInt8	*pBuffer;	

	// Name
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("MESH_DATA :");
	Stream.DebugOutput(pMeshData->GetName());
	Stream.DebugOutput("\r\n");

	// For vertex buffers
	Stream.DebugOutput("Number of vertex buffers :");
	Stream << pMeshData->_uiNbrVertexBuffers;
	Stream.DebugOutput("\r\n");

	for	(i = 0 ; i < pMeshData->_uiNbrVertexBuffers ; ++i)
	{
		Stream.DebugOutput("    Vertex Buffer :");
		Stream.DebugOutput("\r\n");

		// For vertex declaration
		Stream.DebugOutput("        Vertex Declaration : ");
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Number of vertex element : ");
		Stream << pMeshData->_puiNbrVertexElement[i];
		Stream.DebugOutput("\r\n");

		for	(j = 0 ; j < pMeshData->_puiNbrVertexElement[i] ; ++j)
		{
			Stream.DebugOutput("        Vertex Element : ");
			Stream.DebugOutput("\r\n");

			if (Stream.IsFileTextStreamDebug())
			{
				Stream.DebugOutput("                Stream : ");
				Stream << (UInt32)pMeshData->_pVertexElement[i][j]._nStream;
				Stream.DebugOutput("\r\n");
			
				Stream.DebugOutput("                Offset : ");
				Stream << (UInt32)pMeshData->_pVertexElement[i][j]._nOffset;
				Stream.DebugOutput("\r\n");

				Stream.DebugOutput("                Type : ");
				switch (pMeshData->_pVertexElement[i][j]._nType)
				{
				case DT_FLOAT1:
					Stream.DebugOutput("DT_FLOAT1");
					break;
				
				case DT_FLOAT2:
					Stream.DebugOutput("DT_FLOAT2");
					break;

				case DT_FLOAT3:
					Stream.DebugOutput("DT_FLOAT3");
					break;

				case DT_FLOAT4:
					Stream.DebugOutput("DT_FLOAT4");
					break;

				case DT_COLOR:
					Stream.DebugOutput("DT_COLOR");
					break;

				case DT_FLOAT16_2:
					Stream.DebugOutput("DT_FLOAT16_2");
					break;

				case DT_FLOAT16_4:
					Stream.DebugOutput("DT_FLOAT16_4");
					break;
				}
				Stream.DebugOutput("\r\n");

				Stream.DebugOutput("                Usage : ");
				switch (pMeshData->_pVertexElement[i][j]._nUsage)
				{
				case DU_POSITION:
					Stream.DebugOutput("DU_POSITION");
					break;

				case DU_NORMAL:
					Stream.DebugOutput("DU_NORMAL");
					break;

				case DU_BLENDWEIGHT:
					Stream.DebugOutput("DU_BLENDWEIGHT");
					break;

				case DU_BLENDINDICES:
					Stream.DebugOutput("DU_BLENDINDICES");
					break;

				case DU_COLOR:
					Stream.DebugOutput("DU_COLOR");
					break;

				case DU_TEXCOORD0:
					Stream.DebugOutput("DU_TEXCOORD0");
					break;

				case DU_TEXCOORD1:
					Stream.DebugOutput("DU_TEXCOORD1");
					break;

				case DU_TEXCOORD2:
					Stream.DebugOutput("DU_TEXCOORD2");
					break;

				case DU_TEXCOORD3:
					Stream.DebugOutput("DU_TEXCOORD3");
					break;

				case DU_TEXCOORD4:
					Stream.DebugOutput("DU_TEXCOORD4");
					break;

				case DU_TEXCOORD5:
					Stream.DebugOutput("DU_TEXCOORD5");
					break;

				case DU_TEXCOORD6:
					Stream.DebugOutput("DU_TEXCOORD6");
					break;

				case DU_TEXCOORD7:
					Stream.DebugOutput("DU_TEXCOORD7");
					break;
				}
				Stream.DebugOutput("\r\n");
			}
			else
			{
				Stream << pMeshData->_pVertexElement[i][j]._nStream;
				Stream << pMeshData->_pVertexElement[i][j]._nOffset;
				Stream << pMeshData->_pVertexElement[i][j]._nType;
				Stream << pMeshData->_pVertexElement[i][j]._nUsage;
			}
			
		}

		// For vertex buffers
		Stream.DebugOutput("        Number of vertices : ");
		Stream << pMeshData->_puiNbrVertices[i];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Number of vertices : ");
		Stream << pMeshData->_puiVertexSizes[i];
		Stream.DebugOutput("\r\n");

		if (Stream.IsFileTextStreamDebug() == false)
		{
			BufferSize = pMeshData->_puiNbrVertices[i] * pMeshData->_puiVertexSizes[i];
			pBuffer = (UInt8 *)(pMeshData->_pVertices[i]);
			while	(BufferSize > 0)
			{
				Stream << *pBuffer;
				pBuffer++;
				BufferSize--;
			}
		}
	}

	// For index buffers
	Stream.DebugOutput("Number of index buffers : ");
	Stream << pMeshData->_uiNbrIndexBuffers;
	Stream.DebugOutput("\r\n");

	for	(i = 0 ; i < pMeshData->_uiNbrIndexBuffers ; ++i)
	{
		Stream.DebugOutput("    Index Buffer : ");
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Number of indices :");
		Stream << pMeshData->_puiNbrIndices[i];
		Stream.DebugOutput("\r\n");

		if (Stream.IsFileTextStreamDebug() == false)
		{
			for	(j = 0 ; j < pMeshData->_puiNbrIndices[i] ; ++j)
			{
				Stream << pMeshData->_pIndices[i][j];
			}
		}
	}

	// For primitives
	Stream.DebugOutput("Number of primitives : ");
	Stream << pMeshData->_uiNbrPrimitives;
	Stream.DebugOutput("\r\n");

	for	(i = 0 ; i < pMeshData->_uiNbrPrimitives ; ++i)
	{
		Stream.DebugOutput("    Primitive : ");
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Type : ");
		Stream << (UInt32)pMeshData->_pPrimitives[i].GetType();
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Per Polygone Sorting: ");
		Stream << pMeshData->_pPrimitives[i].GetPerPolygoneSorting();
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Vertex buffer ID : ");
		Stream << pMeshData->_pPrimitives[i].GetVertexBufferIdx();
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        First vertex : ");
		Stream << pMeshData->_pPrimitives[i].GetFirstVertex();
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Number of vertices : ");
		Stream << pMeshData->_pPrimitives[i].GetNbrVertices();
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Index buffer ID : ");
		Stream << pMeshData->_pPrimitives[i].GetIndexBufferIdx();
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        First index : ");
		Stream << pMeshData->_pPrimitives[i].GetFirstIndex();
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Number of indices : ");
		Stream << pMeshData->_pPrimitives[i].GetNbrIndices();
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Number of joints : ");
		Stream << pMeshData->_pPrimitives[i].GetNbrJoints();
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Display flags : ");
		Stream << pMeshData->_pfDisplayFlags[i];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        joints : ");
		for	(j = 0 ; j < pMeshData->_pPrimitives[i].GetNbrJoints() ; ++j)
		{
			Stream << pMeshData->_pPrimitives[i].GetJointIndex(j);
			Stream.DebugOutput(" ");
		}
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Shader :");
		Stream << pMeshData->_pPrimitives[i].GetShader();
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Local bounding box : ");
		pMeshData->_pLocalBoundingBoxes[i].Write(Stream);
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("        Local bounding sphere : ");
		pMeshData->_pLocalBoundingSpheres[i].Write(Stream);
		Stream.DebugOutput("\r\n");
	}

	// For faces to sort
	if (Stream.IsFileTextStreamDebug() == false)
	{
		Stream << pMeshData->_uiNbrFacesToSort;
		if (pMeshData->_uiNbrFacesToSort)
		{
			for (i = 0 ; i < pMeshData->_uiNbrFacesToSort ; ++i)
			{
				Stream << pMeshData->_pFacesToSort[i];
			}
		}
	}

	// FlowData
	Stream << pMeshData->_iFlowDataNb;
	for (UInt32 iFlowDataP = 0; iFlowDataP < pMeshData->_iFlowDataNb; iFlowDataP++)
	{
		UInt16 bIsFlowDataExistsForChannel = (pMeshData->_pFlowDataArray[iFlowDataP] != NULL ? 1 : 0);
		Stream << bIsFlowDataExistsForChannel;

		if (bIsFlowDataExistsForChannel != 0)
			Stream << *(pMeshData->_pFlowDataArray[iFlowDataP]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetPrimitive
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
const PRIMITIVE &	MESH_DATA::GetPrimitive(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrPrimitives);
	return	(_pPrimitives[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetPrimitive
//	Object:		
//	05-11-08:	ELE - Created
//-----------------------------------------------------------------------------
PRIMITIVE &	MESH_DATA::GetPrimitive(UInt32	nIndex)
{
	QDT_ASSERT(nIndex < _uiNbrPrimitives);
	return	(_pPrimitives[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrFacesToSort
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetNbrFacesToSort(UInt32	uiNbrFacesToSort)
{
	if (uiNbrFacesToSort)
	{
		_uiNbrFacesToSort = uiNbrFacesToSort;
		_pFacesToSort = new FACES_TO_SORT[_uiNbrFacesToSort];
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrPrimitives
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetNbrPrimitives(UInt32	uiNbrPrimitives)
{
	if	(uiNbrPrimitives)
	{
		_uiNbrPrimitives = uiNbrPrimitives;

		_pPrimitives = new PRIMITIVE[uiNbrPrimitives];
		_pLocalBoundingBoxes = new OBB[uiNbrPrimitives];
		_pLocalBoundingSpheres = new SPHERE[uiNbrPrimitives];
		_pfDisplayFlags = new QDT_FLAGS[_uiNbrPrimitives];
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetPrimitive
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetPrimitive(UInt32				nIndex,
								const PRIMITIVE &	Primitive)
{
	QDT_ASSERT(nIndex < _uiNbrPrimitives);
	_pPrimitives[nIndex] = Primitive;
}

//-----------------------------------------------------------------------------
//	Name:		GetFacesToSort
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
const FACES_TO_SORT & MESH_DATA::GetFacesToSort(UInt32 nIndex) const
{
	return (_pFacesToSort[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetFacesToSort
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
FACES_TO_SORT &	MESH_DATA::GetFacesToSort(UInt32	nIndex)
{
	return (_pFacesToSort[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetLocalBoundingBox
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
const OBB &	MESH_DATA::GetLocalBoundingBox(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrPrimitives);
	return	(_pLocalBoundingBoxes[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetLocalBoundingSphere
//	Object:		
//	06-01-30:	ELE - Created
//-----------------------------------------------------------------------------
const SPHERE &	MESH_DATA::GetLocalBoundingSphere(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrPrimitives);
	return	(_pLocalBoundingSpheres[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingBox
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetBoundingBox(UInt32		nIndex,
								  const OBB &	BoundingBox)
{
	QDT_ASSERT(_pLocalBoundingBoxes);
	QDT_ASSERT(nIndex < _uiNbrPrimitives);
	_pLocalBoundingBoxes[nIndex] = BoundingBox;
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingSphere
//	Object:		
//	06-01-30:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetBoundingSphere(UInt32			nIndex,
									 const SPHERE &	BoundingSphere)
{
	QDT_ASSERT(_pLocalBoundingSpheres);
	QDT_ASSERT(nIndex < _uiNbrPrimitives);
	_pLocalBoundingSpheres[nIndex] = BoundingSphere;
}

//-----------------------------------------------------------------------------
//	Name:		SetDisplayFlags
//	Object:		
//	06-03-14:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetDisplayFlags(UInt32				nIndex,
								   const QDT_FLAGS &	Flags)
{
	QDT_ASSERT(_pfDisplayFlags);
	QDT_ASSERT(nIndex < _uiNbrPrimitives);
	_pfDisplayFlags[nIndex] = Flags;
}

//-----------------------------------------------------------------------------
//	Name:		SetCastShadows
//	Object:		
//	06-01-30:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetCastShadows(UInt32 nIndex, 
								  Bool b)
{
	QDT_ASSERT(_pfDisplayFlags);
	QDT_ASSERT(nIndex < _uiNbrPrimitives);
	_pfDisplayFlags[nIndex].Set(MESH::DISPLAY_FLAG_CAST_SHADOWS, b);
}

//-----------------------------------------------------------------------------
//	Name:		SetReceiveShadows
//	Object:		
//	06-03-28:	SBE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetReceiveShadows(UInt32 nIndex, 
									Bool b)
{
	QDT_ASSERT(_pfDisplayFlags);
	QDT_ASSERT(nIndex < _uiNbrPrimitives);
	_pfDisplayFlags[nIndex].Set(MESH::DISPLAY_FLAG_RECEIVE_SHADOWS, b);
}

//-----------------------------------------------------------------------------
//	Name:		SetVisible
//	Object:		
//	06-01-30:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetVisible(UInt32 nIndex, 
							  Bool b)
{
	QDT_ASSERT(_pfDisplayFlags);
	QDT_ASSERT(nIndex < _uiNbrPrimitives);
	_pfDisplayFlags[nIndex].Set(MESH::DISPLAY_FLAG_VISIBLE, b);
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrFlowData
//	Object:		
//	06-03-29:	FBO - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetNbrFlowData(UInt32	iFlowDataNb)
{
	if (iFlowDataNb != _iFlowDataNb)
	{
		if (_pFlowDataArray != NULL)
		{
			for (UInt32 iFlowDataP = 0; iFlowDataP < _iFlowDataNb; iFlowDataP++)
			{
				if (_pFlowDataArray[iFlowDataP] != NULL)
					delete _pFlowDataArray[iFlowDataP];
				delete _pFlowDataArray;
			}
		}

		_iFlowDataNb = iFlowDataNb;
		if (_iFlowDataNb > 0)
		{
			_pFlowDataArray = new FLOW_DATA*[_iFlowDataNb];
			memset(_pFlowDataArray, 0, sizeof(UInt32) * _iFlowDataNb);
		}
		else
		{
			_pFlowDataArray = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetFlowData
//	Object:		
//	06-03-29:	FBO - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetFlowData(UInt32		iFlowDataIndex,
							  FLOW_DATA *	pFlowData)
{
	QDT_ASSERT(iFlowDataIndex < _iFlowDataNb);

	// Array itself should not be NULL according to _iFlowDataNb
	if (_pFlowDataArray[iFlowDataIndex] != NULL)
		delete _pFlowDataArray[iFlowDataIndex];

	// NULL value is allowed to disable a flowdata for the specified channel
	_pFlowDataArray[iFlowDataIndex] = pFlowData;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
