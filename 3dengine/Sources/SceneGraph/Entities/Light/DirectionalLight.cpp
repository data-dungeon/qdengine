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
//	CLASS:	DIRECTIONAL_LIGHT
//
//	01-07-04:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Light/DirectionalLight)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		DIRECTIONAL_LIGHT constructor
//	Object:		
//	01-07-03:	RMA - Created
//-----------------------------------------------------------------------------
DIRECTIONAL_LIGHT::DIRECTIONAL_LIGHT()
:
LIGHT_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_DIRECTIONAL_LIGHT)
{
	_LightSource._eType = LIGHT_DIRECTIONAL;
	_LightSource._rInvSquareRadius = 1.0f;
	_LightSource._rAmbient = 0.0f;
	_LightSource._rDiffuse = 1.0f;
	_LightSource._rSpecular = 1.0f;
}

//-----------------------------------------------------------------------------
//	Name:		~DIRECTIONAL_LIGHT destructor
//	Object:		
//	01-07-03:	RMA - Created
//-----------------------------------------------------------------------------
DIRECTIONAL_LIGHT::~DIRECTIONAL_LIGHT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
DIRECTIONAL_LIGHT &	DIRECTIONAL_LIGHT::operator=(const DIRECTIONAL_LIGHT &	C)
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
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::Copy(const DIRECTIONAL_LIGHT &	C)
{
	LIGHT_ABC::Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::Instanciate(COM_INSTANCE &	Instance)
{
	DIRECTIONAL_LIGHT *	pNewLight;

	Instance.SetHandle(DIRECTIONAL_LIGHT::GetGenericCommunicatorDescriptor()->Create());
	pNewLight = static_cast<DIRECTIONAL_LIGHT *>(Instance.GetCommunicator());

	*pNewLight = *this;

	InstanciateNode(pNewLight);
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateRec
//	Object:		
//	06-01-18:	ELE - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::InstanciateRec(COM_INSTANCE &	Instance)
{
	DIRECTIONAL_LIGHT *	pNewLight;

	Instance.SetHandle(DIRECTIONAL_LIGHT::GetGenericCommunicatorDescriptor()->Create());
	pNewLight = static_cast<DIRECTIONAL_LIGHT *>(Instance.GetCommunicator());

	*pNewLight = *this;

	InstanciateNodeRec(pNewLight);
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-07-06:	RMA - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::Display()
{
	QDT_NOT_IMPLEMENTED();
/*
	LIGHT_ABC::Display();

#ifndef _MASTER
	static COLOR_FLOAT	Yellow(1.0f, 1.0f, 1.0f, 0.0f);
	static COLOR_FLOAT	Green(1.0f, 0.0f, 1.0f, 0.0f);

	NODE	*pNode = GetNode();

	// Display three arrows
	static const SEGMENT Segment1(VECTOR( 1.5f, -1.5f,   0.0f), VECTOR( 1.5f, -1.5f, -10.0f));
	static const SEGMENT Segment2(VECTOR(-1.5f, -1.5f,   0.0f), VECTOR(-1.5f, -1.5f, -10.0f));
	static const SEGMENT Segment3(VECTOR( 0.0f,  2.0f,   0.0f), VECTOR( 0.0f,  2.0f, -10.0f));
	static const SEGMENT Segment4(VECTOR( 1.5f, -1.5f, -10.0f), VECTOR( 5.0f, -5.0f,  -7.0f));
	static const SEGMENT Segment5(VECTOR(-1.5f, -1.5f, -10.0f), VECTOR(-5.0f, -5.0f,  -7.0f));
	static const SEGMENT Segment6(VECTOR( 0.0f,  2.0f, -10.0f), VECTOR( 0.0f,  5.5f,  -7.0f));

	if (DISPLAY_MANAGER::Instance()->DisplayLights() && !GetFlags().IsSet(DONT_LIT_DYNAMIC_OBJECTS))
	{
		RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(pNode->GetGlobalMatrix());

		Segment1.Display(_Color);
		Segment2.Display(_Color);
		Segment3.Display(_Color);
		Segment4.Display(_Color);
		Segment5.Display(_Color);
		Segment6.Display(_Color);

		if	(_rFocalAngle > 0.0f)
		{
			FRUSTRUM	Frustum;

			RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(&MATRIX::IDENTITY);

			Frustum.Set(_rFocalAngle, _rNear, _rFar, _rAspectRatio);
			Frustum.Transform(pNode->GetGlobalMatrix());
			Frustum.Display(Yellow);
		}
	}
	
	if (GetFlags().IsSet(MARKER))
	{
		DisplayMarker();
	}
#endif*/
}

//-----------------------------------------------------------------------------
//	Name:		TestLighting
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
bool	DIRECTIONAL_LIGHT::TestLighting(const SPHERE &	BSphere) const
{
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
bool	DIRECTIONAL_LIGHT::TestVisibility(const FRUSTRUM &	Frustum)
{
	_bVisible = true;
	return	(_bVisible);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("DIRLIGHT", 7, CM_ReadChunk7);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("DIRLIGHT", 8, CM_ReadChunk8);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("DIRLIGHT", 9, CM_ReadChunk9);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("DIRLIGHT", 9, CM_WriteChunk9);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	DIRECTIONAL_LIGHT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DIRECTIONAL_LIGHT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	DIRECTIONAL_LIGHT::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("DIRECTIONAL_LIGHT"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DIRECTIONAL_LIGHT,
											QDT_COMMENT("E4816146-5112-BF5A-6B7E-721F6D19BF96"),
											sizeof(DIRECTIONAL_LIGHT),
											ComInit,
											QDT_COMMENT("Class for directional lights"),
											LIGHT_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::EM_Construct(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	DIRECTIONAL_LIGHT	*pDirectionalLight;
	pDirectionalLight = static_cast<DIRECTIONAL_LIGHT *>(pO);

	QDT_NEW_PLACED(pDirectionalLight, DIRECTIONAL_LIGHT)();
	QDT_ASSERT(pDirectionalLight);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::EM_Destruct(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	DIRECTIONAL_LIGHT	*pDirectionalLight;
	pDirectionalLight = static_cast<DIRECTIONAL_LIGHT *>(pO);

	QDT_DELETE_PLACED(pDirectionalLight, DIRECTIONAL_LIGHT);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk7
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::CM_ReadChunk7(IO_STREAM & Stream, 
										 COMMUNICATOR * pCommunicator)
{
	DIRECTIONAL_LIGHT	*pLight;
	pLight = static_cast<DIRECTIONAL_LIGHT *>(pCommunicator);

	Stream >> pLight->_NodeInst;

	Stream >> pLight->_LightSource._rColorR;
	Stream >> pLight->_LightSource._rColorG;
	Stream >> pLight->_LightSource._rColorB;
	Stream >> pLight->_LightSource._rIntensity;

	pLight->CastShadow(false);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk8
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::CM_ReadChunk8(IO_STREAM & Stream, 
										 COMMUNICATOR * pCommunicator)
{
	DIRECTIONAL_LIGHT	*pLight;
	pLight = static_cast<DIRECTIONAL_LIGHT *>(pCommunicator);
	Bool bCastShadow, bSoftShadow;

	Stream >> pLight->_NodeInst;

	Stream >> pLight->_LightSource._rColorR;
	Stream >> pLight->_LightSource._rColorG;
	Stream >> pLight->_LightSource._rColorB;
	Stream >> pLight->_LightSource._rIntensity;
	Stream >> bCastShadow;
	Stream >> bSoftShadow;

	pLight->CastShadow(bCastShadow);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk9
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::CM_ReadChunk9(IO_STREAM & Stream, 
										 COMMUNICATOR * pCommunicator)
{
	DIRECTIONAL_LIGHT	*pLight;
	pLight = static_cast<DIRECTIONAL_LIGHT *>(pCommunicator);
	Bool bCastShadow;

	Stream >> pLight->_LightSource._rColorR;
	Stream >> pLight->_LightSource._rColorG;
	Stream >> pLight->_LightSource._rColorB;
	Stream >> pLight->_LightSource._rIntensity;
	Stream >> bCastShadow;

	pLight->CastShadow(bCastShadow);
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk9
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::CM_WriteChunk9(IO_STREAM & Stream, 
										  COMMUNICATOR * pCommunicator)
{
	DIRECTIONAL_LIGHT	*pLight;
	pLight = static_cast<DIRECTIONAL_LIGHT *>(pCommunicator);

	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("DIRECTIONAL_LIGHT :");
	Stream.DebugOutput(pLight->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    ColorR : ");
	Stream << pLight->_LightSource._rColorR;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    ColorG : ");
	Stream << pLight->_LightSource._rColorG;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("    ColorB : ");
	Stream << pLight->_LightSource._rColorB;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("    Intensity : ");
	Stream << pLight->_LightSource._rIntensity;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    CastShadow : ");
	Stream << pLight->GetCastShadow();
	Stream.DebugOutput("\r\n");
}

//-----------------------------------------------------------------------------
//	Name:		UpdateGlobalPos
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::UpdateGlobalPos()
{
	NODE *	pNode = GetNode();
	const MATRIX &GlobalMat = pNode->GetGlobalMatrix();

	VECTOR V(VECTOR::UNIT_Z);
	GlobalMat.TransformDirection(V, _LightSource._vDir);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateDepthRenderTarget
//	Object:		
//	06-02-10:	SBE - Created
//-----------------------------------------------------------------------------
void	DIRECTIONAL_LIGHT::UpdateDepthRenderTarget(const VECTOR &	CamPosition, 
												   SCENE_GRAPH * pSceneGraph)
{
	QDT_NOT_IMPLEMENTED();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
