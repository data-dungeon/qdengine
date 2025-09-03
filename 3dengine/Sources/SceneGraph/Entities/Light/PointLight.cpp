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
//	CLASS:	POINT_LIGHT
//
//	01-07-04:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Light/PointLight)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Frustrum/Frustrum)

#ifdef _DEBUG
	#include "PointLight.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		POINT_LIGHT constructor
//	Object:		
//	01-07-03:	RMA - Created
//-----------------------------------------------------------------------------
POINT_LIGHT::POINT_LIGHT()
:
LIGHT_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_POINT_LIGHT)
{
	_LightSource._eType = LIGHT_POINT;
	_LightSource._rAttenuationPower;
	_LightSource._rInvSquareRadius = 1.0f;
	_BoundingSphere.SetCenter(VECTOR(0.0f, 0.0f, 0.0f));
	_BoundingSphere.SetRadius(1.0f);
}

//-----------------------------------------------------------------------------
//	Name:		~SPOT_LIGHT destructor
//	Object:		
//	01-07-03:	RMA - Created
//-----------------------------------------------------------------------------
POINT_LIGHT::~POINT_LIGHT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
POINT_LIGHT &	POINT_LIGHT::operator=(const POINT_LIGHT &	C)
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
void	POINT_LIGHT::Copy(const POINT_LIGHT &	C)
{
	LIGHT_ABC::Copy(C);

	_BoundingSphere = C._BoundingSphere;
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::Instanciate(COM_INSTANCE &	Instance)
{
	POINT_LIGHT *	pNewLight;

	Instance.SetHandle(POINT_LIGHT::GetGenericCommunicatorDescriptor()->Create());
	pNewLight = static_cast<POINT_LIGHT *>(Instance.GetCommunicator());

	*pNewLight = *this;

	InstanciateNode(pNewLight);
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateRec
//	Object:		
//	06-01-18:	ELE - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::InstanciateRec(COM_INSTANCE &	Instance)
{
	POINT_LIGHT *	pNewLight;

	Instance.SetHandle(POINT_LIGHT::GetGenericCommunicatorDescriptor()->Create());
	pNewLight = static_cast<POINT_LIGHT *>(Instance.GetCommunicator());

	*pNewLight = *this;

	InstanciateNodeRec(pNewLight);
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-07-05:	RMA - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::Display()
{
	QDT_NOT_IMPLEMENTED();
/*
	LIGHT_ABC::Display();

#ifndef _MASTER
	static COLOR_FLOAT	Yellow(1.0f, 1.0f, 1.0f, 0.0f);
	static COLOR_FLOAT	Green(1.0f, 0.0f, 1.0f, 0.0f);

	NODE	*pNode = GetNode();

	// Display a sphere
	static const SPHERE Sphere(VECTOR(0.0f, 0.0f, 0.0f), 5.0f);
	static const float rStep = 50.0f;

	if (GetFlags().IsSet(DONT_LIT_DYNAMIC_OBJECTS)) return;

	if (DISPLAY_MANAGER::Instance()->DisplayLightBoundingVolumes())
	{
		if (_Decay != LIGHT_ABC::NO_DECAY)
		{
			RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(pNode->GetGlobalMatrix());
			_BSphere.Display(_Color, rStep);
		}
	}
	
	if (DISPLAY_MANAGER::Instance()->DisplayLights())
	{
		RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(pNode->GetGlobalMatrix());
		Sphere.Display(_Color, rStep);

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
bool	POINT_LIGHT::TestLighting(const SPHERE &	BSphere) const
{
	return	(BSphere.Intersect(_GlobalBoundingSphere));
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
bool	POINT_LIGHT::TestVisibility(const FRUSTRUM &	Frustum)
{
	_bVisible = _GlobalBoundingSphere.Intersect(Frustum.GetHexahedron());

	return	(_bVisible);
}

//-----------------------------------------------------------------------------
//	Name:		SetIntensity
//	Object:	
//	Note:		Update range	
//	01-07-05:	RMA - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::SetIntensity(float rIntensity)
{
	_LightSource._rIntensity = rIntensity;
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PTLIGHT", 7, CM_ReadChunk7);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PTLIGHT", 8, CM_ReadChunk8);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PTLIGHT", 9, CM_ReadChunk9);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PTLIGHT", 10, CM_ReadChunk10);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PTLIGHT", 11, CM_ReadChunk11);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("PTLIGHT", 11, CM_WriteChunk11);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	POINT_LIGHT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_POINT_LIGHT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	POINT_LIGHT::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("POINT_LIGHT"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_POINT_LIGHT,
											QDT_COMMENT("B935A3CB-C7A3-2148-74D6-48B69172DA2A"),
											sizeof(POINT_LIGHT),
											ComInit,
											QDT_COMMENT("Class for point lights"),
											LIGHT_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::EM_Construct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	POINT_LIGHT	*pPointLight;
	pPointLight = static_cast<POINT_LIGHT *>(pO);

	QDT_NEW_PLACED(pPointLight, POINT_LIGHT)();
	QDT_ASSERT(pPointLight);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::EM_Destruct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	POINT_LIGHT	*pPointLight;
	pPointLight = static_cast<POINT_LIGHT *>(pO);

	QDT_DELETE_PLACED(pPointLight, POINT_LIGHT);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk7
//	Object:		
//	04-01-12:	RMA - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::CM_ReadChunk7(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	UInt32 nDecay;
	POINT_LIGHT	*pPointLight;
	pPointLight = static_cast<POINT_LIGHT *>(pCommunicator);

	Stream >> pPointLight->_NodeInst;

	Stream >> nDecay;
	Stream >> pPointLight->_LightSource._rColorR;
	Stream >> pPointLight->_LightSource._rColorG;
	Stream >> pPointLight->_LightSource._rColorB;
	Stream >> pPointLight->_LightSource._rIntensity;
	Stream >> pPointLight->_LightSource._rAmbient;
	Stream >> pPointLight->_LightSource._rDiffuse;
	Stream >> pPointLight->_LightSource._rSpecular;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk8
//	Object:		
//	06-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::CM_ReadChunk8(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	POINT_LIGHT	*pPointLight;
	Float32 rRadius;
	pPointLight = static_cast<POINT_LIGHT *>(pCommunicator);

	Stream >> pPointLight->_LightSource._rColorR;
	Stream >> pPointLight->_LightSource._rColorG;
	Stream >> pPointLight->_LightSource._rColorB;
	Stream >> pPointLight->_LightSource._rIntensity;
	Stream >> pPointLight->_LightSource._rAmbient;
	Stream >> pPointLight->_LightSource._rDiffuse;
	Stream >> pPointLight->_LightSource._rSpecular;
	Stream >> rRadius;
	
	pPointLight->SetRadius(rRadius);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk9
//	Object:		
//	06-01-17:	RMA - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::CM_ReadChunk9(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	POINT_LIGHT	*pPointLight;
	Float32 rRadius;
	pPointLight = static_cast<POINT_LIGHT *>(pCommunicator);

	Stream >> pPointLight->_NodeInst;

	Stream >> pPointLight->_LightSource._rColorR;
	Stream >> pPointLight->_LightSource._rColorG;
	Stream >> pPointLight->_LightSource._rColorB;
	Stream >> pPointLight->_LightSource._rIntensity;
	Stream >> pPointLight->_LightSource._rAmbient;
	Stream >> pPointLight->_LightSource._rDiffuse;
	Stream >> pPointLight->_LightSource._rSpecular;
	Stream >> pPointLight->_LightSource._rAttenuationPower;
	Stream >> rRadius;
	
	pPointLight->SetRadius(rRadius);
	pPointLight->CastShadow(false);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk10
//	Object:		
//	06-01-17:	RMA - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::CM_ReadChunk10(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	POINT_LIGHT	*pPointLight;
	Float32 rRadius;
	Bool bCastShadow, bSoftShadow;
	pPointLight = static_cast<POINT_LIGHT *>(pCommunicator);

	Stream >> pPointLight->_NodeInst;

	Stream >> pPointLight->_LightSource._rColorR;
	Stream >> pPointLight->_LightSource._rColorG;
	Stream >> pPointLight->_LightSource._rColorB;
	Stream >> pPointLight->_LightSource._rIntensity;
	Stream >> pPointLight->_LightSource._rAmbient;
	Stream >> pPointLight->_LightSource._rDiffuse;
	Stream >> pPointLight->_LightSource._rSpecular;
	Stream >> pPointLight->_LightSource._rAttenuationPower;
	Stream >> rRadius;
	Stream >> bCastShadow;
	Stream >> bSoftShadow;
	
	pPointLight->SetRadius(rRadius);
	pPointLight->CastShadow(bCastShadow);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk11
//	Object:		
//	06-01-17:	RMA - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::CM_ReadChunk11(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	POINT_LIGHT	*pPointLight;
	Float32 rRadius;
	Bool bCastShadow;
	pPointLight = static_cast<POINT_LIGHT *>(pCommunicator);

	Stream >> pPointLight->_LightSource._rColorR;
	Stream >> pPointLight->_LightSource._rColorG;
	Stream >> pPointLight->_LightSource._rColorB;
	Stream >> pPointLight->_LightSource._rIntensity;
	Stream >> pPointLight->_LightSource._rAmbient;
	Stream >> pPointLight->_LightSource._rDiffuse;
	Stream >> pPointLight->_LightSource._rSpecular;
	Stream >> pPointLight->_LightSource._rAttenuationPower;
	Stream >> rRadius;
	Stream >> bCastShadow;
	
	pPointLight->SetRadius(rRadius);
	pPointLight->CastShadow(bCastShadow);
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk11
//	Object:		
//	06-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::CM_WriteChunk11(IO_STREAM &		Stream,
								    COMMUNICATOR *	pCommunicator)
{
	POINT_LIGHT	*pLight;
	pLight = static_cast<POINT_LIGHT *>(pCommunicator);

	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("POINT_LIGHT :");
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
	
	Stream.DebugOutput("    Ambient : ");
	Stream << pLight->_LightSource._rAmbient;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("    Diffuse : ");
	Stream << pLight->_LightSource._rDiffuse;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("    Specular : ");
	Stream << pLight->_LightSource._rSpecular;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Attenuation power : ");
	Stream << pLight->_LightSource._rAttenuationPower;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Radius : ");
	Stream << pLight->_BoundingSphere.GetRadius();
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
void	POINT_LIGHT::UpdateGlobalPos()
{
	NODE *	pNode = GetNode();
	const MATRIX &GlobalMat = pNode->GetGlobalMatrix();

	VECTOR P(VECTOR::ZERO);
	GlobalMat.TransformPosition(P, _LightSource._vPos);

	_GlobalBoundingSphere = _BoundingSphere;
	_GlobalBoundingSphere.Transform(GlobalMat);
}

//-----------------------------------------------------------------------------
//	Name:		SetRadius
//	Object:		
//	06-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::SetRadius(Float32	r)
{
	_BoundingSphere.SetRadius(r);
	_LightSource._rInvSquareRadius = 1.0f / (r * r);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateDepthRenderTarget
//	Object:		
//	06-02-10:	SBE - Created
//-----------------------------------------------------------------------------
void	POINT_LIGHT::UpdateDepthRenderTarget(const VECTOR &	CamPosition, 
											 SCENE_GRAPH * pSceneGraph)
{
	QDT_NOT_IMPLEMENTED();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
