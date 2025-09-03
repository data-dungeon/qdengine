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
//	CLASS:	PATH_FIND_UNIT
//	The PATH_FIND_UNIT class implements ...
//
//	02-07-11:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	PATH_FIND_UNIT : public COMMUNICATOR
{
public:

										PATH_FIND_UNIT();
	explicit							PATH_FIND_UNIT(const PATH_FIND_UNIT & PU);
									   ~PATH_FIND_UNIT();

	PATH_FIND_UNIT &					operator = (const PATH_FIND_UNIT & PU);
			
	float								GetCurrentDirection();
	float								GetDistanceToTarget();
	
	void								GoTo(POINT3D &Target);
	
	_INLINE_ INDEX_INT*					GetNodeIndex() const	{ return (_piNode); }
	_INLINE_ QUADTREE *					GetQuadtree() const		{ return (static_cast<QUADTREE*>(_piSector->GetObject())); }

	_INLINE_ bool						IsInMove() const		{ return (_bInMove); }
	_INLINE_ bool						IsInSector() const		{ return (_piSector != NULL); }
	_INLINE_ float						GetRadius() const		{ return (_rRadius); }
	_INLINE_ void						SetRadius(float r)		{ _rRadius = r; }

	static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();

private:

	
	void								Copy(const PATH_FIND_UNIT & C);
	void								FindSector();


	float								_rRadius;

	CIRCLE								_BoundingVolume;
	bool								_bInMove;

	INDEX_INT						   *_piNode;
	INDEX_INT						   *_piPath;
	INDEX_INT						   *_piSector;

	static	COMMUNICATOR_DESCRIPTOR		_CD;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
