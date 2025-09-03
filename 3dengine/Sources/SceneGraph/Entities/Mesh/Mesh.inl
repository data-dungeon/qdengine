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
//	CLASS:	MESH
//
//	05-10-27:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetFlags
//	Object:		Get a flag mask
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
Bool MESH::GetMeshFlags(UInt32	uiFlagsMask) const
{
	return (_fMeshFlags.IsSet(uiFlagsMask));
}

//-----------------------------------------------------------------------------
//	Name:		SetFlags
//	Object:		Set a flag mask
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void MESH::SetMeshFlags(UInt32	uiFlagsMask,
						Bool	b)
{
	_fMeshFlags.Set(uiFlagsMask, b);
}

//-----------------------------------------------------------------------------
//	Name:		IsInvisible
//	Object:		
//	06-01-30:	ELE - Created
//-----------------------------------------------------------------------------
bool	MESH::GetVisible() const
{
	return (_fDisplayFlags.IsSet(DISPLAY_FLAG_VISIBLE));
}

//-----------------------------------------------------------------------------
//	Name:		SetInvisible
//	Object:		
//	06-01-30:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::SetVisible(bool	b)
{
	_fDisplayFlags.Set(DISPLAY_FLAG_VISIBLE, b);
}

//-----------------------------------------------------------------------------
//	Name:		GetCastShadow
//	Object:		
//	06-03-10:	RMA - Created
//-----------------------------------------------------------------------------
bool	MESH::GetCastShadows() const
{
	return (_fDisplayFlags.IsSet(DISPLAY_FLAG_CAST_SHADOWS));
}

//-----------------------------------------------------------------------------
//	Name:		SetCastShadow
//	Object:		
//	06-03-10:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH::SetCastShadows(bool	b)
{
	_fDisplayFlags.Set(DISPLAY_FLAG_CAST_SHADOWS, b);
}

//-----------------------------------------------------------------------------
//	Name:		GetReceiveShadow
//	Object:		
//	06-03-28:	SBE - Created
//-----------------------------------------------------------------------------
bool	MESH::GetReceiveShadows() const
{
	return (_fDisplayFlags.IsSet(DISPLAY_FLAG_RECEIVE_SHADOWS));
}

//-----------------------------------------------------------------------------
//	Name:		SetReceiveShadow
//	Object:		
//	06-03-28:	SBE - Created
//-----------------------------------------------------------------------------
void	MESH::SetReceiveShadows(bool	b)
{
	_fDisplayFlags.Set(DISPLAY_FLAG_RECEIVE_SHADOWS, b);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrVertexBuffers
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	MESH::GetNbrVertexBuffers() const
{
	if	(_MeshDataCI.HasHandle() && _MeshDataCI.IsLoaded())
	{
		return	(static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator())->GetNbrVertexBuffers());
	}
	else
	{
		return	(0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrIndexBuffers
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	MESH::GetNbrIndexBuffers() const
{
	if	(_MeshDataCI.HasHandle() && _MeshDataCI.IsLoaded())
	{
		return	(static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator())->GetNbrIndexBuffers());
	}
	else
	{
		return	(0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPrimitives
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	MESH::GetNbrPrimitives() const
{
	return	(_nNbrPrimitives);
}

//-----------------------------------------------------------------------------
//	Name:		GetGlobalBoundingBoxes
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
OBB *	MESH::GetGlobalBoundingBoxes()
{
	return	(_pGlobalBoundingBox);
}

//-----------------------------------------------------------------------------
//	Name:		GetGlobalBoundingBox
//	Object:		
//	05-11-02:	ELE - Created
//-----------------------------------------------------------------------------
OBB *	MESH::GetGlobalBoundingBox(UInt32	nIndex)
{
	QDT_ASSERT(nIndex < (Int32)_nNbrPrimitives);
	return	(&_pGlobalBoundingBox[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetStaticActor
//	Object:		
//	05-11-14:	ELE - Created
//-----------------------------------------------------------------------------
I_PHYSIC_STATIC_ACTOR *	MESH::GetStaticActor()
{
	return	(_pStaticActor);
}

//-----------------------------------------------------------------------------
//	Name:		SetMeshData
//	Object:		
//	06-01-10:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::SetMeshData(const COM_INSTANCE &	MeshDataInst)
{
	if (_MeshDataCI != MeshDataInst)
	{
		_fMeshFlags.Set(MESH_FLAG_INIT, false);
		Clear();
		_MeshDataCI = MeshDataInst;
		//Init();
	}
}

//-----------------------------------------------------------------------------
//	Name:		InitMeshData
//	Object:		
//	06-02-02:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::InitMeshData(const COM_INSTANCE &	MeshDataInst)
{
	_MeshDataCI = MeshDataInst;
}

//-----------------------------------------------------------------------------
//	Name:		InitPhysicData
//	Object:		
//	06-02-21:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::InitPhysicData(const COM_INSTANCE &	PhysicDataInst)
{
	_PhysicDataCI = PhysicDataInst;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
