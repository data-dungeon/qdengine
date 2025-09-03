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
//	CLASS:	SHADER_CUSTOM
//
//	05-11-09:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Material/ShaderCustom)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderContainer)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderError)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureABC)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/Viewport/Viewport)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(Renderer/IRenderTarget)
#include	INCL_KRENDERER(Renderer/ITexture)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include	INCL_KMATH(Math/SphericalHarmonics/SphericalHarmonics)
#include	INCL_KMATH(Math/SphericalHarmonics/shRotateMatrix)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

I_PIXEL_SHADER *	SHADER_CUSTOM::s_pPass[MAX_PASSES];
UInt32				SHADER_CUSTOM::s_uiNbrOfPasses = 0;

//-----------------------------------------------------------------------------
//	Name:		SHADER_CUSTOM constructor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_CUSTOM::SHADER_CUSTOM()
:
SHADER_ABC(GetGenericCommunicatorDescriptor()),
_pSkinnedVertexShader(NULL),
_pVertexShader(NULL),
_uiVertexShaderSize(0),
_pRawVertexShader(NULL),
_uiSkinnedVertexShaderSize(0),
_pRawSkinnedVertexShader(NULL),
_pTextures(NULL),
_bVSNeedEyePosition(false),
_bPSNeedEyeVector(false),
_bPSNeedEyePosition(false),
_bPSNeedObjectToViewMatrix(false),
_bPSNeedObjectToWorldMatrix(false),
_uiNbrOfShaderContainers(0),
_pShaderContainerArray(NULL),
_pPixelShaderSize(NULL),
_pPixelShaderDescriptor(0),
_ppRawPixelShader(NULL),
_bPSNeedSphericalHarmonics(false)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SHADER_CUSTOM destructor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_CUSTOM::~SHADER_CUSTOM()
{
	if (_pVertexShader)
	{
		RENDER_CONTEXT::ReleaseVertexShader(_pVertexShader);
	}
	if (_pSkinnedVertexShader)
	{
		RENDER_CONTEXT::ReleaseVertexShader(_pSkinnedVertexShader);
	}

	delete [] _pShaderContainerArray;
	delete [] _pTextures;

	// This code is useful only when exporting textures
	if(_pRawVertexShader)
	{
		delete [] _pRawVertexShader;
		_pRawVertexShader = NULL;
	}

	if(_pRawSkinnedVertexShader)
	{
		delete [] _pRawSkinnedVertexShader;
		_pRawSkinnedVertexShader = NULL;
	}

	if	(_ppRawPixelShader)
	{
		for (UInt32 i = 0 ; i < _uiNbrOfShaderContainers ; ++i)
		{
			if (_ppRawPixelShader[i])
			{
				delete [] _ppRawPixelShader[i];
			}
		}

		delete [] _ppRawPixelShader;
		_ppRawPixelShader = NULL;
	}

	if(_pPixelShaderSize)
	{
		delete [] _pPixelShaderSize;
		_pPixelShaderSize = NULL;
	}
	//
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPasses
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 SHADER_CUSTOM::GetNbrPasses()
{
	if (_bSupportLighting)
	{
		UInt32 uiNbrOfLights = DISPLAY_MANAGER::Instance()->GetNbrLights();

		if (uiNbrOfLights)
		{
			for (UInt32 i = 0 ; i < uiNbrOfLights ; ++i)
			{
				UInt32 uiDescriptor = 0;
				const S_LIGHT_SOURCE & LightSource = DISPLAY_MANAGER::Instance()->GetLight(i);

				LIGHT_TYPE eLightType = LightSource._eType;
				SHADOW_BLUR_MODE eShadowBlurMode = LightSource._nShadowBlurMode;

				switch (eLightType)
				{
				case LIGHT_UNDEFINED:
					QDT_FAIL();

				case LIGHT_POINT:
					{
						if (i == 0)
						{
							if (_bPSNeedSphericalHarmonics && DISPLAY_MANAGER::Instance()->GetCurrentViewport()->IsSHAmbientEnabled())
							{
								uiDescriptor |= SD_AMBIENT_EMISSIVE_SPHERICAL_HARMONICS;
							}
							else
							{
								uiDescriptor |= SD_AMBIENT_EMISSIVE;
							}
							uiDescriptor |= SD_LIGHT_POINT << 4;
						}
						else
						{
							uiDescriptor |= SD_LIGHT_POINT;
						}
					}
					break;

				case LIGHT_POINT_SHADOW:
					{
						UInt32 nDecal = 0;

						if (i == 0)
						{
							if (_bPSNeedSphericalHarmonics && DISPLAY_MANAGER::Instance()->GetCurrentViewport()->IsSHAmbientEnabled())
							{
								uiDescriptor |= SD_AMBIENT_EMISSIVE_SPHERICAL_HARMONICS;
							}
							else
							{
								uiDescriptor |= SD_AMBIENT_EMISSIVE;
							}

							nDecal = 4;
						}

						if (_bSupportShadowing)
						{
							uiDescriptor |= SD_LIGHT_POINT_SHADOW << nDecal;
						}
						else
						{
							uiDescriptor |= SD_LIGHT_POINT << nDecal;
						}
					}
					break;

				case LIGHT_SPOT:
					{
						if (i == 0)
						{
							if (_bPSNeedSphericalHarmonics && DISPLAY_MANAGER::Instance()->GetCurrentViewport()->IsSHAmbientEnabled())
							{
								uiDescriptor |= SD_AMBIENT_EMISSIVE_SPHERICAL_HARMONICS;
							}
							else
							{
								uiDescriptor |= SD_AMBIENT_EMISSIVE;
							}
							uiDescriptor |=  SD_LIGHT_SPOT << 4;
						}
						else
						{
							uiDescriptor |=  SD_LIGHT_SPOT;
						}
					}
					break;

				case LIGHT_SPOT_SHADOW:
				case LIGHT_SPOT_HARDWARE_SHADOW:
					{
						UInt32 uiShadowDescriptor = SD_LIGHT_SPOT_SHADOW;

						if (eLightType == LIGHT_SPOT_HARDWARE_SHADOW)
						{
							uiShadowDescriptor = SD_LIGHT_SPOT_HARDWARE_SHADOW;

							if (eShadowBlurMode != SBM_NONE)
							{
								uiShadowDescriptor += eShadowBlurMode;
							}
						}

						if (DISPLAY_MANAGER::Instance()->GetCurrentViewport()->AreShadowsEnabled() && _bSupportShadowing)
						{
							if (i == 0)
							{
								if (_bPSNeedSphericalHarmonics && DISPLAY_MANAGER::Instance()->GetCurrentViewport()->IsSHAmbientEnabled())
								{
									uiDescriptor |= SD_AMBIENT_EMISSIVE_SPHERICAL_HARMONICS;
								}
								else
								{
									uiDescriptor |= SD_AMBIENT_EMISSIVE;
								}
								uiDescriptor |=  uiShadowDescriptor << 4;
							}
							else
							{
								uiDescriptor |=  uiShadowDescriptor;
							}
						}
						else
						{
							if (i == 0)
							{
								if (_bPSNeedSphericalHarmonics && DISPLAY_MANAGER::Instance()->GetCurrentViewport()->IsSHAmbientEnabled())
								{
									uiDescriptor |= SD_AMBIENT_EMISSIVE_SPHERICAL_HARMONICS;
								}
								else
								{
									uiDescriptor |= SD_AMBIENT_EMISSIVE;
								}
								uiDescriptor |=  SD_LIGHT_SPOT << 4;
							}
							else
							{
								uiDescriptor |=  SD_LIGHT_SPOT;
							}
						}
					}
					break;			

				case LIGHT_DIRECTIONAL:
					{
						if (i == 0)
						{
							if (_bPSNeedSphericalHarmonics && DISPLAY_MANAGER::Instance()->GetCurrentViewport()->IsSHAmbientEnabled())
							{
								uiDescriptor |= SD_AMBIENT_EMISSIVE_SPHERICAL_HARMONICS;
							}
							else
							{
								uiDescriptor |= SD_AMBIENT_EMISSIVE;
							}
							uiDescriptor |=  SD_LIGHT_DIRECTIONAL << 4;
						}
						else
						{
							uiDescriptor |=  SD_LIGHT_DIRECTIONAL;
						}
					}
					break;
				}

				s_pPass[i] = FindShader(uiDescriptor);
				QDT_ASSERT(s_pPass[i]);
			}

			if (NeedSeparateDepthPass())
			{
				return (s_uiNbrOfPasses = uiNbrOfLights + 1);
			}
			else
			{
				return (s_uiNbrOfPasses = uiNbrOfLights);
			}
		}
		else
		{
			if (_bPSNeedSphericalHarmonics && DISPLAY_MANAGER::Instance()->GetCurrentViewport()->IsSHAmbientEnabled())
			{
				s_pPass[0] = FindShader(SD_AMBIENT_EMISSIVE_SPHERICAL_HARMONICS);
			}
			else
			{
				s_pPass[0] = FindShader(SD_AMBIENT_EMISSIVE);
			}
		}
	}
	else
	{
		s_pPass[0] = FindShader(SD_AMBIENT_EMISSIVE);
	}

	if (NeedSeparateDepthPass())
	{
		return (s_uiNbrOfPasses = 2);
	}
	else
	{
		return (s_uiNbrOfPasses = 1);
	}
}

//-----------------------------------------------------------------------------
//	Name:		FindShader
//	Object:		
//	05-12-13:	RMA - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER * SHADER_CUSTOM::FindShader(UInt32 uiDescription)
{
	for (UInt32 i = 0 ; i < _uiNbrOfShaderContainers ; ++i)
	{
		if (_pShaderContainerArray[i].GetDescription() == uiDescription)
		{
			return (_pShaderContainerArray[i].GetPixelShader());
		}
	}

	// The shader doesn't manager this type of light!
	// Please ask to someone of the engine team
	QDT_FAIL();

	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPasses
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetPass(UInt32 uiPass, 
							const VECTOR & Pos)
{
	Bool bNeedSeparateDepthPass = NeedSeparateDepthPass ();

	_uiCurrentPixelRegister = 0;
	_uiCurrentVertexShaderRegister = 0;

	if (uiPass == s_uiNbrOfPasses-1 && bNeedSeparateDepthPass == true)
	{
		// This is the last pass which write the depth in alpha for depth of field
		// I'm reusing the error shader which now support depth of field
		SHADER_ERROR * pShader = (SHADER_ERROR *)DISPLAY_MANAGER::Instance()->GetErrorShader().GetCommunicator();
		
		RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);
		RENDER_CONTEXT::EnableColorWrite(false, false, false, true);

		RENDER_CONTEXT::SetDepthTestFunc(CF_EQUAL);

		if	(DISPLAY_MANAGER::Instance()->IsSkinningEnabled())
		{
			RENDER_CONTEXT::SetVertexShader(pShader->GetSkinnedVertexShader());
			RENDER_CONTEXT::SetPixelShader(pShader->GetPixelShader());
	
			SetModelViewProj();
			SetFocalPlane();
			SetSkinningMatrices(); 
		}
		else
		{
			RENDER_CONTEXT::SetVertexShader(pShader->GetVertexShader());
			RENDER_CONTEXT::SetPixelShader(pShader->GetPixelShader());

			SetModelViewProj();
			SetFocalPlane();
		}

		return;
	}

	if (uiPass == 0 && bNeedSeparateDepthPass == false)
	{
		RENDER_CONTEXT::EnableColorWrite(true, true, true, true);
	}
	else
	{
		RENDER_CONTEXT::EnableColorWrite(true, true, true, false);
	}

	PrepareBlending(uiPass);
	
	RENDER_CONTEXT::SetPixelShader(s_pPass[uiPass]);

	if	(DISPLAY_MANAGER::Instance()->IsSkinningEnabled())
	{
		QDT_ASSERT(_pSkinnedVertexShader);
		RENDER_CONTEXT::SetVertexShader(_pSkinnedVertexShader);

		if (uiPass == 0)
		{
			SetModelViewProj();
			SetFocalPlane();
			SetEyePosition();
			SetSkinningMatrices();
		}
	}
	else
	{
		QDT_ASSERT(_pVertexShader);
		RENDER_CONTEXT::SetVertexShader(_pVertexShader);

		if (uiPass == 0)
		{
			SetModelViewProj();
			SetFocalPlane();
			SetEyePosition();
		}
	}

	SetTextures();
	PushConstants(uiPass, Pos);
	SetLights(uiPass);	
}

//-----------------------------------------------------------------------------
//	Name:		Restore
//	Object:		
//	06-02-23:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::Restore()
{
	SHADER_ABC::Restore();
	RENDER_CONTEXT::SetDepthTestFunc(CF_LESSEQUAL);
	RENDER_CONTEXT::EnableColorWrite(true, true, true, true);
}

//-----------------------------------------------------------------------------
//	Name:		SetModelViewProj
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetModelViewProj()
{
	const MATRIX & WorldViewProj = DISPLAY_MANAGER::Instance()->GetWorldViewProjMatrix();
	RENDER_CONTEXT::SetVertexShaderConstant(_uiCurrentVertexShaderRegister, WorldViewProj);
	_uiCurrentVertexShaderRegister += 4;
}

//-----------------------------------------------------------------------------
//	Name:		SetFocalPlane
//	Object:		
//	06-02-23:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::SetFocalPlane()
{
	DISPLAY_MANAGER * pDisplay = DISPLAY_MANAGER::Instance();

	CAMERA * pCam = pDisplay->GetActiveCamera();
	Float32 fFarDepthOfField = pCam->GetFarDepthOfField();

	MATRIX mViewToWorld = pCam->GetNode()->GetGlobalMatrix();
	MATRIX mWorldToObject = pDisplay->GetInvWorldMatrix();

	MATRIX mViewToObject;
	mViewToObject.Mul(mViewToWorld, mWorldToObject);

	PLANE CameraPlane(0.0f, 0.0f, -1.0f, 0.0f);
	CameraPlane.Transform(mViewToObject);

	Float32 f1, f2, f3, f4;
	CameraPlane.GetCoeff(f1, f2, f3, f4);

	RENDER_CONTEXT::SetVertexShaderConstant( _uiCurrentVertexShaderRegister++,
											f1 / fFarDepthOfField,
											f2 / fFarDepthOfField,
											f3 / fFarDepthOfField,
											f4 / fFarDepthOfField);
}

//-----------------------------------------------------------------------------
//	Name:		SetEyePosition
//	Object:		
//	06-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::SetEyePosition()
{
	if (_bVSNeedEyePosition)
	{
		const MATRIX & mWorldToObject = DISPLAY_MANAGER::Instance()->GetInvWorldMatrix();
		const MATRIX & mCameraToWorld = DISPLAY_MANAGER::Instance()->GetCameraNode()->GetGlobalMatrix();
		VECTOR vEyePos(VECTOR::ZERO);
		mCameraToWorld.TransformPosition(vEyePos);
		mWorldToObject.TransformPosition(vEyePos);
		RENDER_CONTEXT::SetVertexShaderConstant(_uiCurrentVertexShaderRegister++, vEyePos);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSkinningMatrices
//	Object:		
//	05-11-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::SetSkinningMatrices()
{
	DISPLAY_MANAGER *	pDisplay = DISPLAY_MANAGER::Instance();

	UInt32	i, j, Index = 0, nNbrMatrices = pDisplay->GetNbrSkinningMatrices();
	Float32	rValues[12 * QD_MAX_SKINNING_MATRIX];

	for	(i = 0 ; i < nNbrMatrices ; ++i)
	{
		const MATRIX & SkinningMatrix = pDisplay->GetSkinningMatrix(i);

		for	(j = 0 ; j < 3 ; ++j)
		{
			rValues[Index++] = SkinningMatrix(0, j);
			rValues[Index++] = SkinningMatrix(1, j);
			rValues[Index++] = SkinningMatrix(2, j);
			rValues[Index++] = SkinningMatrix(3, j);
		}
	}

	RENDER_CONTEXT::SetVertexShaderArrayConstant(_uiCurrentVertexShaderRegister, 12 * nNbrMatrices, &rValues[0]);
	_uiCurrentVertexShaderRegister += 3 * QD_MAX_SKINNING_MATRIX;
}

//-----------------------------------------------------------------------------
//	Name:		AddProperty
//	Object:		
//	06-02-08:	FBO - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::AddProperty(const QDT_STRING &	sName,
								   Float32				f1,
								   Float32				f2,
								   Float32				f3,
								   Float32				f4)
{
	MATERIAL_PROPERTY New;
	_vProperties.PushTail(New);
	_vProperties[_vProperties.GetSize() - 1]._sName = sName;
	_vProperties[_vProperties.GetSize() - 1]._f1 = f1;
	_vProperties[_vProperties.GetSize() - 1]._f2 = f2;
	_vProperties[_vProperties.GetSize() - 1]._f3 = f3;
	_vProperties[_vProperties.GetSize() - 1]._f4 = f4;
}

//-----------------------------------------------------------------------------
//	Name:		PushMaterialProperties
//	Object:		
//	06-02-08:	FBO - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::PushMaterialProperties()
{
	for (UInt32 i = 0 ; i < _vProperties.GetSize() ; ++i)
	{
		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, 
											   _vProperties[i]._f1,
											   _vProperties[i]._f2,
											   _vProperties[i]._f3,
											   _vProperties[i]._f4);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushConstants
//	Object:		
//	06-02-17:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::PushConstants(UInt32 uiPass, 
									 const VECTOR & Pos)
{
	if (uiPass == 0 && _bSupportLighting)
	{
		if (_bPSNeedSphericalHarmonics && DISPLAY_MANAGER::Instance()->GetCurrentViewport()->IsSHAmbientEnabled())
		{
			// Get scene graph
			CAMERA * pCamera = DISPLAY_MANAGER::Instance()->GetActiveCamera();
			SCENE_GRAPH * pSG = pCamera->GetSceneGraph();

			// Use normalization cube (for now)
			SPHERICAL_HARMONICS WorldSH;
			//WorldSH.InitNormal();

			pSG->GetLocalAmbient(Pos, WorldSH);

			// Multiply spherical harmonics by ambient color
			COLOR_FLOAT cAmbient = DISPLAY_MANAGER::Instance()->GetAmbientColor();
			WorldSH.MultiplyColor(cAmbient.GetRed(), cAmbient.GetGreen(), cAmbient.GetBlue());

			// Get world to object matrix
			MATRIX	WorldToObject = DISPLAY_MANAGER::Instance()->GetInvWorldMatrix();

			// Rotate spherical harmonic
			static SHROTATE_MATRIX RotateMatrix;
			SPHERICAL_HARMONICS ObjectSH;
			RotateMatrix.Ivanic_SHRotate(WorldToObject);
			RotateMatrix.Transform(&WorldSH, &ObjectSH);

			// Send coeffs to the shader
			Float32 coeffs[28];
			ObjectSH.ComputeSHIrradEnvMapConstants(coeffs);
			RENDER_CONTEXT::SetPixelShaderArrayConstant(_uiCurrentPixelRegister, 28, coeffs);
			_uiCurrentPixelRegister += 7;
		}
		else
		{
			RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, DISPLAY_MANAGER::Instance()->GetAmbientColor());
		}
	}

	PushMaterialProperties();

	if (_bPSNeedEyeVector)
	{
		const MATRIX & mCameraToWorld = DISPLAY_MANAGER::Instance()->GetCameraNode()->GetGlobalMatrix();
		const MATRIX & mWorldToObject = DISPLAY_MANAGER::Instance()->GetInvWorldMatrix();		

		VECTOR vEyeVector(VECTOR::UNIT_Z);
		mCameraToWorld.TransformDirection(vEyeVector);
		mWorldToObject.TransformDirection(vEyeVector);
		vEyeVector.Normalize();
		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, vEyeVector);
	}

	if (_bPSNeedEyePosition)
	{
		const MATRIX & mCameraToWorld = DISPLAY_MANAGER::Instance()->GetCameraNode()->GetGlobalMatrix();
		const MATRIX & mWorldToObject = DISPLAY_MANAGER::Instance()->GetInvWorldMatrix();

		VECTOR vEyePosition(VECTOR::ZERO);
		mCameraToWorld.TransformPosition(vEyePosition);
		mWorldToObject.TransformPosition(vEyePosition);
		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, vEyePosition);
	}

	if (_bPSNeedObjectToViewMatrix)
	{
		const MATRIX & mObjectToWorld = DISPLAY_MANAGER::Instance()->GetWorldMatrix();
		const MATRIX & mWorldToCamera = DISPLAY_MANAGER::Instance()->GetViewMatrix();

		MATRIX mObjectToCamera;
		mObjectToCamera.Mul(mObjectToWorld, mWorldToCamera);

		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister, mObjectToCamera);
		_uiCurrentPixelRegister += 4;
	}

	if (_bPSNeedObjectToWorldMatrix)
	{
		const MATRIX & mObjectToWorld = DISPLAY_MANAGER::Instance()->GetWorldMatrix();

		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister, mObjectToWorld);
		_uiCurrentPixelRegister += 4;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetLights
//	Object:		
//	05-11-21:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetLights(UInt32 uiPass)
{
	if (_bSupportLighting)
	{
		if (_bSupportLighting && DISPLAY_MANAGER::Instance()->GetNbrLights() > 0)
		{
			const S_LIGHT_SOURCE & LightSource = DISPLAY_MANAGER::Instance()->GetLight(uiPass);

			COLOR_FLOAT cAmbient = COLOR_FLOAT(1.0f, LightSource._rColorR, LightSource._rColorG, LightSource._rColorB) * LightSource._rIntensity * LightSource._rAmbient;
			COLOR_FLOAT cDiffuse = COLOR_FLOAT(1.0f, LightSource._rColorR, LightSource._rColorG, LightSource._rColorB) * LightSource._rIntensity * LightSource._rDiffuse;
			COLOR_FLOAT cSpecular = COLOR_FLOAT(1.0f, LightSource._rColorR, LightSource._rColorG, LightSource._rColorB) * LightSource._rIntensity * LightSource._rSpecular;

			switch (LightSource._eType)
			{
			default:
			case LIGHT_UNDEFINED:
				QDT_FAIL();
				break;

			case LIGHT_POINT:
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, LightSource._vPos);	
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, cAmbient);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, cDiffuse);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, cSpecular);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, VECTOR(LightSource._rInvSquareRadius, LightSource._rAttenuationPower, 0.0f));
				break;

			case LIGHT_SPOT:
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, LightSource._vPos);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, LightSource._vDir);	
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, MATH::Cos(LightSource._rSpotOuterAngle));
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, MATH::Cos(LightSource._rSpotInnerAngle));
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, cAmbient);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, cDiffuse);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, cSpecular);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, VECTOR(LightSource._rInvSquareRadius, LightSource._rAttenuationPower, 0.0f));
				break;

			case LIGHT_SPOT_SHADOW:
			case LIGHT_SPOT_HARDWARE_SHADOW:
			{
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, LightSource._vPos);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, LightSource._vDir);	
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, MATH::Cos(LightSource._rSpotOuterAngle));
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, MATH::Cos(LightSource._rSpotInnerAngle));
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, cAmbient);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, cDiffuse);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, cSpecular);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, VECTOR(LightSource._rInvSquareRadius, LightSource._rAttenuationPower, 0.0f));
				
				if (DISPLAY_MANAGER::Instance()->GetCurrentViewport()->AreShadowsEnabled() && _bSupportShadowing)
				{
					SetSpotShadow(&LightSource);
				}

				break;
			}

			case LIGHT_DIRECTIONAL:
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, LightSource._vDir);	
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, cDiffuse);
				RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, cSpecular);
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetTextures
//	Object:		
//	05-11-18:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetTextures()
{
	UInt32 i;
	for (i = 0 ; i < _uiNbrTextures ; ++i)
	{
		QDT_ASSERT(_pTextures[i].HasHandle());
		ENGINE_TEXTURE_ABC * pTex = (ENGINE_TEXTURE_ABC*)_pTextures[i].GetCommunicator();
		QDT_ASSERT(pTex);
		RENDER_CONTEXT::SetPixelShaderTexture(i, pTex->GetTexture());
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSupportLighting
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetSupportLighting(Bool b)
{
	_bSupportLighting = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrTextures
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetNbrTextures(UInt32 uiNbr)
{
	if (_uiNbrTextures != uiNbr || _pTextures == NULL)
	{
		if (_pTextures != NULL)
			delete [] _pTextures;

		_uiNbrTextures = uiNbr;
		if (_uiNbrTextures > 0)
			_pTextures = new COM_INSTANCE[_uiNbrTextures];
		else
			_pTextures = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrTextures
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetTexture(UInt32 i, 
							   const COM_INSTANCE & ciTexture)
{
	_pTextures[i] = ciTexture;
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * SHADER_CUSTOM::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_CUSTOM));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * SHADER_CUSTOM::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("SHADER_CUSTOM"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SHADER_CUSTOM,
										QDT_COMMENT("43044766-6DFA-F943-8BE3-26908EE51CD1"), 
										sizeof(SHADER_CUSTOM), 
										ComInit, 
										"",
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SHADCUST", 0, CM_ReadChunk0);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SHADCUST", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SHADCUST", 2, CM_ReadChunk2);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SHADCUST", 3, CM_ReadChunk3);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SHADCUST", 4, CM_ReadChunk4);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SHADCUST", 5, CM_ReadChunk5);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SHADCUST", 6, CM_ReadChunk6);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("SHADCUST", 6, CM_WriteChunk6);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::EM_Construct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	SHADER_CUSTOM	*pShader;
	pShader = static_cast<SHADER_CUSTOM *>(pO);

	QDT_NEW_PLACED(pShader, SHADER_CUSTOM)();
	QDT_ASSERT(pShader);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-11-09:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::EM_Destruct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	SHADER_CUSTOM	*pShader;
	pShader = static_cast<SHADER_CUSTOM *>(pO);

	QDT_DELETE_PLACED(pShader, SHADER_CUSTOM);
}

//-----------------------------------------------------------------------------
//	Name:		SetSkinnedVertexShader
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetSkinnedVertexShader(UInt8 * pShader, 
										   UInt32 uiSize)
{
	_uiSkinnedVertexShaderSize = uiSize;
	_pRawSkinnedVertexShader = pShader;
}
	
//-----------------------------------------------------------------------------
//	Name:		SetVertexShader
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetVertexShader(UInt8 * pShader, 
									UInt32 uiSize)
{
	_uiVertexShaderSize = uiSize;
	_pRawVertexShader = pShader;
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShader
//	Object:		
//	06-02-07:	FBO - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::SetVertexShader(I_VERTEX_SHADER *	pShader)
{
	_pVertexShader = pShader;
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexShader
//	Object:		Use only in maya
//	06-03-20:	FBO - Created
//-----------------------------------------------------------------------------
I_VERTEX_SHADER *	SHADER_CUSTOM::GetVertexShader()
{
	return _pVertexShader;
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShader
//	Object:		
//	06-02-07:	FBO - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::SetPixelShader(UInt32			uiIndex,
									  UInt32			uiDescription,
									  I_PIXEL_SHADER *	pShader)
{
	_pShaderContainerArray[uiIndex].SetDescription(uiDescription);
	_pShaderContainerArray[uiIndex].SetPixelShader(pShader);
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelShader
//	Object:		Use only in maya
//	06-03-20:	FBO - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER *	SHADER_CUSTOM::GetPixelShader(UInt32	uiIndex)
{
	QDT_ASSERT(uiIndex < _uiNbrOfShaderContainers);

	return _pShaderContainerArray[uiIndex].GetPixelShader();
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShader
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetPixelShader(UInt32 uiIndex, 
								   UInt32 uiDescription, 
								   UInt8 * pShader, 
								   UInt32 uiSize)
{
	_pPixelShaderSize[uiIndex] = uiSize;
	_pPixelShaderDescriptor[uiIndex] = uiDescription;
	_ppRawPixelShader[uiIndex] = pShader;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk0
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::CM_ReadChunk0(IO_STREAM & Stream, 
								  COMMUNICATOR * pCommunicator)
{
	SHADER_CUSTOM	*pShader;
	pShader = static_cast<SHADER_CUSTOM *>(pCommunicator);
	UInt32 i, j;
	Int32 iBlending;

	Stream >> iBlending; pShader->_eBlending = static_cast<SHADER_BLENDING>(iBlending);
	Stream >> pShader->_bEnableAlphaTest;
	Stream >> pShader->_rAlphaTestValue;
	Stream >> pShader->_bPSNeedEyeVector;
	Stream >> pShader->_bSupportLighting;
	Stream >> pShader->_uiNbrTextures;

	pShader->_pTextures = new COM_INSTANCE[pShader->_uiNbrTextures];

	for (i = 0 ; i < pShader->_uiNbrTextures ; ++i)
	{
		Stream >> pShader->_pTextures[i];
	}

	Stream >> pShader->_uiVertexShaderSize;
	if (pShader->_uiVertexShaderSize)
	{
		pShader->_pRawVertexShader = new UInt8 [pShader->_uiVertexShaderSize];
		for (i = 0 ; i < pShader->_uiVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawVertexShader[i];
		}
	}

	Stream >> pShader->_uiSkinnedVertexShaderSize;
	if (pShader->_uiSkinnedVertexShaderSize)
	{
		pShader->_pRawSkinnedVertexShader = new UInt8 [pShader->_uiSkinnedVertexShaderSize];
		for (i = 0 ; i < pShader->_uiSkinnedVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawSkinnedVertexShader[i];
		}
	}

	Stream >> pShader->_uiNbrOfShaderContainers;
	if (pShader->_uiNbrOfShaderContainers)
	{
		pShader->_pShaderContainerArray = new SHADER_CONTAINER[pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderSize = new UInt32 [pShader->_uiNbrOfShaderContainers];
		pShader->_ppRawPixelShader = new UInt8 * [pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderDescriptor = new UInt32 [pShader->_uiNbrOfShaderContainers];

		for (i = 0 ; i < pShader->_uiNbrOfShaderContainers ; ++i)
		{
			Stream >> pShader->_pPixelShaderSize[i];
			Stream >> pShader->_pPixelShaderDescriptor[i];
			pShader->_ppRawPixelShader[i] = new UInt8 [pShader->_pPixelShaderSize[i]];
			for (j = 0 ; j < pShader->_pPixelShaderSize[i] ; ++j)
			{	
				Stream >> pShader->_ppRawPixelShader[i][j];
			}
		}
	}

	pShader->CreateShaders();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	06-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::CM_ReadChunk1(IO_STREAM &	Stream,
									 COMMUNICATOR *	pCommunicator)
{
	SHADER_CUSTOM	*pShader;
	pShader = static_cast<SHADER_CUSTOM *>(pCommunicator);
	UInt32 i, j;
	Int32 iBlending;

	Stream >> iBlending; pShader->_eBlending = static_cast<SHADER_BLENDING>(iBlending);
	Stream >> pShader->_bEnableAlphaTest;
	Stream >> pShader->_rAlphaTestValue;
	Stream >> pShader->_bPSNeedEyeVector;
	Stream >> pShader->_bVSNeedEyePosition;
	Stream >> pShader->_bSupportLighting;
	Stream >> pShader->_uiNbrTextures;

	pShader->_pTextures = new COM_INSTANCE[pShader->_uiNbrTextures];

	for (i = 0 ; i < pShader->_uiNbrTextures ; ++i)
	{
		Stream >> pShader->_pTextures[i];
	}

	Stream >> pShader->_uiVertexShaderSize;
	if (pShader->_uiVertexShaderSize)
	{
		pShader->_pRawVertexShader = new UInt8 [pShader->_uiVertexShaderSize];
		for (i = 0 ; i < pShader->_uiVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawVertexShader[i];
		}
	}

	Stream >> pShader->_uiSkinnedVertexShaderSize;
	if (pShader->_uiSkinnedVertexShaderSize)
	{
		pShader->_pRawSkinnedVertexShader = new UInt8 [pShader->_uiSkinnedVertexShaderSize];
		for (i = 0 ; i < pShader->_uiSkinnedVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawSkinnedVertexShader[i];
		}
	}

	Stream >> pShader->_uiNbrOfShaderContainers;
	if (pShader->_uiNbrOfShaderContainers)
	{
		pShader->_pShaderContainerArray = new SHADER_CONTAINER[pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderSize = new UInt32 [pShader->_uiNbrOfShaderContainers];
		pShader->_ppRawPixelShader = new UInt8 * [pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderDescriptor = new UInt32 [pShader->_uiNbrOfShaderContainers];

		for (i = 0 ; i < pShader->_uiNbrOfShaderContainers ; ++i)
		{
			Stream >> pShader->_pPixelShaderSize[i];
			Stream >> pShader->_pPixelShaderDescriptor[i];
			pShader->_ppRawPixelShader[i] = new UInt8 [pShader->_pPixelShaderSize[i]];
			for (j = 0 ; j < pShader->_pPixelShaderSize[i] ; ++j)
			{	
				Stream >> pShader->_ppRawPixelShader[i][j];
			}
		}
	}

	pShader->CreateShaders();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk2
//	Object:		
//	06-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::CM_ReadChunk2(IO_STREAM &	Stream,
									 COMMUNICATOR *	pCommunicator)
{
	SHADER_CUSTOM	*pShader;
	pShader = static_cast<SHADER_CUSTOM *>(pCommunicator);
	UInt32 i, j;
	Int32 iBlending;

	Stream >> iBlending; pShader->_eBlending = static_cast<SHADER_BLENDING>(iBlending);
	Stream >> pShader->_bEnableAlphaTest;
	Stream >> pShader->_rAlphaTestValue;
	Stream >> pShader->_bVSNeedEyePosition;
	Stream >> pShader->_bPSNeedEyeVector;
	Stream >> pShader->_bPSNeedEyePosition;
	Stream >> pShader->_bPSNeedObjectToViewMatrix;
	pShader->_bPSNeedObjectToWorldMatrix = false;
	Stream >> pShader->_bSupportLighting;
	Stream >> pShader->_uiNbrTextures;
	
	pShader->_pTextures = new COM_INSTANCE[pShader->_uiNbrTextures];

	for (i = 0 ; i < pShader->_uiNbrTextures ; ++i)
	{
		Stream >> pShader->_pTextures[i];
	}

	Stream >> pShader->_uiVertexShaderSize;
	if (pShader->_uiVertexShaderSize)
	{
		pShader->_pRawVertexShader = new UInt8 [pShader->_uiVertexShaderSize];
		for (i = 0 ; i < pShader->_uiVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawVertexShader[i];
		}
	}

	Stream >> pShader->_uiSkinnedVertexShaderSize;
	if (pShader->_uiSkinnedVertexShaderSize)
	{
		pShader->_pRawSkinnedVertexShader = new UInt8 [pShader->_uiSkinnedVertexShaderSize];
		for (i = 0 ; i < pShader->_uiSkinnedVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawSkinnedVertexShader[i];
		}
	}

	Stream >> pShader->_uiNbrOfShaderContainers;
	if (pShader->_uiNbrOfShaderContainers)
	{
		pShader->_pShaderContainerArray = new SHADER_CONTAINER[pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderSize = new UInt32 [pShader->_uiNbrOfShaderContainers];
		pShader->_ppRawPixelShader = new UInt8 * [pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderDescriptor = new UInt32 [pShader->_uiNbrOfShaderContainers];

		for (i = 0 ; i < pShader->_uiNbrOfShaderContainers ; ++i)
		{
			Stream >> pShader->_pPixelShaderSize[i];
			Stream >> pShader->_pPixelShaderDescriptor[i];
			pShader->_ppRawPixelShader[i] = new UInt8 [pShader->_pPixelShaderSize[i]];
			for (j = 0 ; j < pShader->_pPixelShaderSize[i] ; ++j)
			{	
				Stream >> pShader->_ppRawPixelShader[i][j];
			}
		}
	}

	pShader->CreateShaders();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk3
//	Object:		
//	06-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::CM_ReadChunk3(IO_STREAM &	Stream,
									 COMMUNICATOR *	pCommunicator)
{
	SHADER_CUSTOM	*pShader;
	pShader = static_cast<SHADER_CUSTOM *>(pCommunicator);
	UInt32 i, j;
	Int32 iBlending;

	Stream >> iBlending; pShader->_eBlending = static_cast<SHADER_BLENDING>(iBlending);
	Stream >> pShader->_bEnableAlphaTest;
	Stream >> pShader->_rAlphaTestValue;
	Stream >> pShader->_bVSNeedEyePosition;
	Stream >> pShader->_bPSNeedEyeVector;
	Stream >> pShader->_bPSNeedEyePosition;
	Stream >> pShader->_bPSNeedObjectToViewMatrix;
	Stream >> pShader->_bPSNeedObjectToWorldMatrix;
	Stream >> pShader->_bSupportLighting;
	Stream >> pShader->_uiNbrTextures;
	
	pShader->_pTextures = new COM_INSTANCE[pShader->_uiNbrTextures];

	for (i = 0 ; i < pShader->_uiNbrTextures ; ++i)
	{
		Stream >> pShader->_pTextures[i];
	}

	Stream >> pShader->_uiVertexShaderSize;
	if (pShader->_uiVertexShaderSize)
	{
		pShader->_pRawVertexShader = new UInt8 [pShader->_uiVertexShaderSize];
		for (i = 0 ; i < pShader->_uiVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawVertexShader[i];
		}
	}

	Stream >> pShader->_uiSkinnedVertexShaderSize;
	if (pShader->_uiSkinnedVertexShaderSize)
	{
		pShader->_pRawSkinnedVertexShader = new UInt8 [pShader->_uiSkinnedVertexShaderSize];
		for (i = 0 ; i < pShader->_uiSkinnedVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawSkinnedVertexShader[i];
		}
	}

	Stream >> pShader->_uiNbrOfShaderContainers;
	if (pShader->_uiNbrOfShaderContainers)
	{
		pShader->_pShaderContainerArray = new SHADER_CONTAINER[pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderSize = new UInt32 [pShader->_uiNbrOfShaderContainers];
		pShader->_ppRawPixelShader = new UInt8 * [pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderDescriptor = new UInt32 [pShader->_uiNbrOfShaderContainers];

		for (i = 0 ; i < pShader->_uiNbrOfShaderContainers ; ++i)
		{
			Stream >> pShader->_pPixelShaderSize[i];
			Stream >> pShader->_pPixelShaderDescriptor[i];
			pShader->_ppRawPixelShader[i] = new UInt8 [pShader->_pPixelShaderSize[i]];
			for (j = 0 ; j < pShader->_pPixelShaderSize[i] ; ++j)
			{	
				Stream >> pShader->_ppRawPixelShader[i][j];
			}
		}
	}

	pShader->CreateShaders();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk4
//	Object:		
//	06-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::CM_ReadChunk4(IO_STREAM &	Stream,
									 COMMUNICATOR *	pCommunicator)
{
	SHADER_CUSTOM	*pShader;
	pShader = static_cast<SHADER_CUSTOM *>(pCommunicator);
	UInt32 i, j;
	Int32 iBlending;

	Stream >> iBlending; pShader->_eBlending = static_cast<SHADER_BLENDING>(iBlending);
	Stream >> pShader->_bEnableAlphaTest;
	Stream >> pShader->_bDoubleSided;
	Stream >> pShader->_bDoubleRendering;
	Stream >> pShader->_rAlphaTestValue;
	Stream >> pShader->_bVSNeedEyePosition;
	Stream >> pShader->_bPSNeedEyeVector;
	Stream >> pShader->_bPSNeedEyePosition;
	Stream >> pShader->_bPSNeedObjectToViewMatrix;
	Stream >> pShader->_bPSNeedObjectToWorldMatrix;
	Stream >> pShader->_bSupportLighting;
	Stream >> pShader->_uiNbrTextures;
	
	pShader->_pTextures = new COM_INSTANCE[pShader->_uiNbrTextures];

	for (i = 0 ; i < pShader->_uiNbrTextures ; ++i)
	{
		Stream >> pShader->_pTextures[i];
	}

	Stream >> pShader->_uiVertexShaderSize;
	if (pShader->_uiVertexShaderSize)
	{
		pShader->_pRawVertexShader = new UInt8 [pShader->_uiVertexShaderSize];
		for (i = 0 ; i < pShader->_uiVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawVertexShader[i];
		}
	}

	Stream >> pShader->_uiSkinnedVertexShaderSize;
	if (pShader->_uiSkinnedVertexShaderSize)
	{
		pShader->_pRawSkinnedVertexShader = new UInt8 [pShader->_uiSkinnedVertexShaderSize];
		for (i = 0 ; i < pShader->_uiSkinnedVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawSkinnedVertexShader[i];
		}
	}

	Stream >> pShader->_uiNbrOfShaderContainers;
	if (pShader->_uiNbrOfShaderContainers)
	{
		pShader->_pShaderContainerArray = new SHADER_CONTAINER[pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderSize = new UInt32 [pShader->_uiNbrOfShaderContainers];
		pShader->_ppRawPixelShader = new UInt8 * [pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderDescriptor = new UInt32 [pShader->_uiNbrOfShaderContainers];

		for (i = 0 ; i < pShader->_uiNbrOfShaderContainers ; ++i)
		{
			Stream >> pShader->_pPixelShaderSize[i];
			Stream >> pShader->_pPixelShaderDescriptor[i];
			pShader->_ppRawPixelShader[i] = new UInt8 [pShader->_pPixelShaderSize[i]];
			for (j = 0 ; j < pShader->_pPixelShaderSize[i] ; ++j)
			{	
				Stream >> pShader->_ppRawPixelShader[i][j];
			}
		}
	}

	pShader->CreateShaders();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk5
//	Object:		
//	06-01-15:	PDE - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::CM_ReadChunk5(IO_STREAM &	Stream,
									 COMMUNICATOR *	pCommunicator)
{
	SHADER_CUSTOM	*pShader;
	pShader = static_cast<SHADER_CUSTOM *>(pCommunicator);
	UInt32 i, j;
	Int32 iBlending;

	Stream >> iBlending; pShader->_eBlending = static_cast<SHADER_BLENDING>(iBlending);
	Stream >> pShader->_bEnableAlphaTest;
	Stream >> pShader->_bDoubleSided;
	Stream >> pShader->_bDoubleRendering;
	Stream >> pShader->_bPSNeedSphericalHarmonics;
	Stream >> pShader->_rAlphaTestValue;
	Stream >> pShader->_bVSNeedEyePosition;
	Stream >> pShader->_bPSNeedEyeVector;
	Stream >> pShader->_bPSNeedEyePosition;
	Stream >> pShader->_bPSNeedObjectToViewMatrix;
	Stream >> pShader->_bPSNeedObjectToWorldMatrix;
	Stream >> pShader->_bSupportLighting;
	Stream >> pShader->_uiNbrTextures;
	
	pShader->_pTextures = new COM_INSTANCE[pShader->_uiNbrTextures];

	for (i = 0 ; i < pShader->_uiNbrTextures ; ++i)
	{
		Stream >> pShader->_pTextures[i];
	}

	Stream >> pShader->_uiVertexShaderSize;
	if (pShader->_uiVertexShaderSize)
	{
		pShader->_pRawVertexShader = new UInt8 [pShader->_uiVertexShaderSize];
		for (i = 0 ; i < pShader->_uiVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawVertexShader[i];
		}
	}

	Stream >> pShader->_uiSkinnedVertexShaderSize;
	if (pShader->_uiSkinnedVertexShaderSize)
	{
		pShader->_pRawSkinnedVertexShader = new UInt8 [pShader->_uiSkinnedVertexShaderSize];
		for (i = 0 ; i < pShader->_uiSkinnedVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawSkinnedVertexShader[i];
		}
	}

	Stream >> pShader->_uiNbrOfShaderContainers;
	if (pShader->_uiNbrOfShaderContainers)
	{
		pShader->_pShaderContainerArray = new SHADER_CONTAINER[pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderSize = new UInt32 [pShader->_uiNbrOfShaderContainers];
		pShader->_ppRawPixelShader = new UInt8 * [pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderDescriptor = new UInt32 [pShader->_uiNbrOfShaderContainers];

		for (i = 0 ; i < pShader->_uiNbrOfShaderContainers ; ++i)
		{
			Stream >> pShader->_pPixelShaderSize[i];
			Stream >> pShader->_pPixelShaderDescriptor[i];
			pShader->_ppRawPixelShader[i] = new UInt8 [pShader->_pPixelShaderSize[i]];
			for (j = 0 ; j < pShader->_pPixelShaderSize[i] ; ++j)
			{	
				Stream >> pShader->_ppRawPixelShader[i][j];
			}
		}
	}

	pShader->CreateShaders();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk6
//	Object:		
//	06-03-30:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::CM_ReadChunk6(IO_STREAM &	Stream,
									 COMMUNICATOR *	pCommunicator)
{
	SHADER_CUSTOM	*pShader;
	pShader = static_cast<SHADER_CUSTOM *>(pCommunicator);
	UInt32 i, j;
	Int32 iBlending;

	Stream >> iBlending; pShader->_eBlending = static_cast<SHADER_BLENDING>(iBlending);
	Stream >> pShader->_bEnableAlphaTest;
	Stream >> pShader->_bDoubleSided;
	Stream >> pShader->_bDoubleRendering;
	Stream >> pShader->_bPSNeedSphericalHarmonics;
	Stream >> pShader->_rAlphaTestValue;
	Stream >> pShader->_bVSNeedEyePosition;
	Stream >> pShader->_bPSNeedEyeVector;
	Stream >> pShader->_bPSNeedEyePosition;
	Stream >> pShader->_bPSNeedObjectToViewMatrix;
	Stream >> pShader->_bPSNeedObjectToWorldMatrix;
	Stream >> pShader->_bSupportLighting;
	Stream >> pShader->_uiTransparencyPriority;
	Stream >> pShader->_uiNbrTextures;
	
	pShader->_pTextures = new COM_INSTANCE[pShader->_uiNbrTextures];

	for (i = 0 ; i < pShader->_uiNbrTextures ; ++i)
	{
		Stream >> pShader->_pTextures[i];
	}

	Stream >> pShader->_uiVertexShaderSize;
	if (pShader->_uiVertexShaderSize)
	{
		pShader->_pRawVertexShader = new UInt8 [pShader->_uiVertexShaderSize];
		for (i = 0 ; i < pShader->_uiVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawVertexShader[i];
		}
	}

	Stream >> pShader->_uiSkinnedVertexShaderSize;
	if (pShader->_uiSkinnedVertexShaderSize)
	{
		pShader->_pRawSkinnedVertexShader = new UInt8 [pShader->_uiSkinnedVertexShaderSize];
		for (i = 0 ; i < pShader->_uiSkinnedVertexShaderSize ; ++i)
		{
			Stream >> pShader->_pRawSkinnedVertexShader[i];
		}
	}

	Stream >> pShader->_uiNbrOfShaderContainers;
	if (pShader->_uiNbrOfShaderContainers)
	{
		pShader->_pShaderContainerArray = new SHADER_CONTAINER[pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderSize = new UInt32 [pShader->_uiNbrOfShaderContainers];
		pShader->_ppRawPixelShader = new UInt8 * [pShader->_uiNbrOfShaderContainers];
		pShader->_pPixelShaderDescriptor = new UInt32 [pShader->_uiNbrOfShaderContainers];

		for (i = 0 ; i < pShader->_uiNbrOfShaderContainers ; ++i)
		{
			Stream >> pShader->_pPixelShaderSize[i];
			Stream >> pShader->_pPixelShaderDescriptor[i];
			pShader->_ppRawPixelShader[i] = new UInt8 [pShader->_pPixelShaderSize[i]];
			for (j = 0 ; j < pShader->_pPixelShaderSize[i] ; ++j)
			{	
				Stream >> pShader->_ppRawPixelShader[i][j];
			}
		}
	}

	pShader->CreateShaders();
}
	
//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk6
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::CM_WriteChunk6(IO_STREAM & Stream, 
								   COMMUNICATOR * pCommunicator)
{
	SHADER_CUSTOM	*pShader;
	pShader = static_cast<SHADER_CUSTOM *>(pCommunicator);
	UInt32 i, j;

	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("SHADER_CUSTOM :");
	Stream.DebugOutput(pShader->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Blending : ");
	Stream << pShader->_eBlending;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Alpha Test : ");
	Stream << pShader->_bEnableAlphaTest;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Double Sided : ");
	Stream << pShader->_bDoubleSided;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Double Rendering : ");
	Stream << pShader->_bDoubleRendering;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Spherical harmonics : ");
	Stream << pShader->_bPSNeedSphericalHarmonics;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    AlphaTestValue : ");
	Stream << pShader->_rAlphaTestValue;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    VS Need eye pos : ");
	Stream << pShader->_bVSNeedEyePosition;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    PS Need eye vector : ");
	Stream << pShader->_bPSNeedEyeVector;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    PS Need eye position : ");
	Stream << pShader->_bPSNeedEyePosition;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("    PS Need object to space view matrix : ");
	Stream << pShader->_bPSNeedObjectToViewMatrix;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    PS Need object to space world matrix : ");
	Stream << pShader->_bPSNeedObjectToWorldMatrix;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Support lighting : ");
	Stream << pShader->_bSupportLighting;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Transparency Priority : ");
	Stream << pShader->_uiTransparencyPriority;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Number of textures : ");
	Stream << pShader->_uiNbrTextures;
	Stream.DebugOutput("\r\n");

	for (i = 0 ; i < pShader->_uiNbrTextures ; ++i)
	{
		Stream << pShader->_pTextures[i];
		Stream.DebugOutput("\r\n");
	}

	if (Stream.IsFileTextStreamDebug() == false)
	{
		Stream << pShader->_uiVertexShaderSize;
		for (i = 0 ; i < pShader->_uiVertexShaderSize ; ++i)
		{
			Stream << pShader->_pRawVertexShader[i];
		}

		Stream << pShader->_uiSkinnedVertexShaderSize;
		for (i = 0 ; i < pShader->_uiSkinnedVertexShaderSize ; ++i)
		{
			Stream << pShader->_pRawSkinnedVertexShader[i];
		}

		Stream << pShader->_uiNbrOfShaderContainers;

		for (i = 0 ; i < pShader->_uiNbrOfShaderContainers ; ++i)
		{
			Stream << pShader->_pPixelShaderSize[i];
			Stream << pShader->_pPixelShaderDescriptor[i];
			for (j = 0 ; j < pShader->_pPixelShaderSize[i] ; ++j)
			{	
				Stream << pShader->_ppRawPixelShader[i][j];
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CreateShaders
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::CreateShaders()
{
#if defined (PLATFORM_PC)
	if (RENDER_CONTEXT::SupportShaderModel(SM_D3D_VS_1_1) && RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_2_0))
#endif
	{
		if (_uiVertexShaderSize > 0)
		{
			_pVertexShader			= RENDER_CONTEXT::CreateVertexShader(_pRawVertexShader);
		}

		if (_uiSkinnedVertexShaderSize > 0)
		{
			_pSkinnedVertexShader	= RENDER_CONTEXT::CreateVertexShader(_pRawSkinnedVertexShader);
		}

		if (_uiNbrOfShaderContainers > 0)
		{
			for (UInt32 i = 0 ; i < _uiNbrOfShaderContainers ; ++i)
			{
				_pShaderContainerArray[i].SetPixelShader(RENDER_CONTEXT::CreatePixelShader(_ppRawPixelShader[i]));
				_pShaderContainerArray[i].SetDescription(_pPixelShaderDescriptor[i]);
			}
		}
	}

	delete [] _pRawVertexShader;
	_pRawVertexShader = NULL;
	delete [] _pRawSkinnedVertexShader;
	_pRawSkinnedVertexShader = NULL;

	for (UInt32 i = 0 ; i < _uiNbrOfShaderContainers ; ++i)
	{
		delete [] _ppRawPixelShader[i];
	}

	delete [] _ppRawPixelShader;
	_ppRawPixelShader = NULL;
	delete [] _pPixelShaderSize;
	_pPixelShaderSize = NULL;
	delete [] _pPixelShaderDescriptor;
	_pPixelShaderDescriptor = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		SetVSNeedEyePosition
//	Object:		
//	06-01-24:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetVSNeedEyePosition(Bool b)
{
	_bVSNeedEyePosition = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetPSNeedEyeVector
//	Object:		
//	05-11-23:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetPSNeedEyeVector(Bool b)
{
	_bPSNeedEyeVector = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetPSNeedEyePosition
//	Object:		
//	06-01-24:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::SetPSNeedEyePosition(Bool	b)
{
	_bPSNeedEyePosition = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetPSNeedObjectToViewMatrix
//	Object:		
//	06-01-24:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::SetPSNeedObjectToViewMatrix(Bool	b)
{
	_bPSNeedObjectToViewMatrix = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetPSNeedObjectToWorldMatrix
//	Object:		
//	06-01-24:	RMA - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::SetPSNeedObjectToWorldMatrix(Bool	b)
{
	_bPSNeedObjectToWorldMatrix = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetNumberOfContainer
//	Object:		
//	05-12-13:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::SetNbrOfShaderContainer(UInt32 uiNbrOfShaderContainers)
{
	if (_uiNbrOfShaderContainers != uiNbrOfShaderContainers)
	{
		if (_pPixelShaderSize != NULL)
			delete _pPixelShaderSize;
		_pPixelShaderSize = new UInt32[uiNbrOfShaderContainers];
		
		if (_pPixelShaderDescriptor != NULL)
			delete _pPixelShaderDescriptor;
		_pPixelShaderDescriptor = new UInt32[uiNbrOfShaderContainers];
		
		if (_ppRawPixelShader != NULL)
		{
			for (UInt32 i = 0 ; i < _uiNbrOfShaderContainers ; ++i)
			{
				if (_ppRawPixelShader[i] != NULL)
					delete _ppRawPixelShader[i];
			}
			delete _ppRawPixelShader;
		}
		_ppRawPixelShader = new UInt8 *[uiNbrOfShaderContainers];
		for (UInt32 i = 0 ; i < uiNbrOfShaderContainers ; ++i)
			_ppRawPixelShader[i] = NULL;

		_uiNbrOfShaderContainers = uiNbrOfShaderContainers;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSpotShadow
//	Object:		
//	06-02-16:	SBE - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::SetSpotShadow(const S_LIGHT_SOURCE * pLightSource)
{
	// Set Shadow Map render target
	I_RENDER_TARGET * pRenderTarget = NULL;
	I_RENDER_TARGET * pDepthBuffer = pLightSource->_pShadowAttributes->_pAssociatedDepthRenderTargets->pDepthBuffer;
	Bool bHWShadowMap = false;

	if (pDepthBuffer->GetPixelFormat() == PF_D24S8)
	{
		pRenderTarget = pDepthBuffer;
		bHWShadowMap = true;
		//pRenderTarget->GetTexture()->SetFilterType(FT_BILINEAR);
	}				
	else
	{
		pRenderTarget = pLightSource->_pShadowAttributes->_pAssociatedDepthRenderTargets->pRenderTarget;
	}

	QDT_ASSERT(pRenderTarget);
	
	RENDER_CONTEXT::SetPixelShaderTexture(_uiNbrTextures, pRenderTarget->GetTexture());

	// Compute Matrix
	QDT_ASSERT(pLightSource->_pShadowAttributes);
	MATRIX Temp, ModelLightViewProj;

	Temp.Mul(DISPLAY_MANAGER::Instance()->GetWorldMatrix(), pLightSource->_pShadowAttributes->_mView);

	// Specific matrix hack
	if (bHWShadowMap == false) 
	{
		ModelLightViewProj.Mul(Temp, pLightSource->_pShadowAttributes->_mProj);

		if (RENDER_CONTEXT::GetRenderContextType() == RENDER_CONTEXT::RCT_DIRECT3D9)
		{
			MATRIX InvertYMat;

			InvertYMat.Identity();
			InvertYMat(1,1) *= -1.f;
			ModelLightViewProj.Mul(InvertYMat);
		}

		// Set Matrix
		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister, ModelLightViewProj);
		_uiCurrentPixelRegister += 4;

		// Set Bias

		#ifdef PLATFORM_PS3
			static Float32 rBiasFactor = 0.001f;
		#else
			static Float32 rBiasFactor;
		#endif

		#ifdef PLATFORM_PC
			switch (RENDER_CONTEXT::GetRenderContextType())
			{
				default:
				case RENDER_CONTEXT::RCT_UNDEFINED:
					QDT_FAIL();

				case RENDER_CONTEXT::RCT_DIRECT3D9:
				{
					rBiasFactor = 0.0017f;				
				}
				break;

				case RENDER_CONTEXT::RCT_OPENGL:
				case RENDER_CONTEXT::RCT_OPENGL_FOR_MAYA:
				{
					rBiasFactor = 0.001f;				
				}
			break;
			}
		#endif

		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, pLightSource->_rBias * rBiasFactor);

		// Shadow Color
		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, pLightSource->_rShadowR, pLightSource->_rShadowG, pLightSource->_rShadowB, 1.0f);				
	}
	else
	{
		MATRIX Temp2;

		// Set Bias	

		#ifdef PLATFORM_PS3
			static Float32 rBiasFactor = 1.f; //0.001f; //0.005f; NE PAS ARCHIVER !
		#else
			static Float32 rBiasFactor;
		#endif

		#ifdef PLATFORM_PC
			switch (RENDER_CONTEXT::GetRenderContextType())
			{
				default:
				case RENDER_CONTEXT::RCT_UNDEFINED:
					QDT_FAIL();

				case RENDER_CONTEXT::RCT_DIRECT3D9:
				{
					rBiasFactor = 0.001f;
				}
				break;

				case RENDER_CONTEXT::RCT_OPENGL:
				case RENDER_CONTEXT::RCT_OPENGL_FOR_MAYA:
				{
					rBiasFactor = 0.005f;
				}
			break;
			}
		#endif

		Float32 fOffsetX = 0.5f + (0.5f / (Float32)pRenderTarget->GetWidth());
		Float32 fOffsetY = 0.5f + (0.5f / (Float32)pRenderTarget->GetHeight());
		UInt32 rRange = 1;

		// NE PAS ARCHIVER !!
		Float32 rBias = pLightSource->_rBias * rBiasFactor,
				rSign = 1.0f;

		#ifndef PLATFORM_PS3
			if (RENDER_CONTEXT::GetRenderContextType() == RENDER_CONTEXT::RCT_DIRECT3D9)
				rSign = -1.0f;
		#endif

		MATRIX _mTexScaleBiasMat(0.5f,		0.0f,			0.0f,					0.0f,
								 0.0f,		rSign * 0.5f,   0.0f,					0.0f,
								 0.0f,		0.0f,			(Float32) rRange,		0.0f,
								 fOffsetX,	fOffsetY,		rBias/*0.0f/*rSign * rBias*/,	1.0f );


//		MATRIX _mTexScaleBiasMat(0.5f,		0.0f,			0.0f,				0.0f,
//								 0.0f,		0.5f,			0.0f,				0.0f,
//								 0.0f,		0.0f,			(Float32) rRange,	0.0f,
//								 fOffsetX,	fOffsetY,		0.0f/*-rBias*/,				1.0f );

		Temp2.Mul(Temp, pLightSource->_pShadowAttributes->_mProj);
		ModelLightViewProj.Mul(Temp2, _mTexScaleBiasMat);

		// Set Matrix
		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister, ModelLightViewProj);
		_uiCurrentPixelRegister += 4;

		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, pLightSource->_rBias * rBiasFactor);

		// Shadow Color
		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, pLightSource->_rShadowR, pLightSource->_rShadowG, pLightSource->_rShadowB, 1.0f);				

		static Float32 rBlurRadiusFactor = 1.f; //0.25f;
		RENDER_CONTEXT::SetPixelShaderConstant(_uiCurrentPixelRegister++, rBlurRadiusFactor * pLightSource->_rShadowBlurRadius, rBlurRadiusFactor * pLightSource->_rShadowBlurRadius, 0.f, 0.f);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AllocateContainers
//	Object:		
//	06-02-07:	FBO - Created
//-----------------------------------------------------------------------------
void SHADER_CUSTOM::AllocateContainers()
{
	// Call SetNbrOfShaderContainer before this methods
	QDT_ASSERT(_uiNbrOfShaderContainers > 0);

	if (_pShaderContainerArray != NULL)
		delete _pShaderContainerArray;
	_pShaderContainerArray = new SHADER_CONTAINER[_uiNbrOfShaderContainers];
}

//-----------------------------------------------------------------------------
//	Name:		SetPSNeedSphericalHarmonics
//	Object:		
//	06-03-08:	PDE - Created
//-----------------------------------------------------------------------------
void	SHADER_CUSTOM::SetPSNeedSphericalHarmonics(Bool	b)
{
	_bPSNeedSphericalHarmonics = b;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
