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
//	CLASS:	BLOC
//	The BLOC class implements ...
//
//	02-04-15:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	BLOC
{
public:
	///////////////////////////////////////////////////
	enum	BLOC_STATUS
	{
		BLOC_NOTHING = 0,
		BLOC_IS_LOADING,
		BLOC_LOADED,
	};
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	BLOC();
	explicit			BLOC(const BLOC & C);
						~BLOC();
	BLOC&				operator=(const BLOC & C);
	void				Copy(const BLOC & C);
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	_INLINE_ int					GetIndex()const;
	_INLINE_ const AABB&			GetBox()const;
	_INLINE_ const COM_INSTANCE &	GetNodeInst( int nNumNode ) const;
	_INLINE_ void					AddNodeInst( const COM_INSTANCE & Inst );
	_INLINE_ void					SetIndex( int nIndex );
	_INLINE_ void					SetBox( const AABB& Box );
	_INLINE_ int					GetNbrNodes()const;
	_INLINE_ bool					IsLoading()const;
	_INLINE_ bool					IsLoaded()const;
	_INLINE_ void					SetLoading( bool b = true );
	_INLINE_ void					SetLoaded( bool b = true );
	_INLINE_ void					SetSGNodeInst( const COM_INSTANCE & Inst );
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	void						LoadEntities( READ_TASK *pFatherReadTask );
	bool						Unload();
	void						UnloadBlocking();
	bool						CheckAllNodesLoaded();
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	void						AddNodeToSceneGraph( const COM_INSTANCE & Inst );
	int							GetSizeToWrite();
#ifndef _MASTER
	void						WriteInBuffer( MEM_STREAM *pStream );
#endif
#ifndef _MASTER_ONLY_DATABANK
	void						ReadInBuffer( MEM_STREAM *pStream );
#endif
	///////////////////////////////////////////////////

protected:
	///////////////////////////////////////////////////
	bool						MustLoadNode( const COM_INSTANCE & Inst );
	void						AddNodeToLoad( READ_TASK *pRT, const COM_INSTANCE & Inst );
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	int							_nIndex;
	AABB						_Box;
	QDT_VECTOR<COM_INSTANCE>	_vNodeInst;
	QDT_FLAGS					_Flags;
	COM_INSTANCE				_SGNodeInst;
	///////////////////////////////////////////////////
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
