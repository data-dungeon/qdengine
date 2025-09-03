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
//	CLASS:	GLOW
//
//	02-04-23:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Glow/Glow)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComBasis/AttributeDescriptor)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(ComplexType/TypeColor)

#ifdef	_DEBUG
	#include	"Glow.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GLOW constructor
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
GLOW::GLOW()
:
ENTITY_ABC				(GetGenericCommunicatorDescriptor(), ENTITY_GLOW),
_rSpinAngle				(0.0f),
_rOuterAngle			(MATH::PI() / 2.0f),
_rInnerAngle			(0.0f),
_MaterialInst			(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_rLensFlareInnerAngle	(0.0f),
_nNbrCircles			(0),
_pCirclesPosition		(NULL),
_pCirclesIntensity		(NULL),
_pCirclesSize			(NULL),
_pCirclesSpinAngle		(NULL),
_pCirclesOwnColor		(NULL),
_pCirclesColorMin		(NULL),
_pCirclesColorMax		(NULL),
_pCirclesUVs			(NULL),
_bToDisplay				(true),
_bSmooth				(false),
_nElapsedTime			(0),
_nSpawnTime				(100),
_nFlags					(GLOW_MITIGATION | GLOW_AUTO_SPIN),
_rDistance				(10.0f)
{
	_Normal.SetXYZ(0.0f, 0.0f, 1.0f);

	_DiffuseColorMin.SetARGB(255, 255, 255, 255);
	_DiffuseColorMax.SetARGB(255, 255, 255, 255);

	_ScaleMin.SetXYZ(0.0f, 0.0f, 1.0f);
	_ScaleMax.SetXYZ(1.0f, 1.0f, 1.0f);

	_pUVs[0].SetXY(0.0f, 0.0f);
	_pUVs[1].SetXY(1.0f, 0.0f);
	_pUVs[2].SetXY(0.0f, 1.0f);
	_pUVs[3].SetXY(1.0f, 1.0f);

	SetFlags(GLOW_ALL_AXIS, true);


	SetActive(true);
}

//-----------------------------------------------------------------------------
//	Name:		GLOW destructor
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
GLOW::~GLOW()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::Display(DISPLAY_PARAMETERS &	DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
/*
	if	(IsActive() && _MaterialInst.HasHandle())
	{
		float	rAngle;
		VECTOR	PositionInViewCS;

		bool bVisible = IsVisible(DisplayParameters, rAngle, PositionInViewCS, true);

		if	(_bToDisplay)
		{
			_bSmooth = false;

			if	(bVisible || _bSmooth)
			{
				float					rSpinAngle, rIntensity = 0.0f;
				bool					bZTest;
				MATERIAL_ABC			*pMaterial = NULL;
				VECTOR					Scale;
				COLOR					NewDiffuseColor, InterpolatedColor, Black(0, 0, 0, 0);

				pMaterial = static_cast<MATERIAL_ABC *>(_MaterialInst.GetCommunicator());

				bZTest = GetFlags(GLOW_ZTEST) > 0;
				if	(bZTest && (_rDistance > 0.0f))
				{
					VECTOR	V(PositionInViewCS);

					V.SetLength(_rDistance);
					PositionInViewCS.Sub(V);
				}

				if	(GetFlags(GLOW_MITIGATION) > 0)
				{
					if	(rAngle < _rOuterAngle)
					{
						if	(rAngle <= _rInnerAngle)
						{
							Scale = _ScaleMax;

							if	(_bSmooth)
							{
								NewDiffuseColor.Interpolate((float)_nElapsedTime / (float)_nSpawnTime, Black, _DiffuseColorMax);
							}
							else
							{
								NewDiffuseColor = _DiffuseColorMax;
							}

							rIntensity = 1.0f;
						}
						else
						{
							float	rFactor = 1.0f - (rAngle - _rInnerAngle) / (_rOuterAngle - _rInnerAngle);

							Scale.SetX(_ScaleMax.GetX() * rFactor + _ScaleMin.GetX() * (1.0f - rFactor));
							Scale.SetY(_ScaleMax.GetY() * rFactor + _ScaleMin.GetY() * (1.0f - rFactor));

							InterpolatedColor.Interpolate(rFactor, _DiffuseColorMin, _DiffuseColorMax);

							if	(_bSmooth)
							{
								NewDiffuseColor.Interpolate((float)_nElapsedTime / (float)_nSpawnTime, Black, InterpolatedColor);
							}
							else
							{
								NewDiffuseColor = InterpolatedColor;
							}

							rIntensity = rFactor;
						}
					}
					else //Color and size min
					{
						NewDiffuseColor = _DiffuseColorMin;
						Scale.SetX(_ScaleMin.GetX());
						Scale.SetY(_ScaleMin.GetY());
					}
				}
				else
				{
					rAngle = 0.0f;

					Scale = _ScaleMax;

					if	(_bSmooth)
					{
						NewDiffuseColor.Interpolate((float)_nElapsedTime / (float)_nSpawnTime, Black, _DiffuseColorMax);
					}
					else
					{
						NewDiffuseColor = _DiffuseColorMax;
					}
				}

				rSpinAngle = _rSpinAngle;
				if	(GetFlags(GLOW_AUTO_SPIN) > 0)
				{
					VECTOR	V(PositionInViewCS);

					rSpinAngle += MATH::DegreToRadian(V.Length() * rSpinAngle);// * 0.025f);
				}

				SCENE_GRAPH *pSG = DisplayParameters.GetCurrentSG();

				if(GetFlags(GLOW_ALL_AXIS))
				{
					///////////////////////////////////
					// Attenuation Glow / near plane //
					VECTOR	GlowGlobalPosition;
					GlowGlobalPosition = DisplayParameters.GetNode()->GetGlobalMatrix().GetTranslation();
					float	rSignedDistance = MATH::Abs(DisplayParameters.GetFrustrum().GetHexahedron().GetPlaneNear().GetSignedDistance(GlowGlobalPosition)) - _rDistance;
					float	rGlowSize = 2.f * MATH::Sqrt( Scale.GetX()*Scale.GetX() + Scale.GetY()*Scale.GetY() );
					float	rFactor = 1.f;

					if (rSignedDistance < 0.f)				rFactor = 0.f;
					else if (rSignedDistance < rGlowSize)	rFactor = (1.f - (rGlowSize - rSignedDistance) / rGlowSize);					

					NewDiffuseColor = NewDiffuseColor * rFactor;
					///////////////////////////////////

					pSG->GetParticleSystem()->AddGlow(PositionInViewCS, Scale, &_pUVs[0], rSpinAngle, NewDiffuseColor, pMaterial->GetShader(), bZTest);					
				
				}
				else
				{
					MATRIX	ViewMatrix;
					NODE	*pCameraNode = DISPLAY_MANAGER::Instance()->GetCameraNode();			
					VECTOR	GlowGlobalPosition = DisplayParameters.GetNode()->GetGlobalMatrix().GetTranslation();
					QUATERNION q(false);

					ViewMatrix.InverseAffine(pCameraNode->GetGlobalMatrix());

					if(GetFlags(GLOW_UP_AXIS))
					{
						ViewMatrix.QHeading( q );
					}
					else
					{
						ViewMatrix.QPitch( q );
					}

					VECTOR pPositions[2] = {VECTOR(_ScaleMax.GetX(), _ScaleMax.GetY(), 0.0f), VECTOR()};

					MATRIX mRot;
					mRot.Identity();

					mRot.SetQuaternion(q);
					mRot.SetTranslation(GlowGlobalPosition);
					mRot.Mul(ViewMatrix);
					
					mRot.TransformPosition(pPositions[0], pPositions[1]);
					mRot.TransformPosition(VECTOR::ZERO, pPositions[0]);		
					

					pSG->GetParticleSystem()->AddSprite(pPositions, &_pUVs[0], NewDiffuseColor, pMaterial->GetShader(), bZTest);					
					
				}
			

				if	( (GetFlags(GLOW_LENS_FLARE)) && (_nNbrCircles > 0) )				
				{
					DrawLensFlare(DisplayParameters, PositionInViewCS, Scale, NewDiffuseColor, rAngle, rIntensity, pMaterial);
				}
			}
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-03-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	GLOW::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		IsVisible
//	Object:		
//	02-04-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	GLOW::IsVisible(DISPLAY_PARAMETERS &	DisplayParameters,
						float &					rAngle,
						VECTOR &				PositionInViewCS,
						bool					bDoVisibility)
{
	bool	bVisible = true;

	DISPLAY_MANAGER *pDisplay = DISPLAY_MANAGER::Instance();

	if (bDoVisibility)
	{
		VECTOR	GlowGlobalPosition;

		GlowGlobalPosition = DisplayParameters.GetNode()->GetGlobalMatrix().GetTranslation();

		bVisible = ! DisplayParameters.GetFrustrum().GetHexahedron().GetPlaneNear().IsOutside(GlowGlobalPosition);
		_bToDisplay = true;
	}
	else
	{
		bVisible = true;
		_bToDisplay = true;
	}

	const MATRIX		*pGlobalMatrix;
	VECTOR				Position;

	MATRIX	ViewMatrix;
	NODE	*pCameraNode = DISPLAY_MANAGER::Instance()->GetCameraNode();
	pGlobalMatrix = &DisplayParameters.GetNode()->GetGlobalMatrix();

	ViewMatrix.InverseAffine(pCameraNode->GetGlobalMatrix());

	Position = pGlobalMatrix->GetTranslation();
	ViewMatrix.TransformPosition(Position, PositionInViewCS);

	if	(GetFlags(GLOW_MITIGATION) > 0)
	{
		float	rDotProduct;
		VECTOR	NormalInViewCS, V(PositionInViewCS);
		
		V.Normalize();

		pGlobalMatrix->TransformDirection(_Normal, NormalInViewCS);
		ViewMatrix.TransformDirection(NormalInViewCS);

		rDotProduct = -V.Dot(NormalInViewCS);
		rAngle = MATH::ACos(rDotProduct);

		if	(rAngle < _rOuterAngle)
		{
			return	(bVisible && VisibilityRayCast(DisplayParameters));
		}
		else
		{
			return	(bVisible);
		}
	}
	else
	{
		rAngle = 180.0f;

		return	(bVisible && VisibilityRayCast(DisplayParameters));
	}
}

//-----------------------------------------------------------------------------
//	Name:		VisibilityRayCast
//	Object:		
//	02-05-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	GLOW::VisibilityRayCast(DISPLAY_PARAMETERS &	DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
/*
	// Raycast
	if	(GetFlags(GLOW_RAY_CAST) > 0)
	{
		float				rDistance = 0.0f;
		VECTOR				GlowPosition;
		VECTOR				ViewPosition;
		VECTOR				V;
		RAY					Ray;
		RAY_CAST_PARAMETERS RayCastParameters;
		NODE				*pSGNode;

		ViewPosition = DISPLAY_MANAGER::Instance()->GetCameraNode()->GetGlobalMatrix().GetTranslation();
		GlowPosition = DisplayParameters.GetNode()->GetGlobalMatrix().GetTranslation();

		V.SetFromPoints(ViewPosition, GlowPosition);
		rDistance = V.Length() - _rDistance;
		V.Normalize();

		Ray.SetOrigin(ViewPosition);
		Ray.SetDirection(V);

		RayCastParameters.SetRay(Ray);
		RayCastParameters.SetFlags(RAY_CAST_PARAMETERS::CULLING, RAY_CAST_PARAMETERS::CULLING);

		pSGNode = DISPLAY_MANAGER::Instance()->GetCurrentViewport()->GetSGNode();
		pSGNode->RayCastTree(RayCastParameters);

		if	(RayCastParameters.GetImpactNodeID() != 0)
		{
			if	(RayCastParameters.GetImpactDistance() > rDistance)
			{
				return	(true);
			}
			else
			{
				return	(false);
			}
		}
	}
*/
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		DrawLensFlare
//	Object:		
//	02-04-26:	ELE - Created
//-----------------------------------------------------------------------------
/*void	GLOW::DrawLensFlare(DISPLAY_PARAMETERS &	DisplayParameters,
							const VECTOR &			GlowPosition,
							const VECTOR &			GlowScale,
							const COLOR &			GlowColor,
							const float				rGlowAngle,
							const float				rGlowIntensity,
							MATERIAL_ABC *			pMaterial)
{
	int					i;
	bool				bZTest;
	float				rLength, rScaleFactor, rSpinAngle;
	float				rNearClippingDistance = DISPLAY_MANAGER::Instance()->GetActiveCamera()->GetNearClippingDistance();
	VECTOR				V;
	VECTOR				GlowPos, GlowOnNearPlane, Pos, Center, CirclePos, ViewPoint(0.0f, 0.0f, 0.0f);
	PLANE				NearPlane(0.0f, 0.0f, 1.0f, rNearClippingDistance);
	VECTOR				Scale;
	RAY					Ray;
	COLOR				DiffuseColor, FinalColor, Black(0, 0, 0, 0);
	NODE				*pSGNode = static_cast<NODE *>(DisplayParameters.GetNode()->GetSceneGraphNodeInst().GetCommunicator());
	SCENE_GRAPH			*pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());

	if	(GetFlags(GLOW_FIXED_SIZE) > 0)
	{
		V.SetFromPoints(GlowPosition, ViewPoint);
		V.Normalize();

		Ray.SetOrigin(GlowPosition);
		Ray.SetDirection(V);
		
		INTERSECTION::IntersectPlaneWithRay(NearPlane, Ray, GlowOnNearPlane);

		GlowOnNearPlane.SetZ(-rNearClippingDistance);

		GlowPos = GlowOnNearPlane;

		rScaleFactor = 500.0f;
	}
	else
	{
		GlowPos = GlowPosition;

		rScaleFactor = 1.0f;
	}

	Center.SetXYZ(0.0f, 0.0f, GlowPos.GetZ());
	Pos.SetXYZ(-GlowPos.GetX(), -GlowPos.GetY(), GlowPos.GetZ());

	V.SetFromPoints(GlowPos, Pos);
	rLength = V.Length();

	bZTest = GetFlags(GLOW_ZTEST) > 0;
	
	for	(i = 0 ; i < _nNbrCircles ; ++i)
	{
		V.SetFromPoints(GlowPos, Center);

		float	rNewLength = rLength * _pCirclesPosition[i];
		if	(rNewLength < 0.0f)
		{
			V.SetLength(-rNewLength);
			V.Neg();
		}
		else
		{
			V.SetLength(rNewLength);
		}
		
		CirclePos = GlowPos + V;
		
		// Keep the lens flare only circle
//		Scale.SetX(GlowScale.GetX() * _pCirclesSize[i] / rScaleFactor);
//		Scale.SetY(GlowScale.GetY() * _pCirclesSize[i] / rScaleFactor);
		Scale.SetX(_pCirclesSize[i] / rScaleFactor);
		Scale.SetY(_pCirclesSize[i] / rScaleFactor);

		if	(GetFlags(GLOW_MITIGATION) > 0)
		{
			if	(rGlowAngle > _rOuterAngle)
			{
				DiffuseColor = _pCirclesColorMin[i];
			}
			else
			{
				if	(rGlowAngle < _rLensFlareInnerAngle)
				{
					DiffuseColor = _pCirclesColorMax[i];
				}
				else
				{
					float	rFactor = 1.0f - (rGlowAngle - _rLensFlareInnerAngle) / (_rOuterAngle - _rLensFlareInnerAngle);
					DiffuseColor.Interpolate(rFactor, _pCirclesColorMin[i], _pCirclesColorMax[i]);
				}
			}
		}
		else
		{
			DiffuseColor = _pCirclesColorMax[i];
		}

		if	(_bSmooth)
		{
			FinalColor.Interpolate((float)_nElapsedTime / (float)_nSpawnTime, Black, DiffuseColor);
		}
		else
		{
			FinalColor = DiffuseColor;
		}

		rSpinAngle = _pCirclesSpinAngle[i];
		if	(GetFlags(GLOW_AUTO_SPIN) > 0)
		{
			VECTOR	V(GlowPosition);

			rSpinAngle += MATH::DegreToRadian(V.Length() * rSpinAngle);// * 0.025f);
		}

		if	(_vCirclesMaterialInst[i].HasHandle())
		{
			MATERIAL_ABC * pCircleMaterial = static_cast<MATERIAL_ABC *>(_vCirclesMaterialInst[i].GetCommunicator());
			pSG->GetParticleSystem()->AddGlow(CirclePos, Scale, &_pCirclesUVs[i][0], rSpinAngle, FinalColor, pCircleMaterial->GetShader(), bZTest);	
		}
		else
		{
			pSG->GetParticleSystem()->AddGlow(CirclePos, Scale, &_pUVs[0], rSpinAngle, FinalColor, pMaterial->GetShader(), bZTest);
		}
	}
}*/

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *		pMD;
	ATTRIBUTE_DESCRIPTOR	*pAD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("GLOW", 0, CM_ReadChunk);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("GLOW", 1, CM_ReadChunk1);
#endif
#ifndef _MASTER	
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("GLOW", 1, CM_WriteChunk);
#endif

	//Attributes
	pAD = new	ATTRIBUTE_DESCRIPTOR("color", QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pAD->SetAccessors(EM_GetColorMax, EM_SetColorMax);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("distance", QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetDistance, EM_SetDistance);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("scaleMaxX", QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetScaleMax_X, EM_SetScaleMax_X);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("scaleMaxY", QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetScaleMax_Y, EM_SetScaleMax_Y);
	pCD->AddAttribute(pAD);

	// Methods
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetNormal"), QDT_COMMENT("Set the normal of the glow (direction from which glow size and intensity are maximum)."));
	pMD->AddParameter(QDT_COMMENT("rNormalX"), QDT_COMMENT("The x componant of normal."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rNormalY"), QDT_COMMENT("The y componant of normal."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rNormalZ"), QDT_COMMENT("The z componant of normal."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_NORMAL, pMD, EM_SetNormal);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetOuterAngle"), QDT_COMMENT("Set the outer angle of the glow (angle in which glow size, intensity and lens flare decrease)."));
	pMD->AddParameter(QDT_COMMENT("rAngle"), QDT_COMMENT("Outer angle in degres."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_OUTER_ANGLE, pMD, EM_SetOuterAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetInnerAngle"), QDT_COMMENT("Set the inner angle of the glow (angle in which glow size and intensity are maximum, and glow lens flare decrease)."));
	pMD->AddParameter(QDT_COMMENT("rAngle"), QDT_COMMENT("Inner angle in degres."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_INNER_ANGLE, pMD, EM_SetInnerAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetColorMax"), QDT_COMMENT("Set the maximum diffuse color of the glow."));
	pMD->AddParameter(QDT_COMMENT("nAlpha"), QDT_COMMENT("The alpla componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nRed"),	 QDT_COMMENT("The red componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nGreen"), QDT_COMMENT("The green componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nBlue"),  QDT_COMMENT("The blue componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_COLORMAX, pMD, EM_SetColorMax);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetColorMin"), QDT_COMMENT("Set the minimum diffuse color of the glow."));
	pMD->AddParameter(QDT_COMMENT("nAlpha"), QDT_COMMENT("The alpla componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nRed"),	 QDT_COMMENT("The red componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nGreen"), QDT_COMMENT("The green componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nBlue"),  QDT_COMMENT("The blue componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_COLORMIN, pMD, EM_SetColorMin);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetScaleMax"), QDT_COMMENT("Set the maximum size of the glow."));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT("The size on x axis."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT("The size on y axis."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_SCALEMAX, pMD, EM_SetScaleMax);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetScaleMin"), QDT_COMMENT("Set the minimum size of the glow."));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT("The size on x axis."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT("The size on y axis."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_SCALEMIN, pMD, EM_SetScaleMin);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetUVs"), QDT_COMMENT("Set mapping coordinates of each of the four points of the glow."));
	pMD->AddParameter(QDT_COMMENT("rU1"), QDT_COMMENT("U coordinate of point 1."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rV1"), QDT_COMMENT("V coordinate of point 1."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rU2"), QDT_COMMENT("U coordinate of point 2."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rV2"), QDT_COMMENT("V coordinate of point 2."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rU3"), QDT_COMMENT("U coordinate of point 3."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rV3"), QDT_COMMENT("V coordinate of point 3."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rU4"), QDT_COMMENT("U coordinate of point 4."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rV4"), QDT_COMMENT("V coordinate of point 4."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_UVS, pMD, EM_SetUVs);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSpinAngle"), QDT_COMMENT("Set the rotation angle of the glow."));
	pMD->AddParameter(QDT_COMMENT("rAngle"), QDT_COMMENT("Rotation angle in degres."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_SPIN_ANGLE, pMD, EM_SetSpinAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetMaterial"), QDT_COMMENT("Set the material of the glow."));
	//pMD->AddParameter(QDT_COMMENT("Material"), QDT_COMMENT("Glow material."), MATERIAL::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_MATERIAL, pMD, EM_SetMaterial);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetDistance"), QDT_COMMENT("Set the shift distance for glow position in case of activated z-test."));
	pMD->AddParameter(QDT_COMMENT("rDistance"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_DISTANCE, pMD, EM_SetDistance);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableZTest"), QDT_COMMENT("Enable z-test for glow occlusion."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_ENABLE_ZTEST, pMD, EM_EnableZTest);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableMitigation"), QDT_COMMENT("Enable glow mitigation :\n    Size changes between ScaleMin and ScaleMax with angle between view normal and glow normal.\nIf mitigation is disabled ScaleMax is used for glow size."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_ENABLE_MITIGATION, pMD, EM_EnableMitigation);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableLensFlare"), QDT_COMMENT("Enable glow lens flare."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_ENABLE_LENS_FLARE, pMD, EM_EnableLensFlare);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetLensFlareInnerAngle"), QDT_COMMENT("Set the inner angle of the lens flare (angle in which lens flare is not visible)."));
	pMD->AddParameter(QDT_COMMENT("rAngle"), QDT_COMMENT("Inner angle in degres."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_LENS_FLARE_INNER_ANGLE, pMD, EM_SetLensFlareInnerAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetNbrCircles"), QDT_COMMENT("Set the number of circles in the lens flare."));
	pMD->AddParameter(QDT_COMMENT("nNbrCircles"), QDT_COMMENT("Number of circles."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_NBR_CIRCLES, pMD, EM_SetNbrCircles);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetCircleParameters"), QDT_COMMENT("Set the parameters of a circle on lens flare axis."));
	pMD->AddParameter(QDT_COMMENT("nCircleIndex"), QDT_COMMENT("Circle index."), TYPE_INT::GetDescriptor());	
	pMD->AddParameter(QDT_COMMENT("rPosition"), QDT_COMMENT("In percent of axis size."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rIntensity"), QDT_COMMENT("In percent of axis size."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rSize"), QDT_COMMENT("In percent of axis size."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rSpinAngle"), QDT_COMMENT("In degrees."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_CIRCLE_PARAMETERS, pMD, EM_SetCircleParameters);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableCircleOwnColor"), QDT_COMMENT("Set the number of circles in the lens flare."));
	pMD->AddParameter(QDT_COMMENT("nCircleIndex"), QDT_COMMENT("Circle index."), TYPE_INT::GetDescriptor());	
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_ENABLE_CIRCLE_OWN_COLOR, pMD, EM_EnableCircleOwnColor);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetCircleColorMax"), QDT_COMMENT("Set the maximum diffuse color of the lens flare circle."));
	pMD->AddParameter(QDT_COMMENT("nCircleIndex"), QDT_COMMENT("Circle index."), TYPE_INT::GetDescriptor());	
	pMD->AddParameter(QDT_COMMENT("nAlpha"), QDT_COMMENT("The alpla componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nRed"),	 QDT_COMMENT("The red componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nGreen"), QDT_COMMENT("The green componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nBlue"),	 QDT_COMMENT("The blue componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_CIRCLE_COLORMAX, pMD, EM_SetCircleColorMax);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetCircleColorMin"), QDT_COMMENT("Set the minimum diffuse color of the lens flare circle."));
	pMD->AddParameter(QDT_COMMENT("nCircleIndex"), QDT_COMMENT("Circle index."), TYPE_INT::GetDescriptor());	
	pMD->AddParameter(QDT_COMMENT("nAlpha"), QDT_COMMENT("The alpla componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nRed"),	 QDT_COMMENT("The red componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nGreen"), QDT_COMMENT("The green componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("nBlue"),	 QDT_COMMENT("The blue componant of the color (must be between 0 and 255)."), TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_CIRCLE_COLORMIN, pMD, EM_SetCircleColorMin);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetCircleMaterial"), QDT_COMMENT("Set the material of the lens flare circle."));
	pMD->AddParameter(QDT_COMMENT("nCircleIndex"), QDT_COMMENT("Circle index."), TYPE_INT::GetDescriptor());	
	//pMD->AddParameter(QDT_COMMENT("Material"), QDT_COMMENT("Lens flare circle material."), MATERIAL::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_CIRCLE_MATERIAL, pMD, EM_SetCircleMaterial);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetCircleUVs"), QDT_COMMENT("Set mapping coordinates of each of the four points of the lens flare circle."));
	pMD->AddParameter(QDT_COMMENT("nCircleIndex"), QDT_COMMENT("Circle index."), TYPE_INT::GetDescriptor());	
	pMD->AddParameter(QDT_COMMENT("rU1"), QDT_COMMENT("U coordinate of point 1."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rV1"), QDT_COMMENT("V coordinate of point 1."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rU2"), QDT_COMMENT("U coordinate of point 2."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rV2"), QDT_COMMENT("V coordinate of point 2."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rU3"), QDT_COMMENT("U coordinate of point 3."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rV3"), QDT_COMMENT("V coordinate of point 3."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rU4"), QDT_COMMENT("U coordinate of point 4."), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rV4"), QDT_COMMENT("V coordinate of point 4."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_CIRCLE_UVS, pMD, EM_SetCircleUVs);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSpawnTime"), QDT_COMMENT("Duration of glow fade before appearance or disappearance."));
	pMD->AddParameter(QDT_COMMENT("nTime"), QDT_COMMENT("In milliseconds."), TYPE_INT::GetDescriptor());	
	pCD->AddMethod(CMD_GLOW_SET_SPAWN_TIME, pMD, EM_SetSpawnTime);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableAutoSpin"), QDT_COMMENT("Enable glow auto spin :\n    Spin angle change with distance between glow an camera."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());	
	pCD->AddMethod(CMD_GLOW_ENABLE_AUTO_SPIN, pMD, EM_EnableAutoSpin);
	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableRayCast"), QDT_COMMENT("Enable ray cast for occlusion testing.\nIt's very expensive in computing time, use it cautiously."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());	
	pCD->AddMethod(CMD_GLOW_ENABLE_RAY_CAST, pMD, EM_EnableRayCast);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableFixedSize"), QDT_COMMENT("Lens flare circles size is constant whatever distance from camera to glow (don't affect glow itself)."));
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());	
	pCD->AddMethod(CMD_GLOW_ENABLE_FIXED_SIZE, pMD, EM_EnableFixedSize);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetColorMax"), QDT_COMMENT("Get the color Max."));
	pMD->SetReturn(QDT_COMMENT("Color"), QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_GET_COLORMAX, pMD, EM_GetColorMax);	

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetDistance"), QDT_COMMENT("Get the distance offset."));
	pMD->SetReturn(QDT_COMMENT("Distance"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_GET_DISTANCE, pMD, EM_GetDistance);	

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetActive"), QDT_COMMENT("Make the glow active."));
	pMD->AddParameter(QDT_COMMENT(""), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_ACTIVE, pMD, EM_SetActive);	

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsActive"), QDT_COMMENT("Is the glow active?"));
	pMD->SetReturn(QDT_COMMENT(""), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_IS_ACTIVE, pMD, EM_IsActive);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetScaleMax_X"), QDT_COMMENT("Set the maximum size of the glow on X."));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT("The size on x axis."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_SCALEMAX_X, pMD, EM_SetScaleMax_X);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetScaleMax_Y"), QDT_COMMENT("Set the maximum size of the glow on Y."));
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT("The size on y axis."), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_SET_SCALEMAX_Y, pMD, EM_SetScaleMax_Y);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetScaleMax_X"), QDT_COMMENT("Get the maximum size of the glow on X."));
	pMD->SetReturn(QDT_COMMENT("rSizeX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_GET_SCALEMAX_X, pMD, EM_GetScaleMax_X);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetScaleMax_Y"), QDT_COMMENT("Get the maximum size of the glow on Y."));
	pMD->SetReturn(QDT_COMMENT("rSizeY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GLOW_GET_SCALEMAX_Y, pMD, EM_GetScaleMax_Y);


}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	GLOW::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_GLOW) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	GLOW::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("GLOW"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_GLOW,
										QDT_COMMENT("D35A22DB-E1F3-0BD6-08AA-92BCA2E1D75E"), 
										sizeof(GLOW), 
										ComInit, 
										QDT_COMMENT("Class for glow FX like lensflares."),
										ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_Construct(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	GLOW *pGlow;
	pGlow = static_cast<GLOW *>(pO);

	QDT_NEW_PLACED(pGlow, GLOW)();
	QDT_ASSERT(pGlow);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_Destruct(void * const	pO,
						  void * const	pR,
						  void * const	pP)
{
	GLOW *pGlow;
	pGlow = static_cast<GLOW *>(pO);

	QDT_DELETE_PLACED(pGlow, GLOW);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetNormal
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetNormal(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	struct PARAMETERS
	{
		float	_rX;
		float	_rY;
		float	_rZ;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pG->SetNormal(VECTOR(pParam->_rX, pParam->_rY, pParam->_rZ));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetOuterAngle
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetOuterAngle(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	float*			prAngle = static_cast<float*>(pP);

	pG->SetOuterAngle(*prAngle);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetInnerAngle
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetInnerAngle(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	float*			prAngle = static_cast<float*>(pP);

	pG->SetInnerAngle(*prAngle);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetColorMax
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetColorMax(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	COLOR			*pColor			= static_cast<COLOR*>(pP);

	pG->SetColorMax(*pColor);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetColorMin
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetColorMin(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	struct PARAMETERS
	{
		int	_nA;
		int	_nR;
		int	_nG;
		int	_nB;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pG->SetColorMin(COLOR(pParam->_nA, pParam->_nR, pParam->_nG, pParam->_nB));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScaleMax
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetScaleMax(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	struct PARAMETERS
	{
		float	_rX;
		float	_rY;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pG->SetScaleMax(VECTOR(pParam->_rX, pParam->_rY, 1.0f));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScaleMax_X
//	Object:		
//	04-11-29:	FPR - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetScaleMax_X(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	float*			pParam = static_cast<float*>(pP);

	pG->SetScaleMax(VECTOR(*pParam, pG->GetScaleMax().GetY(), 1.0f));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScaleMax_Y
//	Object:		
//	04-11-29:	FPR - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetScaleMax_Y(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	float*			pParam = static_cast<float*>(pP);

	pG->SetScaleMax(VECTOR(pG->GetScaleMax().GetX(), *pParam, 1.0f));
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetScaleMax_X
//	Object:		
//	04-11-29:	FPR - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_GetScaleMax_X(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetScaleMax_Y
//	Object:		
//	04-11-29:	FPR - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_GetScaleMax_Y(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	QDT_NOT_IMPLEMENTED();
}



//-----------------------------------------------------------------------------
//	Name:		EM_SetScaleMin
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetScaleMin(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	struct PARAMETERS
	{
		float	_rX;
		float	_rY;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pG->SetScaleMin(VECTOR(pParam->_rX, pParam->_rY, 1.0f));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetUVs
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetUVs(void * const	pO,
						void * const	pR,
						void * const	pP)
{
	struct PARAMETERS
	{
		float	_rU1;
		float	_rV1;
		float	_rU2;
		float	_rV2;
		float	_rU3;
		float	_rV3;
		float	_rU4;
		float	_rV4;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	POINT2D	UV[4];
	
	UV[0].SetXY(pParam->_rU1, pParam->_rV1);
	UV[1].SetXY(pParam->_rU2, pParam->_rV2);
	UV[2].SetXY(pParam->_rU3, pParam->_rV3);
	UV[3].SetXY(pParam->_rU4, pParam->_rV4);

	pG->SetUVs(UV);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetAngle
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetSpinAngle(void * const	pO,
						  void * const	pR,
						  void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	float*			prAngle = static_cast<float*>(pP);

	pG->SetSpinAngle(*prAngle);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetMaterial
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetMaterial(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	/*COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	COM_INSTANCE*	pInst = static_cast<COM_INSTANCE*>(pP);

	pG->SetMaterialInst(*pInst);*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetDistance
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetDistance(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	float*			prDistance = static_cast<float*>(pP);

	pG->SetDistance(*prDistance);
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableZTest
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_EnableZTest(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	bool*			pbValue = static_cast<bool*>(pP);

	if	(*pbValue)
	{
		pG->SetFlags(GLOW::GLOW_ZTEST, GLOW::GLOW_ZTEST);
	}
	else
	{
		pG->SetFlags(GLOW::GLOW_ZTEST, 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableMitigation
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_EnableMitigation(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	bool*			pbValue = static_cast<bool*>(pP);

	if	(*pbValue)
	{
		pG->SetFlags(GLOW::GLOW_MITIGATION, GLOW::GLOW_MITIGATION);
	}
	else
	{
		pG->SetFlags(GLOW::GLOW_MITIGATION, 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableLensFlare
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_EnableLensFlare(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	bool*			pbValue = static_cast<bool*>(pP);

	if	(*pbValue)
	{
		pG->SetFlags(GLOW::GLOW_LENS_FLARE, GLOW::GLOW_LENS_FLARE);
	}
	else
	{
		pG->SetFlags(GLOW::GLOW_LENS_FLARE, 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetLensFlareInnerAngle
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetLensFlareInnerAngle(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	float*			prAngle = static_cast<float*>(pP);

	pG->SetLensFlareInnerAngle(*prAngle);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetNbrCircles
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetNbrCircles(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	int*			pn = static_cast<int*>(pP);

	pG->SetNbrCircles(*pn);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetCircleParameters
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetCircleParameters(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	struct PARAMETERS
	{
		int		_nIndex;
		float	_rPosition;
		float	_rIntensity;
		float	_rSize;
		float	_rSpinAngle;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pG->SetCirclePosition(pParam->_nIndex, pParam->_rPosition);
	pG->SetCircleIntensity(pParam->_nIndex, pParam->_rIntensity);
	pG->SetCircleSize(pParam->_nIndex, pParam->_rSize);
	pG->SetCircleSpinAngle(pParam->_nIndex, pParam->_rSpinAngle);
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableCircleOwnColor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_EnableCircleOwnColor(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	struct PARAMETERS
	{
		int		_nIndex;
		bool	_bValue;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pG->EnableCircleOwnColor(pParam->_nIndex, pParam->_bValue);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetCircleColorMax
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetCircleColorMax(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	struct PARAMETERS
	{
		int		_nIndex;
		int		_nA;
		int		_nR;
		int		_nG;
		int		_nB;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pG->SetCircleColorMax(pParam->_nIndex, COLOR(pParam->_nA, pParam->_nR, pParam->_nG, pParam->_nB));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetCircleColorMin
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetCircleColorMin(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	struct PARAMETERS
	{
		int		_nIndex;
		int		_nA;
		int		_nR;
		int		_nG;
		int		_nB;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pG->SetCircleColorMax(pParam->_nIndex, COLOR(pParam->_nA, pParam->_nR, pParam->_nG, pParam->_nB));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetCircleMaterial
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetCircleMaterial(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	/*struct PARAMETERS
	{
		int				_nIndex;
		COM_INSTANCE	_Inst;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	pG->SetCircleMaterialInst(pParam->_nIndex, pParam->_Inst);*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetCircleUVs
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetCircleUVs(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	struct PARAMETERS
	{
		int		_nIndex;
		float	_rU1;
		float	_rV1;
		float	_rU2;
		float	_rV2;
		float	_rU3;
		float	_rV3;
		float	_rU4;
		float	_rV4;
	};

	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	PARAMETERS*		pParam = static_cast<PARAMETERS*>(pP);

	POINT2D	UV[4];
	
	UV[0].SetXY(pParam->_rU1, pParam->_rV1);
	UV[1].SetXY(pParam->_rU2, pParam->_rV2);
	UV[2].SetXY(pParam->_rU3, pParam->_rV3);
	UV[3].SetXY(pParam->_rU4, pParam->_rV4);

	pG->SetCircleUVs(pParam->_nIndex, UV);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetSpawnTime
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetSpawnTime(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	int*			pn = static_cast<int*>(pP);

	pG->SetSpawnTime(*pn);
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableAutoSpin
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_EnableAutoSpin(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	bool*			pbValue = static_cast<bool*>(pP);

	if	(*pbValue)
	{
		pG->SetFlags(GLOW::GLOW_AUTO_SPIN, GLOW::GLOW_AUTO_SPIN);
	}
	else
	{
		pG->SetFlags(GLOW::GLOW_AUTO_SPIN, 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableRayCast
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_EnableRayCast(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	bool*			pbValue = static_cast<bool*>(pP);

	if	(*pbValue)
	{
		pG->SetFlags(GLOW::GLOW_RAY_CAST, GLOW::GLOW_RAY_CAST);
	}
	else
	{
		pG->SetFlags(GLOW::GLOW_RAY_CAST, 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableFixedSize
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_EnableFixedSize(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	COM_INSTANCE *	pComInstance = static_cast<COM_INSTANCE *>(pO);
	GLOW *			pG = static_cast<GLOW*>(pComInstance->GetCommunicator());
	bool*			pbValue = static_cast<bool*>(pP);

	if	(*pbValue)
	{
		pG->SetFlags(GLOW::GLOW_FIXED_SIZE, GLOW::GLOW_FIXED_SIZE);
	}
	else
	{
		pG->SetFlags(GLOW::GLOW_FIXED_SIZE, 0);
	}
}


//-----------------------------------------------------------------------------
//	Name:		EM_GetColorMax
//	Object:		
//	04-02-18:	FPR - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_GetColorMax(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	COM_INSTANCE			*pGlowInst	= static_cast<COM_INSTANCE*>(pO);
	GLOW					*pGlow		= static_cast<GLOW*>(pGlowInst->GetCommunicator());
	COLOR					*pColor		= static_cast<COLOR*>(pR);

	*pColor = pGlow->GetColorMax();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetDistance
//	Object:		
//	04-03-12:	FPR - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_GetDistance(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	COM_INSTANCE			*pGlowInst	= static_cast<COM_INSTANCE*>(pO);
	GLOW					*pGlow		= static_cast<GLOW*>(pGlowInst->GetCommunicator());
	float					*pDistance	= static_cast<float*>(pR);

	*pDistance = pGlow->GetDistance();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetActive
//	Object:		
//	04-03-30:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_SetActive(void * const pO, 
						   void * const pR, 
						   void * const pP)
{
	COM_INSTANCE			*pGlowInst	= static_cast<COM_INSTANCE*>(pO);
	GLOW					*pGlow		= static_cast<GLOW*>(pGlowInst->GetCommunicator());
	bool					*pb			= static_cast<bool*>(pP);

	pGlow->SetActive(*pb);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsActive
//	Object:		
//	04-03-30:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::EM_IsActive(void * const pO, void * const pR, void * const pP)
{
	COM_INSTANCE			*pGlowInst	= static_cast<COM_INSTANCE*>(pO);
	GLOW					*pGlow		= static_cast<GLOW*>(pGlowInst->GetCommunicator());
	bool					*pb			= static_cast<bool*>(pR);

	*pb = pGlow->IsActive();
}

#ifndef _MASTER_ONLY_DATABANK

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-08-20:	FPR - Created
//-----------------------------------------------------------------------------
void	GLOW::CM_ReadChunk(IO_STREAM &		Stream,
						   COMMUNICATOR *	pCommunicator)
{
	GLOW	*pGlow;
	pGlow = static_cast<GLOW *>(pCommunicator);

	//pGlow->ReadCommon(Stream);

	Stream	>> pGlow->_Normal
			>> pGlow->_rOuterAngle
			>> pGlow->_rInnerAngle
			>> pGlow->_DiffuseColorMin
			>> pGlow->_DiffuseColorMax
			>> pGlow->_ScaleMin
			>> pGlow->_ScaleMax
			>> pGlow->_pUVs[0]
			>> pGlow->_pUVs[1]
			>> pGlow->_pUVs[2]
			>> pGlow->_pUVs[3]
			>> pGlow->_rSpinAngle
			>> pGlow->_rDistance 
			>> pGlow->_MaterialInst			
			>> pGlow->_nFlags
			>> pGlow->_nNbrCircles
			>> pGlow->_rLensFlareInnerAngle;

	if ( !pGlow->GetFlags(GLOW_ALL_AXIS) && !pGlow->GetFlags(GLOW_UP_AXIS) && !pGlow->GetFlags(GLOW_RIGHT_AXIS) )
	{
		pGlow->SetFlags(GLOW_ALL_AXIS, GLOW_ALL_AXIS);
	}

	pGlow->_pCirclesPosition = new float[pGlow->_nNbrCircles];
	pGlow->_pCirclesIntensity = new float[pGlow->_nNbrCircles];
	pGlow->_pCirclesSize = new float[pGlow->_nNbrCircles];
	pGlow->_pCirclesSpinAngle = new float[pGlow->_nNbrCircles];
	pGlow->_pCirclesOwnColor = new bool[pGlow->_nNbrCircles];
	pGlow->_pCirclesColorMin = new COLOR[pGlow->_nNbrCircles];
	pGlow->_pCirclesColorMax = new COLOR[pGlow->_nNbrCircles];
		
	pGlow->_vCirclesMaterialInst.Clear();
	pGlow->_vCirclesMaterialInst.Reserve(pGlow->_nNbrCircles);

	pGlow->_pCirclesUVs = new POINT2D*[pGlow->_nNbrCircles];
	int i;
	for	(i = 0 ; i < pGlow->_nNbrCircles ; ++i)
	{
		pGlow->_pCirclesUVs[i] = new POINT2D[4];
	}

	for(i=0; i<pGlow->_nNbrCircles; ++i)
	{
		Stream	>> pGlow->_pCirclesPosition[i];
		Stream	>> pGlow->_pCirclesIntensity[i];
		Stream	>> pGlow->_pCirclesSize[i];
		Stream	>> pGlow->_pCirclesSpinAngle[i];
		Stream	>> pGlow->_pCirclesOwnColor[i];
		Stream	>> pGlow->_pCirclesColorMin[i];
		Stream	>> pGlow->_pCirclesColorMax[i];
		COM_INSTANCE tmp;
		Stream	>> tmp;
		pGlow->_vCirclesMaterialInst.PushTail(tmp);
		Stream	>> pGlow->_pCirclesUVs[i][0];
		Stream	>> pGlow->_pCirclesUVs[i][1];
		Stream	>> pGlow->_pCirclesUVs[i][2];
		Stream	>> pGlow->_pCirclesUVs[i][3];
	}

	pGlow->SetActive(true);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	03-12-10:	FPR - Created
//-----------------------------------------------------------------------------
void	GLOW::CM_ReadChunk1(IO_STREAM &		Stream,
							COMMUNICATOR *	pCommunicator)
{
	GLOW	*pGlow;
	pGlow = static_cast<GLOW *>(pCommunicator);

	//pGlow->ReadCommon(Stream);

	Stream	>> pGlow->_Normal
			>> pGlow->_rOuterAngle
			>> pGlow->_rInnerAngle
			>> pGlow->_DiffuseColorMin
			>> pGlow->_DiffuseColorMax
			>> pGlow->_ScaleMin
			>> pGlow->_ScaleMax
			>> pGlow->_pUVs[0]
			>> pGlow->_pUVs[1]
			>> pGlow->_pUVs[2]
			>> pGlow->_pUVs[3]
			>> pGlow->_rSpinAngle
			>> pGlow->_rDistance 
			>> pGlow->_MaterialInst			
			>> pGlow->_nFlags
			>> pGlow->_nNbrCircles
			>> pGlow->_rLensFlareInnerAngle
			>> pGlow->_nSpawnTime;

	if ( !pGlow->GetFlags(GLOW_ALL_AXIS) && !pGlow->GetFlags(GLOW_UP_AXIS) && !pGlow->GetFlags(GLOW_RIGHT_AXIS) )
	{
		pGlow->SetFlags(GLOW_ALL_AXIS, GLOW_ALL_AXIS);
	}

	pGlow->_pCirclesPosition = new float[pGlow->_nNbrCircles];
	pGlow->_pCirclesIntensity = new float[pGlow->_nNbrCircles];
	pGlow->_pCirclesSize = new float[pGlow->_nNbrCircles];
	pGlow->_pCirclesSpinAngle = new float[pGlow->_nNbrCircles];
	pGlow->_pCirclesOwnColor = new bool[pGlow->_nNbrCircles];
	pGlow->_pCirclesColorMin = new COLOR[pGlow->_nNbrCircles];
	pGlow->_pCirclesColorMax = new COLOR[pGlow->_nNbrCircles];
		
	pGlow->_vCirclesMaterialInst.Clear();
	pGlow->_vCirclesMaterialInst.Reserve(pGlow->_nNbrCircles);

	pGlow->_pCirclesUVs = new POINT2D*[pGlow->_nNbrCircles];
	int i;
	for	(i = 0 ; i < pGlow->_nNbrCircles ; ++i)
	{
		pGlow->_pCirclesUVs[i] = new POINT2D[4];
	}

	for(i=0; i<pGlow->_nNbrCircles; ++i)
	{
		Stream	>> pGlow->_pCirclesPosition[i];
		Stream	>> pGlow->_pCirclesIntensity[i];
		Stream	>> pGlow->_pCirclesSize[i];
		Stream	>> pGlow->_pCirclesSpinAngle[i];
		Stream	>> pGlow->_pCirclesOwnColor[i];
		Stream	>> pGlow->_pCirclesColorMin[i];
		Stream	>> pGlow->_pCirclesColorMax[i];
		COM_INSTANCE tmp;
		Stream	>> tmp;
		pGlow->_vCirclesMaterialInst.PushTail(tmp);
		Stream	>> pGlow->_pCirclesUVs[i][0];
		Stream	>> pGlow->_pCirclesUVs[i][1];
		Stream	>> pGlow->_pCirclesUVs[i][2];
		Stream	>> pGlow->_pCirclesUVs[i][3];
	}

	pGlow->SetActive(true);
}

#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-08-20:	FPR - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	GLOW::CM_WriteChunk(IO_STREAM &		Stream,
							COMMUNICATOR *	pCommunicator)
{
	GLOW	*pGlow;
	pGlow = static_cast<GLOW *>(pCommunicator);

	Stream.DebugOutput("Glow :\t");
	Stream.DebugOutput(pGlow->GetName());
	Stream.DebugOutput("\r\n");

	//pGlow->WriteCommon(Stream);

	Stream.DebugOutput("Normal :\t");
	Stream	<< pGlow->_Normal;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("OuterAngle :\t");
	Stream	<< pGlow->_rOuterAngle;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("InnerAngle :\t");
	Stream	<< pGlow->_rInnerAngle;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("DiffuseColorMin :\t");
	Stream	<< pGlow->_DiffuseColorMin;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("DiffuseColorMax :\t");
	Stream	<< pGlow->_DiffuseColorMax;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("ScaleMin :\t");
	Stream	<< pGlow->_ScaleMin;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("ScaleMax :\t");
	Stream	<< pGlow->_ScaleMax;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("UVs 0:\t");
	Stream	<< pGlow->_pUVs[0];
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("UVs 1:\t");
	Stream	<< pGlow->_pUVs[1];
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("UVs 2:\t");
	Stream	<< pGlow->_pUVs[2];
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("UVs 3:\t");
	Stream	<< pGlow->_pUVs[3];
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("SpinAngle :\t");
	Stream	<< pGlow->_rSpinAngle;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("Distance :\t");
	Stream	<< pGlow->_rDistance;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Material :\t");
	Stream	<< pGlow->_MaterialInst;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Flags :\t");
	Stream << pGlow->_nFlags;
	Stream.DebugOutput("\t");
	Stream.DebugOutputBinary(pGlow->_nFlags);
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Nb Circles :\t");
	Stream	<< pGlow->_nNbrCircles;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Lens Flare Inner Angle:\t");
	Stream	<< pGlow->_rLensFlareInnerAngle;
	Stream.DebugOutput("\r\n");	

	Stream.DebugOutput("Spawn time:\t");
	Stream	<< pGlow->_nSpawnTime;
	Stream.DebugOutput("\r\n");	

	int i;
	for(i=0; i<pGlow->_nNbrCircles; ++i)
	{
		Stream.DebugOutput("Circle Pos :\t");
		Stream	<< pGlow->_pCirclesPosition[i];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Circle Intensity :\t");
		Stream	<< pGlow->_pCirclesIntensity[i];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Circle Size :\t");
		Stream	<< pGlow->_pCirclesSize[i];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Circle Spin angle :\t");
		Stream	<< pGlow->_pCirclesSpinAngle[i];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Circle Own color :\t");
		Stream	<< pGlow->_pCirclesOwnColor[i];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Circle Color Min :\t");
		Stream	<< pGlow->_pCirclesColorMin[i];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Circle Color Max :\t");
		Stream	<< pGlow->_pCirclesColorMax[i];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Circle Material :\t");
		Stream	<< pGlow->_vCirclesMaterialInst[i];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Circle uv 0 :\t");
		Stream	<< pGlow->_pCirclesUVs[i][0];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Circle uv 1 :\t");
		Stream	<< pGlow->_pCirclesUVs[i][1];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Circle uv 2 :\t");
		Stream	<< pGlow->_pCirclesUVs[i][2];
		Stream.DebugOutput("\r\n");

		Stream.DebugOutput("Circle uv 3 :\t");
		Stream	<< pGlow->_pCirclesUVs[i][3];
		Stream.DebugOutput("\r\n");

	
	}

	Stream.DebugOutput("\r\n");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		SetNbrCircles
//	Object:		
//	02-04-26:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetNbrCircles(int	nNbrCircles)
{
	int	i;

	if	(_pCirclesPosition)
	{
		delete [] _pCirclesPosition;
	}
	_pCirclesPosition = new float[nNbrCircles];

	if	(_pCirclesIntensity)
	{
		delete [] _pCirclesIntensity;
	}
	_pCirclesIntensity = new float[nNbrCircles];

	if	(_pCirclesSize)
	{
		delete [] _pCirclesSize;
	}
	_pCirclesSize = new float[nNbrCircles];

	if	(_pCirclesSpinAngle)
	{
		delete [] _pCirclesSpinAngle;
	}
	_pCirclesSpinAngle = new float[nNbrCircles];

	for	(i = 0 ; i < nNbrCircles ; ++i)
	{
		_pCirclesSpinAngle[i] = 0.0f;
	}

	if	(_pCirclesOwnColor)
	{
		delete [] _pCirclesOwnColor;
	}
	_pCirclesOwnColor = new bool[nNbrCircles];

	for	(i = 0 ; i < nNbrCircles ; ++i)
	{
		_pCirclesOwnColor[i] = false;
	}

	if	(_pCirclesColorMin)
	{
		delete [] _pCirclesColorMin;
	}
	_pCirclesColorMin = new COLOR[nNbrCircles];

	if	(_pCirclesColorMax)
	{
		delete [] _pCirclesColorMax;
	}
	_pCirclesColorMax = new COLOR[nNbrCircles];

	_vCirclesMaterialInst.Clear();
	_vCirclesMaterialInst.Reserve(nNbrCircles);

	if	(_pCirclesUVs)
	{
		for	(i = 0 ; i < _nNbrCircles ; ++i)
		{
			delete [] _pCirclesUVs[i];
		}
		delete [] _pCirclesUVs;
	}
	_pCirclesUVs = new POINT2D*[nNbrCircles];
	for	(i = 0 ; i < nNbrCircles ; ++i)
	{
		_pCirclesUVs[i] = new POINT2D[4];
	}

	_nNbrCircles = nNbrCircles;
}

//-----------------------------------------------------------------------------
//	Name:		SetCircleUVs
//	Object:		
//	02-04-29:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetCircleUVs(int			nCircleIndex,
						   POINT2D *	pUVs)
{
	if	(nCircleIndex < _nNbrCircles)
	{
		_pCirclesUVs[nCircleIndex][0] = pUVs[0];
		_pCirclesUVs[nCircleIndex][1] = pUVs[1];
		_pCirclesUVs[nCircleIndex][2] = pUVs[2];
		_pCirclesUVs[nCircleIndex][3] = pUVs[3];
	}
}

//=============================================================================
//	CODE ENDS HERE
//============================================================================

