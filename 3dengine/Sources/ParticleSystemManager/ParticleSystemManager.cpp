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
// CLASS: PARTICLE_SYSTEM_MANAGER
//
//
//	00-1113		JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif
//*****************************************************************************
//
// CLASS DESCRIPTION ( PARTICLE_SYSTEM_MANAGER )
//
//	The PARTICLE_SYSTEM_MANAGER class implements ...
//
//*****************************************************************************

#include	"include.h"
#include	INC_KRNCOM(OldCom/Chunk)
#include	INC_KERNEL(Time)
#include	INC_KERNEL(Database)
#include	INC_3DENGINE(SceneGraph\Node)
#include	INC_3DENGINE(SceneGraph\Entities\ParticleSystem)
#include	INC_3DENGINE(M3D)

#ifndef	_DEBUG
	#include	INL_3DENGINE(M3D)
#endif

PARTICLE_SYSTEM_MANAGER	*PARTICLE_SYSTEM_MANAGER::_pInstance = NULL;

int PARTICLE_SYSTEM_MANAGER::_nNbChunk = 0;
CHUNK PARTICLE_SYSTEM_MANAGER::_ChunkList[PARTICLE_SYSTEM_MANAGER::NB_CHUNK];

//=============================================================================
// CODE STARTS HERE
//=============================================================================
//-----------------------------------------------------------------------------
// Name:	MOTEUR3D::Instance
// Object:	For singleton : Instance 
// 01-01-24 FKE Created: 
//-----------------------------------------------------------------------------
PARTICLE_SYSTEM_MANAGER* PARTICLE_SYSTEM_MANAGER::Instance()
{
	if (_pInstance == NULL) 
	{
		DATABASE_MANAGER::Instance()->CreateDynamic("particle_manager", M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_PARTICLE_SYSTEM_MANAGER);
	}
	
	return (_pInstance); 
}

//-----------------------------------------------------------------------------
// Name:	MOTEUR3D::Suicide
// Object:  For singleton : suicide
//01-01-24	FKE Created: 
//-----------------------------------------------------------------------------
void PARTICLE_SYSTEM_MANAGER::Suicide()
{
	QDT_ASSERT(_pInstance != NULL);
	
	if (_pInstance != NULL) 
	{
		INDEX_INT *pII = DATABASE_MANAGER::Instance()->GetByTypeAndName( M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_PARTICLE_SYSTEM_MANAGER, "particle_manager");
		DATABASE_MANAGER::Instance()->Delete( M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_PARTICLE_SYSTEM_MANAGER, pII->GetId());
	}
}


//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER constructor
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
PARTICLE_SYSTEM_MANAGER::PARTICLE_SYSTEM_MANAGER()
{
	_vParticleSystemNodes.Reserve( 10);
	_vParticleSystemNodes.Clear();

	_vParticleShapes.Reserve( 10 );
	_vParticleShapes.Clear();
	if (_nNbChunk == 0)
	{
		_nNbChunk = NB_CHUNK;
		_ChunkList[0].Set("PIPO", 1);
	}
	_pInstance = this;
}

//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER destructor
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
PARTICLE_SYSTEM_MANAGER::~PARTICLE_SYSTEM_MANAGER()
{
	QDT_VECTOR< INDEX_INT* > ::ITERATOR		itParticleShapes;
	
	itParticleShapes = _vParticleShapes.GetHead();
	
	for ( itParticleShapes ; itParticleShapes != _vParticleShapes.GetTail() ; ++itParticleShapes )
	{
							
		INDEX_INT *pII = *itParticleShapes;
		DATABASE_MANAGER::Instance()->Delete(pII->GetComObject()->GetType(), pII->GetId());
	}
	_pInstance = NULL;
}


const CHUNK&	PARTICLE_SYSTEM_MANAGER::GetChunk(int i) const
{
	QDT_ASSERT(i < _nNbChunk);
	return _ChunkList[_nNbChunk];
}

int PARTICLE_SYSTEM_MANAGER::GetNbChunk() const
{
	return _nNbChunk;
}

//-----------------------------------------------------------------------------
// Name: PARTICULE_SYSTEM_MANAGER::Read
// Object: 
//01-01-25 FKE Created: 
//-----------------------------------------------------------------------------
bool PARTICLE_SYSTEM_MANAGER::Read(const CHUNK &Chunk, IO_STREAM &s)
{
	return false;
}

//-----------------------------------------------------------------------------
// Name: PARTICULE_SYSTEM_MANAGER::Write
// Object: 
//01-01-25 FKE Created: 
//-----------------------------------------------------------------------------
bool PARTICLE_SYSTEM_MANAGER::Write(const CHUNK &Chunk, IO_STREAM &s) const
{
	return false;
}

//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::Init
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM_MANAGER::Init()
{

}

//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::Loop
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM_MANAGER::Loop()
{
	float rTime = 1.0f / TIME::GetScaleRate(); 	

	// Update of the particle shapes

	INDEX_INT	*pParticleShape;
	QDT_VECTOR< INDEX_INT* > ::ITERATOR itParticleShapes;

	itParticleShapes = _vParticleShapes.GetHead();
	
	for ( itParticleShapes ; itParticleShapes != _vParticleShapes.GetTail() ; ++itParticleShapes )
	{					
		pParticleShape = (*itParticleShapes);

		((PARTICLE_SHAPE *)pParticleShape->GetObject())->Update(rTime) ;
	}

	QDT_DLIST<INDEX_INT *>::ITERATOR It		= MOTEUR3D::Instance()->GetSceneGraphsList().GetHead();
	QDT_DLIST<INDEX_INT *>::ITERATOR ItEnd	= MOTEUR3D::Instance()->GetSceneGraphsList().GetTail();

	while (It != ItEnd)
	{
		NODE *pSGNode = static_cast<NODE *>((*It)->GetObject()); 
		pSGNode->UpdateParticleSystemTree(rTime);
		++It;
	}
}

//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::Close
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM_MANAGER::Close()
{

}


//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::GetParticleSystemNode
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
INDEX_INT*	PARTICLE_SYSTEM_MANAGER::GetParticleSystemNode( int nIndex )const
{
		
	return ( *_vParticleSystemNodes.GetIterator( nIndex ) );
}

//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::CreateParticleShape
// Object:
//00 - 12 - 12 JVA Created:
//-----------------------------------------------------------------------------
INDEX_INT*	PARTICLE_SYSTEM_MANAGER::CreateParticleShape( char *pcName )
{
	INDEX_INT	*pII;
	
	pII = DATABASE_MANAGER::Instance()->CreateDynamic(pcName, M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_PARTICLE_SHAPE);

	return ( pII );
}

//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::CreateParticleShapeSprite
// Object:
//00 - 12 - 12 JVA Created:
//-----------------------------------------------------------------------------
INDEX_INT*	PARTICLE_SYSTEM_MANAGER::CreateParticleShapeSprite( char *pcName )
{
	INDEX_INT	*pII;
	
	pII = DATABASE_MANAGER::Instance()->CreateDynamic(pcName, M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_PARTICLE_SHAPE_SPRITE);

	return ( pII );
}

//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::GetParticleShape
// Object:
//00 - 12 - 13 JVA Created:
//-----------------------------------------------------------------------------
INDEX_INT*	PARTICLE_SYSTEM_MANAGER::GetParticleShape( int nIndex )const
{
	return ( *_vParticleShapes.GetIterator( nIndex ) );
}

//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::AddParticleShape
// Object:
//00 - 12 - 12 JVA Created:
//-----------------------------------------------------------------------------
void	PARTICLE_SYSTEM_MANAGER::AddParticleShape( INDEX_INT *pParticleShape )
{
	QDT_ASSERT( pParticleShape );

	_vParticleShapes.PushTail( pParticleShape );
}



//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::GetDesc
// Object:
//00 - 12 - 05 JVA Created:
//-----------------------------------------------------------------------------
DESC*	PARTICLE_SYSTEM_MANAGER::GetDesc()const
{
	return ( PARTICLE_SYSTEM_MANAGER_DESC::Instance() );
}


//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::ReceiveCommandProc
// Object: 
//00-11-13 JVA Created: 
//-----------------------------------------------------------------------------
bool	PARTICLE_SYSTEM_MANAGER::ReceiveCommandProc(COMMAND_ID CommandID, PARAMETER_STACK &Stack)
{
	return GetDesc()->ReceiveCommandProc(this, CommandID, Stack);
}


//-----------------------------------------------------------------------------
// Name: PARTICLE_SYSTEM_MANAGER::GetObject
// Object: 
//01-05-11 FKE Created: 
//-----------------------------------------------------------------------------
void	*PARTICLE_SYSTEM_MANAGER::GetObject()
{
	return this;
}
//=============================================================================
// CODE ENDS HERE
//=============================================================================
