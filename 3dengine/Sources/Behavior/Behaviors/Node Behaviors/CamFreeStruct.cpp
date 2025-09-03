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
// CLASS: CAM_FREE_STRUCT
//
//
//	00-12-11	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (CAM_FREE_STRUCT)
//
//	The CAM_FREE_STRUCT class implements ...
//
//*****************************************************************************
 
#include	"Root.h" 
#include	"CamFreeStruct.h"

#include INCL_KCORE(Time/Time_ABC)
#include INCL_KSERV(Input/DeviceObserver)
#include INCL_KSERV(Input/Input)
#include INCL_3DENGINE(Behavior/Behaviors/Node Behaviors/CamFree)
#include INCL_3DENGINE(SceneGraph/Node/Node)
#include INCL_3DENGINE(SceneGraph/Entities/Camera/Camera)
#include INCL_3DENGINE(Display/Viewport/Viewport)
#include INCL_3DENGINE(Display/Viewport/ViewportList)
#include INCL_3DENGINE(Display/DisplayManager)

#define		PAD_MAX_RANGE 128.0f

//=============================================================================
// CODE STARTS HERE
//=============================================================================

float				CAM_FREE_STRUCT::_rRotStep(0.05f);
float				CAM_FREE_STRUCT::_rTransStep(5.0f);
float				CAM_FREE_STRUCT::_rRollStep(0.05f);
float				CAM_FREE_STRUCT::_rFocalStep(0.01f);
float				CAM_FREE_STRUCT::_rMoreStep(1.5f);
int					CAM_FREE_STRUCT::_nObjectType[KEYMOV_NBR_CTRLS];
CAM_FREE_STRUCT	*	CAM_FREE_STRUCT::_pActiveCamFree = NULL;
OBJECT_OBSERVER		CAM_FREE_STRUCT::_pKeyInst[KEYMOV_NBR_CTRLS];
DEVICE_OBSERVER		CAM_FREE_STRUCT::_ConnectInst;
DEVICE_OBSERVER		CAM_FREE_STRUCT::_DisconnectInst;

static bool			sbConnectDisconnectInitialized = false;

//-----------------------------------------------------------------------------
//	Name:		CAM_FREE_STRUCT constructor
//	Object:		
//	03-03-26:	CVI - Created
//-----------------------------------------------------------------------------
CAM_FREE_STRUCT::CAM_FREE_STRUCT()
:
_rMulCoeff			(1.0f),
_bIsFast			(false),
_rCurrentRoll		(0.0f),
_VirtualNodeInst	(NODE::GetGenericCommunicatorDescriptor()->Create())
{
	for (Int8 nIdx = 0;nIdx < KEYMOV_NBR_CTRLS;nIdx++)
	{
		_fnct[nIdx] = NULL;
	}

	InitDefaultKeys();
	SetLoop( true );	
	
	SetEndTime( QDT_FLOAT32_MAX );

}

//-----------------------------------------------------------------------------
//	Name: CAM_FREE_STRUCT::BE_CAM_FREE_STRUCT
//	Object: 
//	01-01-24:	RMA - Created
//-----------------------------------------------------------------------------
CAM_FREE_STRUCT::CAM_FREE_STRUCT(const CAM_FREE_STRUCT &	BehCamFree)
{
	Copy(BehCamFree);
}

//-----------------------------------------------------------------------------
// Name: CAM_FREE_STRUCT::~CAM_FREE_STRUCT
// Object: Destructor
//00-12-11 RMA Created: 
//-----------------------------------------------------------------------------
CAM_FREE_STRUCT::~CAM_FREE_STRUCT()
{
	UInt32 i;
	for	(i = 0 ; i < KEYMOV_NBR_CTRLS ; i++)
	{
		_pKeyInst[i].RemoveEventCallback(_fnct[i], reinterpret_cast<IntPtr>(this));
		INPUT::Instance()->RemoveObjectObserver(&_pKeyInst[i]);
	}

#if defined (PLATFORM_PS3)
	if (sbConnectDisconnectInitialized)
	{
		sbConnectDisconnectInitialized = false;

		INPUT::Instance()->RemoveDeviceObserver(&_ConnectInst);
		INPUT::Instance()->RemoveDeviceObserver(&_DisconnectInst);
	}
#endif

	if	(_pActiveCamFree == this)
	{
		_pActiveCamFree = NULL;
	}
}	

//-----------------------------------------------------------------------------
//	Name: CAM_FREE_STRUCT::operator
//	Object: 
//	01-01-24:	RMA - Created
//-----------------------------------------------------------------------------
CAM_FREE_STRUCT & CAM_FREE_STRUCT::operator=(const CAM_FREE_STRUCT & BehCamFree)
{
	if ( this != &BehCamFree )
	{	
		Copy ( BehCamFree ) ;
	}

	return ( *this ) ;
}

//-----------------------------------------------------------------------------
//	Name:		SetEntityInst
//	Object:		
//	06-01-10:	ELE - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::SetEntityInst(const COM_INSTANCE &	EntityInst)
{
	_EntityInst = EntityInst;
}

//-----------------------------------------------------------------------------
//	Name:		InitDefaultKeys
//	Object:		
//	01-07-06:	cvi - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::InitDefaultKeys()
{
	int i;
	INPUT_DEVICE_INFO	DeviceInfo;

	// Build Keyboard Device Info
	DeviceInfo.SetNbrKeys	( INPUT::KEYBOARD_DEFAULT_NB_KEYS );
	_nHandle = INPUT::Instance()->GetDevice( DeviceInfo );
//	void (*fnct[KEYMOV_NBR_CTRLS])(int, int);
	//int	ObjectType[KEYMOV_NBR_CTRLS];
	
	if (_nHandle != -1)
	{
		_fnct[KEYMOV_FAST_1]			= OnKeyFast;
		_nObjectType[KEYMOV_FAST_1] 	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_FAST_1, INPUT::KEY_LSHIFT);

		_fnct[KEYMOV_FAST_2]			= OnKeyFast;
		_nObjectType[KEYMOV_FAST_2] 	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_FAST_2, INPUT::KEY_RSHIFT);

		_fnct[KEYMOV_GO_FORWARD]		= OnKeyGoFoward;
		_nObjectType[KEYMOV_GO_FORWARD] = INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_GO_FORWARD, INPUT::KEY_PAGE_UP);

		_fnct[KEYMOV_GO_BACKWARD]		 = OnKeyGoBackward;
		_nObjectType[KEYMOV_GO_BACKWARD] = INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_GO_BACKWARD,	 INPUT::KEY_PAGE_DOWN);

		_fnct[KEYMOV_GO_UP]			 	= OnKeyGoUp;
		_nObjectType[KEYMOV_GO_UP]   	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_GO_UP, INPUT::KEY_NUMPAD8);

		_fnct[KEYMOV_GO_DOWN]		 	= OnKeyGoDown;
		_nObjectType[KEYMOV_GO_DOWN] 	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_GO_DOWN, INPUT::KEY_NUMPAD2);

		_fnct[KEYMOV_GO_LEFT]			= OnKeyGoLeft;
		_nObjectType[KEYMOV_GO_LEFT]	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_GO_LEFT, INPUT::KEY_NUMPAD4);

		_fnct[KEYMOV_GO_RIGHT]			= OnKeyGoRight;
		_nObjectType[KEYMOV_GO_RIGHT]	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_GO_RIGHT, INPUT::KEY_NUMPAD6);

		_fnct[KEYMOV_TURN_LEFT]			= OnKeyTurnLeft;
		_nObjectType[KEYMOV_TURN_LEFT]	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_TURN_LEFT, INPUT::KEY_LEFT);

		_fnct[KEYMOV_TURN_RIGHT]		= OnKeyTurnRight;
		_nObjectType[KEYMOV_TURN_RIGHT] = INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_TURN_RIGHT, INPUT::KEY_RIGHT);

		_fnct[KEYMOV_TURN_UP]			= OnKeyTurnUp;
		_nObjectType[KEYMOV_TURN_UP]	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_TURN_UP, INPUT::KEY_UP);

		_fnct[KEYMOV_TURN_DOWN]			= OnKeyTurnDown;
		_nObjectType[KEYMOV_TURN_DOWN]	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_TURN_DOWN, INPUT::KEY_DOWN);

		_fnct[KEYMOV_ROLL_LEFT]			= OnKeyRollLeft;
		_nObjectType[KEYMOV_ROLL_LEFT]	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_ROLL_LEFT, INPUT::KEY_NUMPAD7);

		_fnct[KEYMOV_ROLL_RIGHT]		= OnKeyRollRight;
		_nObjectType[KEYMOV_ROLL_RIGHT] = INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_ROLL_RIGHT, INPUT::KEY_NUMPAD9);

		_fnct[KEYMOV_FOCAL_PLUS]		 = OnKeyFocalPlus;
		_nObjectType[KEYMOV_FOCAL_PLUS]  = INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_FOCAL_PLUS, INPUT::KEY_NUMPAD3);

		_fnct[KEYMOV_FOCAL_MINUS]		 = OnKeyFocalMinus;
		_nObjectType[KEYMOV_FOCAL_MINUS] = INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_FOCAL_MINUS, INPUT::KEY_NUMPAD1);

		_fnct[KEYMOV_MORE]				= OnKeyMore;
		_nObjectType[KEYMOV_MORE]		= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_MORE, INPUT::KEY_ADD);

		_fnct[KEYMOV_LESS]				= OnKeyLess;
		_nObjectType[KEYMOV_LESS]		= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_LESS, INPUT::KEY_SUBTRACT);

		_fnct[KEYMOV_RESET]				= OnKeyReset;
		_nObjectType[KEYMOV_RESET]		= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey( KEYMOV_RESET, INPUT::KEY_NUMPAD5);

		//------------ DOF --------------
		_fnct[KEYMOV_DOF_ENABLE]		= OnKeyDofEnable;
		_nObjectType[KEYMOV_DOF_ENABLE] = INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_ENABLE, INPUT::KEY_NUMPAD0);

		_fnct[KEYMOV_DOF_DISABLE]		 = OnKeyDofDisable;
		_nObjectType[KEYMOV_DOF_DISABLE] = INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_DISABLE, INPUT::KEY_DECIMAL);

		// DOF debug
		_fnct[KEYMOV_DOF_DEBUG_ENABLE]			= OnKeyDofDebugEnable;
		_nObjectType[KEYMOV_DOF_DEBUG_ENABLE]	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_DEBUG_ENABLE, INPUT::KEY_U);

		_fnct[KEYMOV_DOF_DEBUG_DISABLE]			= OnKeyDofDebugDisable;
		_nObjectType[KEYMOV_DOF_DEBUG_DISABLE]	= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_DEBUG_DISABLE, INPUT::KEY_J);

		// DOF Far
		_fnct[KEYMOV_DOF_FAR_LESS]				= OnKeyDofFarLess;
		_nObjectType[KEYMOV_DOF_FAR_LESS]		= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_FAR_LESS, INPUT::KEY_I);

		_fnct[KEYMOV_DOF_FAR_MORE]				= OnKeyDofFarMore;
		_nObjectType[KEYMOV_DOF_FAR_MORE]		= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_FAR_MORE, INPUT::KEY_K);

		// DOF Plane
		_fnct[KEYMOV_DOF_PLANE_LESS]			= OnKeyDofPlaneLess;
		_nObjectType[KEYMOV_DOF_PLANE_LESS]		= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_PLANE_LESS, INPUT::KEY_L);

		_fnct[KEYMOV_DOF_PLANE_MORE]			= OnKeyDofPlaneMore;
		_nObjectType[KEYMOV_DOF_PLANE_MORE]		= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_PLANE_MORE, INPUT::KEY_O);

		// DOF Range
		_fnct[KEYMOV_DOF_RANGE_LESS]			= OnKeyDofRangeLess;
		_nObjectType[KEYMOV_DOF_RANGE_LESS]		= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_RANGE_LESS, INPUT::KEY_SEMICOLON);

		_fnct[KEYMOV_DOF_RANGE_MORE]			= OnKeyDofRangeMore;
		_nObjectType[KEYMOV_DOF_RANGE_MORE]		= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_RANGE_MORE, INPUT::KEY_P);

		// DOF SafeRange
		_fnct[KEYMOV_DOF_SAFERANGE_LESS]		= OnKeyDofSafeRangeLess;
		_nObjectType[KEYMOV_DOF_SAFERANGE_LESS] = INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_SAFERANGE_LESS, INPUT::KEY_APOSTROPHE);

		_fnct[KEYMOV_DOF_SAFERANGE_MORE]		= OnKeyDofSafeRangeMore;
		_nObjectType[KEYMOV_DOF_SAFERANGE_MORE] = INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_SAFERANGE_MORE, INPUT::KEY_LBRACKET);

		// DOF blur-circle of confusion
		_fnct[KEYMOV_DOF_BLUR_LESS]				= OnKeyDofBlurLess;
		_nObjectType[KEYMOV_DOF_BLUR_LESS]		= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_BLUR_LESS, INPUT::KEY_BACKSLASH);

		_fnct[KEYMOV_DOF_BLUR_MORE]				= OnKeyDofBlurMore;
		_nObjectType[KEYMOV_DOF_BLUR_MORE]		= INPUT_OBJECT_ABC::OBJ_KEY;
		SetKey(KEYMOV_DOF_BLUR_MORE, INPUT::KEY_RBRACKET);
	}
	else
	{
#if defined (PLATFORM_PS3)		//handle = INPUT::Instance()->GetDevice(1, 1, 0, 0, 0, 1, 1, 9, 0, 0);

		if (!sbConnectDisconnectInitialized)
		{
			sbConnectDisconnectInitialized = true;

			_ConnectInst.AddConnectCallback( &OnConnect, reinterpret_cast<IntPtr>(this) );
			_DisconnectInst.AddDisconnectCallback( &OnDisconnect, reinterpret_cast<IntPtr>(this) );

			INPUT::Instance()->AddDeviceObserver(0, &_ConnectInst);
			INPUT::Instance()->AddDeviceObserver(0, &_DisconnectInst);
		}

		// Build Pad Device Info
		DeviceInfo.Reset();
		DeviceInfo.SetNbrXAbsAxes	( INPUT::PS2_PAD_DEFAULT_NB_XAXIS );
		DeviceInfo.SetNbrYAbsAxes	( INPUT::PS2_PAD_DEFAULT_NB_YAXIS );
		DeviceInfo.SetNbrSliderAxes	( INPUT::PS2_PAD_DEFAULT_NB_SLIDERAXIS );
		DeviceInfo.SetNbrButtons	( INPUT::PS2_PAD_DEFAULT_NB_BUTTON );

		_nHandle = INPUT::Instance()->GetDevice( DeviceInfo );
		if (_nHandle == -1)
		{
			QDT_Warning(QDT_COMMENT("No Device for CAM_FREE_STRUCT!!!"));
			return;
		}

		_fnct[KEYMOV_FAST_1] = OnKeyFast;
		_nObjectType[KEYMOV_FAST_1] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_FAST_1, 2);		// CROSS
		_fnct[KEYMOV_FAST_2] = NULL;

		_fnct[KEYMOV_GO_FORWARD] = OnYAxis1;
		_nObjectType[KEYMOV_GO_FORWARD] = INPUT_OBJECT_ABC::OBJ_Y_AXIS_ABS;
		SetKey( KEYMOV_GO_FORWARD, 0);
		_fnct[KEYMOV_GO_BACKWARD] = NULL;

		_fnct[KEYMOV_GO_UP] = OnKeyGoUp;
		_nObjectType[KEYMOV_GO_UP] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_GO_UP, 12);		// UP

		_fnct[KEYMOV_GO_DOWN] = OnKeyGoDown;
		_nObjectType[KEYMOV_GO_DOWN] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_GO_DOWN, 14);	// DOWN

		_fnct[KEYMOV_GO_LEFT] = OnXAxis1;
		_nObjectType[KEYMOV_GO_LEFT] = INPUT_OBJECT_ABC::OBJ_X_AXIS_ABS;
		SetKey( KEYMOV_GO_LEFT,	0);
		_fnct[KEYMOV_GO_RIGHT] = NULL;

		_fnct[KEYMOV_TURN_LEFT] = OnXAxis2;
		_nObjectType[KEYMOV_TURN_LEFT] = INPUT_OBJECT_ABC::OBJ_RZ_AXIS;
		SetKey( KEYMOV_TURN_LEFT, 0);
		_fnct[KEYMOV_TURN_RIGHT] = NULL;

		_fnct[KEYMOV_TURN_UP] = OnYAxis2;
		_nObjectType[KEYMOV_TURN_UP] = INPUT_OBJECT_ABC::OBJ_SLIDER_AXIS;
		SetKey( KEYMOV_TURN_UP,	0);
		_fnct[KEYMOV_TURN_DOWN] = NULL;

		_fnct[KEYMOV_ROLL_LEFT] = OnKeyRollLeft;
		_nObjectType[KEYMOV_ROLL_LEFT] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_ROLL_LEFT, 15);				// LEFT

		_fnct[KEYMOV_ROLL_RIGHT] = OnKeyRollRight;
		_nObjectType[KEYMOV_ROLL_RIGHT] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_ROLL_RIGHT, 13);				// RIGHT

		_fnct[KEYMOV_FOCAL_PLUS] = OnKeyFocalPlus;
		_nObjectType[KEYMOV_FOCAL_PLUS] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_FOCAL_PLUS, 6);				// L1

		_fnct[KEYMOV_FOCAL_MINUS] = OnKeyFocalMinus;
		_nObjectType[KEYMOV_FOCAL_MINUS] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_FOCAL_MINUS, 7);				// R1

		_fnct[KEYMOV_MORE] = OnKeyMore;
		_nObjectType[KEYMOV_MORE] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_MORE, 4);					// L2

		_fnct[KEYMOV_LESS] = OnKeyLess;
		_nObjectType[KEYMOV_LESS] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_LESS, 5);					// R2

		_fnct[KEYMOV_RESET] = OnKeyReset;
		_nObjectType[KEYMOV_RESET] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_RESET, 8);					// SELECT

#elif defined (PLATFORM_XBOX)
		//handle = INPUT::Instance()->GetDevice(1, 1, 0, 0, 0, 1, 1, 9, 0, 0);
		// Build Pad Device Info
		DeviceInfo.Reset();
		DeviceInfo.SetNbrXAbsAxes	( INPUT::PS2_PAD_DEFAULT_NB_XAXIS );
		DeviceInfo.SetNbrYAbsAxes	( INPUT::PS2_PAD_DEFAULT_NB_YAXIS );
		DeviceInfo.SetNbrSliderAxes	( INPUT::PS2_PAD_DEFAULT_NB_SLIDERAXIS );
		DeviceInfo.SetNbrButtons	( INPUT::PS2_PAD_DEFAULT_NB_BUTTON );

		_nHandle = INPUT::Instance()->GetDevice( DeviceInfo );
		if (_nHandle == -1)
		{
			QDT_Warning(QDT_COMMENT("No Device for CAM_FREE_STRUCT!!!"));
			return;
		}

		_fnct[KEYMOV_FAST_1]		= OnKeyFast;
		_nObjectType[KEYMOV_FAST_1] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_FAST_1, 6);						// Right Trigger
		_fnct[KEYMOV_FAST_2]		= NULL;
		_fnct[KEYMOV_GO_UP]			= OnKeyGoUp;
		_nObjectType[KEYMOV_GO_UP] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_GO_UP, 12);						// DPad-Up
		_fnct[KEYMOV_GO_DOWN]		= OnKeyGoDown;
		_nObjectType[KEYMOV_GO_DOWN] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_GO_DOWN, 14);					// DPad-Down
		_fnct[KEYMOV_GO_LEFT]		= OnXAxis1;
		_nObjectType[KEYMOV_GO_LEFT] = INPUT_OBJECT_ABC::OBJ_X_AXIS_ABS;
		SetKey( KEYMOV_GO_LEFT,	0);						// LThumb-X
		_fnct[KEYMOV_GO_RIGHT]		= NULL;
		_fnct[KEYMOV_GO_FORWARD]	= OnYAxis1;
		_nObjectType[KEYMOV_GO_FORWARD] = INPUT_OBJECT_ABC::OBJ_Y_AXIS_ABS;
		SetKey( KEYMOV_GO_FORWARD, 0);					// LThumb-Y
		_fnct[KEYMOV_GO_BACKWARD]	= NULL;
		_fnct[KEYMOV_TURN_LEFT]		= OnXAxis2;
		_nObjectType[KEYMOV_TURN_LEFT] = INPUT_OBJECT_ABC::OBJ_RZ_AXIS;
		SetKey( KEYMOV_TURN_LEFT, 0);					// RThumb-X
		_fnct[KEYMOV_TURN_RIGHT]	= NULL;
		_fnct[KEYMOV_TURN_UP]		= OnYAxis2;
		_nObjectType[KEYMOV_TURN_UP] = INPUT_OBJECT_ABC::OBJ_SLIDER_AXIS;
		SetKey( KEYMOV_TURN_UP,	0);						// RThumb-Y
		_fnct[KEYMOV_TURN_DOWN]		= NULL;
		_fnct[KEYMOV_ROLL_LEFT]		= OnKeyRollLeft;
		_nObjectType[KEYMOV_ROLL_LEFT] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_ROLL_LEFT, 15);					// DPad-Left
		_fnct[KEYMOV_ROLL_RIGHT]	= OnKeyRollRight;
		_nObjectType[KEYMOV_ROLL_RIGHT] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_ROLL_RIGHT, 13);					// DPad-Right
		_fnct[KEYMOV_FOCAL_PLUS]	= OnKeyFocalPlus;
		_nObjectType[KEYMOV_FOCAL_PLUS] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_FOCAL_PLUS, 7);					// Digital-Black
		_fnct[KEYMOV_FOCAL_MINUS]	= OnKeyFocalMinus;
		_nObjectType[KEYMOV_FOCAL_MINUS] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_FOCAL_MINUS, 5);					// Digital-White
		_fnct[KEYMOV_MORE]			= OnKeyMore;
		_nObjectType[KEYMOV_MORE] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_MORE, 3);						// Digital-Y
		_fnct[KEYMOV_LESS]			= OnKeyLess;
		_nObjectType[KEYMOV_LESS] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_LESS, 1);						// Digital-X
		_fnct[KEYMOV_RESET]			= OnKeyReset;
		_nObjectType[KEYMOV_RESET] = INPUT_OBJECT_ABC::OBJ_BUTTON;
		SetKey( KEYMOV_RESET, 8);						// Back
#endif
	}

	for (i = 0;i < KEYMOV_NBR_CTRLS;i++)
	{
		_fMov[i] = 0;
		if (_fnct[i] != NULL)
		{
			_pKeyInst[i].AddEventCallback( _fnct[i], reinterpret_cast<IntPtr>(this) );
			INPUT::Instance()->AddObjectObserver(_nHandle, (INPUT_OBJECT_ABC::OBJ_TYPE)_nObjectType[i], _nKeysDef[i], &_pKeyInst[i]);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		StoreRotTrans
//	Object:		
//	01-10-22:	cvi - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::StoreRotTrans( float rTime )
{
	if (_nHandle == -1)
	{
		InitDefaultKeys();
		return;
	}
	
	float		rotY;
	float		rRoll;
	CAM_FREE	*pCam;

	VECTOR		OldPos;
	GetHolderNode()->GetGlobalPosition(OldPos);
	QUATERNION	OldQuat			= GetHolderNode()->GetQuaternion();
	NODE*		pVirtualNode	= static_cast<NODE*>(_VirtualNodeInst.GetCommunicator());

	///////////////////////////////////////////////////////////////////////////////
	pCam = CAM_FREE::GetFreeCamAnimFromStruct( this );
	if ( pCam->IsActived() == false )
	{
		return;
	}
	///////////////////////////////////////////////////////////////////////////////

	float rSpeedFactor = TIME::Instance()->GetDefaultClock()->GetSpeedFactor();

	//	Fast\Slow mode
	if	(_bIsFast)
	{
		rSpeedFactor *= 3;
	}

	// DAC: rotation have to be modified by _rMulCoeff for the scripters, they need it to setup cameras using free cam
	//	Camera speed 
	rSpeedFactor *= _rMulCoeff;

#ifdef PLATFORM_XBOX
	float	rRotSpeedFactor = rSpeedFactor * 3;
#else
	float	rRotSpeedFactor = rSpeedFactor;
#endif

	///////////////////////////////////////////////////////////////////////////////
	GetHolderNode()->MoveForward( (_fMov[KEYMOV_GO_FORWARD] - _fMov[KEYMOV_GO_BACKWARD])* rSpeedFactor * _rTransStep);
	GetHolderNode()->MoveUp( (_fMov[KEYMOV_GO_UP] - _fMov[KEYMOV_GO_DOWN]) * rSpeedFactor * _rTransStep);
	GetHolderNode()->StrafeLeft( (_fMov[KEYMOV_GO_LEFT] - _fMov[KEYMOV_GO_RIGHT]) * rSpeedFactor * _rTransStep);
	GetHolderNode()->LookUp( (_fMov[KEYMOV_TURN_UP] - _fMov[KEYMOV_TURN_DOWN]) * ( rRotSpeedFactor) * _rRotStep );
	rRoll = (_fMov[KEYMOV_ROLL_LEFT] - _fMov[KEYMOV_ROLL_RIGHT]) * rRotSpeedFactor * _rRollStep;
	GetHolderNode()->RollLeft(rRoll);
	_rCurrentRoll += rRoll;
	///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	pVirtualNode->MoveForward( (_fMov[KEYMOV_GO_FORWARD] - _fMov[KEYMOV_GO_BACKWARD])* rSpeedFactor * _rTransStep);
	pVirtualNode->MoveUp( (_fMov[KEYMOV_GO_UP] - _fMov[KEYMOV_GO_DOWN]) * rSpeedFactor * _rTransStep);
	pVirtualNode->StrafeLeft( (_fMov[KEYMOV_GO_LEFT] - _fMov[KEYMOV_GO_RIGHT]) * rSpeedFactor * _rTransStep);
	pVirtualNode->LookUp( (_fMov[KEYMOV_TURN_UP] - _fMov[KEYMOV_TURN_DOWN]) * rRotSpeedFactor * _rRotStep );
	rRoll = (_fMov[KEYMOV_ROLL_LEFT] - _fMov[KEYMOV_ROLL_RIGHT]) * rRotSpeedFactor * _rRollStep;
	pVirtualNode->RollLeft(rRoll);
	///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	//	Special case for Y rotation...
#ifdef PLATFORM_XBOX
	rotY = (( rRotSpeedFactor) * _rRotStep * (_fMov[KEYMOV_TURN_LEFT] - _fMov[KEYMOV_TURN_RIGHT]) * 2.0f);
#else
	rotY = ( rRotSpeedFactor) * _rRotStep * (_fMov[KEYMOV_TURN_LEFT] - _fMov[KEYMOV_TURN_RIGHT]);
#endif
	if	(rotY != 0.0f)
	{
		VECTOR		V( 0.0f, 1.0f, 0.0f );
		QUATERNION	Q(V, rotY * 0.5f );
		MATRIX		Mat;
		MATRIX		NewMat;
		VECTOR		SavePos;

		Mat.Identity();

		//	Get current global matrix
		const MATRIX & OldMat = GetHolderNode()->GetGlobalMatrix();
		//	Save translation
		SavePos = OldMat.GetTranslation();

		//	Rotation around Y
		Mat.SetQuaternion(Q);

		//	Rotate node around Y
		NewMat.Mul( OldMat, Mat );
		//	Restore position
		NewMat.SetPosition(SavePos);
		//	Set new global matrix ( function also update node local matrix )
		// so we don't need to do it
		GetHolderNode()->SetGlobalMatrix(NewMat);
	}
	///////////////////////////////////////////////////////////////////////////////

// JPE: Je vois pas l'interet de faire les choses deux fois, cela dit, je désactive ca que sur XBox
// Histoire d'etre sur que j'interfere avec rien sur les autres plateformes.
#ifndef PLATFORM_XBOX
	///////////////////////////////////////////////////////////////////////////////
	//	Special case for Y rotation...
	rotY = ( rRotSpeedFactor) * _rRotStep * (_fMov[KEYMOV_TURN_LEFT] - _fMov[KEYMOV_TURN_RIGHT]);
	if	(rotY != 0.0f)
	{
		VECTOR		V( 0.0f, 1.0f, 0.0f );
		QUATERNION	Q(V, rotY * 0.5f );
		MATRIX		Mat;
		MATRIX		NewMat;
		VECTOR		SavePos;

		Mat.Identity();

		//	Get current global matrix
		const MATRIX & OldMat = pVirtualNode->GetGlobalMatrix();
		//	Save translation
		SavePos = OldMat.GetTranslation();

		//	Rotation around Y
		Mat.SetQuaternion(Q);

		//	Rotate node around Y
		NewMat.Mul( OldMat, Mat );
		//	Restore position
		NewMat.SetPosition(SavePos);
		//	Set new global matrix ( function also update node local matrix )
		// so we don't need to do it
		pVirtualNode->SetGlobalMatrix(NewMat);
	}
	///////////////////////////////////////////////////////////////////////////////
#endif

	///////////////////////////////////////////////////////////////////////////////
	QUATERNION	QId;
	VECTOR		VId(VECTOR::ZERO);

	_CurrentQV.SetQuat( QId );
	_CurrentQV.SetVector( VId );
	///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	ENTITY_ABC	*pEntity = static_cast<ENTITY_ABC *>(_EntityInst.GetCommunicator());
	if	(pEntity && (pEntity->GetEntityType() == ENTITY_CAMERA))
	{
		CAMERA * pC = static_cast<CAMERA *>(pEntity);

	//	Focal handling
		float rFocalAngle = pC->GetHorizontalFieldOfView() + ((_fMov[KEYMOV_FOCAL_PLUS] - _fMov[KEYMOV_FOCAL_MINUS]) * rRotSpeedFactor * _rFocalStep);
		if	(rFocalAngle < 0.01f)
		{
			rFocalAngle = 0.01f;
		}
		else if	(rFocalAngle > MATH::PI() - 0.01f)
		{
			rFocalAngle = MATH::PI() - 0.01f;
		}
		pC->SetHorizontalFieldOfView(rFocalAngle);

		// dof
		//_fMov[KEYMOV_DOF_ENABLE] and _fMov[KEYMOV_DOF_DISABLE] are not used, we use the camera flag 
		if( pC->GetEnableDepthOfField())
		{
			float rDofFarDist =pC->GetFarDepthOfField() +  ((_fMov[KEYMOV_DOF_FAR_MORE] - _fMov[KEYMOV_DOF_FAR_LESS]) * rSpeedFactor * _rTransStep);
			pC->SetFarDepthOfField(rDofFarDist);

			float rDofPlaneDist =pC->GetFocusPlane() +  ((_fMov[KEYMOV_DOF_PLANE_MORE] - _fMov[KEYMOV_DOF_PLANE_LESS]) * rSpeedFactor * _rTransStep);
			pC->SetFocusPlane(rDofPlaneDist);

			float rDofRange =pC->GetFocusRange() +  ((_fMov[KEYMOV_DOF_RANGE_MORE] - _fMov[KEYMOV_DOF_RANGE_LESS]) * rSpeedFactor * _rTransStep);
			pC->SetFocusRange(rDofRange);

			float rDofSafeRange =pC->GetFocusSafeRange() +  ((_fMov[KEYMOV_DOF_SAFERANGE_MORE] - _fMov[KEYMOV_DOF_SAFERANGE_LESS]) * rSpeedFactor * _rTransStep);
			pC->SetFocusSafeRange(rDofSafeRange);

			float rCircleOfConfusion =pC->GetCircleOfConfusion() +  ((_fMov[KEYMOV_DOF_BLUR_MORE] - _fMov[KEYMOV_DOF_BLUR_LESS]) * 0.3f);
			pC->SetCircleOfConfusion(rCircleOfConfusion);

			DISPLAY_MANAGER* pDisplayManager =DISPLAY_MANAGER::Instance();
			if (pDisplayManager!=NULL)
			{
				VIEWPORT_LIST*	pViewportList =pDisplayManager->GetViewportList();
				int i,nViewport=pViewportList->GetNbrViewports();
				for (i=0; i<nViewport; ++i)
				{
					VIEWPORT* pViewport =pViewportList->GetViewport(i);
					if (_fMov[KEYMOV_DOF_DEBUG_ENABLE]!=0.0f)
						pViewport->EnableDepthOfFieldDebug(true);
					if (_fMov[KEYMOV_DOF_DEBUG_DISABLE]!=0.0f)
						pViewport->EnableDepthOfFieldDebug(false);
				}
			}
		}
	}
	

	GetHolderNode()->GetGlobalPosition(_LastPos);
	_LastPos.Sub( OldPos );
}


//-----------------------------------------------------------------------------
//	Name:		ReadRotation
//	Object:		
//	01-10-22:	cvi - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::ReadRotation(float		rTime,
							   QUATERNION &	Quat)
{

	Quat.Id();
}

//-----------------------------------------------------------------------------
//	Name:		ReadTranslation
//	Object:		
//	01-10-22:	cvi - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::ReadTranslation(float		rTime,
								  VECTOR &	Vect)
{
	Vect.SetXYZ( 0.0f, 0.0f, 0.0f );
}


//-----------------------------------------------------------------------------
//	Name:		GetRoll
//	Object:		
//	01-08-30:	VPI - Created
//-----------------------------------------------------------------------------
float	CAM_FREE_STRUCT::GetRoll() const
{
	return	(_rCurrentRoll);
}

//-----------------------------------------------------------------------------
//	Name:		SetRoll
//	Object:		
//	01-08-30:	VPI - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::SetRoll(const float	rRoll)
{
	//apply wanted roll
	NODE *	pCamFreeNode = GetHolderNode();
	pCamFreeNode->RollRight(_rCurrentRoll - rRoll);
	
	//and save it
	_rCurrentRoll = rRoll;
}


//-----------------------------------------------------------------------------
//	Name:		ResetRoll
//	Object:		
//	01-08-31:	VPI - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::ReinitRoll(const float	rRoll)
{
	//restore the roll
	_rCurrentRoll = rRoll;
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyFast
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyFast(IntPtr	nParam,
							int	nVal)
{		
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT(pCurCam);
	CAM_FREE	*pCam;

	//if not active dont touch 
	if	(pCurCam)		
	{	
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		if (pCam->IsActived() == false)
		{
			return;
		}
		
		
		if	(nVal)
		{
			pCurCam->_bIsFast = true;
		}
		else
		{
			pCurCam->_bIsFast = false;
		}
		
	}
#ifndef _MASTER
	else //error checking 
	{
		QDT_STRING	strMsg;
		strMsg.Format("CAM_FREE_STRUCT: a key has been pressed but the camera that must be moved doesn't exist !");
		QDT_Error(strMsg);
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyGoFoward
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyGoFoward(IntPtr	nParam,
								int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT(pCurCam);
	CAM_FREE	*pCam;
	
	if( pCurCam )
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_GO_FORWARD] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_GO_FORWARD] = 0.0f;
		}
	}	
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyGoBackward
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyGoBackward(IntPtr	nParam,
								  int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;
	
	if( pCurCam )
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_GO_BACKWARD] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_GO_BACKWARD] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyGoUp
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyGoUp(IntPtr	nParam,
							int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if( pCurCam )
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );		
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_GO_UP] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_GO_UP] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyGoDown
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyGoDown(IntPtr	nParam,
							  int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;	

	if( pCurCam )
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_GO_DOWN] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_GO_DOWN] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyGoLeft
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyGoLeft(IntPtr	nParam,
							  int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;
	
	if( pCurCam )
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_GO_LEFT] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_GO_LEFT] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyGoRight
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyGoRight(IntPtr	nParam,
							   int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;
	
	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_GO_RIGHT] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_GO_RIGHT] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyTurnLeft
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyTurnLeft(IntPtr	nParam,
								int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;
	
	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_TURN_LEFT] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_TURN_LEFT] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyTurnRight
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyTurnRight(IntPtr	nParam,
								 int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;
	
	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_TURN_RIGHT] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_TURN_RIGHT] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyTurnUp
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyTurnUp(IntPtr	nParam,
							  int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;
	
	if( pCurCam )
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_TURN_UP] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_TURN_UP] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyTurnDown
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyTurnDown(IntPtr	nParam,
								int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;
	
	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_TURN_DOWN] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_TURN_DOWN] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyRollLeft
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyRollLeft(IntPtr	nParam,
								int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;
	
	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_ROLL_LEFT] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_ROLL_LEFT] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyRollRight
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyRollRight(IntPtr	nParam,
								 int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_ROLL_RIGHT] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_ROLL_RIGHT] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyFocalPlus
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyFocalPlus(IntPtr	nParam,
								 int	nVal)
{	
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_FOCAL_PLUS] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_FOCAL_PLUS] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyFocalMinus
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyFocalMinus(IntPtr	nParam,
								  int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;
	
	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_FOCAL_MINUS] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_FOCAL_MINUS] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyMore
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyMore(IntPtr	nParam,
							int	nVal)
{	
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;
	
	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if	(nVal)
		{
			pCurCam->_rMulCoeff *= _rMoreStep;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyLess
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyLess(IntPtr	nParam,
							int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;
	
	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if	(nVal)
		{
			pCurCam->_rMulCoeff /= _rMoreStep;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyReset
//	Object:		
//	01-08-30:	ONA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyReset(IntPtr	nParam,
							 int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if( pCurCam )
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}
		
		if	(nVal)
		{
			NODE *	pCamFreeNode = pCurCam->GetHolderNode();
			if ( !pCamFreeNode )	return;

			ENTITY_ABC	*pEntity = static_cast<ENTITY_ABC *>(pCurCam->_EntityInst.GetCommunicator());
			if	(pEntity && (pEntity->GetEntityType() == ENTITY_CAMERA))
			{
				CAMERA *	pC;
				pC = static_cast<CAMERA *>(pEntity);

				pC->SetHorizontalFieldOfView(CAMERA_DEFAULT_HORIZONTAL_FIELD_OF_VIEW);
				pC->SetFarDepthOfField(CAMERA_DEFAULT_FAR_DEPTH_OF_FIELD);
				pC->SetFocusPlane(CAMERA_DEFAULT_FOCUS_PLANE);
				pC->SetFocusRange(CAMERA_DEFAULT_FOCUS_RANGE);
				pC->SetFocusSafeRange(CAMERA_DEFAULT_FOCUS_SAFE_RANGE);
				pC->SetCircleOfConfusion(CAMERA_DEFAULT_CIRCLE_OF_CONFUSION);

				pCurCam->SetRoll(0.0f);
			}
		}
	}
}

//----------------------------------------------------------------------------
//--------------------- DOF 
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofEnable
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofEnable(IntPtr	nParam, int nVal)
{	
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			// DAC: we use the flag within the camera, otherwise we have a synch problem between input
			// and gui in IAM
			COM_INSTANCE CI = pCurCam->_EntityInst;
			if	(CI.HasHandle())
			{
				ENTITY_ABC	*pEntity = static_cast<ENTITY_ABC *>(CI.GetCommunicator());
				if	(pEntity && (pEntity->GetEntityType() == ENTITY_CAMERA))
				{
					CAMERA * pC = static_cast<CAMERA *>(pEntity);
					pC->SetEnableDepthOfField(true);	
				}
			}
			//
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofDisable
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofDisable(IntPtr	nParam, int nVal)
{	
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			// DAC: we use the flag within the camera, otherwise we have a synch problem between input
			// and gui in IAM
			COM_INSTANCE CI = pCurCam->_EntityInst;
			if	(CI.HasHandle())
			{
				ENTITY_ABC	*pEntity = static_cast<ENTITY_ABC *>(CI.GetCommunicator());
				if	(pEntity && (pEntity->GetEntityType() == ENTITY_CAMERA))
				{
					CAMERA * pC = static_cast<CAMERA *>(pEntity);
					pC->SetEnableDepthOfField(false);	
				}
			}
			//
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofDebugEnable
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofDebugEnable(IntPtr	nParam, int nVal)
{	
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_DEBUG_ENABLE] = 1.0f;
			pCurCam->_fMov[KEYMOV_DOF_DEBUG_DISABLE] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofDebugDisable
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofDebugDisable(IntPtr	nParam, int nVal)
{	
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_DEBUG_DISABLE] = 1.0f;
			pCurCam->_fMov[KEYMOV_DOF_DEBUG_ENABLE] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofFarLess
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofFarLess(IntPtr	nParam, int nVal)
{	
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_FAR_LESS] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_DOF_FAR_LESS] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofFarMore
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofFarMore(IntPtr	nParam,int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_FAR_MORE] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_DOF_FAR_MORE] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofPlaneLess
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofPlaneLess(IntPtr	nParam, int nVal)
{	
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_PLANE_LESS] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_DOF_PLANE_LESS] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofPlaneMore
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofPlaneMore(IntPtr	nParam,int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_PLANE_MORE] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_DOF_PLANE_MORE] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofRangeLess
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofRangeLess(IntPtr	nParam, int nVal)
{	
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_RANGE_LESS] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_DOF_RANGE_LESS] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofRangeMore
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofRangeMore(IntPtr	nParam,int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_RANGE_MORE] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_DOF_RANGE_MORE] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofSafeRangeLess
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofSafeRangeLess(IntPtr	nParam, int nVal)
{	
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_SAFERANGE_LESS] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_DOF_SAFERANGE_LESS] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofSafeRangeMore
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofSafeRangeMore(IntPtr	nParam,int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_SAFERANGE_MORE] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_DOF_SAFERANGE_MORE] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofBlurLess
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofBlurLess(IntPtr	nParam, int nVal)
{	
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_BLUR_LESS] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_DOF_BLUR_LESS] = 0.0f;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnKeyDofBlurMore
//	Object:		
//	06-02-26:	BFA - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnKeyDofBlurMore(IntPtr	nParam,int	nVal)
{
	CAM_FREE_STRUCT *	pCurCam = reinterpret_cast<CAM_FREE_STRUCT *>(nParam);
	QDT_ASSERT( pCurCam );
	CAM_FREE	*pCam;

	if(pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		//if not active dont touch 
		if	(pCam->IsActived() == false)
		{	
			return;
		}

		if (nVal)
		{
			pCurCam->_fMov[KEYMOV_DOF_BLUR_MORE] = 1.0f;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_DOF_BLUR_MORE] = 0.0f;
		}
	}
}

void CAM_FREE_STRUCT::OnXAxis2(IntPtr nParam, int nVal)
{
	CAM_FREE_STRUCT *pCurCam = (CAM_FREE_STRUCT *)(nParam);
	CAM_FREE	*pCam;

	if (pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		if (!pCam->IsActived())
		{
			return;
		}
		if (nVal > -32 && nVal < 32)
		{
			pCurCam->_fMov[KEYMOV_TURN_LEFT] = 0;
			pCurCam->_fMov[KEYMOV_TURN_RIGHT] = 0;
		}
		else if (nVal < 0)
		{
			pCurCam->_fMov[KEYMOV_TURN_LEFT] = -((float)nVal / PAD_MAX_RANGE);
			pCurCam->_fMov[KEYMOV_TURN_RIGHT] = 0;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_TURN_LEFT] = 0;
			pCurCam->_fMov[KEYMOV_TURN_RIGHT] = (float)nVal / PAD_MAX_RANGE;
		}
	}
}

void CAM_FREE_STRUCT::OnYAxis2(IntPtr nParam, int nVal)
{
	CAM_FREE_STRUCT *pCurCam = (CAM_FREE_STRUCT *)(nParam);
	CAM_FREE	*pCam;

	if (pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		if (!pCam->IsActived())
		{
			return;
		}
		if (nVal > -32 && nVal < 32)
		{
			pCurCam->_fMov[KEYMOV_TURN_UP] = 0;
			pCurCam->_fMov[KEYMOV_TURN_DOWN] = 0;
		}
		else if (nVal < 0)
		{
			pCurCam->_fMov[KEYMOV_TURN_UP] = -((float)nVal / PAD_MAX_RANGE);
			pCurCam->_fMov[KEYMOV_TURN_DOWN] = 0;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_TURN_UP] = 0;
			pCurCam->_fMov[KEYMOV_TURN_DOWN] = (float)nVal / PAD_MAX_RANGE;
		}
	}
}

void CAM_FREE_STRUCT::OnXAxis1(IntPtr nParam, int nVal)
{
	CAM_FREE_STRUCT *pCurCam = (CAM_FREE_STRUCT *)(nParam);
	CAM_FREE	*pCam;

	if (pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		if (!pCam->IsActived())
		{
			return;
		}
		if (nVal > -32 && nVal < 32)
		{
			pCurCam->_fMov[KEYMOV_GO_LEFT] = 0;
			pCurCam->_fMov[KEYMOV_GO_RIGHT] = 0;
		}
		else if (nVal < 0)
		{
			pCurCam->_fMov[KEYMOV_GO_LEFT] = -((float)nVal / PAD_MAX_RANGE);
			pCurCam->_fMov[KEYMOV_GO_RIGHT] = 0;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_GO_LEFT] = 0;
			pCurCam->_fMov[KEYMOV_GO_RIGHT] = (float)nVal / PAD_MAX_RANGE;
		}
	}
}

void CAM_FREE_STRUCT::OnYAxis1(IntPtr nParam, int nVal)
{
	CAM_FREE_STRUCT *pCurCam = (CAM_FREE_STRUCT *)(nParam);
	CAM_FREE	*pCam;

	if (pCurCam)
	{
		pCam = CAM_FREE::GetFreeCamAnimFromStruct( pCurCam );
		if (!pCam->IsActived())
		{
			return;
		}
		if (nVal > -32 && nVal < 32)
		{
			pCurCam->_fMov[KEYMOV_GO_FORWARD] = 0;
			pCurCam->_fMov[KEYMOV_GO_BACKWARD] = 0;
		}
		else if (nVal < 0)
		{
			pCurCam->_fMov[KEYMOV_GO_FORWARD] = -((float)nVal / PAD_MAX_RANGE);
			pCurCam->_fMov[KEYMOV_GO_BACKWARD] = 0;
		}
		else
		{
			pCurCam->_fMov[KEYMOV_GO_FORWARD] = 0;
			pCurCam->_fMov[KEYMOV_GO_BACKWARD] = (float)nVal / PAD_MAX_RANGE;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnConnect
//	Object:		
//	04-03-09:	SBE - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnConnect(IntPtr	nParam)
{
}

//-----------------------------------------------------------------------------
//	Name:		OnDisconnect
//	Object:		
//	04-03-09:	SBE - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::OnDisconnect(IntPtr	nParam)
{
	CAM_FREE_STRUCT *pCurCam = (CAM_FREE_STRUCT *)(nParam);	
	pCurCam->_nHandle = -1;	
}

//-----------------------------------------------------------------------------
//	Name:		SetKey
//	Object:		
//	01-08-01:	DAC - Created
//-----------------------------------------------------------------------------
void	CAM_FREE_STRUCT::SetKey(KEYMOV_TYPE	nKeyType,
						 int			nKeyCode)
{
	_nKeysDef[nKeyType ] = nKeyCode;
}

//-----------------------------------------------------------------------------
//	Name:		GetKey
//	Object:		
//	01-08-01:	DAC - Created
//-----------------------------------------------------------------------------
int	CAM_FREE_STRUCT::GetKey(KEYMOV_TYPE	nKeyType) const
{
	QDT_ASSERT( ( nKeyType > UNKOWN_KEYMOV_TYPE ) && ( nKeyType < KEYMOV_NBR_CTRLS ) );
	return ( _nKeysDef[ nKeyType ] ) ;
}
//=============================================================================
// CODE ENDS HERE
//=============================================================================

