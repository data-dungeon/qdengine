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
//
//	01-07-03:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Light/SpotLight)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Mesh)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderShadow)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Frustrum/Frustrum)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(Renderer/IVertexBuffer)
#include	INCL_KRENDERER(Renderer/IIndexBuffer)
#include	INCL_KRENDERER(Renderer/IRenderTarget)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(ComBasis/ComStack)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SPOT_LIGHT constructor
//	Object:		
//	01-07-03:	RMA - Created
//-----------------------------------------------------------------------------
SPOT_LIGHT::SPOT_LIGHT()
:
LIGHT_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_SPOT_LIGHT)
{
	_LightSource._eType = LIGHT_SPOT;
	_LightSource._rAttenuationPower;
	_LightSource._rInvSquareRadius = 1.0f;
	_BoundingCone.SetTop(VECTOR::ZERO);
	_BoundingCone.SetAxe(VECTOR::NEG_UNIT_Z);
	_BoundingCone.SetRadius(1.0f);
	_fShadowScale = 1.0f;
}

//-----------------------------------------------------------------------------
//	Name:		~SPOT_LIGHT destructor
//	Object:		
//	01-07-03:	RMA - Created
//-----------------------------------------------------------------------------
SPOT_LIGHT::~SPOT_LIGHT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
SPOT_LIGHT &	SPOT_LIGHT::operator=(const SPOT_LIGHT &	C)
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
void	SPOT_LIGHT::Copy(const SPOT_LIGHT &	C)
{
	LIGHT_ABC::Copy(C);

	_BoundingCone = C._BoundingCone;
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::Instanciate(COM_INSTANCE &	Instance)
{
	SPOT_LIGHT *	pNewLight;

	Instance.SetHandle(SPOT_LIGHT::GetGenericCommunicatorDescriptor()->Create());
	pNewLight = static_cast<SPOT_LIGHT *>(Instance.GetCommunicator());

	*pNewLight = *this;

	InstanciateNode(pNewLight);
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateRec
//	Object:		
//	06-01-18:	ELE - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::InstanciateRec(COM_INSTANCE &	Instance)
{
	SPOT_LIGHT *	pNewLight;

	Instance.SetHandle(SPOT_LIGHT::GetGenericCommunicatorDescriptor()->Create());
	pNewLight = static_cast<SPOT_LIGHT *>(Instance.GetCommunicator());

	*pNewLight = *this;

	InstanciateNodeRec(pNewLight);
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-07-06:	RMA - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::Display()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		TestLighting
//	Object:		
//	03-09-09:	RMA - Created
//-----------------------------------------------------------------------------
bool	SPOT_LIGHT::TestLighting(const SPHERE &	BSphere) const
{
	return	(BSphere.Intersect(_GlobalBoundingCone));
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
bool	SPOT_LIGHT::TestVisibility(const FRUSTRUM &	Frustum)
{
	_bVisible = _GlobalBoundingCone.Intersect(Frustum.GetHexahedron());

	return	(_bVisible);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	METHOD_DESCRIPTOR*	pMD;
	pMD	=METHOD_DESCRIPTOR::Allocate("SetColor",	QDT_COMMENT("Set light color"), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rRed"),		QDT_COMMENT("Red component"),	TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(	QDT_COMMENT("rGreen"),		QDT_COMMENT("Green component"), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(	QDT_COMMENT("rBlue"),		QDT_COMMENT("Blue component"),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_SPOTLIGHT_SETCOLOR, pMD, CM_SetColor);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetOuterAngle",	QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rAngle"),			QDT_COMMENT(""),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_SPOTLIGHT_SETOUTERANGLE, pMD, CM_SetOuterAngle);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetInnerAngle",	QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rAngle"),			QDT_COMMENT(""),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_SPOTLIGHT_SETINNERANGLE, pMD, CM_SetInnerAngle);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetIntensity",	QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rIntensity"),		QDT_COMMENT(""),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_SPOTLIGHT_SETINTENSITY, pMD, CM_SetIntensity);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetAmbient",		QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rAmbiant"),		QDT_COMMENT(""),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_SPOTLIGHT_SETAMBIENT, pMD, CM_SetAmbient);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetDiffuse",		QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rDiffuse"),		QDT_COMMENT(""),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_SPOTLIGHT_SETDIFFUSE, pMD, CM_SetDiffuse);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetSpecular",		QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rSpecular"),		QDT_COMMENT(""),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_SPOTLIGHT_SETSPECULAR, pMD, CM_SetSpecular);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetRadius",		QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rAngle"),			QDT_COMMENT(""),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_SPOTLIGHT_SETRADIUS, pMD, CM_SetRadius);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetAttenuationPower",	QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rAttenuationPower"),	QDT_COMMENT(""),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_SPOTLIGHT_SETATTENUATIONPOWER, pMD, CM_SetAttenuationPower);

	pMD	=METHOD_DESCRIPTOR::Allocate("SetShadowScale",	QDT_COMMENT(""),	ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(	QDT_COMMENT("rShadowScale"),	QDT_COMMENT(""),	TYPE_FLOAT::GetDescriptor());
	pCD->AddCommand(	CMD_SPOTLIGHT_SETSHADOWSCALE, pMD, CM_SetShadowScale);

	// Chunk
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SPTLIGHT", 7, CM_ReadChunk7);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SPTLIGHT", 8, CM_ReadChunk8);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SPTLIGHT", 9, CM_ReadChunk9);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SPTLIGHT", 10, CM_ReadChunk10);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SPTLIGHT", 11, CM_ReadChunk11);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SPTLIGHT", 12, CM_ReadChunk12);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SPTLIGHT", 13, CM_ReadChunk13);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("SPTLIGHT", 13, CM_WriteChunk13);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	SPOT_LIGHT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SPOT_LIGHT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	SPOT_LIGHT::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("SPOT_LIGHT"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SPOT_LIGHT,
											QDT_COMMENT("0762E1E2-137E-AB57-0DC8-58805236477F"),
											sizeof(SPOT_LIGHT),
											ComInit,
											QDT_COMMENT("Class for spot lights"),
											LIGHT_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::EM_Construct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	SPOT_LIGHT	*pSpotLight;
	pSpotLight = static_cast<SPOT_LIGHT *>(pO);

	QDT_NEW_PLACED(pSpotLight, SPOT_LIGHT)();
	QDT_ASSERT(pSpotLight);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::EM_Destruct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	SPOT_LIGHT	*pSpotLight;
	pSpotLight = static_cast<SPOT_LIGHT *>(pO);

	QDT_DELETE_PLACED(pSpotLight, SPOT_LIGHT);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk7
//	Object:		
//	04-01-12:	RMA - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::CM_ReadChunk7(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	UInt32 nDecay;
	SPOT_LIGHT	*pLight;
	pLight = static_cast<SPOT_LIGHT *>(pCommunicator);

	Stream >> pLight->_NodeInst;

	Stream >> nDecay;
	Stream >> pLight->_LightSource._rColorR;
	Stream >> pLight->_LightSource._rColorG;
	Stream >> pLight->_LightSource._rColorB;
	Stream >> pLight->_LightSource._rIntensity;
	Stream >> pLight->_LightSource._rAmbient;
	Stream >> pLight->_LightSource._rDiffuse;
	Stream >> pLight->_LightSource._rSpecular;
	Stream >> pLight->_LightSource._rSpotInnerAngle;
	Stream >> pLight->_LightSource._rSpotOuterAngle;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk8
//	Object:		
//	06-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::CM_ReadChunk8(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	Float32 rRadius;
	SPOT_LIGHT	*pLight;
	pLight = static_cast<SPOT_LIGHT *>(pCommunicator);

	Stream >> pLight->_LightSource._rColorR;
	Stream >> pLight->_LightSource._rColorG;
	Stream >> pLight->_LightSource._rColorB;
	Stream >> pLight->_LightSource._rIntensity;
	Stream >> pLight->_LightSource._rAmbient;
	Stream >> pLight->_LightSource._rDiffuse;
	Stream >> pLight->_LightSource._rSpecular;
	Stream >> pLight->_LightSource._rSpotInnerAngle;
	Stream >> pLight->_LightSource._rSpotOuterAngle;
	Stream >> rRadius;

	pLight->SetRadius(rRadius);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk9
//	Object:		
//	06-01-17:	RMA - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::CM_ReadChunk9(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	Float32 rRadius, rOutAngle;
	SPOT_LIGHT	*pLight;
	pLight = static_cast<SPOT_LIGHT *>(pCommunicator);

	Stream >> pLight->_NodeInst;

	Stream >> pLight->_LightSource._rColorR;
	Stream >> pLight->_LightSource._rColorG;
	Stream >> pLight->_LightSource._rColorB;
	Stream >> pLight->_LightSource._rIntensity;
	Stream >> pLight->_LightSource._rAmbient;
	Stream >> pLight->_LightSource._rDiffuse;
	Stream >> pLight->_LightSource._rSpecular;
	Stream >> pLight->_LightSource._rSpotInnerAngle;
	Stream >> rOutAngle;
	Stream >> pLight->_LightSource._rAttenuationPower;
	Stream >> rRadius;

	pLight->SetRadius(rRadius);
	pLight->SetOuterAngle(rOutAngle);	

	pLight->CastShadow(false);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk10
//	Object:		
//	06-01-17:	RMA - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::CM_ReadChunk10(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	Float32 rRadius, rOutAngle;
	Bool bCastShadow, bSoftShadow;
	SPOT_LIGHT	*pLight;
	pLight = static_cast<SPOT_LIGHT *>(pCommunicator);

	Stream >> pLight->_NodeInst;

	Stream >> pLight->_LightSource._rColorR;
	Stream >> pLight->_LightSource._rColorG;
	Stream >> pLight->_LightSource._rColorB;
	Stream >> pLight->_LightSource._rIntensity;
	Stream >> pLight->_LightSource._rAmbient;
	Stream >> pLight->_LightSource._rDiffuse;
	Stream >> pLight->_LightSource._rSpecular;
	Stream >> pLight->_LightSource._rSpotInnerAngle;
	Stream >> rOutAngle;
	Stream >> pLight->_LightSource._rAttenuationPower;
	Stream >> rRadius;
	Stream >> bCastShadow;
	Stream >> bSoftShadow;

	pLight->SetRadius(rRadius);
	pLight->SetOuterAngle(rOutAngle);	

	pLight->CastShadow(bCastShadow);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk11
//	Object:		
//	06-03-06:	RMA - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::CM_ReadChunk11(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	Float32 rRadius, rOutAngle;
	Bool bCastShadow, bSoftShadow;
	SPOT_LIGHT	*pLight;
	pLight = static_cast<SPOT_LIGHT *>(pCommunicator);

	Stream >> pLight->_NodeInst;

	Stream >> pLight->_LightSource._rColorR;
	Stream >> pLight->_LightSource._rColorG;
	Stream >> pLight->_LightSource._rColorB;
	Stream >> pLight->_LightSource._rIntensity;
	Stream >> pLight->_LightSource._rAmbient;
	Stream >> pLight->_LightSource._rDiffuse;
	Stream >> pLight->_LightSource._rSpecular;
	Stream >> pLight->_LightSource._rSpotInnerAngle;
	Stream >> rOutAngle;
	Stream >> pLight->_LightSource._rAttenuationPower;
	Stream >> rRadius;
	Stream >> bCastShadow;
	Stream >> bSoftShadow;
	Stream >> pLight->_LightSource._rBias;
	Stream >> pLight->_LightSource._rShadowR;
	Stream >> pLight->_LightSource._rShadowG;
	Stream >> pLight->_LightSource._rShadowB;

	pLight->SetRadius(rRadius);
	pLight->SetOuterAngle(rOutAngle);	

	pLight->CastShadow(bCastShadow);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk12
//	Object:		
//	06-03-06:	RMA - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::CM_ReadChunk12(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	Float32 rRadius, rOutAngle;
	Bool bCastShadow, bSoftShadow;
	SPOT_LIGHT	*pLight;
	pLight = static_cast<SPOT_LIGHT *>(pCommunicator);

	Stream >> pLight->_NodeInst;

	Stream >> pLight->_LightSource._rColorR;
	Stream >> pLight->_LightSource._rColorG;
	Stream >> pLight->_LightSource._rColorB;
	Stream >> pLight->_LightSource._rIntensity;
	Stream >> pLight->_LightSource._rAmbient;
	Stream >> pLight->_LightSource._rDiffuse;
	Stream >> pLight->_LightSource._rSpecular;
	Stream >> pLight->_LightSource._rSpotInnerAngle;
	Stream >> rOutAngle;
	Stream >> pLight->_LightSource._rAttenuationPower;
	Stream >> rRadius;
	Stream >> bCastShadow;
	Stream >> bSoftShadow;
	Stream >> pLight->_LightSource._rBias;
	Stream >> pLight->_LightSource._rShadowR;
	Stream >> pLight->_LightSource._rShadowG;
	Stream >> pLight->_LightSource._rShadowB;
	Stream >> pLight->_fShadowScale;

	pLight->SetRadius(rRadius);
	pLight->SetOuterAngle(rOutAngle);	

	pLight->CastShadow(bCastShadow);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk13
//	Object:		
//	06-03-06:	RMA - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::CM_ReadChunk13(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	Float32 rRadius, rOutAngle;
	Bool bCastShadow;
	UInt32 nShadowBlurMode;
	SPOT_LIGHT	*pLight;
	pLight = static_cast<SPOT_LIGHT *>(pCommunicator);

	Stream >> pLight->_LightSource._rColorR;
	Stream >> pLight->_LightSource._rColorG;
	Stream >> pLight->_LightSource._rColorB;
	Stream >> pLight->_LightSource._rIntensity;
	Stream >> pLight->_LightSource._rAmbient;
	Stream >> pLight->_LightSource._rDiffuse;
	Stream >> pLight->_LightSource._rSpecular;
	Stream >> pLight->_LightSource._rSpotInnerAngle;
	Stream >> rOutAngle;
	Stream >> pLight->_LightSource._rAttenuationPower;
	Stream >> rRadius;
	Stream >> bCastShadow;
	Stream >> pLight->_LightSource._rBias;
	Stream >> pLight->_LightSource._rShadowR;
	Stream >> pLight->_LightSource._rShadowG;
	Stream >> pLight->_LightSource._rShadowB;
	Stream >> pLight->_fShadowScale;
	Stream >> nShadowBlurMode;
	Stream >> pLight->_LightSource._rShadowBlurRadius;

	pLight->SetRadius(rRadius);
	pLight->SetOuterAngle(rOutAngle);	

	pLight->SetShadowBlurMode((SHADOW_BLUR_MODE)nShadowBlurMode);
	pLight->CastShadow(bCastShadow);
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk13
//	Object:		
//	03-03-24:	ELE - Created
//  06-03-06:   RMA - Modified : Added shadow bias and color
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::CM_WriteChunk13(IO_STREAM &		Stream,
								    COMMUNICATOR *	pCommunicator)
{
	UInt32 nShadowBlurMode;;
	SPOT_LIGHT	*pLight;
	pLight = static_cast<SPOT_LIGHT *>(pCommunicator);

	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("SPOT_LIGHT :");
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

	Stream.DebugOutput("    Inner angle : ");
	Stream << pLight->_LightSource._rSpotInnerAngle;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("    Outer angle : ");
	Stream << pLight->_LightSource._rSpotOuterAngle;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Attenuation power : ");
	Stream << pLight->_LightSource._rAttenuationPower;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Radius : ");
	Stream << pLight->GetRadius();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    CastShadow : ");
	Stream << pLight->GetCastShadow();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Shadow bias : ");
	Stream << pLight->_LightSource._rBias;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Shadow color R : ");
	Stream << pLight->_LightSource._rShadowR;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("    Shadow color G : ");
	Stream << pLight->_LightSource._rShadowG;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Shadow color B : ");
	Stream << pLight->_LightSource._rShadowB;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Shadow scale : ");
	Stream << pLight->_fShadowScale;
	Stream.DebugOutput("\r\n");

	nShadowBlurMode = pLight->_LightSource._nShadowBlurMode;
	Stream.DebugOutput("    Shadow blur mode : ");
	Stream << nShadowBlurMode;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Shadow blur radius : ");
	Stream << pLight->_LightSource._rShadowBlurRadius;
	Stream.DebugOutput("\r\n");
}

//-----------------------------------------------------------------------------
//	Name:		UpdateGlobalPos
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::UpdateGlobalPos()
{
	NODE *	pNode = GetNode();
	const MATRIX &GlobalMat = pNode->GetGlobalMatrix();

	VECTOR V(VECTOR::UNIT_Z);
	VECTOR P(VECTOR::ZERO);
	GlobalMat.TransformDirection(V, _LightSource._vDir);
	GlobalMat.TransformPosition(P, _LightSource._vPos);

	_GlobalBoundingCone = _BoundingCone;
	_GlobalBoundingCone.Transform(GlobalMat);

	// Additional normalization because maya set a scale in GlobalMat
	VECTOR vAxe = _GlobalBoundingCone.GetAxe();
	vAxe.Normalize();
	_GlobalBoundingCone.SetAxe(vAxe);
}

//-----------------------------------------------------------------------------
//	Name:		SetRadius
//	Object:		
//	01-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::SetRadius(Float32 r)
{
	_BoundingCone.SetRadius(r);
	_LightSource._rInvSquareRadius = 1.0f / (r * r);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateDepthRenderTarget
//	Object:		
//	06-02-10:	SBE - Created
//-----------------------------------------------------------------------------
void	SPOT_LIGHT::UpdateDepthRenderTarget(const VECTOR &	CamPosition,
											SCENE_GRAPH * pSceneGraph)
{
    S_SHADOW_ATTRIBUTES * pSA = _LightSource._pShadowAttributes;
	QDT_ASSERT(pSA != NULL);

	DISPLAY_MANAGER * pDM = DISPLAY_MANAGER::Instance();
	Bool bUseHardWareSM;

	// todo choisir la SHADOW render target en fonction de la distance avec position de la camera

	SHADOW_RENDERTARGETS * pSRT;
	if (GetType() == LIGHT_SPOT_HARDWARE_SHADOW)
	{
		pSRT = pDM->GetHardWareShadowRenderTargets();
	}
	else
	{
		pSRT = pDM->GetBasicShadowRenderTargets();
	}

	QDT_ASSERT(pSRT);
	#ifndef PLATFORM_PS3
		QDT_ASSERT(pSRT->pRenderTarget);
	#endif
	QDT_ASSERT(pSRT->pDepthBuffer);

	pSRT->pLight = this;
	pSA->_pAssociatedDepthRenderTargets = pSRT;

	// Backup Context
	I_RENDER_TARGET * pCurRT = RENDER_CONTEXT::GetCurrentRenderTarget();
	I_RENDER_TARGET * pCurDB = RENDER_CONTEXT::GetCurrentDepthBuffer();
	CUBEMAP_FACE CurFace = RENDER_CONTEXT::GetCurrentRenderTargetCubeFace();

	// Init Render Targets & Shaders
	SHADER_SHADOW * pShadow = (SHADER_SHADOW *)pDM->GetShadowShader().GetCommunicator();
	if (pSRT->pDepthBuffer->GetPixelFormat() == PF_D24S8_DEPTHSTENCIL)
	{
		RENDER_CONTEXT::SetCurrentRenderTarget(pSRT->pRenderTarget);
		bUseHardWareSM = false;	
		pShadow->SetShadowType(SHADER_SHADOW::ST_SPOT);
	}
	else
	{
		#ifndef PLATFORM_PS3
			RENDER_CONTEXT::SetCurrentRenderTarget(pSRT->pRenderTarget);
		#else
			//RENDER_CONTEXT::SetCurrentRenderTarget(RENDER_CONTEXT::GetFrontBuffer());
			//RENDER_CONTEXT::SetCurrentDepthBuffer(RENDER_CONTEXT::GetDepthBuffer());
		#endif
		bUseHardWareSM = true;
		pShadow->SetShadowType(SHADER_SHADOW::ST_HARDWARE_SPOT);
	}

	RENDER_CONTEXT::SetCurrentDepthBuffer(pSRT->pDepthBuffer);

	Int32 iOldViewportX;
	Int32 iOldViewportY;
	Int32 iOldViewportWidth;
	Int32 iOldViewportHeight;

	RENDER_CONTEXT::GetViewport(iOldViewportX, iOldViewportY, iOldViewportWidth, iOldViewportHeight);
	RENDER_CONTEXT::SetViewport(0, 0, pSRT->pDepthBuffer->GetWidth(), pSRT->pDepthBuffer->GetHeight());

	if (bUseHardWareSM)
	{
		RENDER_CONTEXT::Clear(CF_CLEAR_COLOR | CF_CLEAR_ZBUFFER, 0, 0, 0, 0);
	}
	else
	{
		RENDER_CONTEXT::Clear(CF_CLEAR_COLOR | CF_CLEAR_ZBUFFER, 255, 255, 255, 0);
	}

	// Copy precomputed depth render target with static meshes
	if (pSA->_pDepthRenderTargetOfStaticMeshes != NULL)
	{
		// TODO !!
		if (pSRT->pRenderTarget != NULL)
		{
			QDT_ASSERT(pSA->_pDepthRenderTargetOfStaticMeshes->GetPixelFormat() == PF_R32F);

			//RENDER_CONTEXT::BlitToRenderTarget(_pDepthRenderTargetOfStaticMeshes);
		}
		else
		{
			//RENDER_CONTEXT::BlitToDepthBuffer(_pDepthRenderTargetOfStaticMeshes);
		}
	}
	
	// Set Light Parameters
	VECTOR vLightEyePt(_GlobalBoundingCone.GetTop());
	VECTOR vLightLookatPt(vLightEyePt + _GlobalBoundingCone.GetAxe());
	VECTOR vLightUpVec   ( 0.0f, 1.0f, 0.0f );

	GetNode()->GetGlobalMatrix().TransformDirection(vLightUpVec);
	vLightUpVec.Normalize();

	pSA->_mView.LookAt(vLightEyePt, vLightLookatPt, vLightUpVec);

	/*switch (RENDER_CONTEXT::GetRenderContextType())
	{
	default:
	case RENDER_CONTEXT::RCT_UNDEFINED:
		QDT_FAIL();

	case RENDER_CONTEXT::RCT_DIRECT3D9:*/
		pSA->_mProj.SetPerspectiveFov_D3D(GetOuterAngle() * 2.0f * _fShadowScale, 1.0f, 10.0f, _BoundingCone.GetRadius());
		/*break;

	case RENDER_CONTEXT::RCT_OPENGL:
	case RENDER_CONTEXT::RCT_OPENGL_FOR_MAYA:
	case RENDER_CONTEXT::RCT_PSGL:
		pSA->_mProj.SetPerspectiveFov_OGL(GetOuterAngle() * 2.0f * _fShadowScale, 1.0f, 10.0f, _BoundingCone.GetRadius());
		/*break;
	}	*/

	pDM->SetViewMatrix(pSA->_mView);
	pDM->SetProjMatrix(pSA->_mProj);
	
	pDM->EnableShaderReplacement(true);
	pDM->SetReplacementShader(pDM->GetShadowShader());

	pSceneGraph->Display(_GlobalBoundingCone, MESH::DISPLAY_FLAG_CAST_SHADOWS);

	// Restore color write
	if (bUseHardWareSM)
	{
		RENDER_CONTEXT::EnableColorWrite(true, true, true, true);
	}

	RENDER_CONTEXT::SetCurrentRenderTarget(RENDER_CONTEXT::GetFrontBuffer());
	RENDER_CONTEXT::SetCurrentDepthBuffer(RENDER_CONTEXT::GetDepthBuffer());

	// Restore Renderer Context
	RENDER_CONTEXT::SetCurrentRenderTarget(pCurRT, 0, CurFace);
	RENDER_CONTEXT::SetCurrentDepthBuffer(pCurDB);
	RENDER_CONTEXT::SetViewport(0, 0, pCurRT->GetWidth(), pCurRT->GetHeight());

	// Restore display manager parameter
	CAMERA * pCam = pDM->GetActiveCamera();
	pDM->SetViewMatrix(pCam->GetViewMatrix());
	pDM->SetProjMatrix(pCam->GetProjectionMatrix());
	pDM->EnableShaderReplacement(false);

	RENDER_CONTEXT::SetViewport(iOldViewportX, iOldViewportY, iOldViewportWidth, iOldViewportHeight);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetColor
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int SPOT_LIGHT::CM_SetColor(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_SPOTLIGHT_SETCOLOR);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	SPOT_LIGHT* pThis =static_cast<SPOT_LIGHT*>(CI.GetCommunicator());

	Float32	rRed, rGreen, rBlue;
	Stack.GetParameter(0, rRed);
	Stack.GetParameter(1, rGreen);
	Stack.GetParameter(2, rBlue);

	pThis->SetColor(rRed, rGreen, rBlue);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetOuterAngle
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int SPOT_LIGHT::CM_SetOuterAngle(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_SPOTLIGHT_SETOUTERANGLE);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	SPOT_LIGHT* pThis =static_cast<SPOT_LIGHT*>(CI.GetCommunicator());

	Float32	rAngle;
	Stack.GetParameter(0, rAngle);

	pThis->SetOuterAngle(rAngle);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetInnerAngle
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int SPOT_LIGHT::CM_SetInnerAngle(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_SPOTLIGHT_SETINNERANGLE);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	SPOT_LIGHT* pThis =static_cast<SPOT_LIGHT*>(CI.GetCommunicator());

	Float32	rAngle;
	Stack.GetParameter(0, rAngle);

	pThis->SetInnerAngle(rAngle);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetIntensity
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int SPOT_LIGHT::CM_SetIntensity(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_SPOTLIGHT_SETINTENSITY);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	SPOT_LIGHT* pThis =static_cast<SPOT_LIGHT*>(CI.GetCommunicator());

	Float32	rIntensity;
	Stack.GetParameter(0, rIntensity);

	pThis->SetIntensity(rIntensity);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetAmbient
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int SPOT_LIGHT::CM_SetAmbient(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_SPOTLIGHT_SETAMBIENT);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	SPOT_LIGHT* pThis =static_cast<SPOT_LIGHT*>(CI.GetCommunicator());

	Float32	rAmbient;
	Stack.GetParameter(0, rAmbient);

	pThis->SetAmbient(rAmbient);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetDiffuse
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int SPOT_LIGHT::CM_SetDiffuse(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_SPOTLIGHT_SETDIFFUSE);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	SPOT_LIGHT* pThis =static_cast<SPOT_LIGHT*>(CI.GetCommunicator());

	Float32	rDiffuse;
	Stack.GetParameter(0, rDiffuse);

	pThis->SetDiffuse(rDiffuse);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetSpecular
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int SPOT_LIGHT::CM_SetSpecular(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_SPOTLIGHT_SETSPECULAR);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	SPOT_LIGHT* pThis =static_cast<SPOT_LIGHT*>(CI.GetCommunicator());

	Float32	rSpecular;
	Stack.GetParameter(0, rSpecular);

	pThis->SetSpecular(rSpecular);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetRadius
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int SPOT_LIGHT::CM_SetRadius(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_SPOTLIGHT_SETRADIUS);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	SPOT_LIGHT* pThis =static_cast<SPOT_LIGHT*>(CI.GetCommunicator());

	Float32	rRadius;
	Stack.GetParameter(0, rRadius);

	pThis->SetRadius(rRadius);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetAttenuationPower
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int SPOT_LIGHT::CM_SetAttenuationPower(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_SPOTLIGHT_SETATTENUATIONPOWER);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	SPOT_LIGHT* pThis =static_cast<SPOT_LIGHT*>(CI.GetCommunicator());

	Float32	rAttenuationPower;
	Stack.GetParameter(0, rAttenuationPower);

	pThis->SetAttenuationPower(rAttenuationPower);

	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetShadowScale
//	Object:		
//	06-03-31:	BFA - Created
//-----------------------------------------------------------------------------
int SPOT_LIGHT::CM_SetShadowScale(COM_PARAM P)
{
	COM_STACK		Stack(P, GetGenericCommunicatorDescriptor(), CMD_SPOTLIGHT_SETSHADOWSCALE);
	COM_INSTANCE	CI;
	Stack.GetThis(CI);
	SPOT_LIGHT* pThis =static_cast<SPOT_LIGHT*>(CI.GetCommunicator());

	Float32	rShadowScale;
	Stack.GetParameter(0, rShadowScale);

	pThis->SetShadowScale(rShadowScale);

	return (COM_STACK::NO_RET_VALUE);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
