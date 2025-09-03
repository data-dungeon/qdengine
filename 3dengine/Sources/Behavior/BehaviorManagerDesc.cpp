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
// CLASS: BEHAVIOR_MANAGER_DESC
//
//
//	00-11-20	JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__ ;
#endif	//	QDT_INCLUDE_CI

//*****************************************************************************
//
// CLASS DESCRIPTION ( BEHAVIOR_MANAGER_DESC )
//
//	The BEHAVIOR_MANAGER_DESC class implements ...
//
//*****************************************************************************

#include	"include.h"
#include	INC_KRNCORE(Time)
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)

#include	INC_KRNCORE(Math/Geometry/Quaternions)

#include	INC_3DENGINE(M3D)
#include	INC_3DENGINE(SceneGraph\Node)
#include	INC_3DENGINE(Behavior\Behaviors\Entity Behaviors)

#ifndef _DEBUG
	#include	INL_3DENGINE(SceneGraph/Node)
	
	#include	INL_KRNCORE(Math/Geometry/Matrix)
	#include	INL_KRNCORE(Math/Geometry/Quaternions)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point3D)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)

#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================
BEHAVIOR_MANAGER_DESC*	BEHAVIOR_MANAGER_DESC::_pInstance = NULL;
bool					BEHAVIOR_MANAGER_DESC::_bDestroyedOnce = false;
static const char * const pcComment = "Ask 3D Engine to document this class.";
//-----------------------------------------------------------------------------
// Name: BEHAVIOR_MANAGER_DESC constructor
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
BEHAVIOR_MANAGER_DESC::BEHAVIOR_MANAGER_DESC()
:
DESC(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_BEHAVIOR_MANAGER, "m3d_behavior", true, NB_COMMANDS, pcComment)
{
	_pCommandDescsTab[0].Init(COMMAND_PLAY_ANIMATION , 6 , "PlayAnimation" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[0].SetParameterDesc(0 , OTYPE_KERNEL_OBJECT_ID	, 1 , PARAMETER_DESC::IN_PARAMETER , "Node" ) ;
	_pCommandDescsTab[0].SetParameterDesc(1 , OTYPE_KERNEL_OBJECT_ID	, 1 , PARAMETER_DESC::IN_PARAMETER , "Animation" ) ;
	_pCommandDescsTab[0].SetParameterDesc(2 , OTYPE_KERNEL_INTEGER	, 1 , PARAMETER_DESC::IN_PARAMETER , "Flags" ) ;
	_pCommandDescsTab[0].SetParameterDesc(3 , OTYPE_KERNEL_INTEGER	, 1 , PARAMETER_DESC::IN_PARAMETER , "Repetition" ) ;
	_pCommandDescsTab[0].SetParameterDesc(4 , OTYPE_KERNEL_FLOAT		, 1 , PARAMETER_DESC::IN_PARAMETER , "Speed" ) ;
	_pCommandDescsTab[0].SetParameterDesc(5 , OTYPE_KERNEL_FLOAT		, 1 , PARAMETER_DESC::IN_PARAMETER , "Weight" ) ;
	
	_pCommandDescsTab[1].Init(COMMAND_APPLY_ROTATION , 4 , "ApplyRotation" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[1].SetParameterDesc(0 , OTYPE_KERNEL_OBJECT_ID	, 1 , PARAMETER_DESC::IN_PARAMETER , "Node");
	_pCommandDescsTab[1].SetParameterDesc(1 , OTYPE_KERNEL_VECTOR	, 1 , PARAMETER_DESC::IN_PARAMETER , "RotAxe");
	_pCommandDescsTab[1].SetParameterDesc(2 , OTYPE_KERNEL_FLOAT		, 1 , PARAMETER_DESC::IN_PARAMETER , "Speed");
	_pCommandDescsTab[1].SetParameterDesc(3 , OTYPE_KERNEL_FLOAT		, 1 , PARAMETER_DESC::IN_PARAMETER , "Weight");
	
	_pCommandDescsTab[2].Init(COMMAND_APPLY_SKIN, 2 , "ApplySkin" , "Ask 3D Engine to document this command.");
	_pCommandDescsTab[2].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "SkeletonNode");
	_pCommandDescsTab[2].SetParameterDesc( 1 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Skin");
	
	_pCommandDescsTab[3].Init( COMMAND_FREE_CAMERA , 1 , "FreeCamera" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[3].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "CameraNode");
	
	_pCommandDescsTab[4].Init( COMMAND_TARGET_CAMERA , 2 , "TargetCamera" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[4].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "CameraNode");
	_pCommandDescsTab[4].SetParameterDesc( 1 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "TargetNode");
	
	_pCommandDescsTab[5].Init( COMMAND_SET_SPEED , 3 , "SetSpeed" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[5].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Id");
	_pCommandDescsTab[5].SetParameterDesc( 1 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Id");
	_pCommandDescsTab[5].SetParameterDesc( 2 , OTYPE_KERNEL_FLOAT		, 1 , PARAMETER_DESC::IN_PARAMETER , "rSpeed");
	
	_pCommandDescsTab[6].Init( COMMAND_CLEAR_ANIMATION , 3 , "ClearAnimation" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[6].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Id");
	_pCommandDescsTab[6].SetParameterDesc( 1 , OTYPE_KERNEL_INTEGER	, 1 , PARAMETER_DESC::IN_PARAMETER , "n");
	_pCommandDescsTab[6].SetParameterDesc( 2 , OTYPE_KERNEL_INTEGER	, 1 , PARAMETER_DESC::IN_PARAMETER , "n");

	_pCommandDescsTab[7].Init( COMMAND_DELETE_ANIMATION , 2 , "DeleteAnimation" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[7].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Node");
	_pCommandDescsTab[7].SetParameterDesc( 1 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Animation");

	_pCommandDescsTab[8].Init( COMMAND_PURGE_BEHAVIOR , 1 , "PurgeBehavior" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[8].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Node");

	_pCommandDescsTab[9].Init( COMMAND_DETACH_SKIN , 1 , "DetachSkin" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[9].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Node");

	_pCommandDescsTab[10].Init( COMMAND_MOVE_TO , 4 , "MoveToNode" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[10].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Node");
	_pCommandDescsTab[10].SetParameterDesc( 1 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Node");
	_pCommandDescsTab[10].SetParameterDesc( 2 , OTYPE_KERNEL_INTEGER	, 1 , PARAMETER_DESC::IN_PARAMETER , "Time to go (ms)");
	_pCommandDescsTab[10].SetParameterDesc( 3 , OTYPE_KERNEL_INTEGER	, 1 , PARAMETER_DESC::IN_PARAMETER , "Flags" ) ;

	_pCommandDescsTab[11].Init( COMMAND_MOVE_TO2 , 5 , "MoveToPoint" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[11].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Node");
	_pCommandDescsTab[11].SetParameterDesc( 1 , OTYPE_KERNEL_POINT3D	, 1 , PARAMETER_DESC::IN_PARAMETER , "Position");
	_pCommandDescsTab[11].SetParameterDesc( 2 , OTYPE_KERNEL_QUATERNION, 1 , PARAMETER_DESC::IN_PARAMETER , "Angle");
	_pCommandDescsTab[11].SetParameterDesc( 3 , OTYPE_KERNEL_INTEGER	, 1 , PARAMETER_DESC::IN_PARAMETER , "Time to go (ms)");
	_pCommandDescsTab[11].SetParameterDesc( 4 , OTYPE_KERNEL_INTEGER	, 1 , PARAMETER_DESC::IN_PARAMETER , "Flags" ) ;

	_pCommandDescsTab[12].Init( COMMAND_PLAY_FACE , 3 , "PlayFace" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[12].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Node");
	_pCommandDescsTab[12].SetParameterDesc( 1 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Facial Animation");
	_pCommandDescsTab[12].SetParameterDesc( 2 , OTYPE_KERNEL_OBJECT_ID	, 1 , PARAMETER_DESC::IN_PARAMETER , "Sound");

	_pCommandDescsTab[13].Init( CMD_ANIMATE_BLEND_SHAPE , 2 , 
								"BlendShapeAnimate", 
								"The blend shape play the given track" );
	_pCommandDescsTab[13].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID , 1 , PARAMETER_DESC::IN_PARAMETER , "Node");
	_pCommandDescsTab[13].SetParameterDesc( 1 , OTYPE_KERNEL_OBJECT_ID	, 1 , PARAMETER_DESC::IN_PARAMETER , "BlendShapePC");
	
	_pCommandDescsTab[14].Init( COMMAND_CONSTRAINT_AIM , 4 , "AimConstraint" , "Ask 3D Engine to document this command." );
	_pCommandDescsTab[14].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID	, 1 , PARAMETER_DESC::IN_PARAMETER , "Tracker node");
	_pCommandDescsTab[14].SetParameterDesc( 1 , OTYPE_KERNEL_OBJECT_ID	, 1 , PARAMETER_DESC::IN_PARAMETER , "Target node");
	_pCommandDescsTab[14].SetParameterDesc( 2 , OTYPE_KERNEL_INTEGER		, 1 , PARAMETER_DESC::IN_PARAMETER , "Flags" ) ;
	_pCommandDescsTab[14].SetParameterDesc( 3 , OTYPE_KERNEL_FLOAT		, 1 , PARAMETER_DESC::IN_PARAMETER , "Inter Value" ) ;

	_pCommandDescsTab[15].Init(COMMAND_CREATE_HIPS_ANIM , 1 , "CreateHipsAnim" , "Create an HIPS_ANIM object.");
	_pCommandDescsTab[15].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::OUT_PARAMETER, "HipsAnimID");
	
	_pCommandDescsTab[16].Init(COMMAND_CREATE_HIPS_ANIM_COMB , 1 , "CreateHipsAnimComb" , "Create an HIPS_ANIM_COMB object.");
	_pCommandDescsTab[16].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::OUT_PARAMETER, "HipsAnimCombID");

	_pCommandDescsTab[17].Init(CMD_INTERPOLATE_BLEND_SHAPE, 4,
							   "BlendShapeInterpolate",
							   "Set the weight of the specified blend shape target. The weight will be interpolated until delay (in ms) specified. Set nDelay to 0 for immediate effect");
	_pCommandDescsTab[17].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID,	1, PARAMETER_DESC::IN_PARAMETER, "Node");
	_pCommandDescsTab[17].SetParameterDesc(1, OTYPE_KERNEL_INTEGER,		1, PARAMETER_DESC::IN_PARAMETER, "nTarget");
	_pCommandDescsTab[17].SetParameterDesc(2, OTYPE_KERNEL_INTEGER,		1, PARAMETER_DESC::IN_PARAMETER, "nDelay");
	_pCommandDescsTab[17].SetParameterDesc(3, OTYPE_KERNEL_FLOAT,		1, PARAMETER_DESC::IN_PARAMETER, "rWeight");

	_pCommandDescsTab[18].Init(CMD_BLINK_BLEND_SHAPE, 3,
							   "BlendShapeBlink",
							   "Blink a target of the specified blend shape. You can specify the duration of the blink");
	_pCommandDescsTab[18].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID,	1, PARAMETER_DESC::IN_PARAMETER, "Node");
	_pCommandDescsTab[18].SetParameterDesc(1, OTYPE_KERNEL_INTEGER,		1, PARAMETER_DESC::IN_PARAMETER, "nTarget");
	_pCommandDescsTab[18].SetParameterDesc(2, OTYPE_KERNEL_INTEGER,		1, PARAMETER_DESC::IN_PARAMETER, "nDuration");

	_pCommandDescsTab[19].Init(CMD_PERIODIC_BLINK_BLEND_SHAPE, 6,
							   "BlendShapePeriodicBlink",
							   "Blink a target of the specified blend shape. You can specify the duration of the blink");
	_pCommandDescsTab[19].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID,	1, PARAMETER_DESC::IN_PARAMETER, "Node");
	_pCommandDescsTab[19].SetParameterDesc(1, OTYPE_KERNEL_INTEGER,		1, PARAMETER_DESC::IN_PARAMETER, "nTarget");
	_pCommandDescsTab[19].SetParameterDesc(2, OTYPE_KERNEL_INTEGER,		1, PARAMETER_DESC::IN_PARAMETER, "nDuration");
	_pCommandDescsTab[19].SetParameterDesc(3, OTYPE_KERNEL_INTEGER,		1, PARAMETER_DESC::IN_PARAMETER, "nPeriod");
	_pCommandDescsTab[19].SetParameterDesc(4, OTYPE_KERNEL_INTEGER,		1, PARAMETER_DESC::IN_PARAMETER, "nRandom");
	_pCommandDescsTab[19].SetParameterDesc(5, OTYPE_KERNEL_FLOAT,		1, PARAMETER_DESC::IN_PARAMETER, "rWeight");

	_pCommandDescsTab[20].Init(CMD_GET_BLEND_SHAPE_VALUE , 3 , 
							   "GetBlendShapeValue" , 
							   "Get the value of the specified blend shape target for a given. It returns 0.0 if the target doesn't exist or if the node has no blend shape");
	_pCommandDescsTab[20].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER,  "Node");
	_pCommandDescsTab[20].SetParameterDesc(1, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::IN_PARAMETER,  "TargetAnimID");
	_pCommandDescsTab[20].SetParameterDesc(2, OTYPE_KERNEL_FLOAT	, 1, PARAMETER_DESC::OUT_PARAMETER, "rOutValue");
	
	_pCommandDescsTab[21].Init(COMMAND_CREATE_TARGET_ANIM , 1 , "CreateTargetAnim" , "Create an TARGET_ANIM object.");
	_pCommandDescsTab[21].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::OUT_PARAMETER, "TargetAnimID");

	
}

//-----------------------------------------------------------------------------
//	Name: BEHAVIOR_MANAGER_DESC::New
//	Object: 
//	01-03-15:	RMA - Created
//-----------------------------------------------------------------------------
COM_OBJECT* BEHAVIOR_MANAGER_DESC::New()
{
	if (_nNbObjects == 0)
	{
		_nNbObjects++;
		return new BEHAVIOR_MANAGER;
	}
	else
	{
		_nNbObjects++;
		return BEHAVIOR_MANAGER::Instance();
	}
}


//-----------------------------------------------------------------------------
// Name: BEHAVIOR_MANAGER_DESC::Delete
// Object: 
//01-04-25 FKE Created: 
//-----------------------------------------------------------------------------
void BEHAVIOR_MANAGER_DESC::Delete(COM_OBJECT *pC)
{
	_nNbObjects--;
	if (_nNbObjects == 0)
	{
		delete pC;
	}
}
//-----------------------------------------------------------------------------
// Name: BEHAVIOR_MANAGER_DESC destructor
// Object:
//00 - 11 - 16 JVA Created:
//------------------x-----------------------------------------------------------
BEHAVIOR_MANAGER_DESC::~BEHAVIOR_MANAGER_DESC()
{

}
//-----------------------------------------------------------------------------
// Name: BEHAVIOR_MANAGER_DESC::Instance
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
DESC*	BEHAVIOR_MANAGER_DESC::Instance()
{
	QDT_ASSERT( _bDestroyedOnce == false );

	if ( _pInstance == NULL )
	{
		_pInstance = ( new BEHAVIOR_MANAGER_DESC() );
	}
	
	return ( _pInstance );
}
//-----------------------------------------------------------------------------
// Name: BEHAVIOR_MANAGER_DESC::Suicide
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
void	BEHAVIOR_MANAGER_DESC::Suicide()
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
// Name: BEHAVIOR_MANAGER_DESC::ReceiveCommandProc
// Object:
//00 - 11 - 20 JVA Created:
//-----------------------------------------------------------------------------
bool	BEHAVIOR_MANAGER_DESC::ReceiveCommandProc(void *pObject, COMMAND_ID CommandID , PARAMETER_STACK & Stack )const
{
	BEHAVIOR_MANAGER	*pBehavior = static_cast <BEHAVIOR_MANAGER *> (pObject);
	
	QDT_ASSERT(pBehavior);

	switch ( CommandID )
	{
		
	case BEHAVIOR_MANAGER_DESC::COMMAND_PLAY_ANIMATION:
		{
			OBJECT_ID oid = (Stack.GetObjectID(0));

			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter 1 is not a node");
				break;
			}

			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());

			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find node" );
				break;
			}
			
			OBJECT_ID oidAnim = (Stack.GetObjectID(1));

			if (oidAnim.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_ANIM_PC)
			{
				QDT_Warning("Command aborted : the parameter 2 is not an animation");
				break;
			}

			INDEX_INT *pAnimIndexInt = DATABASE_MANAGER::Instance()->GetById(oidAnim.GetType(), oidAnim.GetID());

			if	(!pAnimIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 2, unable to find animation" );
				break;
			}
			int			nFlags		= Stack.GetInteger	( 2 ) ;
			int			nRepeat		= Stack.GetInteger	( 3 ) ;
			float		rSpeed		= Stack.GetFloat	( 4 ) ;
			float		rWeight		= Stack.GetFloat	( 5 ) ;
			
			QDT_FLAGS Flags(nFlags);
			bool bRetVal = false ; 

			QDT_NOT_IMPLEMENTED();
			// bRetVal = pBehavior->PlayAnimation( pIndexInt, pAnimIndexInt, Flags, nRepeat, rSpeed, rWeight ) ;
			
			return ( bRetVal ) ; 
		}
		break;
		
	case BEHAVIOR_MANAGER_DESC::COMMAND_APPLY_ROTATION:
		{
			OBJECT_ID oid = (Stack.GetObjectID(0));

			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter 1 is not a node");
				break;
			}

			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());

			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find node" );
				break;
			}
		
			VECTOR	Vp( 0.0f , 0.0f , 0.0f );
			VECTOR	v( Stack.GetVector( 1 ) );
			float rSpeedRot	= Stack.GetFloat(2) ;
			float rWeight	= Stack.GetFloat(3) ;
			
			// INDEX_INT *pBeh = pBehavior->CreateConstantRotation( v , Vp , 20.0f , 0.0f , 1.0f , rSpeedRot , 0.0f , rWeight );

			QDT_NOT_IMPLEMENTED();
			/*if ( ! pBehavior->ApplyBehaviorToNode(pIndexInt , pBeh ) )
			{
				DATABASE_MANAGER::Instance()->Delete( pBeh->GetComObject()->GetType(), pBeh->GetId() ) ; 
			}*/
		}
		break ; 	
		
	case BEHAVIOR_MANAGER_DESC::COMMAND_APPLY_SKIN:
		// $$$$ >> >> 00 - 11 - 14 GHP MODIF
		{
			NODE *pNode = NULL;
			SKIN *pSkin = NULL;
			
			OBJECT_ID oidSkeleton = (Stack.GetObjectID(0));
			
			if ( oidSkeleton.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE )
			{
				QDT_Error("Command aborted : the parameter 1 is not a node");
				break;
			}

			INDEX_INT *pSkeletonIndexInt = DATABASE_MANAGER::Instance()->GetById(oidSkeleton.GetType(), oidSkeleton.GetID());

			if (!pSkeletonIndexInt)
			{
				QDT_Error( "Command aborted : parameter 1, unable to find node" );
				break;
			}

			OBJECT_ID oidSkin = (Stack.GetObjectID(1));

			if (oidSkin.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_SKIN)
			{
				QDT_Error("Command aborted : the parameter 2 is not a skin");
				break;
			}

			INDEX_INT *pSkinIndexInt = DATABASE_MANAGER::Instance()->GetById(oidSkin.GetType(), oidSkin.GetID());
			
			if (!pSkinIndexInt)
			{
				QDT_Error( "Command aborted : parameter 2, unable to find skin" );
				break;
			}
			
			//	Get skeleton node
			pNode = static_cast< NODE* > ( pSkeletonIndexInt->GetObject() );
			QDT_ASSERT( pNode );

			//	Get skin
			pSkin = static_cast< SKIN* > ( pSkinIndexInt->GetObject() );
			QDT_ASSERT( pSkin );

//FKE : Change de méthode
			if ( pSkeletonIndexInt->IsLoaded() == false )
			{
				pSkeletonIndexInt->ForceLoad();
			}
//FKE		

			pBehavior->ApplySkin( pSkin , pNode );
		}
		break;
		// $$$$ << << END 00 - 11 - 14 GHP MODIF

	case BEHAVIOR_MANAGER_DESC::COMMAND_FREE_CAMERA :
		{
			OBJECT_ID oid = (Stack.GetObjectID(0));

			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter is not a node");
				break;
			}

			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());

			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : unable to find node" );
				break;
			}

			QDT_NOT_IMPLEMENTED();
			// pBehavior->FreeCamera( pIndexInt );
		}
		break;
		
	case BEHAVIOR_MANAGER_DESC::COMMAND_TARGET_CAMERA :
		{
			OBJECT_ID oidCam = (Stack.GetObjectID(0));

			if (oidCam.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter 1 is not a node");
				break;
			}

			INDEX_INT *pCameraIndexInt = DATABASE_MANAGER::Instance()->GetById(oidCam.GetType(), oidCam.GetID());

			if (!pCameraIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find node" );
				break;
			}

			OBJECT_ID oidTarget = (Stack.GetObjectID(1));

			if (oidTarget.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter 2 is not a node");
				break;
			}

			INDEX_INT *pTargetIndexInt = DATABASE_MANAGER::Instance()->GetById(oidTarget.GetType(), oidTarget.GetID());
			
			if (!pTargetIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 2, unable to find node" );
				break;
			}

			QDT_NOT_IMPLEMENTED();
			// pBehavior->TargetCamera( pCameraIndexInt, pTargetIndexInt );
		}
		break;

	case BEHAVIOR_MANAGER_DESC::COMMAND_SET_SPEED :
		{
			OBJECT_ID oidChild = (Stack.GetObjectID(0));
			OBJECT_ID oidBehavior = (Stack.GetObjectID(1));

			INDEX_INT *pChildIndexInt = DATABASE_MANAGER::Instance()->GetById(oidChild.GetType(), oidChild.GetID());
			QDT_ASSERT( pChildIndexInt );

			INDEX_INT *pBehaviorIndexInt = DATABASE_MANAGER::Instance()->GetById(oidBehavior.GetType(), oidBehavior.GetID());
			QDT_ASSERT( pBehaviorIndexInt );
			
			if (pChildIndexInt != NULL)
			{
#ifdef _DEBUG
				QDT_Message("SetSpeed Started");
#endif // _DEBUG
				// pBehavior->SetSpeed( pChildIndexInt, pBehaviorIndexInt, Stack.GetFloat(2) );
#ifdef _DEBUG
				QDT_Message("SetSpeed ENDED");
#endif // _DEBUG
			}
			else
			{
				QDT_Warning( "Unable to find Node" );
				return true;
			}
			break;
		}
	case BEHAVIOR_MANAGER_DESC::COMMAND_DELETE_ANIMATION :
		{
			OBJECT_ID oidChild = (Stack.GetObjectID(0));
			OBJECT_ID oidBehavior = (Stack.GetObjectID(1));

			INDEX_INT *pChildIndexInt = DATABASE_MANAGER::Instance()->GetById(oidChild.GetType(), oidChild.GetID());
			QDT_ASSERT( pChildIndexInt );

			INDEX_INT *pBehaviorIndexInt = DATABASE_MANAGER::Instance()->GetById(oidBehavior.GetType(), oidBehavior.GetID());
			QDT_ASSERT( pBehaviorIndexInt );
			
			if (pChildIndexInt != NULL)
			{
				QDT_NOT_IMPLEMENTED();
				// pBehavior->DeleteAnimation( pChildIndexInt, pBehaviorIndexInt );
			}
			else
			{
				QDT_Warning( "Unable to find Node" );
				return true;
			}
			break;
		}
	case BEHAVIOR_MANAGER_DESC::COMMAND_MOVE_TO : 
		{
			OBJECT_ID oidNode		= (Stack.GetObjectID(0));
			OBJECT_ID oidDest		= (Stack.GetObjectID(1));
			
			INDEX_INT *pNodeIndexInt = DATABASE_MANAGER::Instance()->GetById(oidNode.GetType(), oidNode.GetID());
			QDT_ASSERT( pNodeIndexInt );
			
			INDEX_INT *pDestIndexInt = DATABASE_MANAGER::Instance()->GetById(oidDest.GetType(), oidDest.GetID());
			QDT_ASSERT( pDestIndexInt );
			
			if (!pNodeIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find node source " );
				break;
			}
			if (!pDestIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find dest node" );
				break;
			}

			if (pNodeIndexInt != NULL && pDestIndexInt != NULL )
			{
				int		nTime		= Stack.GetInteger	( 2 ) ;
				int		nFlags		= Stack.GetInteger	( 3 ) ;

				QDT_FLAGS Flags(nFlags) ;

				QDT_NOT_IMPLEMENTED();
				// pBehavior->MoveToNode( pNodeIndexInt, pDestIndexInt,  nTime, Flags);
			}
			break;
		}
		break ; 

		
	case COMMAND_MOVE_TO2 : 
		{
			OBJECT_ID oidNode		= (Stack.GetObjectID(0));
			
			
			INDEX_INT *pNodeIndexInt = DATABASE_MANAGER::Instance()->GetById(oidNode.GetType(), oidNode.GetID());
			QDT_ASSERT( pNodeIndexInt );
			
			if (pNodeIndexInt != NULL )
			{
				POINT3D		destPos		= Stack.GetPoint3D( 1 ) ;
				QUATERNION	destAng( Stack.GetQuaternion( 2 ) ) ;
				int			nTime		= Stack.GetInteger( 3 ) ;
				int			nFlags		= Stack.GetInteger( 4 ) ;
				
				QDT_FLAGS Flags(nFlags) ;

				MATRIX		destMatrix ;
				VECTOR		Translation( destPos ) ;
				destMatrix.SetTranslation( Translation ) ;
				destMatrix.SetQuaternion( destAng ) ;

				QDT_NOT_IMPLEMENTED();
				//pBehavior->MoveTo( pNodeIndexInt, destMatrix,  nTime, Flags);
			}
			else
			{
				QDT_Warning( "Unable to find Node" );
				return true;
			}
			break;
		}
		break ; 

	case COMMAND_CLEAR_ANIMATION:
		{
			OBJECT_ID oid = (Stack.GetObjectID(0));

			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter is not a node");
				break;
			}

			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());

			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : unable to find node" );
				break;
			}

			int		nSequence		= Stack.GetInteger	( 1 ) ;
			int		nBehavior		= Stack.GetInteger	( 2 ) ;

			pBehavior->ClearAnimation( pIndexInt, nSequence, nBehavior ) ;
		}
		break;

	case COMMAND_PURGE_BEHAVIOR :
		{
			OBJECT_ID oid = (Stack.GetObjectID(0));
			
			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter is not a node");
				break;
			}
			
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());
			
			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : unable to find node" );
				break;
			}
			
			pBehavior->PurgeBehavior( pIndexInt ) ;
		}
		break ;

	case COMMAND_DETACH_SKIN:
		{
			OBJECT_ID oid = (Stack.GetObjectID(0));
			NODE		*pSKL;

			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter is not a node");
				break;
			}

			INDEX_INT	*pIndexInt	= DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());

			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : unable to find node" );
				break;
			}

			pSKL = static_cast<NODE*> ( pIndexInt->GetObject() );
			if ( pSKL->GetEntityType() != ENTITY_SKELETON )
			{
				QDT_Warning( "Command aborted : Not a skeleton node" );
				break;
			}
			pBehavior->DetachSkin( pSKL );
			
		}
	case COMMAND_PLAY_FACE :
		{
			QDT_FAIL();	
		}
		break ; 

	case CMD_ANIMATE_BLEND_SHAPE :
		{
			OBJECT_ID oid = (Stack.GetObjectID(0));
			
			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter 1 is not a node");
				break;
			}
			
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());
			
			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find node" );
				break;
			}
			
			OBJECT_ID oidBlendShapePC = (Stack.GetObjectID(1));
			
			if (oidBlendShapePC.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_BLEND_SHAPE_PC)
			{
				QDT_Warning("Command aborted : the parameter 2 is not an BlendShapePC");
				break;
			}
			
			INDEX_INT *pBlendShapePCInt = DATABASE_MANAGER::Instance()->GetById(oidBlendShapePC.GetType(), oidBlendShapePC.GetID());
			
			if	(!pBlendShapePCInt)
			{
				QDT_Warning( "Command aborted : parameter 2, unable to find BlendShapePC" );
				break;
			}

			ENTITY_BEHAVIOR_FACTORY::Instance()->ApplyBlendShapeAnimation(pIndexInt, pBlendShapePCInt);
		}
		break;

	case COMMAND_CONSTRAINT_AIM : 
		{
			OBJECT_ID oidCam = (Stack.GetObjectID(0));
			
			if (oidCam.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter 1 is not a node");
				break;
			}
			
			INDEX_INT *pCameraIndexInt = DATABASE_MANAGER::Instance()->GetById(oidCam.GetType(), oidCam.GetID());
			
			if (!pCameraIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find node" );
				break;
			}
			
			OBJECT_ID oidTarget = (Stack.GetObjectID(1));
			
			if (oidTarget.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter 2 is not a node");
				break;
			}
			
			INDEX_INT *pTargetIndexInt = DATABASE_MANAGER::Instance()->GetById(oidTarget.GetType(), oidTarget.GetID());
			
			if (!pTargetIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 2, unable to find node" );
				break;
			}
			
			int		nFlags		= Stack.GetInteger( 2 ) ;
			QDT_FLAGS Flags(nFlags) ;
			float	rInterCoeff	= Stack.GetFloat(3) ; 

			QDT_NOT_IMPLEMENTED();
			// pBehavior->AimConstraint( pCameraIndexInt, pTargetIndexInt, nFlags, rInterCoeff );
		}
		break ; 

	case	COMMAND_CREATE_HIPS_ANIM:
		{
			INDEX_INT		*pIdx;
			INDEX_INT		*pOld;
			OBJECT_ID		ObjectId;

			// pIdx = pBehavior->CreateHipsAnim();
			QDT_NOT_IMPLEMENTED();
			if ( !pIdx )
			{
				QDT_FAIL();
				return( false );
			}

			//Index/ComObject RefCounting
			//decrement reference on object passed by
			ObjectId  = Stack.GetObjectID(0);
			pOld = DATABASE_MANAGER::Instance()->GetById(ObjectId.GetType(), ObjectId.GetID());
			if	(pOld)
			{
				pOld->RemoveReference();
			}

			//increment reference of the one created by the behavior
			ObjectId = pIdx->GetObjectId();
			pIdx->AddReference();

			Stack.SetObjectID( 0, ObjectId );

			break;
		}

	case	COMMAND_CREATE_HIPS_ANIM_COMB:
		{
			INDEX_INT		*pIdx;
			OBJECT_ID		ObjectId;

			QDT_NOT_IMPLEMENTED();
			// pIdx = pBehavior->CreateHipsAnimComb();
			if ( !pIdx )
			{
				QDT_FAIL();
				return(false);
			}
			ObjectId = pIdx->GetObjectId();
			Stack.SetObjectID( 0, ObjectId );
			break;
		}

	case	CMD_INTERPOLATE_BLEND_SHAPE:
		{
			OBJECT_ID	oid = (Stack.GetObjectID(0));
			int			nTarget		= Stack[1];
			int			nDuration	= Stack[2];
			float		rWeight		= Stack[3];
			
			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter 1 is not a node");
				break;
			}
			
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());
			
			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find node");
				break;
			}

			float rConvert = TIME::Instance()->GetTargetFPS() / 1000.0f;
			float rDuration	= static_cast<float>(nDuration  * rConvert);
			
			ENTITY_BEHAVIOR_FACTORY::Instance()->ApplyBlendShapeInterpolation(pIndexInt, nTarget, rWeight, rDuration);
			break;
		}

	case	CMD_BLINK_BLEND_SHAPE :
		{
			OBJECT_ID	oid = (Stack.GetObjectID(0));
			int			nTarget		= Stack[1];
			int			nDuration	= Stack[2];
			float		rWeight		= Stack[3];
			
			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter 1 is not a node");
				break;
			}
			
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());
			
			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find node");
				break;
			}

			float rConvert = TIME::Instance()->GetTargetFPS() / 1000.0f;
			float rDuration	= static_cast<float>(nDuration  * rConvert);
			
			ENTITY_BEHAVIOR_FACTORY::Instance()->ApplyBlendShapeBlink(pIndexInt, nTarget, rWeight, rDuration);
			break;
		}

	case	CMD_PERIODIC_BLINK_BLEND_SHAPE :
		{
			OBJECT_ID	oid = (Stack.GetObjectID(0));
			int			nTarget		= Stack[1];
			int			nDuration	= Stack[2];
			int			nPeriod		= Stack[3];
			int			nRandom		= Stack[4];
			float		rWeight		= Stack[5];
			
			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter 1 is not a node");
				break;
			}
			
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());
			
			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 0, unable to find node");
				break;
			}

			float rConvert = TIME::Instance()->GetTargetFPS() / 1000.0f;

			float rDuration	= static_cast<float>(nDuration  * rConvert);
			float rPeriod	= static_cast<float>(nPeriod	* rConvert);
			float rRandom	= static_cast<float>(nRandom	* rConvert);
			
			ENTITY_BEHAVIOR_FACTORY::Instance()->ApplyBlendShapePeriodicBlink(pIndexInt, nTarget, rWeight, rDuration, rPeriod, rRandom);
			break;
		}

	case CMD_GET_BLEND_SHAPE_VALUE :
		{
			const OBJECT_ID	&OId		= Stack[0];
			int			nTarget = Stack[1];

			if (OId.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter 1 is not a node");
				break;
			}
			
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(OId.GetType(), OId.GetID());
			
			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find node");
				break;
			}

			float rRes = ENTITY_BEHAVIOR_FACTORY::Instance()->GetBlendShapeValue(pIndexInt, nTarget);

			Stack.SetFloat(2, rRes);
		}
		break;

	case COMMAND_CREATE_TARGET_ANIM:
		{
			INDEX_INT		*pIdx;
			OBJECT_ID		ObjectId;

			QDT_NOT_IMPLEMENTED();
			// pIdx = pBehavior->CreateTargetAnim();
			if ( !pIdx )
			{
				QDT_FAIL();
				return(false);
			}
			ObjectId = pIdx->GetObjectId();
			Stack.SetObjectID( 0, ObjectId );
			break;
		}

	default:
		return DefaultCommand(pBehavior, CommandID, Stack);
	}

	return (true);
}


//=============================================================================
// CODE ENDS HERE
//=============================================================================
