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
// CLASS: LIGHT_DESC
//
//
//	00-11-20	JVA Created:
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif
//*****************************************************************************
//
// CLASS DESCRIPTION ( LIGHT_DESC )
//
//	The LIGHT_DESC class implements ...
//
//*****************************************************************************

#include	"include.h"
#include	INC_3DENGINE(M3D)

#ifndef	_DEBUG
	#include	"Inline.h"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

LIGHT_DESC*	LIGHT_DESC::_pInstance = NULL;
bool			LIGHT_DESC::_bDestroyedOnce = false;


//-----------------------------------------------------------------------------
// Name: LIGHT_DESC constructor
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
LIGHT_DESC::LIGHT_DESC()
:DESC( M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_LIGHT , "m3d_light" , NB_COMMANDS )
{
	
	_pCommandDescsTab[ 0 ].Init( COMMAND_OMNI_LIGHT , 0 , "OmniLight" );

	_pCommandDescsTab[ 1 ].Init( COMMAND_SPOT_LIGHT , 0 , "SpotLight" );

	_pCommandDescsTab[ 2 ].Init( COMMAND_SET_COLOR , 1 , "SetColor" );
	_pCommandDescsTab[ 2 ].SetParameterDesc( 0 , OTYPE_KERNEL_COLOR_FLOAT , 1 , PARAMETER_DESC::IN_PARAMETER , "Color" );

	_pCommandDescsTab[ 3 ].Init( COMMAND_GET_COLOR , 1 , "GetColor" );
	_pCommandDescsTab[ 3 ].SetParameterDesc( 0 , OTYPE_KERNEL_COLOR_FLOAT , 1 , PARAMETER_DESC::OUT_PARAMETER , "Color" );

	_pCommandDescsTab[ 4 ].Init( COMMAND_SET_HOTSPOT_ANGLE , 1 , "SetHotspotAngle" );
	_pCommandDescsTab[ 4 ].SetParameterDesc( 0 , OTYPE_KERNEL_FLOAT , 1 , PARAMETER_DESC::IN_PARAMETER , "HotspotAngle" );

	_pCommandDescsTab[ 5 ].Init( COMMAND_SET_FALLOFF_ANGLE , 1 , "SetFalloffAngle" );
	_pCommandDescsTab[ 5 ].SetParameterDesc( 0 , OTYPE_KERNEL_FLOAT , 1 , PARAMETER_DESC::IN_PARAMETER , "FalloffAngle");

	_pCommandDescsTab[ 6 ].Init( COMMAND_SET_HOTSPOT_DISTANCE , 1 , "SetHotspotDistance" );
	_pCommandDescsTab[ 6 ].SetParameterDesc( 0 , OTYPE_KERNEL_FLOAT , 1 , PARAMETER_DESC::IN_PARAMETER , "HotspotDistance" );

	_pCommandDescsTab[ 7 ].Init( COMMAND_SET_FALLOFF_DISTANCE , 1 , "SetFalloffDistance" );
	_pCommandDescsTab[ 7 ].SetParameterDesc( 0 , OTYPE_KERNEL_FLOAT , 1 , PARAMETER_DESC::IN_PARAMETER , "FalloffDistance");

	_pCommandDescsTab[ 8 ].Init( COMMAND_SET_MULTIPLIER , 1 , "SetMultiplier" );
	_pCommandDescsTab[ 8 ].SetParameterDesc( 0 , OTYPE_KERNEL_FLOAT , 1 , PARAMETER_DESC::IN_PARAMETER , "Multiplier");

}

//-----------------------------------------------------------------------------
// Name: LIGHT_DESC destructor
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
LIGHT_DESC::~LIGHT_DESC()
{

}

COM_OBJECT *
LIGHT_DESC::New()
{
	_nNbObjects++;
	return new LIGHT;
}

//-----------------------------------------------------------------------------
// Name: LIGHT_DESC::Instance
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
DESC*	LIGHT_DESC::Instance()
{
	QDT_ASSERT( _bDestroyedOnce == false );

	if ( _pInstance == NULL )
	{
		_pInstance = ( new LIGHT_DESC() );
	}
	
	return ( _pInstance );
}

//-----------------------------------------------------------------------------
// Name: LIGHT_DESC::Suicide
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
void	LIGHT_DESC::Suicide()
{
	QDT_ASSERT( _bDestroyedOnce == false );
	QDT_ASSERT( _pInstance != NULL );
	
	if ( _pInstance != NULL )
	{
		delete _pInstance;
		_pInstance		 = NULL;
		_bDestroyedOnce	 = false;
	}
}


//-----------------------------------------------------------------------------
// Name: LIGHT_DESC::ReceiveCommandProc
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
bool	LIGHT_DESC::ReceiveCommandProc(void *pObject, COMMAND_ID CommandID , PARAMETER_STACK & Stack )const
{
	LIGHT	*pLight = static_cast <LIGHT *> (pObject);

	QDT_ASSERT(pLight);
	
	switch ( CommandID )
	{

	case LIGHT_DESC::COMMAND_OMNI_LIGHT:
		{
			pLight->SetLightType(LIGHT::POINT_LIGHT);
			break;
		}

	case LIGHT_DESC::COMMAND_SPOT_LIGHT:
		{
			pLight->SetLightType(LIGHT::SPOT_LIGHT);
			break;
		}

	case LIGHT_DESC::COMMAND_SET_COLOR:
		{
			pLight->SetColor(Stack.GetColorFloat(0)) ; 		
			break;
		}

	case LIGHT_DESC::COMMAND_GET_COLOR:
		{
			COLOR_FLOAT Color(pLight->GetColor()) ;
			Stack.SetColorFloat(0 , Color) ; 		
			break;
		}

	case LIGHT_DESC::COMMAND_SET_HOTSPOT_ANGLE:
		{
			pLight->SetHotSpotAngle( Stack.GetFloat( 0 ) ) ; 		
			break;
		}

	case LIGHT_DESC::COMMAND_SET_FALLOFF_ANGLE:
		{
			pLight->SetFallOffAngle( Stack.GetFloat( 0 ) ) ; 					
			break;
		}

	case LIGHT_DESC::COMMAND_SET_HOTSPOT_DISTANCE:
		{
			pLight->SetHotSpotDistance( Stack.GetFloat( 0 ) ) ; 					
			break;
		}

	case LIGHT_DESC::COMMAND_SET_FALLOFF_DISTANCE:
		{
			pLight->SetFallOffDistance( Stack.GetFloat( 0 ) ) ; 					
			break;
		}

	case LIGHT_DESC::COMMAND_SET_MULTIPLIER:
		{
			pLight->SetMultiplier( Stack.GetFloat( 0 ) ) ; 					
			break;
		}
	default:
		return DefaultCommand(pLight, CommandID, Stack);
	}
	return false;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
