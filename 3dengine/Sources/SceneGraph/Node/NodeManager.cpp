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
//	CLASS:	NODE_MANAGER
//
//	02-04-05:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Node/NodeManager)
#include	INCL_3DENGINE(M3D/Moteur3DModule)


NODE_MANAGER *	NODE_MANAGER::_pInstance = NULL;

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		NODE_MANAGER constructor
//	Object:		
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
NODE_MANAGER::NODE_MANAGER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		NODE_MANAGER destructor
//	Object:		
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
NODE_MANAGER::~NODE_MANAGER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
NODE_MANAGER *	NODE_MANAGER::Instance()
{
	if (_pInstance == NULL)
	{
		_pInstance = new NODE_MANAGER;
	}

	return (_pInstance);
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
void 	NODE_MANAGER::Suicide()
{
	delete _pInstance;
	_pInstance = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		AddNode
//	Object:		
//	02-04-11:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_MANAGER::AddNode(const COM_INSTANCE &	NodeInst,
							  int					nEntityType)
{
	switch	(nEntityType)
	{
	default:
		break;

	case	MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_EMITTER:
		AddNode(NodeInst, _dlParticleEmitters);
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveNode
//	Object:		
//	02-04-11:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_MANAGER::RemoveNode(const COM_INSTANCE &	NodeInst,
								 int					nEntityType)
{
	switch	(nEntityType)
	{
	default:
		break;

	case	MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_EMITTER:
		RemoveNode(NodeInst, _dlParticleEmitters);
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddNode
//	Object:		
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE_MANAGER::AddNode(const COM_INSTANCE &			Inst,
							  QDT_DLIST <COM_INSTANCE> &	dList)
{
	QDT_DLIST <COM_INSTANCE>::ITERATOR itEach = dList.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR itTail = dList.GetTail();

	while (itEach != itTail)
	{
		if (*itEach == Inst)
		{
			return;
		}
		itEach++;
	}

	dList.PushTail(Inst);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveNode
//	Object:		
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE_MANAGER::RemoveNode(const COM_INSTANCE &		Inst,
								 QDT_DLIST <COM_INSTANCE> &	dList)
{
	QDT_DLIST <COM_INSTANCE>::ITERATOR itEach = dList.GetHead();
	QDT_DLIST <COM_INSTANCE>::ITERATOR itTail = dList.GetTail();

	while (itEach != itTail)
	{
		if (*itEach == Inst)
		{
			dList.Remove(itEach);
			return;
		}

		++itEach;
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
