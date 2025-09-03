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
//	CLASS:	IHM_ELEMENT_POLY_ABC
//
//	01-09-27:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderABC)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementPoly/IHM_ElementPolyABC)
#include	INCL_3DENGINE(Interface2D3D/InterfaceManager)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Interface)
#include	INCL_3DENGINE(Display/Viewport/Viewport)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KCOM(ComBasis/GlobalVariable)
#include	INCL_KCOM(ComBasis/ComStack)
#include	INCL_KCOM(ComBasis/SingletonDescriptor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(ComplexType/TypePoint2D)
#include	INCL_KCORE(Time/Time_ABC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_POLY_ABC constructor
//	Object:		
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_POLY_ABC::IHM_ELEMENT_POLY_ABC(CLASS_DESCRIPTOR * const pCD)
:
IHM_ELEMENT_ABC(pCD),
_DatabaseMaterialInst	(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
//_pGeometry				(NULL),
_rRotationSpeed			(0.0f),
_eFadeMode				(NO_FADE),
_rFadeTimeMS			(-1.0f),
_rElapsedTimeMS			(-1.0f),
_bHasChanged			(false),
_nFadeMinAlphaValue		(0),
_pLastViewport			(NULL)
{
	_pPoints[0].SetXY(0.0f, 0.0f);
	_pPoints[1].SetXY(100.0f, 0.0f);
	_pPoints[2].SetXY(0.0f, 100.0f);
	_pPoints[3].SetXY(100.0f, 100.0f);

	_pUVs[0].SetXY(0,0);
	_pUVs[1].SetXY(1,0);
	_pUVs[2].SetXY(0,1);
	_pUVs[3].SetXY(1,1);

	_Colors[0] = COLOR::WHITE;
	_Colors[1] = COLOR::WHITE;
	_Colors[2] = COLOR::WHITE;
	_Colors[3] = COLOR::WHITE;
}

//-----------------------------------------------------------------------------
//	Name:		~IHM_ELEMENT_POLY_ABC destructor
//	Object:		
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_POLY_ABC::~IHM_ELEMENT_POLY_ABC()
{
//	MODULE_ABC::RemoveClassDescriptorLoop(this);
}
/*
//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-12-05:	RMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::Display(DISPLAY_PARAMETERS &	DisplayParameters,
									  float					rSizeX,
									  float					rSizeY)
{
	//	if status of object has changed for a loop
	if	(HasChanged())
	{
		Loop();
	}

	if (IsVisible())
	{
		MATERIAL_ABC   *pMaterial = NULL;
		VECTOR		Center(VECTOR::ZERO);

		InitGeometry();

		if	(_DatabaseMaterialInst.HasHandle())
		{
			pMaterial = static_cast<MATERIAL_ABC *>(_DatabaseMaterialInst.GetCommunicator());
			_pUntransformedGeometry->SetMaterialInst(_DatabaseMaterialInst, pMaterial->GetShader());
		}
		else
		{
			_pUntransformedGeometry->SetMaterialInst(_DatabaseMaterialInst, NULL);
		}

		SetVerticesRelativeToWorld(_pPoints, rSizeX, rSizeY);

		_pUntransformedGeometry->Synchronize();

		QDT_VECTOR<const MATRIX*>	vSkinningMatrices;

		_pUntransformedGeometry->SendToDisplayList(&(DisplayParameters.GetNode()->GetGlobalMatrix()),
												   COM_INSTANCE(DisplayParameters.GetNode()),
												   Center,
												   false,
												   false,	//  pMaterial && pMaterial->GetFlags().IsSet(MATERIAL::MAT_DOUBLE_SIDED),
												   false,
												   false,
												   true,
												   vSkinningMatrices,
												   NULL);
	}
}
*/
//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::DisplayElement(VIEWPORT * pViewport)
{

	//	if status of object has changed for a loop
	//if	(HasChanged())
	{
		Loop();
	}

	if	(IsVisible())
	{
		RENDER_CONTEXT::EnableDepthTest(false);

		DISPLAY_MANAGER::Instance()->EnableSkinning(false);
		DISPLAY_MANAGER::Instance()->SetWorldMatrix(MATRIX::IDENTITY);
		DISPLAY_MANAGER::Instance()->SetViewMatrix(MATRIX::IDENTITY);
		DISPLAY_MANAGER::Instance()->SetProjMatrix(MATRIX::IDENTITY);

		COM_INSTANCE MaterialCI = GetMaterialInst();
		SHADER_ABC* pMaterial = (SHADER_ABC *)MaterialCI.GetCommunicator();

		QDT_ASSERT(pMaterial->GetNbrPasses() == 1);

		pMaterial->SetPass(0, VECTOR(0,0,0));

		POINT2D pointList[4];

		pointList[0].SetX((_pPoints[0].GetX()*0.02)-1);
		pointList[0].SetY(-((_pPoints[0].GetY()*0.02)-1));
		pointList[1].SetX((_pPoints[1].GetX()*0.02)-1);
		pointList[1].SetY(-((_pPoints[1].GetY()*0.02)-1));
		pointList[2].SetX((_pPoints[2].GetX()*0.02)-1);
		pointList[2].SetY(-((_pPoints[2].GetY()*0.02)-1));
		pointList[3].SetX((_pPoints[3].GetX()*0.02)-1);
		pointList[3].SetY(-((_pPoints[3].GetY()*0.02)-1));

		DISPLAY_MANAGER::Instance()->PushQuad(	pointList,
												_pUVs,
												_Colors);

/*		InitGeometry();

		bool				bWired = false;
		RENDER_CONTEXT		*pContext = RENDER_MANAGER::Instance()->GetRenderContext();

		bool							bAlphaTest = pContext->IsAlphaTestEnabled();
		RENDER_CONTEXT::CULL_MODE		CullMode = pContext->GetCullMode();						

		pContext->SetCullMode(RENDER_CONTEXT::CULLMODE_NONE);
		pContext->EnableDepthWrite(false);
		pContext->EnableAlphaTest(false);

		bWired = pContext->IsWireFrameEnabled();
		pContext->EnableWireFrame(false);

		if	(_Flags.IsSet(RESIZE_ON_169))
		{
			if	(RENDER_MANAGER::Instance()->GetRenderContext()->GetMonitorAspect() > 1.5f)
			{
				if	(_Flags.IsSet(STATE_169) == false)
				{	
					_Flags.SetFlags(STATE_169, STATE_169);
					_bVerticesToReset = true;
				}
			}
			else
			{
				if	(_Flags.IsSet(STATE_169))
				{	
					_Flags.SetFlags(STATE_169, 0);
					_bVerticesToReset = true;
				}
			}
		}
		
		if	((_bVerticesToReset) || (pViewport != _pLastViewport))
		{
			SetVerticesRelativeToScreen(pViewport);
			_bVerticesToReset = false;
			_pLastViewport = pViewport;
		}

		if	(_PolyFlags.IsSet(TO_SYNC))
		{
			_pGeometry->Synchronize();
			_PolyFlags.Unset(TO_SYNC);
		}

		SHADER_ABC * pShader = NULL;

		if	(_DatabaseMaterialInst.HasHandle())
		{
			pShader = static_cast<MATERIAL_ABC *>(_DatabaseMaterialInst.GetCommunicator())->GetShader();
		}

		pContext->GetRenderPipeline()->DisplayContainer(*_pGeometry, MATRIX::IDENTITY);

		pContext->SetCullMode(CullMode);
		pContext->EnableDepthWrite(true);
		pContext->EnableAlphaTest(bAlphaTest);

		if	(_bWired == false)
		{
			pContext->EnableWireFrame(bWired);
		} */
		
		DISPLAY_MANAGER::Instance()->FlushQuads();
		RENDER_CONTEXT::EnableDepthTest(true);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetUnder2DInterface
//	Object:		
//	01-10-22:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::SetUnder2DInterface(bool	bUnder2DInterface)
{
	_PolyFlags.Set( UNDER_2D_INTERFACE, bUnder2DInterface );
}

//-----------------------------------------------------------------------------
//	Name:		IsUnder2DInterface
//	Object:		
//	01-10-22:	ELE - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_POLY_ABC::IsUnder2DInterface() const
{
	return ( _PolyFlags.IsSet( UNDER_2D_INTERFACE ) );
}

//-----------------------------------------------------------------------------
//	Name:		SetMaterialInst
//	Object:		
//	02-04-15:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::SetMaterialInst(const COM_INSTANCE & MaterialInst)
{
	QDT_ASSERT((MaterialInst.HasHandle() == false ) || 
			   (MaterialInst.GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_COLOR_PER_VERTEX));

	_DatabaseMaterialInst = MaterialInst;
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterialInst
//	Object:		
//	02-10-17:	GGO - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	IHM_ELEMENT_POLY_ABC::GetMaterialInst() const
{
	return ( _DatabaseMaterialInst );
}

//-----------------------------------------------------------------------------
//	Name:		RelativeToScreen
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
VECTOR		IHM_ELEMENT_POLY_ABC::RelativeToScreen(VIEWPORT *		pViewport,
												   const POINT2D &	Point,
												   float			rZ) const
{
	int				W, H, VX, VY, VW, VH;
	VECTOR			POut;
	IHM_INTERFACE *	pInterface = NULL;
	
	INTERFACE_MANAGER::Instance()->GetWindowSize(W, H);

	if (HasInterfaceContainer())
	{
		pInterface = GetInterfaceContainer();
	}

	if (pInterface != NULL)
	{
		pViewport = pInterface->GetViewport();
	}
	
	if (pViewport)
	{
		VX = static_cast<int> ( W * pViewport->GetLeft() / 100.f );
		VY = static_cast<int> ( H * pViewport->GetTop()	/ 100.f );
		VW = static_cast<int> ( W * pViewport->GetWidth() / 100.f );
		VH = static_cast<int> ( H * pViewport->GetHeight() / 100.f );

		POut.SetX(VW * Point.GetX() / 100 + VX);
		POut.SetY(VH * Point.GetY() / 100 + VY);
	}
	else
	{
		POut.SetX(W * Point.GetX() / 100);
		POut.SetY(H * Point.GetY() / 100);
	}

	if	(_Flags.IsSet(STATE_169))
	{
		printf("16/9 resize \n");
		POut.SetX(POut.GetX() - ((POut.GetX() - 256.0f) * 0.125f));
	}

	POut.SetZ(rZ);

	return	(POut);
}

//-----------------------------------------------------------------------------
//	Name:		RelativeToWorld
//	Object:		
//	02-12-05:	RMA - Created
//-----------------------------------------------------------------------------
VECTOR	IHM_ELEMENT_POLY_ABC::RelativeToWorld(const POINT2D &	Point,
											  float				rSizeX,
											  float				rSizeY,
											  float				rZ) const
{
	VECTOR POut;
	POut.SetX((Point.GetX() - 50.0f) / 100.0f * rSizeX);
	POut.SetY((Point.GetY() - 50.0f) / 100.0f * rSizeY);
	POut.SetZ(rZ);
	return (POut);
}

//-----------------------------------------------------------------------------
//	Name:		Synchronize
//	Object:		
//	02-01-21:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::Synchronize()
{
	_bVerticesToReset = true;
}

//-----------------------------------------------------------------------------
//	Name:		DebugDisplay
//	Object:		
//	02-06-26:	GGO - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	IHM_ELEMENT_POLY_ABC::DebugDisplay(VIEWPORT * pViewport)
{
	VECTOR				Pt1, Pt2;
	QDT_VECTOR<SEGMENT>	vSegments;
	int					nPointsCount, nPointIndex;

	nPointsCount = GetPointsCount();

	for ( nPointIndex = 0; nPointIndex < nPointsCount - 1; ++nPointIndex )
	{
		Pt1.SetXYZ( _pPoints[ nPointIndex     ].GetX(), _pPoints[ nPointIndex     ].GetY(), 0.0f );
		Pt2.SetXYZ( _pPoints[ nPointIndex + 1 ].GetX(), _pPoints[ nPointIndex + 1 ].GetY(), 0.0f );
		vSegments.PushTail( SEGMENT( Pt1, Pt2 ) );
	}

	// Last segment.
	Pt1.SetXYZ( _pPoints[ nPointIndex - 1 ].GetX(), _pPoints[ nPointIndex - 1 ].GetY(), 0.0f );
	Pt2.SetXYZ( _pPoints[ 0               ].GetX(), _pPoints[ 0               ].GetY(), 0.0f );
	vSegments.PushTail( SEGMENT( Pt1, Pt2 ) );

	QDT_NOT_IMPLEMENTED();
//	RENDER_MANAGER::Instance()->GetRenderContext()->Render2DSegments( vSegments, INTERFACE_MANAGER::Instance()->GetIHMDebugDisplayColor() );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetPoint
//	Object:		
//	02-07-30:	ELE - Created
//-----------------------------------------------------------------------------
const POINT2D &	IHM_ELEMENT_POLY_ABC::GetPoint(int	nIndex) const
{
	return	(_pPoints[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		InitGeometry
//	Object:		
//	04-02-24:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::InitGeometry()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetColor
//	Object:		
//	02-07-30:	ELE - Created
//-----------------------------------------------------------------------------
COLOR	IHM_ELEMENT_POLY_ABC::GetColor(int	nIndex)
{
	COLOR	Color;

	return _Colors[nIndex];

	//QDT_NOT_IMPLEMENTED();
/*
	InitGeometry();

	if	(nIndex == 3)
	{
		if	(_pGeometry->GetGeometryType() == GEOMETRY_ABC::GEOMETRY_TYPE_INDEXED_TRIANGLE_LIST)
		{
			Color = _pGeometry->GetDiffuseColorCopy(3);
		}
		else
		{
			Color = _pGeometry->GetDiffuseColorCopy(4);
		}
	}
	else
	{
		Color = _pGeometry->GetDiffuseColorCopy(nIndex);
	}
*/
	return	(Color);
}

//-----------------------------------------------------------------------------
//	Name:		GetUV
//	Object:		
//	02-07-30:	ELE - Created
//-----------------------------------------------------------------------------
POINT2D	IHM_ELEMENT_POLY_ABC::GetUV(int	nIndex)
{
	POINT2D	UV;

	QDT_NOT_IMPLEMENTED();
/*
	InitGeometry();

	if	(nIndex == 3)
	{
		if	(_pGeometry->GetGeometryType() == GEOMETRY_ABC::GEOMETRY_TYPE_INDEXED_TRIANGLE_LIST)
		{
			UV = _pGeometry->GetTextureCoordinateCopy(0, 3);
		}
		else
		{
			UV = _pGeometry->GetTextureCoordinateCopy(0, 4);
		}
	}
	else
	{
		UV = _pGeometry->GetTextureCoordinateCopy(0, nIndex);
	}
*/
	return	(UV);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_POLY_ABC::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_POLY_ABC) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_POLY_ABC::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("IHM_ELEMENT_POLY_ABC"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_POLY_ABC, 
										QDT_COMMENT("D86FBD7C-9470-ED40-F9CC-7F351DD7255C"),
										-1, 
										Init, 
										QDT_COMMENT("Astract class for ihm_element_poly"),
										IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pM = METHOD_DESCRIPTOR__Allocate("SetZDepth", "Set the z depth of the element.");
	METHOD_DESCRIPTOR__AddParameter(pM, "ZDepth", "Ask 3D Engine to document this parameter.", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_ZDEPTH, pM, EM_SetZDepth);

	pM = METHOD_DESCRIPTOR__Allocate("Under2DInterface", "Set the position state of the element.");
	METHOD_DESCRIPTOR__AddParameter(pM, "bUnder", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_UNDER_2D_INTERFACE, pM, EM_Under2DInterface);

	pM = METHOD_DESCRIPTOR::Allocate("SetMaterial", "Set the material to be used on the element.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pM->AddParameter(QDT_COMMENT("Material"), QDT_COMMENT(""), SHADER_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddCommand(CMD_SET_MATERIAL, pM, CM_SetMaterial, EM_SetMaterial);

	pM = METHOD_DESCRIPTOR__Allocate("Rotate", "Rotate the current element. The rotation is only applied once.");
	METHOD_DESCRIPTOR__AddParameter(pM, "rRotationAngle", "The rotation angle, in radians.", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Rotation center", "The rotation center, around which the element is rotated.", TYPE_POINT_2D::GetDescriptor());
	pCD->AddMethod(CMD_ROTATE, pM, EM_Rotate);

	pM = METHOD_DESCRIPTOR__Allocate("SetRotation", "Apply a rotation each frame.");
	METHOD_DESCRIPTOR__AddParameter(pM, "rRotationSpeed", "The rotation speed, in radians per second.", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Rotation center", "The rotation center, around which the element is rotated.", TYPE_POINT_2D::GetDescriptor());
	pCD->AddMethod(CMD_SET_ROTATION, pM, EM_SetRotation);

	pM = METHOD_DESCRIPTOR__Allocate("StopRotation", "If the element is rotating each frame (after a call of 'SetRotation'), calling this method stops the rotation.");
	pCD->AddMethod(CMD_STOP_ROTATION, pM, EM_StopRotation);

	pM = METHOD_DESCRIPTOR::Allocate("Fade", "Make a element fade. In order to know if element is correctly set to fade, use 'CanFade' method.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameter(pM, "rFadeTimeMS", "The fade time (in milliseconds).", TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "bFadeIn", "'true' to make the element fade in, 'false' to make it fade out.", TYPE_BOOL::GetDescriptor());
	pCD->AddCommand(CMD_FADE, pM, CM_Fade, EM_Fade);

	pM = METHOD_DESCRIPTOR__Allocate("CanFade", "Use this method to know if you can fade an element.");
	METHOD_DESCRIPTOR__SetReturn(pM, "bCanFade", "'true' if the current element can fade (mainly, if the material is corretly set and has an alpha layer).", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_CAN_FADE, pM, EM_CanFade);

	pM = METHOD_DESCRIPTOR::Allocate("SetFadeMinimumAlphaValue", "Use this method to set the minimum fade alpha value.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameter(pM, "nFadeMinAlphaValue", "The minimum	alpha value.", TYPE_INT::GetDescriptor());
	pCD->AddCommand(CMD_SET_FADE_MINIMUM_ALPHA_VALUE, pM, CM_SetFadeMinimumAlphaValue, EM_SetFadeMinimumAlphaValue);	

	pM = METHOD_DESCRIPTOR__Allocate("IsFading", "Use this method to know if the element is fading.");
	METHOD_DESCRIPTOR__SetReturn(pM, "bIsFading", "'true' if the current element is fading.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_FADING, pM, EM_IsFading);

	pCD->SetLoopMethod( EM_Loop );
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetZDepth
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::EM_SetZDepth(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *			pObjet;	
	IHM_ELEMENT_POLY_ABC *	pIhmElementPoly;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementPoly	= (IHM_ELEMENT_POLY_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementPoly);

	float * ZDepth = (float *) pP;
	pIhmElementPoly->SetZDepth(*ZDepth);

	COM_INSTANCE CI(pIhmElementPoly->GetHandle());
	if	(DISPLAY_MANAGER::Instance()->RemoveIhmElement(CI))
	{
		DISPLAY_MANAGER::Instance()->AddIhmElement(CI);
	}
	
	//	this ihm element need to be updated before drawn by display
	pIhmElementPoly->SetHasChanged(true);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Under2DInterface
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::EM_Under2DInterface(void * const	pO,
												  void * const	pR,
												  void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_POLY_ABC *	pIhmElementPoly;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementPoly	= (IHM_ELEMENT_POLY_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementPoly);

	bool * bUnder = (bool *) pP;

	if	( *bUnder != pIhmElementPoly->IsUnder2DInterface())
	{
		pIhmElementPoly->SetUnder2DInterface(*bUnder);

		COM_INSTANCE CI(pIhmElementPoly->GetHandle());
		if	(DISPLAY_MANAGER::Instance()->RemoveIhmElement(CI))
		{
			DISPLAY_MANAGER::Instance()->AddIhmElement(CI);
		}
	}

	//	this ihm element need to be updated before drawn by display
	pIhmElementPoly->SetHasChanged(true);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetMaterial
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::EM_SetMaterial(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
/*	COM_INSTANCE *			pObjet;	
	IHM_ELEMENT_POLY_ABC *	pIhmElementPoly;
	COM_INSTANCE *			pMaterialInst;
	
	pObjet			= (COM_INSTANCE *) pO;
	pIhmElementPoly	= (IHM_ELEMENT_POLY_ABC *) pObjet->GetCommunicator();
	pMaterialInst	= (COM_INSTANCE *) pP;
	
	QDT_ASSERT(pIhmElementPoly);

	pIhmElementPoly->SetMaterialInst(*pMaterialInst);

	//	this ihm element need to be updated before drawn by display
	pIhmElementPoly->SetHasChanged(true);*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetMaterial
//	Object:		
//	06-03-20:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_POLY_ABC::CM_SetMaterial(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_SET_MATERIAL);
	COM_INSTANCE pObjet;
	COM_INSTANCE material;
	Stack.GetThis(pObjet);
	
	IHM_ELEMENT_POLY_ABC	*pPoly = static_cast<IHM_ELEMENT_POLY_ABC *>(pObjet.GetCommunicator());

	Stack.GetParameter(0, material);
	pPoly->SetMaterialInst(material);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		SetRotationSpeedAndCenter
//	Object:		
//	02-12-16:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::SetRotationSpeedAndCenter(const float		rRotationSpeed,
														const POINT2D &	RotationCenter)
{
	_rRotationSpeed	= rRotationSpeed;
	_RotationCenter	= RotationCenter;
}

//-----------------------------------------------------------------------------
//	Name:		GetRotationSpeed
//	Object:		
//	02-12-16:	GGO - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_POLY_ABC::GetRotationSpeed() const
{
	return ( _rRotationSpeed );
}

//-----------------------------------------------------------------------------
//	Name:		GetRotationCenter
//	Object:		
//	02-12-16:	GGO - Created
//-----------------------------------------------------------------------------
const POINT2D &	IHM_ELEMENT_POLY_ABC::GetRotationCenter() const
{
	return ( _RotationCenter );
}

//-----------------------------------------------------------------------------
//	Name:		Rotate
//	Object:		
//	02-12-16:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::Rotate(const float		rRotationAngle,
									 const POINT2D &	RotationCenter)
{
	float	rAngle, rRadius, rDx, rDy, rCos, rSin, rRatioX, rRatioY, rRotationCenterX, rRotationCenterY;
	int		nPointIndex, nPointsCount;
	POINT2D	ScreenPoint;

	rRatioX	= 0.01f * INTERFACE_MANAGER::Instance()->GetWindowWidth();
	rRatioY	= 0.01f * INTERFACE_MANAGER::Instance()->GetWindowHeight();
	
	rRotationCenterX	= RotationCenter.GetX() * rRatioX;
	rRotationCenterY	= RotationCenter.GetY() * rRatioY;

	nPointsCount = GetPointsCount();
	for ( nPointIndex = 0; nPointIndex < nPointsCount; ++nPointIndex )
	{
		ScreenPoint.SetX( _pPoints[ nPointIndex ].GetX() * rRatioX );
		ScreenPoint.SetY( _pPoints[ nPointIndex ].GetY() * rRatioY );

		rDx = ScreenPoint.GetX() - rRotationCenterX;
		rDy = ScreenPoint.GetY() - rRotationCenterY;
		
		rRadius	= MATH::Sqrt( rDx * rDx + rDy * rDy );
		rAngle  = MATH::ATan( rDy, rDx );
		rAngle += rRotationAngle;

		MATH::GetCosinusAndSinus(rAngle, rCos, rSin);

		_pPoints[ nPointIndex ].SetX( ( rRadius * rCos + rRotationCenterX ) / rRatioX );
		_pPoints[ nPointIndex ].SetY( ( rRadius * rSin + rRotationCenterY ) / rRatioY );
	}

	_bVerticesToReset = true;
}

//-----------------------------------------------------------------------------
//	Name:		EM_Rotate
//	Object:		
//	02-12-16:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::EM_Rotate(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	struct	PARAMS
	{
		float	_rRotationAngle;
		POINT2D	_RotationCenter;
	};

	PARAMS *				pParameters;
	COM_INSTANCE *			pObjet;	
	IHM_ELEMENT_POLY_ABC *	pIhmElementPoly;
	
	pObjet			= static_cast<COM_INSTANCE*>( pO );
	pIhmElementPoly	= static_cast<IHM_ELEMENT_POLY_ABC *>( pObjet->GetCommunicator() );
	pParameters		= static_cast<PARAMS*>( pP );
	QDT_ASSERT(pIhmElementPoly);

	pIhmElementPoly->Rotate( pParameters->_rRotationAngle, pParameters->_RotationCenter );
	
	//	this ihm element need to be updated before drawn by display
	pIhmElementPoly->SetHasChanged(true);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetRotation
//	Object:		
//	02-12-16:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::EM_SetRotation(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
	struct	PARAMS
	{
		float	_rRotationSpeed;
		POINT2D	_RotationCenter;
	};

	PARAMS *				pParameters;
	COM_INSTANCE *			pObjet;	
	IHM_ELEMENT_POLY_ABC *	pIhmElementPoly;
	
	pObjet			= static_cast<COM_INSTANCE*>( pO );
	pIhmElementPoly	= static_cast<IHM_ELEMENT_POLY_ABC *>( pObjet->GetCommunicator() );
	pParameters		= static_cast<PARAMS*>( pP );
	QDT_ASSERT(pIhmElementPoly);

	pIhmElementPoly->SetRotationSpeedAndCenter( pParameters->_rRotationSpeed, pParameters->_RotationCenter );	

	//	this ihm element need to be updated before drawn by display
	pIhmElementPoly->SetHasChanged(true);
}

//-----------------------------------------------------------------------------
//	Name:		EM_StopRotation
//	Object:		
//	02-12-16:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::EM_StopRotation(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	COM_INSTANCE *			pObjet;	
	IHM_ELEMENT_POLY_ABC *	pIhmElementPoly;
	
	pObjet			= static_cast<COM_INSTANCE*>( pO );
	pIhmElementPoly	= static_cast<IHM_ELEMENT_POLY_ABC *>( pObjet->GetCommunicator() );
	QDT_ASSERT( pIhmElementPoly );

	pIhmElementPoly->SetRotationSpeedAndCenter( 0.0f, pIhmElementPoly->GetRotationCenter() );

	//	this ihm element need to be updated before drawn by display
	pIhmElementPoly->SetHasChanged(true);
}

//-----------------------------------------------------------------------------
//	Name:		FadeIn
//	Object:		
//	02-12-17:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::Fade(const float	rTimeMS,
								   const bool	bFadeIn)
{
#ifndef _MASTER
	if (CanFade() == false)
	{
		QDT_Warning("You try to fade a ihm element which can't fade (i.e. wrong blending mode)");
	}
#endif

	_rElapsedTimeMS	= rTimeMS;
	_rFadeTimeMS	= rTimeMS;
	
	if (bFadeIn)
		_eFadeMode = FADE_IN;
	else
		_eFadeMode = FADE_OUT;
}

//-----------------------------------------------------------------------------
//	Name:		EM_Fade
//	Object:		
//	02-12-17:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::EM_Fade(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	struct	PARAMS
	{
		float	_rTimeS;
		bool	_bFadeIn;
	};

	PARAMS *				pParameters;
	COM_INSTANCE *			pObjet;	
	IHM_ELEMENT_POLY_ABC *	pIhmElementPoly;
	
	pObjet			= static_cast<COM_INSTANCE*>( pO );
	pIhmElementPoly	= static_cast<IHM_ELEMENT_POLY_ABC *>( pObjet->GetCommunicator() );
	pParameters		= static_cast<PARAMS*>( pP );
	QDT_ASSERT(pIhmElementPoly);

	pIhmElementPoly->Fade( pParameters->_rTimeS, pParameters->_bFadeIn );	

	//	this ihm element need to be updated before drawn by display
	pIhmElementPoly->SetHasChanged(true);
}

//-----------------------------------------------------------------------------
//	Name:		CanFade
//	Object:		
//	02-12-17:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_POLY_ABC::CanFade() const
{
/*	if (_DatabaseMaterialInst.HasHandle() == true)
	{
		MATERIAL_ABC *pMaterial = static_cast<MATERIAL_ABC *>(_DatabaseMaterialInst.GetCommunicator());
		if (pMaterial->GetTransparency())
		{
			return (true);
		}
	}

	return (false);*/

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		EM_CanFade
//	Object:		
//	02-12-17:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::EM_CanFade(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	COM_INSTANCE *			pObjet;	
	IHM_ELEMENT_POLY_ABC *	pIhmElementPoly;
	bool *					pResult;
	
	pObjet			= static_cast<COM_INSTANCE*>( pO );
	pIhmElementPoly	= static_cast<IHM_ELEMENT_POLY_ABC *>( pObjet->GetCommunicator() );
	pResult			= static_cast<bool*>( pR );

	*pResult = pIhmElementPoly->CanFade();
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		
//	03-03-13:	SBE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::Loop()
{
	float					rTime, rAngle;
	unsigned char			nAlpha;
	int						nVertexIndex, nVerticesCount;
	COLOR					pColors[ 4 ];
	
	// Rotate.
	if (GetRotationSpeed() != 0.0f)
	{
		rTime	= TIME::Instance()->GetDefaultClock()->GetSpeedFactor() / 30.0f;
		rAngle	= GetRotationSpeed() * rTime;

		Rotate( rAngle, GetRotationCenter() );
	}

	// Fade.
	if (_eFadeMode != NO_FADE)
	{
		if ( _rElapsedTimeMS > 0.0f )
		{
			float rAlphaMax = 255.0f;

			QDT_ASSERT( _nFadeMinAlphaValue < ((unsigned char) rAlphaMax) );

			rAlphaMax = rAlphaMax - _nFadeMinAlphaValue;

			// Compute alpha.
			if ( _eFadeMode == FADE_IN )
			{
				nAlpha = static_cast<unsigned char>( rAlphaMax - rAlphaMax * _rElapsedTimeMS / _rFadeTimeMS );
			}
			else
			{
				nAlpha = static_cast<unsigned char>( rAlphaMax * _rElapsedTimeMS / _rFadeTimeMS );
			}

			nAlpha += _nFadeMinAlphaValue;			

			// Set new colors.
			nVerticesCount	= GetPointsCount();
			for ( nVertexIndex = 0; nVertexIndex < nVerticesCount; ++nVertexIndex )
			{
				pColors[ nVertexIndex ] = GetColor( nVertexIndex );
				pColors[ nVertexIndex ].SetAlpha( nAlpha );
			}
			SetColors( pColors );

			// Decrease elapsed time.
			rTime = 1000.0f * TIME::Instance()->GetDefaultClock()->GetSpeedFactor() / 30.0f;
			_rElapsedTimeMS -= rTime;

			Show();
		}
		else
		{
			// to avoid display after total fade out
			if ( ( _eFadeMode == FADE_OUT ) && (_nFadeMinAlphaValue == 0) ) 
			{
				Hide();
			}

			_eFadeMode = NO_FADE;
		}
	}
}


//-----------------------------------------------------------------------------
//	Name:		EM_Loop
//	Object:		
//	02-12-16:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::EM_Loop(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	IHM_ELEMENT_POLY_ABC *	pIhmElementPoly;
	
	pIhmElementPoly	= static_cast<IHM_ELEMENT_POLY_ABC *>( pO );
	QDT_ASSERT( pIhmElementPoly );

	pIhmElementPoly->Loop();
}

//-----------------------------------------------------------------------------
//	Name:		HasChanged
//	Object:		
//	03-03-13:	SBE - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_POLY_ABC::HasChanged() const
{
	return	(_bHasChanged);
}

//-----------------------------------------------------------------------------
//	Name:		SetHasChanged
//	Object:		
//	03-03-13:	SBE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::SetHasChanged(const bool	bFlag)
{
	_bHasChanged = bFlag;
}

//-----------------------------------------------------------------------------
//	Name:		SetFadeMinimumAlphaValue
//	Object:		
//	03-03-13:	SBE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::SetFadeMinimumAlphaValue(unsigned int	nValue)
{
	_nFadeMinAlphaValue = nValue;
}


//-----------------------------------------------------------------------------
//	Name:		EM_SetFadeMinimumAlphaValue
//	Object:		
//	03-03-13:	SBE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::EM_SetFadeMinimumAlphaValue(void * const	pO,
														  void * const	pR,
														  void * const	pP)
{
	struct	PARAMS
	{
		unsigned int	_nFadeMinAlphaValue;
	};

	PARAMS *				pParameters;
	COM_INSTANCE *			pObjet;	
	IHM_ELEMENT_POLY_ABC *	pIhmElementPoly;
	
	pObjet			= static_cast<COM_INSTANCE*>( pO );
	pIhmElementPoly	= static_cast<IHM_ELEMENT_POLY_ABC *>( pObjet->GetCommunicator() );
	pParameters		= static_cast<PARAMS*>( pP );
	QDT_ASSERT(pIhmElementPoly);

	pIhmElementPoly->SetFadeMinimumAlphaValue( pParameters->_nFadeMinAlphaValue );	

	//	this ihm element need to be updated before drawn by display
	pIhmElementPoly->SetHasChanged(true);
}

//-----------------------------------------------------------------------------
//	Name:		IsFading
//	Object:		
//	03-03-13:	SBE - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_POLY_ABC::IsFading() const
{
	return ( (_rElapsedTimeMS > 0.0) && (_eFadeMode != NO_FADE) );
}

//-----------------------------------------------------------------------------
//	Name:		SetAddGeometryInDatabank
//	Object:		
//	05-01-27:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::SetGeometryInDatabank(const bool	bAdd)
{
	_PolyFlags.Set( GEOMETRY_IN_DATABANK, bAdd );
}

//-----------------------------------------------------------------------------
//	Name:		SetAddMaterialInDatabank
//	Object:		
//	05-01-27:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::SetMaterialInDatabank(const bool	bAdd)
{
	_PolyFlags.Set( MATERIAL_IN_DATABANK, bAdd );
}

//-----------------------------------------------------------------------------
//	Object:		
//	03-03-13:	SBE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_POLY_ABC::EM_IsFading(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	COM_INSTANCE *			pObjet;	
	IHM_ELEMENT_POLY_ABC *	pIhmElementPoly;
	bool *					pResult;
	
	pObjet			= static_cast<COM_INSTANCE*>( pO );
	pIhmElementPoly	= static_cast<IHM_ELEMENT_POLY_ABC *>( pObjet->GetCommunicator() );
	pResult			= static_cast<bool*>( pR );

	*pResult = pIhmElementPoly->IsFading();	
}

//-----------------------------------------------------------------------------
//	Name:		CM_Fade
//	Object:		
//	06-03-29:	NHO - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_POLY_ABC::CM_Fade(COM_PARAM	P)
{
	COM_STACK				Stack(P, GetGenericCommunicatorDescriptor(), CMD_FADE);
	COM_INSTANCE			ThisCI;
	Stack.GetThis(ThisCI);

	IHM_ELEMENT_POLY_ABC*	pThis = static_cast<IHM_ELEMENT_POLY_ABC*>(ThisCI.GetCommunicator());
	
	Float32					rFadeDuration;
	Bool					bFadeIn;

	Stack.GetParameter(0, rFadeDuration);
	Stack.GetParameter(1, bFadeIn);
	pThis->Fade(rFadeDuration, bFadeIn);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		CM_SetFadeMinimumAlphaValue
//	Object:		
//	06-03-29:	NHO - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_POLY_ABC::CM_SetFadeMinimumAlphaValue(COM_PARAM	P)
{
	COM_STACK				Stack(P, GetGenericCommunicatorDescriptor(), CMD_SET_FADE_MINIMUM_ALPHA_VALUE);
	COM_INSTANCE			ThisCI;
	Stack.GetThis(ThisCI);

	IHM_ELEMENT_POLY_ABC*	pThis = static_cast<IHM_ELEMENT_POLY_ABC*>(ThisCI.GetCommunicator());
	Int32					nFadeMinValue;

	Stack.GetParameter(0, nFadeMinValue);
	pThis->SetFadeMinimumAlphaValue(nFadeMinValue);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

