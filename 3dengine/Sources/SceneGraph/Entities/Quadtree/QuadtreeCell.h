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
//	CLASS:	QUADTREE_CELL
//	The QUADTREE_CELL class implements the cell of a quadtree.
//  Each cell has a parent & fourth child : NW, NE, SW & SE. 
//
//  take care : the orientation of the current space is  with a direct orientation
//
//						 -----> X
//						|
//						|						
//						|
//						v Z
//
//	ONA Beware :
//				The side orientation depends of the side orientation of AABR2D
//				BUT CONVERT THE COORDINATE y in z
//				because AABR2D call x the first element and y the second element
//				In the QUADTREE it corresponds to x and z (y is not used !)
//
//	01-07-23:	RMA - Created
//  01-08-24:	JLT - Modified
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
	_INLINE_ void					SetState(STATE	eState);
	_INLINE_ void					SetParentCell(QUADTREE_CELL *pCell);
	QUADTREE_CELL *					GetNorthWestCell() const { return _pChildCell[QCC_NORTH_WEST_CELL]; }
	QUADTREE_CELL *					GetNorthEastCell() const { return _pChildCell[QCC_NORTH_EAST_CELL]; }
	QUADTREE_CELL *					GetSouthWestCell() const { return _pChildCell[QCC_SOUTH_WEST_CELL]; }
	QUADTREE_CELL *					GetSouthEastCell() const { return _pChildCell[QCC_SOUTH_EAST_CELL]; }
	STATE							GetState() const { return _eState; } 
	QUADTREE_CELL *					GetParentCell() const { return _pParentCell; }

	/** Closed list optimization */
	void							ResetClosedList();
	_INLINE_ void					AddToClosedList();
	_INLINE_ bool					BelongToClosedList();
	
	bool							Subdivise();
	bool							HasChild() const { return _bHasChild; }
	void							ClearChildCell();
	_INLINE_ QUADTREE_CELL*			GetChildCell(CARDINAL eIndex) const;
	_INLINE_ QUADTREE_CELL*			operator[] (unsigned int nIndex) const;
	_INLINE_ const TRIANGLE2D&		GetTriangle(unsigned int index) const;
	_INLINE_ float					GetArea() const;
	void							Invert();
	float							GetIntersectedArea(const AABR2D& bbox) const;
	bool							IntersectWithCircle(const CIRCLE & Circle);
	
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

	_INLINE_ void					ComputeQuad();
	_INLINE_ void					SetNorthWestCell(const QUADTREE_CELL * const pCell);
	_INLINE_ void					SetNorthEastCell(const QUADTREE_CELL * const pCell);
	_INLINE_ void					SetSouthWestCell(const QUADTREE_CELL * const pCell);
	_INLINE_ void					SetSouthEastCell(const QUADTREE_CELL * const pCell);

	/**	Get the cardinal with side and parity */
	_INLINE_ static CARDINAL		GetCardinal(const SIDE eSide, const bool bFirst);
	/** Get the opposite side */
	_INLINE_ static SIDE			GetOppositeSide(const SIDE eSide);

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

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
