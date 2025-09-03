//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: PORTAL_ABC
//
//
//	00-1113		JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif
//*****************************************************************************
//
// CLASS DESCRIPTION ( PORTAL_ABC )
//
//	The PORTAL_ABC class implements ...
//
//*****************************************************************************

#include	"Include.h"

#include	INC_KRNCOM(OldCom/Database\DatabaseManager)

#include	INC_KRNCORE(Display\RenderPipeline)
#include	INC_KRNCORE(Profiler)

#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes2D/OBR)
#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment)
#include	INC_KRNCORE(Math/Geometry/Intersection)
#include	INC_KRNCORE(Math/Geometry/BoundingVolume/Cylinder)
	
#include	INC_3DENGINE(SceneGraph\Entities\Mesh)
#include	INC_3DENGINE(SceneGraph\Node)
#include	INC_3DENGINE(SceneGraph\Material)
#include	INC_3DENGINE(Display)

#ifdef _DEBUG
	#include	"PortalABC.inl"
#else
	#include	"Inline.h"

	#include	INL_KRNCORE(Display/GeometryABC)
	#include	INL_KRNCORE(Display/RenderManager)
	#include	INL_KRNCORE(Display/RenderPipeline)

	#include	INL_KRNCORE(Math/Geometry/Intersection)
	#include	INL_KRNCORE(Math/Geometry/Distance)
	#include	INL_KRNCORE(Math/Geometry/Matrix)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/Plane)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes3D/Frustrum)
	#include	INL_KRNCORE(Math/Geometry/BoundingVolume/Cylinder)
	#include	INL_KRNCORE(Math/Color)

	#include	INL_3DENGINE(SceneGraph\Node)
	#include	INL_3DENGINE(SceneGraph\Entities\Mesh)
	#include	INL_3DENGINE(SceneGraph\Entities\MeshGroup)
	#include	INL_3DENGINE(Display)
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PORTAL_ABC constructor
//	Object:		Default constructor
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
PORTAL_ABC::PORTAL_ABC(COMMUNICATOR_DESCRIPTOR *	pCD,
					   ENTITY_TYPE					eEntityType)
:
ENTITY_ABC				(pCD, eEntityType),
_nFlags					(0),
_nNbrGeometries			(0),
_pGeometriesArray		(NULL),
_pGeometriesMaterialInst(NULL),
_pCopyGeometries		(NULL),
_pSharedVBIndex			(NULL),
_nNbrVertices			(0),
_pSortedVerticesArray	(NULL),
_rFarClippingDistance	(10000.0f),
_nNbrCrossing			(0),
_pBoundingBox			(NULL),
_pOBR					(NULL),
_pPortalZones			(NULL),
_nPriority				(0),
_pAABBTree				(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PORTAL_ABC destructor
//	Object:		Destructor
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
PORTAL_ABC::~PORTAL_ABC()
{
	delete _pOBR;
	delete _pBoundingBox;
	delete _pAABBTree;
	delete [] _pSortedVerticesArray;
	delete [] _pPortalZones;
	delete [] _pGeometriesArray;
	delete [] _pSharedVBIndex;
}

//-----------------------------------------------------------------------------
// Name: PORTAL_ABC::Display
// Object:
// 01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
void PORTAL_ABC::Display(DISPLAY_PARAMETERS &	DisplayParameters)
{
	// code obsolete a refaire entierement 
	// pas urgent car aucun twin portal n'a de materiaux
	// donc ils ne sont pas a afficher.
	// les mirroirs sont affichés ailleurs

/*	int					i;
	bool				bFog;
	COLOR				Color(false);
	COLOR_FLOAT			PortalColor;
	COM_INSTANCE		*pMaterialInst;
	MATERIAL_ABC		*pMaterial;
	RENDER_CONTEXT		*pContext = RENDER_MANAGER::Instance()->GetRenderContext();

	SORTED_PORTAL_ZONE *pCurrentZone = DISPLAY_MANAGER::Instance()->GetCurrentPortalZone();
	QDT_ASSERT(pCurrentZone);

	if	( pCurrentZone->Has(static_cast<PORTAL_ABC *>(COM_INSTANCE(this).GetCommunicator())) == false )
	{		
		MATRIX &	GlobalMatrix = DisplayParameters.GetNode()->GetGlobalMatrix();
		
		pContext->SetWorldTransform0(&GlobalMatrix);
		
		for	(i = 0 ; i < _nNbrGeometries ; ++i)
		{
			pMaterialInst = &_pGeometriesMaterialInst[i];
			if	(pMaterialInst->HasHandle())
			{
				GEOMETRY_REF *pGeometryRef = pContext->GetGeometryRef();
				pGeometryRef->SetGeometry(_pGeometriesArray[i]);
				pGeometryRef->SetGlobalMatrix(&DisplayParameters.GetNode()->GetGlobalMatrix());
				pGeometryRef->SetDoClipping(GetFlags(DO_CLIPPING) > 0);
				pGeometryRef->SetStaticGeometry(GetFlags(STATIC_GEOMETRY) > 0);
				pGeometryRef->SetPrelit(GetFlags(STATIC_LIT) > 0);
				
				pGeometryRef->SetAmbientColor(pContext->GetGlobalAmbientLight());
				
				bFog = pContext->IsFogEnabled();
				pGeometryRef->EnableFog(bFog);
				
				if	(bFog)
				{
					pGeometryRef->SetFogNear(pContext->GetFogStart());
					pGeometryRef->SetFogFar(pContext->GetFogEnd());
					pGeometryRef->SetFogColor(pContext->GetFogColor());
				}
				
				pGeometryRef->SetBoundingBox(_pBoundingBox);
				
#ifdef _DEBUG
				pGeometryRef->SetMeshName(GetName());
#endif
				
				pMaterial = static_cast<MATERIAL_ABC *>(pMaterialInst->GetCommunicator());
				pGeometryRef->SetShader(pMaterial->GetShader());
				
				if	(pMaterial->GetTransparency() && pMaterial->HasOneBitAlpha())
				{
					pGeometryRef->EnableAlphaTest(true);
					pGeometryRef->SetAlphaTestFunc(RENDER_CONTEXT::TESTFUNC_EQUAL);
					pGeometryRef->SetAlphaTestRef(pContext->GetMaxAlphaValue());
				}
				else
				{
					pGeometryRef->EnableAlphaTest(false);
				}
				
#ifdef _DEBUG
				pGeometryRef->SetMaterialName(pMaterial->GetName());
#endif
				
				pContext->Push(pGeometryRef);
			}
		}		
#ifndef _MASTER
		if (DISPLAY_MANAGER::Instance()->DisplayBoundingBoxes())
		{
			DisplayBoundingBox(DisplayParameters);	
		}
#endif
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-03-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	PORTAL_ABC::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	if	( (DISPLAY_MANAGER::Instance()->IsMirrorsEnabled() == false) || (IsOnlyDynamicsReflexionsActivated()) )
	{
		return	(TestVisibility(DisplayParameters, true));
	}

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayZ
//	Object:		Display the portal in z-buffer
//	01-07-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::DisplayZ(const MATRIX & WorldMatrix,
							 const MATRIX &	ViewMatrix,
							 int			nMaskValue)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DisplayMask
//	Object:		
//	03-05-26:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::DisplayMask(const MATRIX &	WorldMatrix,
								const MATRIX &	ViewMatrix,
								int				nLevel,
								int				nMaskValue)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DisplayN
//	Object:		
//	01-10-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::DisplayN(const MATRIX &	WorldMatrix,
							 const MATRIX &	ViewMatrix)
{
	int					i;
	RENDER_CONTEXT		*pContext = RENDER_MANAGER::Instance()->GetRenderContext();
	bool				bWired = pContext->IsWireFrameEnabled();
	MATERIAL_ABC		*pMaterial;
	SHADER_ABC			*pShader;
	COM_INSTANCE		MaterialInst(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);

	if	(bWired)
	{
		pContext->EnableColorWrite(false);
		pContext->EnableWireFrame(false);
	}

	MATRIX TempM;
	TempM.InverseAffine(ViewMatrix);
	pContext->SetViewTransform(&TempM);

	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{	
		MaterialInst = _pGeometriesMaterialInst[i];

		if	(MaterialInst.HasHandle())
		{
			pMaterial = static_cast<MATERIAL_ABC *>(MaterialInst.GetCommunicator());
			pShader = pMaterial->GetShader();

			#ifdef _DEBUG
				pShader->SetName(pMaterial->GetName());
			#endif
		}
		else
		{
			pShader = NULL;
		}

		QDT_VECTOR<const MATRIX *>	vSkinningMatrices;


		GEOMETRY_REF Ref;
		Ref.SetGeometry(_pGeometriesArray[i]);
		Ref.SetGlobalMatrix(&WorldMatrix);
		Ref.SetShader(pShader);
		Ref.SetPrelit(true);
		Ref.SetDoubleSided(false);
		Ref.SetDoClipping(true);
	
		pContext->GetRenderPipeline()->Display(&Ref);
	}

	pContext->EnableColorWrite(true);
	pContext->EnableWireFrame(bWired);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayBoundingBox
//	Object:		Display bouding voulmes
//	01-08-03:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::DisplayBoundingBox(DISPLAY_PARAMETERS &	DisplayParameters) const
{
#ifndef _MASTER
	const static COLOR_FLOAT	Red(1.0f, 1.0f, 0.0f, 0.0f);	// Case dynamic
	const static COLOR_FLOAT	White(1.0f, 1.0f, 1.0f, 1.0f);	// Case static
	const static float			Step = 20.0f;
	RENDER_CONTEXT				*pContext = RENDER_MANAGER::Instance()->GetRenderContext();

	if (GetFlags(STATIC_GEOMETRY))
	{
		pContext->SetWorldTransform0(&MATRIX::IDENTITY);
		_pBoundingBox->Display(White);
	}
	else	// DYNAMIC_GEOMETRY
	{
		pContext->SetWorldTransform0(&DisplayParameters.GetNode()->GetGlobalMatrix());
		_pBoundingBox->Display(Red);
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	01-07-16:	ELE - Created
//-----------------------------------------------------------------------------
bool	PORTAL_ABC::TestVisibility(const DISPLAY_PARAMETERS &	DisplayParameters,
								   bool							bDoVisibility)
{
	bool			bCollide = true;
	bool			bIntersectNear = false;
	OBB				GlobalBoundingBox;
	BOUNDING_VOLUME *pGlobalBV;
	DISPLAY_MANAGER	*pDisplay = DISPLAY_MANAGER::Instance();

	// If the portal is invisible, there is no need to display it
	if (GetFlags(INVISIBLE))
	{
		return (false);
	}

	if (!bDoVisibility)
	{
		SetFlags(DO_CLIPPING, 0);
		return (true);
	}

	if (GetFlags(DYNAMIC_GEOMETRY))
	{
		GlobalBoundingBox = *_pBoundingBox;
		GlobalBoundingBox.Transform(DisplayParameters.GetNode()->GetGlobalMatrix());
		pGlobalBV = &GlobalBoundingBox;
	}
	else
	{
		pGlobalBV = _pBoundingBox;
	}

	SORTED_PORTAL_ZONE *pCurrentZone = DISPLAY_MANAGER::Instance()->GetCurrentPortalZone();
	QDT_ASSERT(pCurrentZone);

	switch	(pCurrentZone->GetType())
	{
		default:
		case SORTED_PORTAL_ZONE::T_CAMERA:
		{
			bCollide = pGlobalBV->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
#ifndef _MASTER
			DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
		}
		break;

		case SORTED_PORTAL_ZONE::T_MIRROR:
		{
			const	QDT_VECTOR < QDT_VECTOR <HEXAHEDRON*> > *pHexahedrons = pDisplay->GetHexahedrons();
			int		MI = pDisplay->GetMirrorIndex();

			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	It	  = pHexahedrons->GetHead() + MI;
	
			bCollide = false;

			QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItL = (*It).GetHead();
			QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItLEnd = (*It).GetTail();

			while	(ItL != ItLEnd)
			{
				bCollide = bCollide || pGlobalBV->IntersectFront(*(*ItL), bIntersectNear);
#ifndef _MASTER
				DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
				++ItL;
			}

			if	(bCollide)
			{
				FRUSTRUM *pMF = DISPLAY_MANAGER::Instance()->GetLastMirroredFrustrum();
				QDT_ASSERT(pMF);

				bCollide = pGlobalBV->Intersect(pMF->GetHexahedron(), bIntersectNear);
#ifndef _MASTER
				DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
			}
		}
		break;
		
		case SORTED_PORTAL_ZONE::T_TWIN:
		{
			QDT_ASSERT(pDisplay);
			const	QDT_VECTOR < QDT_VECTOR <HEXAHEDRON*> > *pHexahedrons = pDisplay->GetHexahedrons();
			QDT_ASSERT(pHexahedrons);

			int		MI = pDisplay->GetMirrorIndex();

			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	It	  = pHexahedrons->GetHead() + MI;
			QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	ItEnd = pHexahedrons->GetTail();

			while	((It != ItEnd) && bCollide)
			{
				bCollide = false;

				QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItL = (*It).GetHead();
				QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItLEnd = (*It).GetTail();

				while	(ItL != ItLEnd)
				{
					HEXAHEDRON * pHex = *ItL;
					QDT_ASSERT(pHex);
					QDT_ASSERT(pGlobalBV);
					bCollide = bCollide || pGlobalBV->Intersect(*pHex, bIntersectNear);
#ifndef _MASTER
					DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
					++ItL;
				}

				++It;
			}

			if	(bCollide)
			{
				FRUSTRUM *pMF = DISPLAY_MANAGER::Instance()->GetLastMirroredFrustrum();
				if	(pMF)
				{
					bCollide = pGlobalBV->Intersect(pMF->GetHexahedron(), bIntersectNear);
#ifndef _MASTER
					DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
				}
				else
				{
					bCollide = pGlobalBV->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
#ifndef _MASTER
					DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
#endif
				}
			}
		}
		break;
	}

/*	if	(pCurrentZone->GetType() != SORTED_PORTAL_ZONE::T_CAMERA)
	{
		const QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> > *pHexahedrons = DISPLAY_MANAGER::Instance()->GetHexahedrons();

		int MI = DISPLAY_MANAGER::Instance()->GetMirrorIndex();

		QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	It = pHexahedrons->GetHead() + MI;
		QDT_VECTOR < QDT_VECTOR <HEXAHEDRON *> >::ITERATOR	ItEnd = pHexahedrons->GetTail();

		while	(It != ItEnd)
		{
			nRes = false;

			QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItL = (*It).GetHead();
			QDT_VECTOR <HEXAHEDRON *>::ITERATOR	ItLEnd = (*It).GetTail();

			while	(ItL != ItLEnd)
			{
				nRes = nRes || pGlobalBV->Intersect(*(*ItL), bIntersectNear);
				DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);

				++ItL;
			}
			
			if	(nRes)
			{
				++It;
			}
			else
			{
				return	(false);
			}
		}
	}
	else
	{
		nRes = pGlobalBV->Intersect(DisplayParameters.GetFrustrum().GetHexahedron(), bIntersectNear);
		DISPLAY_MANAGER::Instance()->GetStats().AddHexahedronIntersections(1);
	}
*/
	if (bCollide)
	{	
		if	(bIntersectNear)
		{
			SetFlags(DO_CLIPPING, DO_CLIPPING);
		}
		else
		{
			SetFlags(DO_CLIPPING, 0);
		}
	}

	return (bCollide);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeNearDistance
//	Object:		
//	03-03-05:	ELE - Created
//-----------------------------------------------------------------------------
float	PORTAL_ABC::ComputeNearDistance(const VECTOR &	ViewPos,
										const VECTOR &	ViewDir)
{
	int		i, nMinIndex = -1;
	float	rLength, rMinDistance = 1000000.0f;
	VECTOR	V;

	for	(i = 0 ; i < _nNbrVertices ; ++i)
	{
		V.SetFromPoints(ViewPos, _pSortedVerticesArray[i]);
		rLength = V.Normalize();

		if	(V.Dot(ViewDir) > 0.0f)
		{
			if	(rLength < rMinDistance)
			{
				rMinDistance = rLength;
				nMinIndex = i;
			}
		}
		else
		{
			nMinIndex = -1;
			break;
		}
	}

	if	(nMinIndex < 0)
	{
		return	(0.0f);
	}
	else
	{
		V.SetFromPoints(ViewPos, _pSortedVerticesArray[nMinIndex]);
		rLength = V.Normalize();

		return	(V.Dot(ViewDir) * rLength);
	}
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		ReadCAttributes
//	Object:		Read from database the common attributes
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::ReadCAttributes7(IO_STREAM & Stream,
									 int			nVersion)
{
	int	i, nMaterialId;

	Stream	>> _nFlags
			>> _nNbrGeometries
			>> _rFarClippingDistance
			>> _nNbrCrossingMax;

	_pGeometriesArray = new GEOMETRY_ABC*[_nNbrGeometries];
	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{
		_pGeometriesArray[i] = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateGeometry();
	}

	_pGeometriesMaterialInst = new COM_INSTANCE[_nNbrGeometries];

	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{
		if	(nVersion == 2)
		{
			// Special case !!!!!
			_pGeometriesArray[i]->SetVersion(1);
		}
		else
		{
			_pGeometriesArray[i]->SetVersion(nVersion);
		}

		Stream >> *_pGeometriesArray[i];

		nMaterialId = _pGeometriesArray[i]->GetMaterialId();
		if	(nMaterialId > -1)
		{
			_pGeometriesMaterialInst[i].SetHandle(COM_SERVICE::RetrieveComHandle(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MATERIAL, nMaterialId));
		}
	}

	if	(nVersion >= 2)
	{
		Stream >> _nNbrVertices;

		_pSortedVerticesArray = new VECTOR[_nNbrVertices];
		for (i = 0 ; i < _nNbrVertices ; ++i)
		{
			Stream >> _pSortedVerticesArray[i];
		}

		_pOBR = new OBR(_nNbrVertices, _pSortedVerticesArray);

		BuildPlane();
	}

	if (GetFlags(STATIC_GEOMETRY))
	{
		_pBoundingBox = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_AABB);	
	}
	else // DYNAMIC
	{
		_pBoundingBox = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_OBB);
	}
	_pBoundingBox->Read(Stream);

	_pPortalZones = new SORTED_PORTAL_ZONE[_nNbrCrossingMax];
}

//-----------------------------------------------------------------------------
//	Name:		ReadCAttributes
//	Object:		Read from database the common attributes
//	01-03-29:	ELE - Created
//	03-06-23:	RMA - _nPriority added
//-----------------------------------------------------------------------------
void	PORTAL_ABC::ReadCAttributes8(IO_STREAM & Stream,
									 int			nVersion)
{
	int	i, nMaterialId;

	Stream	>> _nFlags
			>> _nNbrGeometries
			>> _rFarClippingDistance
			>> _nPriority
			>> _nNbrCrossingMax;

	_pGeometriesArray = new GEOMETRY_ABC*[_nNbrGeometries];
	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{
		_pGeometriesArray[i] = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateGeometry();
	}

	_pGeometriesMaterialInst = new COM_INSTANCE[_nNbrGeometries];

	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{
		if	(nVersion == 2)
		{
			// Special case !!!!!
			_pGeometriesArray[i]->SetVersion(1);
		}
		else
		{
			_pGeometriesArray[i]->SetVersion(nVersion);
		}

		Stream >> *_pGeometriesArray[i];

		nMaterialId = _pGeometriesArray[i]->GetMaterialId();
		if	(nMaterialId > -1)
		{
			_pGeometriesMaterialInst[i].SetHandle(COM_SERVICE::RetrieveComHandle(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MATERIAL, nMaterialId));
		}
	}

	if	(nVersion >= 2)
	{
		Stream >> _nNbrVertices;

		_pSortedVerticesArray = new VECTOR[_nNbrVertices];
		for (i = 0 ; i < _nNbrVertices ; ++i)
		{
			Stream >> _pSortedVerticesArray[i];
		}

		_pOBR = new OBR(_nNbrVertices, _pSortedVerticesArray);

		BuildPlane();
	}

	if (GetFlags(STATIC_GEOMETRY))
	{
		_pBoundingBox = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_AABB);	
	}
	else // DYNAMIC
	{
		_pBoundingBox = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_OBB);
	}
	_pBoundingBox->Read(Stream);

	_pPortalZones = new SORTED_PORTAL_ZONE[_nNbrCrossingMax];
}

//-----------------------------------------------------------------------------
//	Name:		ReadCAttributes9
//	Object:		
//	03-10-16:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::ReadCAttributes9(class IO_STREAM &	Stream,
									 int				nVersion)
{
	int	i;

	Stream	>> _nFlags
			>> _nNbrGeometries
			>> _rFarClippingDistance
			>> _nPriority
			>> _nNbrCrossingMax;

	_pGeometriesArray = new GEOMETRY_ABC*[_nNbrGeometries];
	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{
		_pGeometriesArray[i] = RENDER_MANAGER::Instance()->GetRenderContext()->AllocateGeometry();
	}

	_pGeometriesMaterialInst = new COM_INSTANCE[_nNbrGeometries];

	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{
		if	(nVersion == 2)
		{
			// Special case !!!!!
			_pGeometriesArray[i]->SetVersion(1);
		}
		else
		{
			_pGeometriesArray[i]->SetVersion(nVersion);
		}

		Stream >> *_pGeometriesArray[i];

		Stream >> _pGeometriesMaterialInst[i];
	}

	if	(nVersion >= 2)
	{
		Stream >> _nNbrVertices;

		_pSortedVerticesArray = new VECTOR[_nNbrVertices];
		for (i = 0 ; i < _nNbrVertices ; ++i)
		{
			Stream >> _pSortedVerticesArray[i];
		}

		_pOBR = new OBR(_nNbrVertices, _pSortedVerticesArray);

		BuildPlane();
	}

	if (GetFlags(STATIC_GEOMETRY))
	{
		_pBoundingBox = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_AABB);	
	}
	else // DYNAMIC
	{
		_pBoundingBox = BV_FACTORY::Instance()->Create(BOUNDING_VOLUME::TYPE_OBB);
	}
	_pBoundingBox->Read(Stream);

	_pPortalZones = new SORTED_PORTAL_ZONE[_nNbrCrossingMax];
}

//-----------------------------------------------------------------------------
//	Name:		ReadCAttributes10
//	Object:		
//	04-05-17:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::ReadCAttributes10(class IO_STREAM &	Stream,
									  int				nVersion)
{
	_pAABBTree = new AABB_TREE;
	ReadCAttributes9(Stream, nVersion);
	QDT_ASSERT(_pAABBTree);
	_pAABBTree->Read2(Stream);
}

//-----------------------------------------------------------------------------
//	Name:		ReadCAttributes11
//	Object:		
//	04-05-17:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::ReadCAttributes11(class IO_STREAM &	Stream,
									  int				nVersion)
{
	ReadCAttributes10(Stream, nVersion);

	Stream >> _MeshGroup;

	_pSharedVBIndex = new unsigned int [_nNbrGeometries];
	for (int i = 0 ; i < _nNbrGeometries ; ++i)
	{
		Stream >> _pSharedVBIndex[i];
	}
}

#endif

//-----------------------------------------------------------------------------
//	Name:		WriteCAttributes
//	Object:		Write to database the common attributes
//	01-03-29:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	PORTAL_ABC::WriteCAttributes(IO_STREAM &	Stream,
									 int			nVersion) const
{
	int	i;

	Stream.DebugOutput("Flags :\t");
	Stream	<< _nFlags;
	Stream.DebugOutput("\t");
	Stream.DebugOutputBinary(_nFlags);
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Number of geometries :\t");
	Stream	<< _nNbrGeometries;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Far clipping distance :\t");
	Stream	<< _rFarClippingDistance;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Priority :\t");
	Stream	<< _nPriority;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Number of crossing :\t");
	Stream	<< _nNbrCrossingMax;
	Stream.DebugOutput("\r\n");

	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{
		Stream.DebugOutput("Geometry ");
		Stream.DebugOutput(i);
		Stream.DebugOutput(" : ");

		_pGeometriesArray[i]->SetVersion(nVersion);

		Stream << *_pGeometriesArray[i];

		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Material ");
		Stream.DebugOutput(i);
		Stream.DebugOutput(" :\r\n");
		Stream << _pGeometriesMaterialInst[i];

		Stream.DebugOutput("\r\n");
	}

	Stream.DebugOutput("Number of vertices :\t");
	Stream	<< _nNbrVertices;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Vertices :\r\n");
	for (i = 0 ; i < _nNbrVertices ; ++i)
	{
		Stream.DebugOutput("Vertex ");
		Stream.DebugOutput(i);
		Stream.DebugOutput(" :\t");

		Stream << _pSortedVerticesArray[i];

		Stream.DebugOutput("\r\n");
	}

	if (GetFlags(STATIC_GEOMETRY))
	{
		Stream.DebugOutput("Bounding AABB :\t");		
	}
	else // DYNAMIC
	{
		Stream.DebugOutput("Bounding OBB :\t");
	}

	QDT_ASSERT(_pBoundingBox);
	Stream << *_pBoundingBox;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("AABB Tree : ");
	Stream.DebugOutput("\r\n");
	_pAABBTree->Write(Stream);
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Mesh group : ");
	Stream << _MeshGroup;
	Stream.DebugOutput("\r\n");

	for	(i = 0 ; i < _nNbrGeometries ; ++i)
	{
		Stream.DebugOutput("Geometry ");
		Stream.DebugOutput(i);
		Stream.DebugOutput(" : ");

		Stream.DebugOutput("Shared VB Index : ");
		Stream << _pSharedVBIndex[i];
		Stream.DebugOutput("\r\n");
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetPlane
//	Object:		
//	01-08-08:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::BuildPlane()
{
	PLANE		Plane;
	int			i,j, n;

	if	(_pOBR)
	{
		VECTOR	N(0.0f, 0.0f, -1.0f);
		_Plane = _pOBR->GetPlane(N);
	}
	else
	{
		if	(_pSortedVerticesArray)
		{

			VECTOR	V1;
			V1.Sub(_pSortedVerticesArray[1], _pSortedVerticesArray[0]);

			//**************************************************************//
			//	CVI : 16/08/2001
			/////////////////////
			VECTOR	V2;
			VECTOR	V3;

			i = 1;
			n = this->_nNbrVertices;

			//	Find a vector not null
			while ( V1.IsNull() )
			{
				i++;
				if ( i == n)
				{
					V1.SetFromPoints(_pSortedVerticesArray[0], _pSortedVerticesArray[1]);
					break;
				}
				V1.SetFromPoints(_pSortedVerticesArray[0], _pSortedVerticesArray[i] );
			}

			j = 1;
			V2.SetFromPoints(_pSortedVerticesArray[i], _pSortedVerticesArray[i+j] );

			while ( V2.IsNull())
			{
				j++;
				if ( i+j == n)
				{
					V2.SetFromPoints(_pSortedVerticesArray[0], _pSortedVerticesArray[n-1]);
					break;
				}
				V2.SetFromPoints(_pSortedVerticesArray[i], _pSortedVerticesArray[i+j]);
			}

			V3.Cross(V1, V2);
			QDT_ASSERT( !V3.IsNull());

			//-------------------------------------------------------------//
			//*************************************************************//
			//-------------------------------------------------------------//

			if (V3.GetZ() >= 0)
			{
				_Plane.SetFromPoints(_pSortedVerticesArray[i+j], _pSortedVerticesArray[i], _pSortedVerticesArray[0]) ;
			}
			else
			{
				_Plane.SetFromPoints(_pSortedVerticesArray[0], _pSortedVerticesArray[i], _pSortedVerticesArray[i+j]) ;
			}
		}
		else
		{
			_Plane.SetCoeff(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PORTAL_ABC::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PORTAL_ABC) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PORTAL_ABC::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("PORTAL_ABC"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PORTAL_ABC,
											QDT_COMMENT("94207208-3FED-0F6A-A56E-C5EFECC84F28"),
											sizeof(PORTAL_ABC),
											ComInit,
											QDT_COMMENT("Abstract class for portals"),
											ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::EM_Construct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::EM_Destruct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		COM_PostLoad
//	Object:		
//	04-06-10:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::COM_PostLoad()
{
	UpdateSharedVBs();
}

//-----------------------------------------------------------------------------
//	Name:		COM_PostLoad
//	Object:		
//	04-06-10:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::UpdateSharedVBs()
{
	if (_pSharedVBIndex && _MeshGroup.HasHandle())
	{
		MESH_GROUP * pGroup = (MESH_GROUP*)_MeshGroup.GetCommunicator();

		for (int i = 0 ; i < GetNbrGeometries() ; ++i)
		{
			_pGeometriesArray[i]->SetSharedVertexBuffer(pGroup->GetSharedVB(_pSharedVBIndex[i]));
		}
	}
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================