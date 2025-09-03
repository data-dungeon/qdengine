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
// CLASS: EDGE
//
//
//00-12-22 CVI Created: 
//*****************************************************************************


//=============================================================================
// Name: EDGE
//00-12-22 CVI Created: 
//=============================================================================

class EDGE
{

	enum
	{
		USTART_WAY,
		USTART_INVWAY,
		UEND_WAY,
		UEND_INVWAY,
		VSTART_WAY,
		VSTART_INVWAY,
		VEND_WAY,
		VEND_INVWAY,
	};


public:

	_INLINE_	EDGE();
	_INLINE_	~EDGE();

	_INLINE_	void		Build(const EDGE &);

	//	Accesseurs
	_INLINE_	float		GetIndexStart()const;
	_INLINE_	float		GetIndexEnd()const;
	_INLINE_	int			GetDualEdgeId()const;
	_INLINE_	int			GetType()const;
	_INLINE_	int			GetTypeAs()const;
	_INLINE_	int			GetNbVertex()const;
	_INLINE_	void		SetIndexStart( float n );
	_INLINE_	void		SetIndexEnd( float n );
	_INLINE_	void		SetDualEdgeId( int n );
	_INLINE_	void		SetType( int n );
	_INLINE_	void		SetTypeAs( int n );
	_INLINE_	void		SetNbVertex( int n );

private:
	
	float		_rIndexStart;				//	Debut de l'edge
	float		_rIndexEnd;					//	Fin de l'edge
	int			_nDualEdgeId;				//	Index du DualEdge pour edge complémentaire
	int			_nType;						//	Type de la courbe (u0, u1, v0, v1)
	int			_nAs;						//	Première ou deuxième surface
	int			_nNbrVertex;				//	Nombre de vertex tessélées

};
//=============================================================================
// CODE ENDS HERE
//=============================================================================
