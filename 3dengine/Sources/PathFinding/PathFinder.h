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
//	CLASS:	PATH_FINDER
//	The PATH_FINDER class implements ...
//
//	01-09-19:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	PATH_NODE;
class	PATH_NODE_PTR;
class	OPEN_LIST;
class	CLOSED_LIST;

class	PATH_FINDER
{
public:

	enum
	{
		CMD_PUSH_QUADTREE = CMD_USER_START,
		CMD_FIND_PATH,
		CMD_CHECK_UNIT_POS,
		CMD_PATH_FINDER_LAST,
	};

								PATH_FINDER();
							   ~PATH_FINDER();

	void						PushQuadtree(const COM_INSTANCE & QuadtreeInst);
	void						ClearQuadtrees();
	bool						FindPath(const POINT2D & Source, const POINT2D & Destination, float rRadius, PATH & Path);	
	bool						CheckUnitPos(const POINT2D & Pos, float rRadiusUnit) const;
//	void						AddUnit(INDEX_INT *pPUIndex);
//	bool						RemoveUnit(INDEX_INT *pPUIndex);

	/** For singleton */
	static PATH_FINDER*			Instance();
	static void					Suicide();

	static SINGLETON_DESCRIPTOR *	GetSingletonDescriptor();

private:
								
	QDT_VECTOR<COM_INSTANCE>		 _vQuadtreeInst;
	QUADTREE_CELL					*_pDestQuadtree;
	
//	QDT_DLIST<INDEX_INT *>			_vUnits;

	// Communication
	static void						COM_Init(CLASS_DESCRIPTOR * const pCD);
											
	static void						EM_Construct(void * const pO, void * const pR, void * const pP);
	static void						EM_Destruct(void * const pO, void * const pR, void * const pP);
											
	// Commands								
	static void						EM_PushQuadtree(void * const pO, void * const pR, void * const pP);
	static void						EM_FindPath(void * const pO, void * const pR, void * const pP);
	static void						EM_CheckUnitPos(void * const pO, void * const pR, void * const pP);
											
	static SINGLETON_DESCRIPTOR		_SD;
	static PATH_FINDER			   *_pInstance;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================