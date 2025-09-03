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
// CLASS: PARTICLE_SYSTEM_MANAGER_DESC
//
//
//	00-11-22	JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif
//*****************************************************************************
//
// CLASS DESCRIPTION ( PARTICLE_SYSTEM_MANAGER_DESC )
//
//	The PARTICLE_SYSTEM_MANAGER_DESC class implements ...
//
//*****************************************************************************

#include	"include.h"
#include	INC_3DENGINE(M3D)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

PARTICLE_SYSTEM_MANAGER_DESC*	PARTICLE_SYSTEM_MANAGER_DESC::_pInstance = NULL;
bool			PARTICLE_SYSTEM_MANAGER_DESC::_bDestroyedOnce = false;
static const char * const pcComment = "Ask 3D Engine to document this class.";

//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER_DESC constructor
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
PARTICLE_SYSTEM_MANAGER_DESC::PARTICLE_SYSTEM_MANAGER_DESC()
:DESC(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_PARTICLE_SYSTEM_MANAGER , "m3d_particle_system_manager" , false, NB_COMMANDS, pcComment)
{
	_pCommandDescsTab[ 0 ].Init( COMMAND_CREATE_PARTICLE_SHAPE , 2 , "create_particle_shape", "Ask 3D Engine to document this command");
	_pCommandDescsTab[ 0 ].SetParameterDesc( 0 , OTYPE_KERNEL_CHAR , 32 , PARAMETER_DESC::IN_PARAMETER, "cChar" );
	_pCommandDescsTab[ 0 ].SetParameterDesc( 1 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::OUT_PARAMETER, "Id" );

	_pCommandDescsTab[ 1 ].Init( COMMAND_CREATE_PARTICLE_SHAPE_SPRITE , 2 , "create_particle_shape_sprite", "Ask 3D Engine to document this command");
	_pCommandDescsTab[ 1 ].SetParameterDesc( 0 , OTYPE_KERNEL_CHAR , 32 , PARAMETER_DESC::IN_PARAMETER, "cChar" );
	_pCommandDescsTab[ 1 ].SetParameterDesc( 1 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::OUT_PARAMETER, "Id" );
}

//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER_DESC destructor
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
PARTICLE_SYSTEM_MANAGER_DESC::~PARTICLE_SYSTEM_MANAGER_DESC()
{

}


//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER_DESC::New
// Object: 
//01-04-25 FKE Created: 
//-----------------------------------------------------------------------------
COM_OBJECT		*PARTICLE_SYSTEM_MANAGER_DESC::New()
{
	_nNbObjects++;
	if (_nNbObjects == 1)
	{
		return new PARTICLE_SYSTEM_MANAGER;
	}
	else
	{
		return PARTICLE_SYSTEM_MANAGER::Instance();
	}
}


//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::Delete
// Object: 
//01-04-25 FKE Created: 
//-----------------------------------------------------------------------------
void PARTICLE_SYSTEM_MANAGER_DESC::Delete(COM_OBJECT *pC)
{
	_nNbObjects--;
	if (_nNbObjects == 0)
	{
		delete pC;
	}
}	


//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER_DESC::Instance
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
DESC*	PARTICLE_SYSTEM_MANAGER_DESC::Instance()
{
	QDT_ASSERT( _bDestroyedOnce == false );

	if ( _pInstance == NULL )
	{
		_pInstance = ( new PARTICLE_SYSTEM_MANAGER_DESC() );
	}
	
	return ( _pInstance );
}

//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER_DESC::Suicide
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM_MANAGER_DESC::Suicide()
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
// Name: PARTICLE_SYSTEM_MANAGER_DESC::ReceiveCommandProc
// Object:
//00 - 12 - 05 JVA Created:
//-----------------------------------------------------------------------------
bool	PARTICLE_SYSTEM_MANAGER_DESC::ReceiveCommandProc(void *pObject, COMMAND_ID CommandID , PARAMETER_STACK & Stack )const
{
	PARTICLE_SYSTEM_MANAGER *pManager = static_cast <PARTICLE_SYSTEM_MANAGER *> (pObject);

	OBJECT_ID oid;
	QDT_ASSERT(pManager);
	
	switch ( CommandID )
	{
	case PARTICLE_SYSTEM_MANAGER_DESC::COMMAND_CREATE_PARTICLE_SHAPE:
		{
				
			INDEX_INT	*pParticleShape;
			
			const char *pcChar = Stack[0];
			pParticleShape = pManager->CreateParticleShape( const_cast<char*>(pcChar) );

			if ( ! pParticleShape )
			{
				Stack.SetObjectID( 1 , NULL_ID );
				return true;
			}

			pManager->AddParticleShape( pParticleShape );
		}
		break;

	case PARTICLE_SYSTEM_MANAGER_DESC::COMMAND_CREATE_PARTICLE_SHAPE_SPRITE:
		{
				
			INDEX_INT	*pParticleShapeSprite;
			
			const char *pcChar = Stack[0];
			pParticleShapeSprite = pManager->CreateParticleShapeSprite( const_cast<char*>(pcChar) );

			if ( ! pParticleShapeSprite )
			{
				Stack.SetObjectID( 1 , NULL_ID );
				return true;
			}

			pManager->AddParticleShape( pParticleShapeSprite );
		}
		break;

	default:
		return DefaultCommand(pManager, CommandID, Stack);
	}
	return true;
}


//=============================================================================
// CODE ENDS HERE
//=============================================================================
