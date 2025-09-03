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
//	CLASS:	TRIGGER
//
//	02-03-12:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Trigger/Trigger)
#include	INCL_3DENGINE(SceneGraph/Entities/Camera/Camera)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderABC)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderColorPerVertex)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)
#include	INCL_KMATH(Math/Geometry/Intersection/Intersection)
#include	INCL_KRENDERER(Renderer/SVertexElement)
#include	INCL_KRENDERER(Renderer/IVertexBuffer)
#include	INCL_KRENDERER(Renderer/IVertexDeclaration)
#include	INCL_KRENDERER(Renderer/RendererTypeDefs)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComplexType/TypeColor)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

// For chunks
const COLOR	TRIGGER::ENTER_DIRECTION_COLOR(255, 0, 255, 0);
const COLOR	TRIGGER::ENTER_COLOR(255, 0, 0, 255);
const COLOR	TRIGGER::ACTIVE_COLOR(255, 255, 255, 0);
const COLOR	TRIGGER::DEFAULT_COLOR(255, 255, 0, 0);

S_VERTEX_ELEMENT		TRIGGER::_aVertexElts[2] = {
	{ 0, 0, DT_FLOAT3, DU_POSITION },
	{ 0, 12, DT_COLOR, DU_COLOR }};

I_VERTEX_DECLARATION *	TRIGGER::_pVertexDeclaration = NULL;

//-----------------------------------------------------------------------------
//	Name:		TRIGGER constructor
//	Object:		
//	02-03-12:	RMA - Created
//-----------------------------------------------------------------------------
TRIGGER::TRIGGER()
:
ENTITY_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_TRIGGER),
_rDirection	(0.0f),
_rAperture	(0.0f),
_bActive	(true)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		TRIGGER destructor
//	Object:		
//	02-03-12:	RMA - Created
//-----------------------------------------------------------------------------
TRIGGER::~TRIGGER()
{
	if (_pVertexDeclaration!=NULL)
	{
		RENDER_CONTEXT::ReleaseVertexDeclaration(_pVertexDeclaration);
		_pVertexDeclaration = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Register
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	TRIGGER::Register()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		pSG->AddTrigger(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Unregister
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	TRIGGER::Unregister()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		pSG->RemoveTrigger(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-03-12:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIGGER::Display()
{
	DISPLAY_MANAGER * pDisplay = DISPLAY_MANAGER::Instance();
	Float32			rGlobalAngle;
	int				nNbrHexahedrons, nHexahedronIndex;
	HEXAHEDRON		CurHexahedron;
	COLOR_FLOAT		Color;

	NODE	*pNode = GetNode();
	CAMERA	*pCamera = DISPLAY_MANAGER::Instance()->GetActiveCamera();

	pDisplay->EnableSkinning(false);
	pDisplay->SetWorldMatrix(MATRIX::IDENTITY);

	nNbrHexahedrons = GetNbrHexahedrons();
	for ( nHexahedronIndex = 0; nHexahedronIndex < nNbrHexahedrons; ++nHexahedronIndex )
	{
		rGlobalAngle	= _rDirection + pNode->GetFrontDirAngle();
		Color			= _vHexahedrons[nHexahedronIndex]._Color;	
		CurHexahedron	= _vHexahedrons[nHexahedronIndex]._Hexahedron;	

		CurHexahedron.Transform( pNode->GetGlobalMatrix());
		
		if (_vHexahedrons[nHexahedronIndex]._pVertexBuffer == NULL)
		{
			_vHexahedrons[nHexahedronIndex]._pVertexBuffer = RENDER_CONTEXT::CreateVertexBuffer(14, 16, false);
		}	

		FillVertexBuffer(CurHexahedron, Color, rGlobalAngle, _vHexahedrons[nHexahedronIndex]._pVertexBuffer);	

		if (_pVertexDeclaration == NULL)
		{
			_pVertexDeclaration = RENDER_CONTEXT::CreateVertexDeclaration(&_aVertexElts[0] , 2);
		}

		//SHADER_ABC * pShader = (SHADER_ABC*)pDisplay->GetColorPerVertexShader().GetCommunicator();
		COM_INSTANCE ShaderCI;
		ShaderCI.SetHandle( SHADER_COLOR_PER_VERTEX::GetGenericCommunicatorDescriptor()->Create() );
		SHADER_COLOR_PER_VERTEX* pShader = static_cast<SHADER_COLOR_PER_VERTEX *>( ShaderCI.GetCommunicator() );

		for (UInt32 uiTex = 0 ; uiTex < MAX_TEXTURE_STAGES ; ++uiTex)
		{
			RENDER_CONTEXT::SetPixelShaderTexture(uiTex, NULL);
		}	
		
		UInt32 uiNbrOfPasses = pShader->GetNbrPasses();
		for (UInt32 uiPass = 0 ; uiPass < uiNbrOfPasses ; ++uiPass)
		{
			pShader->SetPass(uiPass, VECTOR(0, 0, 0));
			RENDER_CONTEXT::SetVertexBuffer(0, _vHexahedrons[nHexahedronIndex]._pVertexBuffer);
			RENDER_CONTEXT::SetVertexDeclaration(_pVertexDeclaration);
			RENDER_CONTEXT::DrawPrimitive(PT_LINE_LIST, 7, 0);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		COLOR_HEXAHEDRON
//	Object:		
//	05-12-09:	RMA - Created
//-----------------------------------------------------------------------------
TRIGGER::COLOR_HEXAHEDRON::COLOR_HEXAHEDRON()
:
_pVertexBuffer(NULL)
{
}

//-----------------------------------------------------------------------------
//	Name:		~COLOR_HEXAHEDRON
//	Object:		
//	05-12-09:	RMA - Created
//-----------------------------------------------------------------------------
TRIGGER::COLOR_HEXAHEDRON::~COLOR_HEXAHEDRON()
{
	if (_pVertexBuffer!=NULL)
	{
		RENDER_CONTEXT::ReleaseVertexBuffer(_pVertexBuffer);
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayHexahedronFloor
//	Object:		
//	03-03-18:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::FillVertexBuffer(const HEXAHEDRON & Hexahedron,  
								  const COLOR_FLOAT & Color,
								  Float32 rGlobalAngle,
								  I_VERTEX_BUFFER * pVertexBuffer)
{
	VECTOR		P1, P2, P3, P4, Center, Vector;
	Float32		rSin, rCos;
	static const Float32 DIR_LENGTH = 100.0f;

	// Display the hexahedron.
	if ( ( Hexahedron.GetPoint( P1, HEXAHEDRON::HP_LEFT,  HEXAHEDRON::HP_NEAR, HEXAHEDRON::HP_BOTTOM ) == false ) ||
		 ( Hexahedron.GetPoint( P2, HEXAHEDRON::HP_LEFT,  HEXAHEDRON::HP_FAR,  HEXAHEDRON::HP_BOTTOM ) == false ) ||
		 ( Hexahedron.GetPoint( P3, HEXAHEDRON::HP_RIGHT, HEXAHEDRON::HP_FAR,  HEXAHEDRON::HP_BOTTOM ) == false ) ||
		 ( Hexahedron.GetPoint( P4, HEXAHEDRON::HP_RIGHT, HEXAHEDRON::HP_NEAR, HEXAHEDRON::HP_BOTTOM ) == false ) )
	{
		QDT_FAIL();
		return;
	}

	S_TRIG_VERTEX * pVertices = (S_TRIG_VERTEX *)pVertexBuffer->Lock();

	// Points oare moved up a lin order to avoid z-buffer problems.
	P1.SetY( P1.GetY() + 1.0f );
	P2.SetY( P2.GetY() + 1.0f );
	P3.SetY( P3.GetY() + 1.0f );
	P4.SetY( P4.GetY() + 1.0f );

	UInt32 c = RENDER_CONTEXT::GetPlatformIntColor(Color);

	pVertices[0]._rVx = P1.GetX();
	pVertices[0]._rVy = P1.GetY();
	pVertices[0]._rVz = P1.GetZ();
	pVertices[0]._cDiffuse = c;
	pVertices[1]._rVx = P2.GetX();
	pVertices[1]._rVy = P2.GetY();
	pVertices[1]._rVz = P2.GetZ();
	pVertices[1]._cDiffuse = c;
	pVertices[2]._rVx = P2.GetX();
	pVertices[2]._rVy = P2.GetY();
	pVertices[2]._rVz = P2.GetZ();
	pVertices[2]._cDiffuse = c;
	pVertices[3]._rVx = P3.GetX();
	pVertices[3]._rVy = P3.GetY();
	pVertices[3]._rVz = P3.GetZ();
	pVertices[3]._cDiffuse = c;
	pVertices[4]._rVx = P3.GetX();
	pVertices[4]._rVy = P3.GetY();
	pVertices[4]._rVz = P3.GetZ();
	pVertices[4]._cDiffuse = c;
	pVertices[5]._rVx = P4.GetX();
	pVertices[5]._rVy = P4.GetY();
	pVertices[5]._rVz = P4.GetZ();
	pVertices[5]._cDiffuse = c;
	pVertices[6]._rVx = P4.GetX();
	pVertices[6]._rVy = P4.GetY();
	pVertices[6]._rVz = P4.GetZ();
	pVertices[6]._cDiffuse = c;
	pVertices[7]._rVx = P1.GetX();
	pVertices[7]._rVy = P1.GetY();
	pVertices[7]._rVz = P1.GetZ();
	pVertices[7]._cDiffuse = c;

	// Aperture and direction.
	Center = ( P1 + P2 + P3 + P4 ) * 0.25f;

	MATH::GetCosinusAndSinus( rGlobalAngle + MATH::PI() - _rAperture, rCos, rSin );
	Vector.SetXYZ( DIR_LENGTH * rSin, 0.0, DIR_LENGTH * rCos );
	P1 = Center;
	P2 = Center + Vector;
	pVertices[8]._rVx = P1.GetX();
	pVertices[8]._rVy = P1.GetY();
	pVertices[8]._rVz = P1.GetZ();
	pVertices[8]._cDiffuse = c;
	pVertices[9]._rVx = P2.GetX();
	pVertices[9]._rVy = P2.GetY();
	pVertices[9]._rVz = P2.GetZ();
	pVertices[9]._cDiffuse = c;
	
	MATH::GetCosinusAndSinus( rGlobalAngle + MATH::PI(), rCos, rSin );
	Vector.SetXYZ( DIR_LENGTH * rSin, 0.0, DIR_LENGTH * rCos );
	Vector *= 0.5f;
	P1 = Center;
	P2 = Center + Vector;
	pVertices[10]._rVx = P1.GetX();
	pVertices[10]._rVy = P1.GetY();
	pVertices[10]._rVz = P1.GetZ();
	pVertices[10]._cDiffuse = c;
	pVertices[11]._rVx = P2.GetX();
	pVertices[11]._rVy = P2.GetY();
	pVertices[11]._rVz = P2.GetZ();
	pVertices[11]._cDiffuse = c;

	MATH::GetCosinusAndSinus( rGlobalAngle + MATH::PI() + _rAperture, rCos, rSin );
	Vector.SetXYZ( DIR_LENGTH * rSin, 0.0, DIR_LENGTH * rCos );
	P1 = Center ;
	P2 = Center + Vector;
	pVertices[12]._rVx = P1.GetX();
	pVertices[12]._rVy = P1.GetY();
	pVertices[12]._rVz = P1.GetZ();
	pVertices[12]._cDiffuse = c;
	pVertices[13]._rVx = P2.GetX();
	pVertices[13]._rVy = P2.GetY();
	pVertices[13]._rVz = P2.GetZ();
	pVertices[13]._cDiffuse = c;

	pVertexBuffer->Unlock();
}

//-----------------------------------------------------------------------------
//	Name:		SetDirection
//	Object:		
//	03-01-21:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::SetDirection(const Float32	rAngle)
{
	// Make the angle fit in [0, 2*PI].
	_rDirection = fmodf( rAngle, 2.0f * MATH::PI() );
	if ( _rDirection < 0.0f )
	{
		_rDirection += 2.0f * MATH::PI();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAperture
//	Object:		
//	03-01-21:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::SetAperture(const Float32	rAngle)
{
	_rAperture = MATH::Clamp( rAngle, 0.0f, MATH::PI() );
}

//-----------------------------------------------------------------------------
//	Name:		GetDirection
//	Object:		
//	03-01-21:	GGO - Created
//-----------------------------------------------------------------------------
Float32	TRIGGER::GetDirection() const
{
	return ( _rDirection );
}

//-----------------------------------------------------------------------------
//	Name:		GetAperture
//	Object:		
//	03-01-21:	GGO - Created
//-----------------------------------------------------------------------------
Float32	TRIGGER::GetAperture() const
{
	return ( _rAperture );
}

//-----------------------------------------------------------------------------
//	Name:		DirectionsMatch
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
bool	TRIGGER::DirectionsMatch(Float32	rAngle1,
								 Float32	rAngle2,
								 Float32	rAperture)
{
	// If the zone has a full aperture, the directions matches whatever the angles.
	if ( rAperture >= ( 2.0f * MATH::PI() - 0.01f ) )
	{
		return ( true );
	}

	// We must add PI in order to the angle tha have the same orientation than
	// the action angle.
	// (nice...)
	rAngle1 += MATH::PI();

	Float32 rDiff = rAngle1 - rAngle2;

	// Modulo AFTER calculation & angle between -PI & PI
	rDiff = fmodf( rDiff, 2.0f * MATH::PI());

	if (rDiff > MATH::PI())
	{
		rDiff -= 2 * MATH::PI();
	}

	if (rDiff < -MATH::PI())
	{
		rDiff += 2 * MATH::PI();
	}

	QDT_ASSERT( ( rDiff >= -MATH::PI() ) && ( rDiff <= MATH::PI() ) );

	if ( MATH::Abs(rDiff) < rAperture )
	{
		return ( true );
	}
	else
	{
		return ( false );
	}


	// We must add PI in order to the angle tha have the same orientation than
	// the action angle.
/*	rAngle1 += MATH::PI();

	// Make the angles fit in [0, 2*PI].
	rAngle1 = fmodf( rAngle1, 2.0f * MATH::PI() );
	rAngle2 = fmodf( rAngle2, 2.0f * MATH::PI() );
	if ( rAngle1 < 0.0f )
	{
		rAngle1 += 2.0f * MATH::PI();
	}
	if ( rAngle2 < 0.0f )
	{
		rAngle2 += 2.0f * MATH::PI();
	}

	QDT_ASSERT( ( rAngle1 >= 0.0f ) && ( rAngle1 <= 2.0f * MATH::PI() ) );
	QDT_ASSERT( ( rAngle2 >= 0.0f ) && ( rAngle2 <= 2.0f * MATH::PI() ) );

	// Check if the angle is in the action angle.
	if ( MATH::Abs( rAngle1 - rAngle2 ) < rAperture )
	{
		return ( true );
	}
	else
	{
		return ( false );
	}*/	
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	03-01-23:	GGO - Created
//	04-04-14:	RMA - Optimized : the method GetFrontDirAngle is very costly!
//-----------------------------------------------------------------------------
TRIGGER::TRIGGER_ACTION	TRIGGER::Update(NODE * pEntityNode,
										const AABB & Box,
										Float32 rEntityAngle)
{
	QDT_ASSERT( pEntityNode );
	
	NODE * pThisTriggerNode = GetNode();

	Float32			rTriggerAngle;
	PREVIOUS_STATE	PreviousState;

	// Return the correct state.
	PreviousState = GetPreviousState( pEntityNode );
	switch( PreviousState )
	{
	case PS_OUTSIDE:
		if ( Intersect(Box) == true )
		{
			// Retrive angles.
			rTriggerAngle = GetDirection();
			rTriggerAngle += pThisTriggerNode->GetFrontDirAngle();

			if ( DirectionsMatch( rTriggerAngle, rEntityAngle, _rAperture ) == true )
			{
				AddInside( pEntityNode, true );

				return ( TA_ENTRY_MATCH_DIR );
			}
			else
			{
				AddInside( pEntityNode, false );

				return ( TA_ENTRY_NO_MATCH_DIR );
			}
		}
		else
		{
			return ( TA_OUTSIDE );
		}

	case PS_INSIDE_MATCH:
		if ( Intersect(Box) == true )
		{
			// Retrive angles.
			rTriggerAngle = GetDirection();
			rTriggerAngle += pThisTriggerNode->GetFrontDirAngle();

			if ( DirectionsMatch( rTriggerAngle, rEntityAngle, _rAperture ) == true )
			{
				return ( TA_INSIDE_MATCH );
			}
			else
			{
				SetInsideMatch( pEntityNode, false );

				return ( TA_INSIDE_LOSE_MATCH_DIR );
			}
		}
		else
		{
			RemoveInside( pEntityNode );

			return ( TA_EXIT_MATCH_DIR );
		}

	case PS_INSIDE_NO_MATCH:
		if ( Intersect(Box ) == true )
		{
			// Retrive angles.
			rTriggerAngle = GetDirection();
			rTriggerAngle += pThisTriggerNode->GetFrontDirAngle();

			if ( DirectionsMatch( rTriggerAngle, rEntityAngle, _rAperture ) == true )
			{
				SetInsideMatch( pEntityNode, true );

				return ( TA_INSIDE_GAIN_MATCH_DIR );
			}
			else
			{
				return ( TA_INSIDE_NO_MATCH );
			}
		}
		else
		{
			RemoveInside( pEntityNode );

			return ( TA_EXIT_NO_MATCH_DIR );
		}

	default:
		QDT_FAIL();
		return ( TA_OUTSIDE );
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddInside
//	Object:		
//	03-03-17:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::AddInside(NODE *		pNode,
						   const bool	bMatchDir)
{
	INSIDE_NODE	InsideNode;

	InsideNode._pNode		= pNode;
	InsideNode._bMatchDir	= bMatchDir;

	_vInsideNodes.PushTail( InsideNode );
}

//-----------------------------------------------------------------------------
//	Name:		SetInsideMatch
//	Object:		
//	03-03-17:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::SetInsideMatch(NODE *		pNode,
								const bool	bMatchDir)
{
	int	nInsideNodeIndex, nNbrInsideNode;


	nNbrInsideNode = _vInsideNodes.GetSize();
	for ( nInsideNodeIndex = 0; nInsideNodeIndex < nNbrInsideNode; ++nInsideNodeIndex )
	{
		if ( _vInsideNodes[ nInsideNodeIndex ]._pNode == pNode )
		{
			_vInsideNodes[ nInsideNodeIndex ]._bMatchDir = bMatchDir;
			return;
		}
	}

	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		RemoveEntered
//	Object:		
//	03-03-17:	GGO - Created
//	04-07-02:	RMA - I've removed the QDT_FAIL
//-----------------------------------------------------------------------------
void	TRIGGER::RemoveInside(NODE *	pNode)
{
	int	nInsideNodeIndex, nNbrInsideNode;

	nNbrInsideNode = _vInsideNodes.GetSize();
	for ( nInsideNodeIndex = 0; nInsideNodeIndex < nNbrInsideNode; ++nInsideNodeIndex )
	{
		if ( _vInsideNodes[ nInsideNodeIndex ]._pNode == pNode )
		{
			_vInsideNodes.Remove( _vInsideNodes.GetHead() + nInsideNodeIndex );
			return;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetPreviousState
//	Object:		
//	03-03-17:	GGO - Created
//-----------------------------------------------------------------------------
TRIGGER::PREVIOUS_STATE	TRIGGER::GetPreviousState(NODE *	pNode)
{
	int	nInsideNodeIndex, nNbrInsideNode;

	nNbrInsideNode = _vInsideNodes.GetSize();
	for ( nInsideNodeIndex = 0; nInsideNodeIndex < nNbrInsideNode; ++nInsideNodeIndex )
	{
		if ( _vInsideNodes[ nInsideNodeIndex ]._pNode == pNode )
		{
			if ( _vInsideNodes[ nInsideNodeIndex ]._bMatchDir == true )
			{
				return ( PS_INSIDE_MATCH );
			}
			else
			{
				return ( PS_INSIDE_NO_MATCH );
			}
		}
	}

	return ( PS_OUTSIDE );
}

//-----------------------------------------------------------------------------
//	Name:		AddHexahedron
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::AddHexahedron(const HEXAHEDRON &	Hexahedron,
							   const COLOR &		Color)
{
	QDT_ASSERT( Hexahedron.IsValid() == true );

	COLOR_HEXAHEDRON	ColorHexahedron;
	ColorHexahedron._Hexahedron	= Hexahedron;
	ColorHexahedron._Color		= Color;

	_vHexahedrons.PushTail( ColorHexahedron );
}

//-----------------------------------------------------------------------------
//	Name:		AddHexahedron
//	Object:		
//	03-01-28:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::AddHexahedron(const HEXAHEDRON &	Hexahedron)
{
	AddHexahedron( Hexahedron, DEFAULT_COLOR );
}

//-----------------------------------------------------------------------------
//	Name:		RemoveHexahedron
//	Object:		
//	03-06-13:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::RemoveHexahedron(const int	nHexahedronIndex)
{
	if ( ( nHexahedronIndex < 0 ) || ( nHexahedronIndex >= GetNbrHexahedrons() ) )
	{
		QDT_FAIL();
		return;
	}

	_vHexahedrons.Remove( _vHexahedrons.GetHead() + nHexahedronIndex );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrHexahedrons
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
int	TRIGGER::GetNbrHexahedrons() const
{
	int	nNbrHexahedrons;

	nNbrHexahedrons = _vHexahedrons.GetSize();

	return ( nNbrHexahedrons );
}

//-----------------------------------------------------------------------------
//	Name:		GetHexahedrons
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
const HEXAHEDRON &	TRIGGER::GetHexahedron(const int	nIndex) const
{
	QDT_ASSERT( ( nIndex >= 0 ) && ( nIndex < GetNbrHexahedrons() ) );

	return ( _vHexahedrons[ nIndex ]._Hexahedron );
}

//-----------------------------------------------------------------------------
//	Name:		GetColor
//	Object:		
//	03-01-28:	GGO - Created
//-----------------------------------------------------------------------------
const COLOR &	TRIGGER::GetColor(const int	nIndex) const
{
	QDT_ASSERT( ( nIndex >= 0 ) && ( nIndex < GetNbrHexahedrons() ) );

	if ( (GetActive()) && (_vHexahedrons[ nIndex ]._Color == TRIGGER::DEFAULT_COLOR) )
	{
		if (_vInsideNodes.IsEmpty())
		{
			return (TRIGGER::ACTIVE_COLOR);
		}
		else
		{
			// Entity inside detected
			int	nInsideNodeIndex, nNbrInsideNode;
			nNbrInsideNode = _vInsideNodes.GetSize();
			for ( nInsideNodeIndex = 0; nInsideNodeIndex < nNbrInsideNode; ++nInsideNodeIndex )
			{
				if (_vInsideNodes[ nInsideNodeIndex ]._bMatchDir == true)
				{
					return (TRIGGER::ENTER_DIRECTION_COLOR);
				}
			}
			return (TRIGGER::ENTER_COLOR);
		}
	}
	return ( _vHexahedrons[ nIndex ]._Color );
}

//-----------------------------------------------------------------------------
//	Name:		SetHexahedron
//	Object:		
//	03-01-28:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::SetHexahedron(const int			nIndex,
							   const HEXAHEDRON &	Hexahedron)
{
	QDT_ASSERT( ( nIndex >= 0 ) && ( nIndex < GetNbrHexahedrons() ) );

//	COLOR_HEXAHEDRON	ColorHexadron;

//	ColorHexadron				= _vHexahedrons[ nIndex ];
//	ColorHexadron._Hexahedron	= Hexahedron;
//	_vHexahedrons[ nIndex ]		= ColorHexadron;
	_vHexahedrons[ nIndex ]._Hexahedron = Hexahedron;
}

//-----------------------------------------------------------------------------
//	Name:		SetColor
//	Object:		
//	03-01-28:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::SetColor(const int		nIndex,
						  const COLOR &	Color)
{
	QDT_ASSERT( ( nIndex >= 0 ) && ( nIndex < GetNbrHexahedrons() ) );

//	COLOR_HEXAHEDRON	ColorHexadron;

//	ColorHexadron			= _vHexahedrons[ nIndex ];
//	ColorHexadron._Color	= Color;
//	_vHexahedrons[ nIndex ]	= ColorHexadron;
	_vHexahedrons[ nIndex ]._Color = Color;

//	COLOR c( _vHexahedrons[ nIndex ]._Color );
}

//-----------------------------------------------------------------------------
//	Name:		SetColor
//	Object:		
//	03-01-28:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::SetColor(const COLOR &	Color)
{
	int	nNbrhexahedrons, nHexahedronIndex;

	nNbrhexahedrons = GetNbrHexahedrons();
	for ( nHexahedronIndex = 0; nHexahedronIndex < nNbrhexahedrons; ++nHexahedronIndex )
	{
		SetColor( nHexahedronIndex, Color );
	}
}

//-----------------------------------------------------------------------------
//	Name:		ClearHexahedrons
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::ClearHexahedrons()
{
	_vHexahedrons.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
bool	TRIGGER::Intersect(const AABB & Box) const
{
	const NODE * pThisTriggerNode = GetNode();

	AABB			TransformedBox;
	int				i, nCount = GetNbrHexahedrons();
	const MATRIX &	Matrix = pThisTriggerNode->GetGlobalMatrix();

	// Transform the current AABB in the trigger coordinates
	QDT_ASSERT(Matrix(0, 0) == 1.0f);	// Assuming the trigge has no rotation!!
	QDT_ASSERT(Matrix(0, 1) == 0.0f);
	QDT_ASSERT(Matrix(0, 2) == 0.0f);
	QDT_ASSERT(Matrix(1, 0) == 0.0f);
	QDT_ASSERT(Matrix(1, 1) == 1.0f);
	QDT_ASSERT(Matrix(1, 2) == 0.0f);
	QDT_ASSERT(Matrix(2, 0) == 0.0f);
	QDT_ASSERT(Matrix(2, 1) == 0.0f);
	QDT_ASSERT(Matrix(2, 2) == 1.0f);

	TransformedBox = Box;
	TransformedBox.Translate(-Matrix.GetTranslation());
	
	for (i = 0; i < nCount; ++i)
	{
		// Retrive the current hexahedron and move it into global coordinates.
		QDT_ASSERT(GetHexahedron(i).IsValid() == true);
		if (TransformedBox.Intersect(GetHexahedron(i)) == true)
		{
			return (true);
		}
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-04-02:	FDE - Created
//-----------------------------------------------------------------------------
bool	TRIGGER::Intersect(const VECTOR&	EntityPos) const
{
	const NODE * pThisTriggerNode = GetNode();

	int				nNbrHexahedrons, nHexahedronIndex;
	HEXAHEDRON		CurHexahedron;
	

	nNbrHexahedrons	= GetNbrHexahedrons();
	for ( nHexahedronIndex = 0; nHexahedronIndex < nNbrHexahedrons; ++nHexahedronIndex )
	{
		// Retrive the current hexahedron and move it into global coordinates.
		CurHexahedron = GetHexahedron( nHexahedronIndex );
		CurHexahedron.Transform( pThisTriggerNode->GetGlobalMatrix() );
		QDT_ASSERT( CurHexahedron.IsValid() == true );


		if ( INTERSECTION::TestIntersectionHexahedronPoint( CurHexahedron, EntityPos ) == true )
		{
			return ( true );
		}
	}

	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		SetFromObb
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::SetFromObb(const OBB &	Obb)
{
	HEXAHEDRON	Hexahedron;

	ClearHexahedrons();

	Hexahedron.SetFromObb( Obb );

	AddHexahedron( Hexahedron, DEFAULT_COLOR );
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-02-04:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::Copy(const TRIGGER &	Trigger)
{
	int	nNbrHexahedrons, nHexahedronIndex;

	_rAperture	= Trigger._rAperture;
	_rDirection	= Trigger._rDirection;

	ClearHexahedrons();

	nNbrHexahedrons = Trigger.GetNbrHexahedrons();
	for ( nHexahedronIndex = 0; nHexahedronIndex < nNbrHexahedrons; ++nHexahedronIndex )
	{
		AddHexahedron( Trigger.GetHexahedron( nHexahedronIndex ), Trigger.GetColor( nHexahedronIndex ) );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	03-02-05:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::Transform(const MATRIX &	Matrix)
{
	int	nNbrHexahedrons, nHexahedronIndex;

	nNbrHexahedrons = GetNbrHexahedrons();
	for ( nHexahedronIndex = 0; nHexahedronIndex < nNbrHexahedrons ; ++nHexahedronIndex )
	{
		_vHexahedrons[ nHexahedronIndex ]._Hexahedron.Transform( Matrix );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetCenter
//	Object:		
//	03-02-05:	GGO - Created
//-----------------------------------------------------------------------------
bool	TRIGGER::GetCenter(VECTOR &	Center) const
{
	VECTOR	PointTmp;
	int		nNbrHexahedrons, nHexahedronIndex;

	nNbrHexahedrons = GetNbrHexahedrons();
	if ( nNbrHexahedrons == 0 )
	{
		return ( false );
	}

	if ( GetHexahedron( 0 ).GetCenter( Center ) == false )
	{
		return ( false );
	}

	for ( nHexahedronIndex = 1; nHexahedronIndex < nNbrHexahedrons; ++nHexahedronIndex )
	{
		if ( GetHexahedron( nHexahedronIndex ).GetCenter( PointTmp ) == false )
		{
			return ( false );
		}

		Center += PointTmp;
	}

	Center /= static_cast<Float32>( nNbrHexahedrons );

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		Clean
//	Object:		
//	03-03-06:	FDE - Created
//-----------------------------------------------------------------------------
void	TRIGGER::Clean()
{
	_vInsideNodes.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		GetActive
//	Object:		
//	03-06-25:	GGO - Created
//-----------------------------------------------------------------------------
bool	TRIGGER::GetActive() const
{
	return ( _bActive );
}

//-----------------------------------------------------------------------------
//	Name:		SetActive
//	Object:		
//	03-06-25:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::SetActive(const bool	bActive)
{
	_bActive = bActive;

	if ( bActive == false )
	{
		Clean();
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIGGER::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR	*pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk( "TRIGGER ", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk( "TRIGGER ", 2, CM_ReadChunk2);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("TRIGGER ", 2, CM_WriteChunk);
#endif

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetColor"), QDT_COMMENT("Set color"));
	pMD->AddParameter(QDT_COMMENT("Color"), QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_TRGR_SET_COLOR, pMD, EM_SetColor);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	TRIGGER::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TRIGGER) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	TRIGGER::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("TRIGGER"), 
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TRIGGER,
											QDT_COMMENT("A3CAE7C9-F069-C8B7-1947-8B233AEB8B80"), 
											sizeof(TRIGGER), 
											ComInit, 
											QDT_COMMENT("Trigger"),
											ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIGGER::EM_Construct(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	TRIGGER *pTrigger;
	pTrigger = static_cast<TRIGGER *>(pO);

	QDT_NEW_PLACED(pTrigger, TRIGGER)();
	QDT_ASSERT(pTrigger);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIGGER::EM_Destruct(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	TRIGGER *pTrigger;
	pTrigger = static_cast<TRIGGER *>(pO);

	QDT_DELETE_PLACED(pTrigger, TRIGGER);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetColor
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIGGER::EM_SetColor(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	TRIGGER *		pTrigger = static_cast<TRIGGER*>(pComInstance->GetCommunicator());
	COLOR*			pC = static_cast<COLOR*>(pP);

	pTrigger->SetColor(*pC);
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIGGER::CM_ReadChunk1(IO_STREAM &		Stream,
							   COMMUNICATOR *	pCommunicator)
{
	TRIGGER *pTrigger;
	pTrigger = static_cast<TRIGGER *>(pCommunicator);

	HEXAHEDRON	Hexahedron;
	int			nNbrHexahedrons, nHexahedronIndex;

	pTrigger->ClearHexahedrons();

	Stream >> pTrigger->_rDirection;
	Stream >> pTrigger->_rAperture;
	Stream >> nNbrHexahedrons;

	QDT_VECTOR <VECTOR> vPoints;

	for ( nHexahedronIndex = 0; nHexahedronIndex < nNbrHexahedrons; ++nHexahedronIndex )
	{
		Stream >> Hexahedron;
		pTrigger->AddHexahedron( Hexahedron, DEFAULT_COLOR );
		Hexahedron.ComputePoints(vPoints);
	}

	SPHERE S;
	S.Compute(vPoints);
	pTrigger->SetBoundingSphere(S);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk2
//	Object:		
//	03-06-25:	GGO - Created
//-----------------------------------------------------------------------------
void	TRIGGER::CM_ReadChunk2(IO_STREAM &		Stream,
							   COMMUNICATOR *	pCommunicator)
{
	TRIGGER *pTrigger;
	pTrigger = static_cast<TRIGGER *>(pCommunicator);

	HEXAHEDRON	Hexahedron;
	int			nNbrHexahedrons, nHexahedronIndex;

	pTrigger->ClearHexahedrons();

	Stream >> pTrigger->_rDirection;
	Stream >> pTrigger->_rAperture;
	Stream >> pTrigger->_bActive;
	Stream >> nNbrHexahedrons;

	QDT_VECTOR <VECTOR> vPoints;

	for ( nHexahedronIndex = 0; nHexahedronIndex < nNbrHexahedrons; ++nHexahedronIndex )
	{
		Stream >> Hexahedron;
		pTrigger->AddHexahedron( Hexahedron, DEFAULT_COLOR );
		Hexahedron.ComputePoints(vPoints);
	}

	SPHERE S;
	S.Compute(vPoints);
	pTrigger->SetBoundingSphere(S);
}

#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk1
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	TRIGGER::CM_WriteChunk(IO_STREAM &		Stream,
							   COMMUNICATOR *	pCommunicator)
{
	TRIGGER *pTrigger;
	pTrigger = static_cast<TRIGGER *>(pCommunicator);

	Stream.DebugOutput("Trigger :\t");
	Stream.DebugOutput(pTrigger->GetName());
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Direction :\t");
	Stream << pTrigger->_rDirection;
	
	Stream.DebugOutput("Aperture :\t");
	Stream << pTrigger->_rAperture;

	Stream.DebugOutput("Active :\t");
	Stream << pTrigger->_bActive;
	
	Stream.DebugOutput("Nbr hexahedrons :\t");
	int nNbrHexahedrons = pTrigger->GetNbrHexahedrons();
	Stream << nNbrHexahedrons;
	
	for (int nHexahedronIndex = 0; nHexahedronIndex < nNbrHexahedrons; ++nHexahedronIndex )
	{
		Stream.DebugOutput("Hexahedron :\t");
		Stream	<< pTrigger->GetHexahedron( nHexahedronIndex );
		Stream.DebugOutput("\r\n");
	}
	
	Stream.DebugOutput("\r\n");
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
