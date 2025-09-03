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
//	CLASS:	COLLISION_3D_STRUCT
//	The COLLISION_3D_STRUCT class implements ...
//
//	05-04-29:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	COLLISION_3D_STRUCT
{
public:

	_INLINE_		COLLISION_3D_STRUCT();
	_INLINE_		~COLLISION_3D_STRUCT();

	_INLINE_ void	SetNodeInst(const COM_INSTANCE & NodeInst);
	_INLINE_ void	SetOldPos(const VECTOR & OldPos);
	_INLINE_ void	SetBoundingSphereRadius(float rRadius);
	
	_INLINE_ const COM_INSTANCE &	GetNodeInst() const;
	_INLINE_ const VECTOR &			GetOldPos() const;

	_INLINE_ void	Reset();

	void			Collide();

private:

	bool			Collide(const VECTOR & NodePosition, VECTOR & NewNodePosition);

	COM_INSTANCE	_NodeInst;
	VECTOR			_OldPos;
	float			_rRadius;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
