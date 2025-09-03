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
// CLASS: DUAL_EDGE
//
//
//00-12-22 CVI Created: 
//*****************************************************************************


//=============================================================================
// Name: DUAL_EDGE
//00-12-22 CVI Created: 
//=============================================================================
class DUAL_EDGE
{

public:

	_INLINE_	DUAL_EDGE();
	_INLINE_	~DUAL_EDGE();

	_INLINE_	void		Build(const DUAL_EDGE &);

	// Accesseurs
	_INLINE_	int			GetEdgeId1()const;
	_INLINE_	int			GetEdgeId2()const;
	_INLINE_	int			GetHandleSurf1()const;
	_INLINE_	int			GetHandleSurf2()const;
	_INLINE_	int			GetStatus()const;
	_INLINE_	void		SetEdgeId1( int n );
	_INLINE_	void		SetEdgeId2( int n );
	_INLINE_	void		SetHandleSurf1( int n );
	_INLINE_	void		SetHandleSurf2( int n );
	_INLINE_	void		SetStatus( int n );
	_INLINE_	void		InvertEdges();

protected:

	int			_nEdgeId1;					//	Index edge1
	int			_nEdgeId2;					//	Index edge2
	int			_nHandleSurf1;				//	Handle surface1
	int			_nHandleSurf2;				//	Handle surface2
	int			_nStatus;					//	Status : surface déjà tesseletée etc...

private:
	//	Enum pour le status du dual edge
	enum
	{
		NOT_COMPUTED = 0,
		ONE_COMPUTED = 1,
		SECOND_COMPUTED = 2,
		
	};

};

//=============================================================================
// CODE ENDS HERE
//=============================================================================
