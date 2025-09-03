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
// CLASS: LIGHT
//
//
//	00-11-13	JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif
//*****************************************************************************
//
// CLASS DESCRIPTION ( LIGHT )
//
//	The LIGHT class implements ...
//
//*****************************************************************************

#include	"include.h"
#include	INC_KRNCOM(OldCom/Chunk)
#include	INC_KERNEL(Stream)

#ifdef _DEBUG
	#include "Light.inl"
#else
	#include	"Inline.h"
	#include	INL_3DENGINE(Display)
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

// For chunks
int LIGHT::_nNbChunk = 0;
CHUNK LIGHT::_ChunkList[LIGHT::NB_CHUNK];

//-----------------------------------------------------------------------------
// Name: LIGHT constructor
// Object: Constructor
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
LIGHT::LIGHT()
:
_nType				(LIGHT::POINT_LIGHT),
_Color				(0.0f, 1.0f, 1.0f, 1.0f),
_rHotSpotAngle		(MATH::PI() / 4.0f),
_rFallOffAngle		(MATH::PI() / 4.0f),
_rHotSpotDistance	(10000.0f),
_rFallOffDistance	(10000.0f),	
_rMultiplier		(1.0f),
_nFlags				(0),
_pInfos				(new	SORTED_NODE_INFOS)
{	
	if (_nNbChunk == 0)
	{
		_nNbChunk = NB_CHUNK;
		_ChunkList[0].Set("PIPO", 2);
	}
}

//-----------------------------------------------------------------------------
// Name: LIGHT destructor
// Object: Destructor
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
LIGHT::~LIGHT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name: LIGHT::operator 
//	Object: Operator =
//	01-02-09:	RMA - Created
//-----------------------------------------------------------------------------
LIGHT & LIGHT::operator = (const LIGHT & Light)
{
	if (this != &Light)
	{
		Copy(Light) ;
	}

	return (*this) ;
}

//-----------------------------------------------------------------------------
// Name: ::GetNbChunk
// Object: Get the number of chunks
//01-03-02 FKE Created: 
//-----------------------------------------------------------------------------
int LIGHT::GetNbChunk() const
{
	return _nNbChunk;
}

//-----------------------------------------------------------------------------
// Name: ::GetChunk
// Object: Get a chunk
//01-03-02 FKE Created: 
//-----------------------------------------------------------------------------
const CHUNK& LIGHT::GetChunk(int nIndex) const
{
	QDT_ASSERT(nIndex < _nNbChunk);
	return _ChunkList[nIndex];
}

//-----------------------------------------------------------------------------
// Name: void ::Read
// Object: Read a chunk from a stream
//01-01-25 FKE Created: 
//-----------------------------------------------------------------------------
bool LIGHT::Read(const CHUNK & Chunk, 
				 IO_STREAM   & Stream)
{
	if (Chunk == _ChunkList[0])
	{
		switch (Chunk.GetVersion())
		{
		case 1:
			{
				Stream	>> _nFlags
						>> _nType
						>> _Color
						>> _rHotSpotAngle
						>> _rFallOffAngle
						>> _rHotSpotDistance
						>> _rFallOffDistance
						>> _rMultiplier ;

				#ifdef __RENDERMANAGER__
				// Convert the light to the format of the RENDER_MANAGER
				BuildRMLight();
				#endif

				return (true);
			}
			break;

		case 2:
			{
				Stream	>> _nFlags
						>> _nType
						>> _Color
						>> _rHotSpotAngle
						>> _rFallOffAngle
						>> _rHotSpotDistance
						>> _rFallOffDistance
						>> _rMultiplier
						>> _ShadowColor ;

				#ifdef __RENDERMANAGER__
				// Convert the light to the format of the RENDER_MANAGER
				BuildRMLight();
				#endif

				return (true);
			}
			break;
			
		default:
			// Wrong version
			return (false);
		}
	}

	// Unknown chunk
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		BuildRMLight
//	Object:		Convert the light to the format of the RENDER_MANAGER
//	01-05-14:	ELE - Created
//-----------------------------------------------------------------------------
void	LIGHT::BuildRMLight()
{
	//TODO:Encapsulation par entite LIGHT_SOURCE du RENDER_MANAGER

	switch (_nType)
	{
	case LIGHT::POINT_LIGHT:
		_RMLight.SetType(LIGHT_SOURCE::LIGHTTYPE_OMNI);
		break;

	case LIGHT::SPOT_LIGHT:
		_RMLight.SetType(LIGHT_SOURCE::LIGHTTYPE_SPOT);

		_RMLight.SetSpotInnerAngle(_rHotSpotAngle);
		_RMLight.SetSpotOuterAngle(_rFallOffAngle);
		break;

	case LIGHT::DIRECTIONAL_LIGHT:
		_RMLight.SetType(LIGHT_SOURCE::LIGHTTYPE_DIRECTIONAL);
		break;
	}

	_RMLight.SetDiffuse(_Color.GetRed(), _Color.GetGreen(), _Color.GetBlue());
	_RMLight.SetSpecular(_Color.GetRed(), _Color.GetGreen(), _Color.GetBlue());
	_RMLight.SetAmbient(0, 0, 0);

	_RMLight.SetMaxRange(_rFallOffDistance);

	_RMLight.SetAttenuationFactors(1, 0, 0);
}

//-----------------------------------------------------------------------------
// Name: void ::Write
// Object: Write a chunk in a stream
//01-01-25 FKE Created: 
//-----------------------------------------------------------------------------
bool LIGHT::Write(const CHUNK	& Chunk, 
				  IO_STREAM		& Stream) const
{
	if (Chunk == _ChunkList[0])
	{
		switch (Chunk.GetVersion())
		{
		case 2:
			{
				Stream	<< _nFlags
						<< _nType
						<< _Color
						<< _rHotSpotAngle
						<< _rFallOffAngle
						<< _rHotSpotDistance
						<< _rFallOffDistance
						<< _rMultiplier
						<< _ShadowColor ;

				return (true);
			}
			break;
			
		default:
			// Wrong version
			return (false);
		}
	}

	// Unknown chunk
	return (false);
}

//-----------------------------------------------------------------------------
// Name: LIGHT::SendToDisplay
// Object:	Send a light to the display
//			(before to display a mesh for example)
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void LIGHT::SendToDisplay(const MATRIX & GlobalMatrix)
{
	POINT3D				Pos;
	VECTOR				Direction;
	VECTOR			 	GDir( 0.f , 0.f , - 1.f );

#ifndef __RENDERMANAGER__

	LIGHT_TO_APPLY	   *pLight;

	pLight = DISPLAY::Instance()->CreateLight() ;

	if (!pLight)
	{
		return ;	// Probably allready more than 8 lights...
	}

	switch (_nType)
	{
		case LIGHT::POINT_LIGHT:
			pLight->Omni() ;
			break ;

		case LIGHT::SPOT_LIGHT:
			pLight->Spot() ;
			break ;
	}

	pLight->SetColor(_Color) ;

	pLight->SetAngleFallOff(_rFallOffAngle, MATH::Cos(_rFallOffAngle)) ;
	pLight->SetAngleHotSpot(_rHotSpotAngle, MATH::Cos(_rHotSpotAngle)) ;
	pLight->SetDistFallOff(_rFallOffDistance) ;
	pLight->SetDistHotSpot(_rHotSpotDistance) ;

	pLight->SetMatrix(GlobalMatrix);
	
	GlobalMatrix.Apply(GDir, Direction) ;

	pLight->SetPosition(GlobalMatrix.GetPosition()) ;
	pLight->SetDirection(Direction) ;

#else

	GlobalMatrix.Apply(GDir, Direction);
	Pos = GlobalMatrix.GetPosition();

	_RMLight.SetPosition(Pos.GetX(), Pos.GetY(), Pos.GetZ());
	_RMLight.SetDirection(Direction.GetX(), Direction.GetY(), Direction.GetZ());

	RENDER_MANAGER::Instance()->GetRenderContext()->AddLightSource(&_RMLight);

#endif
}

//-----------------------------------------------------------------------------
//	Name: LIGHT::Display
//	Object: Display a point at the light position if need
//	01-02-14:	RMA - Created
//-----------------------------------------------------------------------------
void LIGHT::Display(DISPLAY_PARAMETERS & DisplayParameters)
{
#ifndef __RENDERMANAGER__
	const POINT3D Center(0.0f, 0.0f, 0.0f);
	const rSize = 2.0;

	if	(DisplayParameters.GetDisplayFlags(VIEWPORT::DISPLAY_LIGHTS))
	{
		DISPLAY::Instance()->SetVertexState(DISPLAY::UNTRANSFORMED);
		DISPLAY::Instance()->SetWorldMatrix(DisplayParameters.GetNode()->GetGlobalMatrix()) ;

		Center.Display(_Color, rSize);

		// The display have to transform all the vertices
		DISPLAY::Instance()->SetVertexState(DISPLAY::UNTRANSFORMED);
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Sort
//	Object:		
//	01-05-14:	ELE - Created
//-----------------------------------------------------------------------------
void LIGHT::Sort(NODE *pNode,
				 DISPLAY_PARAMETERS & DisplayParameters)
{
	_pInfos->SetNode(pNode);
	_pInfos->SetNumObject(-1);

	Display(DisplayParameters);
}

//-----------------------------------------------------------------------------
// Name: LIGHT::GetDesc
// Object:
//00 - 11 - 20 JVA Created:
//-----------------------------------------------------------------------------
DESC* LIGHT::GetDesc()const
{
	return (LIGHT_DESC::Instance()) ;
}

//-----------------------------------------------------------------------------
// Name: JOINT::GetObject
// Object: 
//01-05-11 FKE Created: 
//-----------------------------------------------------------------------------
void	*LIGHT::GetObject()
{
	return this;
}

//-----------------------------------------------------------------------------
// Name: JOINT::ReceiveCommandProc
// Object: 
//01-05-11 FKE Created: 
//-----------------------------------------------------------------------------
bool	LIGHT::ReceiveCommandProc(COMMAND_ID CommandID, PARAMETER_STACK &Stack)
{
	return GetDesc()->ReceiveCommandProc(this, CommandID, Stack);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================

