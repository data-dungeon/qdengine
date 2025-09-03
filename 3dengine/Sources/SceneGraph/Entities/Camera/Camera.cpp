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
// CLASS: CAMERA
//
//
//	00-10-27	JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION ( CAMERA )
//
//	The CAMERA class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(PostProcessing/ViewportState)
#include	INCL_3DENGINE(SceneGraph/Entities/Camera/Camera)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(SceneGraph/Node/Node)

#ifdef _DEBUG
	#include	"Camera.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		CAMERA constructor
//	Object:		Default values 
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
CAMERA::CAMERA(COM_HANDLE *	pCH)
:
ENTITY_ABC					(GetGenericCommunicatorDescriptor(), ENTITY_CAMERA, pCH),
_fFocal						(CAMERA_DEFAULT_FOCAL),
_fHorizontalFieldOfView		(CAMERA_DEFAULT_HORIZONTAL_FIELD_OF_VIEW),
_fAspectRatio				(CAMERA_DEFAULT_ASPECT_RATIO),
_fScale						(CAMERA_DEFAULT_SCALE),
_fNearClippingDistance		(CAMERA_DEFAULT_NEAR_CLIPPING_DISTANCE),
_fFarClippingDistance		(CAMERA_DEFAULT_FAR_CLIPPING_DISTANCE),
_bEnableDepthOfField		(CAMERA_DEFAULT_ENABLE_DEPTH_OF_FIELD),
_fFarDepthOfField			(CAMERA_DEFAULT_FAR_DEPTH_OF_FIELD),
_fFocusPlane				(CAMERA_DEFAULT_FOCUS_PLANE),
_fFocusRange				(CAMERA_DEFAULT_FOCUS_RANGE),
_fFocusSafeRange            (CAMERA_DEFAULT_FOCUS_SAFE_RANGE),
_fCircleOfConfusion			(CAMERA_DEFAULT_CIRCLE_OF_CONFUSION),
_fExposure					(CAMERA_DEFAULT_EXPOSURE),
_fBrightBlurRadius			(CAMERA_DEFAULT_BLUR_RADIUS),
_fBrightThreshold			(CAMERA_DEFAULT_BRIGHT_THRESHOLD),
_fBrightIntensity			(CAMERA_DEFAULT_BRIGHT_INTENSITY),
_bEnableBloom				(CAMERA_DEFAULT_ENABLE_BLOOM),		
_bEnableGhosts				(CAMERA_DEFAULT_ENABLE_GHOSTS),	
_fGhostsDiffraction			(CAMERA_DEFAULT_GHOSTS_DIFFRACTION),	
_fGhostsIntensity			(CAMERA_DEFAULT_GHOSTS_INTENSITY),
_bEnableStars				(CAMERA_DEFAULT_ENABLE_STARS),
_uiStarsNbrBranches			(CAMERA_DEFAULT_STARS_NBR_BRANCHES),
_fStarsDiffraction			(CAMERA_DEFAULT_STARS_DIFFRACTION),
_fStarsIntensity			(CAMERA_DEFAULT_STARS_INTENSITY),
_fStarsSize					(CAMERA_DEFAULT_STARS_SIZE),
_fStarsAngle				(CAMERA_DEFAULT_STARS_ANGLE),
_bEnableNoise				(CAMERA_DEFAULT_ENABLE_NOISE),
_fNoiseCutoff				(CAMERA_DEFAULT_NOISE_CUTOFF),
_fNoiseIntensity			(CAMERA_DEFAULT_NOISE_INTENSITY),
_fNoiseScale				(CAMERA_DEFAULT_NOISE_SCALE),
_fNoiseFps					(CAMERA_DEFAULT_NOISE_FPS),
_fColorControlFade          (CAMERA_DEFAULT_COLORCONTROL_FADE),
_fGamma						(CAMERA_DEFAULT_GAMMA)
{
	// A camera needs a node
	COM_INSTANCE NodeInst = COM_INSTANCE(NODE::GetGenericCommunicatorDescriptor()->Create());
	SetNodeInst(NodeInst);
}

//-----------------------------------------------------------------------------
// Name: CAMERA destructor
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
CAMERA::~CAMERA()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
CAMERA &	CAMERA::operator=(const CAMERA &	C)
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
void	CAMERA::Copy(const CAMERA &	C)
{
	ENTITY_ABC::Copy(C);

	CopyProperties(C);
}

//-----------------------------------------------------------------------------
//	Name:		CopyProperties
//	Object:		needed to copy properties of camera but not node position, cant call Copy(..)
//	06-03-24:	DAC - Created
//-----------------------------------------------------------------------------
void	CAMERA::CopyProperties(const CAMERA &	C)
{
	// Optic
	_fFocal					= C._fFocal;
	_fHorizontalFieldOfView = C._fHorizontalFieldOfView;
	_fAspectRatio			= C._fAspectRatio;
	_fScale					= C._fScale;
	_fNearClippingDistance	= C._fNearClippingDistance;
	_fFarClippingDistance	= C._fFarClippingDistance;

	// Depth of field
	_bEnableDepthOfField	= C._bEnableDepthOfField;
	_fFarDepthOfField		= C._fFarDepthOfField;
	_fFocusPlane			= C._fFocusPlane;		
	_fFocusRange			= C._fFocusRange;			
	_fFocusSafeRange		= C._fFocusSafeRange;
	_fCircleOfConfusion		= C._fCircleOfConfusion;

	// Noise
	_bEnableNoise			= C._bEnableNoise;
	_fNoiseCutoff			= C._fNoiseCutoff;
	_fNoiseIntensity		= C._fNoiseIntensity;
	_fNoiseScale			= C._fNoiseScale;
	_fNoiseFps				= C._fNoiseFps;

	// HDR
	_fExposure				= C._fExposure;
	_fGamma					= C._fGamma;
	_fBrightBlurRadius		= C._fBrightBlurRadius;
	_fBrightThreshold		= C._fBrightThreshold;
	_fBrightIntensity		= C._fBrightIntensity;
	_bEnableBloom			= C._bEnableBloom;

	_bEnableGhosts			= C._bEnableGhosts;
	_fGhostsDiffraction		= C._fGhostsDiffraction;
	_fGhostsIntensity		= C._fGhostsIntensity;

	_bEnableStars			= C._bEnableStars;
	_uiStarsNbrBranches		= C._uiStarsNbrBranches;
	_fStarsDiffraction		= C._fStarsDiffraction;
	_fStarsIntensity		= C._fStarsIntensity;
	_fStarsSize				= C._fStarsSize;
	_fStarsAngle			= C._fStarsAngle;

	// Matrices
	_mProjection			= C._mProjection;
	_mView					= C._mView;									

	// ControlStage
	UInt32 i;
	for (i=0; i<COLORCONTROL_STAGES_COUNT; ++i)
	{
		_colorControlStages[i] = C._colorControlStages[i];
	}
	_fColorControlFade		= C._fColorControlFade;
}


//-----------------------------------------------------------------------------
//	Name:		Register
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	CAMERA::Register()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Unregister
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	CAMERA::Unregister()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	CAMERA::Instanciate(COM_INSTANCE &	Instance)
{
	CAMERA *	pNewCamera;

	Instance.SetHandle(CAMERA::GetGenericCommunicatorDescriptor()->Create());
	pNewCamera = static_cast<CAMERA *>(Instance.GetCommunicator());

	*pNewCamera = *this;

	InstanciateNode(pNewCamera);
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateRec
//	Object:		
//	06-01-18:	ELE - Created
//-----------------------------------------------------------------------------
void	CAMERA::InstanciateRec(COM_INSTANCE &	Instance)
{
	CAMERA *	pNewCamera;

	Instance.SetHandle(CAMERA::GetGenericCommunicatorDescriptor()->Create());
	pNewCamera = static_cast<CAMERA *>(Instance.GetCommunicator());

	*pNewCamera = *this;

	InstanciateNodeRec(pNewCamera);
}

//-----------------------------------------------------------------------------
//	Name:		CAMERA::UpdateViewMatrix
//	Object:		
//	06-01-31:	RMA - Created
//-----------------------------------------------------------------------------
void CAMERA::UpdateMatrices(Float32 fViewportAspectRatio, 
							Float32 fScaleBorderX, 
							Float32 fScaleBorderY)
{
	_mView.InverseAffine(GetNode()->GetGlobalMatrix());

	switch (RENDER_CONTEXT::GetRenderContextType())
	{
	default:
	case RENDER_CONTEXT::RCT_UNDEFINED:
		QDT_FAIL();

	case RENDER_CONTEXT::RCT_DIRECT3D9:
		_mProjection.SetPerspectiveFovMayaD3D(_fHorizontalFieldOfView, fViewportAspectRatio, _fNearClippingDistance, _fFarClippingDistance, _fScale * _fAspectRatio);
		break;

	case RENDER_CONTEXT::RCT_OPENGL:
	case RENDER_CONTEXT::RCT_OPENGL_FOR_MAYA:
	case RENDER_CONTEXT::RCT_PSGL:
		_mProjection.SetPerspectiveFovMayaOGL(_fHorizontalFieldOfView, fViewportAspectRatio, _fNearClippingDistance, _fFarClippingDistance, _fScale * _fAspectRatio);
		break;
	}

	_mProjection(0, 0) /= fScaleBorderX;
	_mProjection(1, 1) /= fScaleBorderY;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateViewportState
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::UpdateViewportState(VIEWPORT_STATE & ViewportState)
{
	ViewportState.EnableAutoExposure(true);
	ViewportState.SetExposure(_fExposure);
	
	ViewportState.EnableBloom(_bEnableBloom);
	ViewportState.SetBloomParams(_fBrightBlurRadius, _fBrightThreshold, _fBrightIntensity);

	ViewportState.EnableGhosts(_bEnableGhosts);
	ViewportState.SetGhostsParams(_fGhostsDiffraction, _fGhostsIntensity);

	ViewportState.EnableStars(_bEnableStars);
	ViewportState.SetStarsParams(_uiStarsNbrBranches, 3, _fStarsDiffraction, _fStarsIntensity, _fStarsSize, _fStarsAngle);


	ViewportState.EnableNoise(_bEnableNoise);
	ViewportState.SetNoiseParams(_fNoiseCutoff, _fNoiseScale, _fNoiseIntensity, 1.0f / _fNoiseFps);


	ViewportState.EnableDof(_bEnableDepthOfField);

	Float32 x0 = _fFocusSafeRange / _fFarDepthOfField;
	Float32 x1 = _fFocusRange / _fFarDepthOfField;

	ViewportState.SetDofParams(_fFocusPlane/_fFarDepthOfField,
		                       1.0f/(x1 - x0),
							   x0/(x1 - x0),
							   _fCircleOfConfusion);

	Bool bEnableColor = false;
	ViewportState.ColorIdentity();

/*
			void ColorIdentity() {_colorTransform.Identity();};
			void ColorSaturation(Float32 sat);
			void ColorMultiply(Float32 r, Float32 g, Float32 b);
			void ColorAdd(Float32 r, Float32 g, Float32 b);

			void ColorHueShift(Float32 hue);
			
			void ColorInvert();
			void ColorScreen(Float32 r, Float32 g, Float32 b);
*/
	for (Int32 i = 0; i < COLORCONTROL_STAGES_COUNT; i++)
	{
		if (_colorControlStages[i]._uiOperationId != CO_NONE)
		{
			bEnableColor = true;

			switch (_colorControlStages[i]._uiOperationId)
			{
			case CO_ADD:
				ViewportState.ColorAdd(_colorControlStages[i]._fParam0, _colorControlStages[i]._fParam1, _colorControlStages[i]._fParam2);
				break;
			case CO_MULTIPLY:
				ViewportState.ColorMultiply(_colorControlStages[i]._fParam0, _colorControlStages[i]._fParam1, _colorControlStages[i]._fParam2);
				break;
			case CO_HUESHIFT:
				ViewportState.ColorHueShift(_colorControlStages[i]._fParam0 * 6.29f);
				break;
			case CO_SATURATION:
				ViewportState.ColorSaturation(_colorControlStages[i]._fParam0);
				break;
			case CO_INVERT:
				ViewportState.ColorInvert();
				break;
			case CO_SCREEN:
				ViewportState.ColorScreen(_colorControlStages[i]._fParam0, _colorControlStages[i]._fParam1, _colorControlStages[i]._fParam2);
				break;
			case CO_BRIGHTNESS_CONTRAST:
				ViewportState.ColorAdd(_colorControlStages[i]._fParam0, _colorControlStages[i]._fParam0, _colorControlStages[i]._fParam0);
				ViewportState.ColorMultiply(_colorControlStages[i]._fParam1, _colorControlStages[i]._fParam1, _colorControlStages[i]._fParam1);
				break;
			default:
				QDT_ASSERT(0);
			}
		}
	}

	if (bEnableColor)
		ViewportState.ColorFadeToIdentity(_fColorControlFade);

	ViewportState.EnableColorTransform(bEnableColor);

//	ViewportState.SetNoiseParams(Float32 Cutoff, Float32 Scale, Float32 Intensity, Float32 Period);

//	ViewportState.EnableDof(Bool val){_bDofEnabled = val;};
//	ViewportState.SetDofParams(Float32 distance, Float32 range, _fCircleOfConfision)
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	CAMERA::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("CAMERA", 1, CM_ReadChunk);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("CAMERA", 1, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	CAMERA::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_CAMERA) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	CAMERA::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("CAMERA"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_CAMERA,
											QDT_COMMENT("15D0ACF8-4F1D-A1DA-AE1E-6BDFBAF8A498"),
											sizeof(CAMERA),
											ComInit,
											QDT_COMMENT("Class for cameras"),
											ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	CAMERA::EM_Construct(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	CAMERA	*pCamera;
	pCamera = static_cast<CAMERA *>(pO);

	COM_HANDLE *pCH = static_cast<COM_HANDLE*>(pP);

	QDT_NEW_PLACED(pCamera, CAMERA)(pCH);
	QDT_ASSERT(pCamera);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	CAMERA::EM_Destruct(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	CAMERA	*pCamera;
	pCamera = static_cast<CAMERA *>(pO);

	QDT_DELETE_PLACED(pCamera, CAMERA);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	CAMERA::CM_ReadChunk(IO_STREAM &		Stream,
							 COMMUNICATOR *	pCommunicator)
{
	CAMERA	*pCamera;
	pCamera = static_cast<CAMERA *>(pCommunicator);

	Stream >> pCamera->_fFocal
		   >> pCamera->_fHorizontalFieldOfView
		   >> pCamera->_fAspectRatio
		   >> pCamera->_fScale
		   >> pCamera->_fNearClippingDistance
		   >> pCamera->_fFarClippingDistance;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	CAMERA::CM_WriteChunk(IO_STREAM &	Stream,
							  COMMUNICATOR *	pCommunicator)
{
	CAMERA	*pCamera;
	pCamera = static_cast<CAMERA *>(pCommunicator);

	Stream.DebugOutput("Camera :\t");
	Stream.DebugOutput(pCamera->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Focale angle :\t");
	Stream	<< pCamera->_fFocal;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Horizontal Field of view :\t");
	Stream	<< pCamera->_fHorizontalFieldOfView;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Aspect Ratio :\t");
	Stream	<< pCamera->_fAspectRatio;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Scale :\t");
	Stream	<< pCamera->_fScale;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Near clipping distance :\t");
	Stream	<< pCamera->_fNearClippingDistance;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Far clipping distance :\t");
	Stream	<< pCamera->_fFarClippingDistance;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("\r\n");
}
#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================
