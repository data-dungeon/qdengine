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
//	CLASS:	ENTITY_ABC
//
//	06-01-05:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SetNodeInst
//	Object:		
//	06-01-05:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::SetNodeInst(const COM_INSTANCE &	NodeInst)
{
	_NodeInst = NodeInst;
}

//-----------------------------------------------------------------------------
//	Name:		GetNodeInst
//	Object:		
//	06-01-05:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	ENTITY_ABC::GetNodeInst() const
{
	return	(_NodeInst);
}

//-----------------------------------------------------------------------------
//	Name:		GetNode
//	Object:		
//	06-01-05:	ELE - Created
//-----------------------------------------------------------------------------
NODE *	ENTITY_ABC::GetNode()
{
	if	(_NodeInst.HasHandle())
	{
		return	(static_cast<NODE *>(_NodeInst.GetCommunicator()));
	}

	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		GetNode
//	Object:		
//	06-01-06:	ELE - Created
//-----------------------------------------------------------------------------
const NODE *	ENTITY_ABC::GetNode() const
{
	if	(_NodeInst.HasHandle())
	{
		return	(static_cast<NODE *>(_NodeInst.GetCommunicator()));
	}

	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		SetSceneGraph
//	Object:		
//	06-01-05:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::SetSceneGraph(SCENE_GRAPH * pSceneGraph)
{
	_pSceneGraph = pSceneGraph;
}

//-----------------------------------------------------------------------------
//	Name:		GetSceneGraph
//	Object:		
//	06-01-05:	ELE - Created
//-----------------------------------------------------------------------------
SCENE_GRAPH *	ENTITY_ABC::GetSceneGraph()
{
	return	(_pSceneGraph);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
