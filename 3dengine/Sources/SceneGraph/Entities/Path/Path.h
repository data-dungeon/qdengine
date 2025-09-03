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
//	CLASS:	PATH
//	The PATH class implements ...
//
//	01-09-19:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	PATH_FINDER;
class	QUADTREE;

class	PATH : public ENTITY_ABC
{
public:

	enum
	{
		CMD_PATH_IS_EMPTY = CMD_ENTITY_LAST,
		CMD_PATH_GET_NEXT_WAY_POINT,
		CMD_PATH_LAST,
	};

	_INLINE_ void			ClearWayPoints()						{ _dlWayPoints.Clear(); }
	_INLINE_ void			AddWayPoint(const POINT2D & WayPoint)	{ _dlWayPoints.InsertHead(WayPoint); }
	_INLINE_ int			GetNbrWayPoints() const					{ return (_dlWayPoints.GetSize()); }
	_INLINE_ bool			IsEmpty() const							{ return (_dlWayPoints.IsEmpty()); }
	_INLINE_ void			SetRadiusUnit(float rRadius)			{ _rRadius = rRadius; }
			 POINT2D		GetNextWayPoint();

	/** Smooth path (suppress waypoints when possible) */
	void					Smooth(QUADTREE *pQuadtree);

	/** Derivation from ENTITY_ABC */
	virtual void			Display(DISPLAY_PARAMETERS & DisplayParameters, bool bDoVisibility);
	virtual bool			TestVisibility(DISPLAY_PARAMETERS & DisplayParameters);

	static void							ComInit(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
	
protected:

	static	void			EM_Construct(void * const pO, void * const pR, void * const pP);
	static	void			EM_Destruct(void * const pO, void * const pR, void * const pP);
	static	void			EM_IsEmpty(void * const pO, void * const pR, void * const pP);
	static	void			EM_GetNextWayPoint(void * const pO, void * const pR, void * const pP);

	static COMMUNICATOR_DESCRIPTOR	_CD;

	/** Constructor & destructor */
							PATH();
						   ~PATH();

	/** Variables */
	QDT_DLIST <POINT2D>		_dlWayPoints;
	QDT_DLIST <POINT2D>		_dlDrawnWayPoints;
	QDT_DLIST <POINT2D>		_dlDebug;
	float					_rHeight;		// For display path only
	float					_rRadius;		// Idem.
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
