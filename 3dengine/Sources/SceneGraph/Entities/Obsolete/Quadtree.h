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
//	CLASS:	QUADTREE
//	The QUADTREE class implements ...
//
//	01-07-23:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class QUADTREE_CELL : public AABR2D
{
public:

	//=============================================================================
	// JLT addons : disposition of the Children cells
	//
	//		
	//=============================================================================
	enum	CARDINAL
	{
		QCC_NORTH_WEST_CELL,
		QCC_NORTH_EAST_CELL,
		QCC_SOUTH_EAST_CELL,
		QCC_SOUTH_WEST_CELL,
		QCC_MAX
	};

	enum	STATE
	{
		QCS_EMPTY,
		QCS_FILLED,
		QCS_SUBDIVISED,
	};

	/** Constructor & destructor */
									QUADTREE_CELL();
									QUADTREE_CELL(const POINT2D &PMin, const POINT2D &PMax);
									QUADTREE_CELL(const AABR2D &AABR2D);
									QUADTREE_CELL(const QUADTREE_CELL &qc);
								   ~QUADTREE_CELL();

	/** Operators */
	const QUADTREE_CELL &			operator=(const QUADTREE_CELL &qc);

	/** Display a representation of the quadtree */
	void							Display(float rHeight, QDT_VECTOR <SEGMENT> & vSegments);

	/** Accessors & modifiors */
	void							SetState(STATE	eState);
	void							SetParentCell(QUADTREE_CELL *pCell);
	QUADTREE_CELL *					GetNorthWestCell() const { return _pChildCell[QCC_NORTH_WEST_CELL]; }
	QUADTREE_CELL *					GetNorthEastCell() const { return _pChildCell[QCC_NORTH_EAST_CELL]; }
	QUADTREE_CELL *					GetSouthWestCell() const { return _pChildCell[QCC_SOUTH_WEST_CELL]; }
	QUADTREE_CELL *					GetSouthEastCell() const { return _pChildCell[QCC_SOUTH_EAST_CELL]; }
	STATE							GetState() const { return _eState; } 
	QUADTREE_CELL *					GetParentCell() const { return _pParentCell; }

	/** Closed list optimization */
	void							ResetClosedList();
	void							AddToClosedList();
	bool							BelongToClosedList();
	
	bool							Subdivise();
	bool							HasChild() const { return _bHasChild; }
	void							ClearChildCell();
	QUADTREE_CELL*					GetChildCell(CARDINAL eIndex) const;
	QUADTREE_CELL*					operator[] (unsigned int nIndex) const;
	const TRIANGLE2D&				GetTriangle(unsigned int index) const;
	float							GetArea() const;
	void							Invert();
	float							GetIntersectedArea(const AABR2D& bbox) const;
	
	/** Derivation from AABR2D (Stream) */
	virtual void					Read(IO_STREAM & Stream);
	virtual void					Write(IO_STREAM & Stream) const;

			void					WriteBinary( IO_STREAM & Stream );
			void					InitBinary();

	friend IO_STREAM&				operator << (IO_STREAM & Stream, const QUADTREE_CELL & Cell)
									{
										Cell.Write(Stream);
										return Stream;
									}
	friend IO_STREAM&				operator >> (IO_STREAM & Stream, QUADTREE_CELL & Cell)
									{
										Cell.Read(Stream);
										return Stream;
									}

	/** Update the list of neightbour */
	void							UpdateNeightbour();
	/** Get the number of neightbours in a side */
	int								GetNbrNeightbours(const SIDE eSide) const;
	/** Get the indexed neightbour in the side */
	QUADTREE_CELL *					GetNeightbour(const SIDE eSide, const int nIndex) const;
	/** Find a QUADTREE_CELL under the point P */
	QUADTREE_CELL *					FindByPos(const POINT2D & P) const;
	/** Get a vector of pointer of QUADTREE_CELL from point PBegin to PEnd */
	QDT_VECTOR<QUADTREE_CELL *> *	GetQuadtreeCellsList(const POINT2D & PBegin, const POINT2D & PEnd) const;

private :

	void							Copy(const QUADTREE_CELL &qc);

	void							ComputeQuad();
	void							SetNorthWestCell(const QUADTREE_CELL * const pCell);
	void							SetNorthEastCell(const QUADTREE_CELL * const pCell);
	void							SetSouthWestCell(const QUADTREE_CELL * const pCell);
	void							SetSouthEastCell(const QUADTREE_CELL * const pCell);

	/**	Get the cardinal with side and parity */
	static CARDINAL					GetCardinal(const SIDE eSide, const bool bFirst);
	/** Get the opposite side */
	static SIDE						GetOppositeSide(const SIDE eSide);
		
	/** Fill the sided neightbours of a QUADTREE_CELL */
	void							FillNeightbour(const SIDE eSide);
	/** Get the sided parent, return the level and a parity vector */
	QUADTREE_CELL *					GetPartParent(const SIDE eSide, int & nLevel, QDT_VECTOR<char> & vbParity);
	/** Get a QUADTREE_CELL by side, a max level and a max parity vector */
	QUADTREE_CELL *					GetRecurse(const SIDE eSide, const int nLevel, QDT_VECTOR<char> & vbParity);
	/** Update the neightbours list by side */
	void							FillNeightbour(QUADTREE_CELL * pQC, const SIDE eSide);
	/** Clear the the neightbours list by side */
	void							ClearNeightbours(const SIDE eSide);


	/** Variables */
	STATE							_eState;
	QUADTREE_CELL *					_pParentCell;
	QUADTREE_CELL *					_pChildCell[QCC_MAX];
	bool							_bHasChild;
	QDT_VECTOR<QUADTREE_CELL *>		_avpNeightbours[SIDE_MAX];
	TRIANGLE2D						_Quad[2];
	bool							_bInClosedList; 
};

class QUADTREE : public ENTITY_ABC
{	
public:

	enum
	{
		CMD_QUADTREE_LAST = CMD_ENTITY_LAST,
	};

								QUADTREE();
	virtual						~QUADTREE();

	/** Accessors & modifiors */
	int							GetNbrCells() const;
	int							GetDepth() const;
	void						SetRootCell(const QUADTREE_CELL * const pRootCell);
	QUADTREE_CELL *				GetRootCell() const;
	void						SetNbrCells(int nNbrCells);
	void						SetDepth(int nDepth);
	void						SetHeight(float rHeight);

	/** Find a cell of the tree fro a given position */
	QUADTREE_CELL *				FindCellByPos(const POINT2D & Pos);

	/** Compute the neighboors of all the cells */
	void						ComputeNeighbours();

	bool						Contain(COM_HANDLE *pUnit) const;
	void						RegisterUnit(COM_HANDLE *pUnit);
	void						UnregisterUnit(COM_HANDLE *pUnit);

	void						Clear();
	void						Invert();
	
	//	DATA_BANK
	void						WriteBinary(VERSION Version, IO_STREAM& Stream);
	void						InitBinary(VERSION Version);
	
	/** Derivation from ENTITY_ABC */
	virtual void				Display(DISPLAY_PARAMETERS & DisplayParameters, bool bDoVisibility);
	virtual bool				TestVisibility(DISPLAY_PARAMETERS & DisplayParameters);

	// Communication
	static void							ComInit(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();

protected :

	/** Variables */
	QUADTREE_CELL			   *_pRootCell;
	int							_nNbrCells;
	int							_nDepth;
	float						_rHeight;
	
	QDT_DLIST<COM_INSTANCE>		_dlUnits;

	// Communication
	static void		EM_Construct(void * const pO, void * const pR, void * const pP);
	static void		EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
	static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

	// Chunk
	static void		CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	static void		CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

	static COMMUNICATOR_DESCRIPTOR	_CD;
	static void*					_pVTable;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
