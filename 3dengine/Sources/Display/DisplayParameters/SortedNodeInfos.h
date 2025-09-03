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
// CLASS: SORTED_NODE_INFOS
//
//
//	01-05-14:	ELE - Created
//*****************************************************************************


//=============================================================================
// Name: SORTED_NODE_INFOS
//	01-05-14:	ELE - Created
//=============================================================================
class	NODE;

class SORTED_NODE_INFOS
{
public:
	_INLINE_ SORTED_NODE_INFOS();
	_INLINE_ ~SORTED_NODE_INFOS();

	_INLINE_ void			ResetClippingPlanes();
	_INLINE_ void			AddClippingPlane(const PLANE & ClippingPlane);

	_INLINE_ NODE*			GetNode() const;
	_INLINE_ int			GetNbrClippingPlanes() const;
	_INLINE_ const PLANE &	GetClippingPlane(int nNumClippingPlane) const;
	_INLINE_ float			GetDistance() const;
	_INLINE_ int			GetNumObject() const;

	_INLINE_ void			SetNode(NODE *pNode);
	_INLINE_ void			SetDistance(float rDistance);
	_INLINE_ void			SetNumObject(int nNumObject);

private:

	NODE	*_pNode;

	int		_nNbrClippingPlanes ;
	PLANE	_ClippingPlanesArray[DISPLAY::MAX_CLIPPLANE] ;

	float	_rDistanceFromCamera;
	int		_nNumObject;
};

//=============================================================================
// CODE ENDS HERE
//=============================================================================
