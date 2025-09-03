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
//	CLASS:	BLOC_CAT
//	The BLOC_CAT class implements ...
//
//	02-04-15:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	BLOC_CAT : public COMMUNICATOR
{
public:

	enum
	{
		CMD_SET_GRAPH_NODE = CMD_COM_USER_START,
		CMD_SET_BASE_NODE,
		CMD_SET_MATRIX_WIDTH,
		CMD_BLOC_CAT_LAST,
	};

	/////////////////////////////////////////////////////////
							BLOC_CAT();
	explicit				BLOC_CAT(const BLOC_CAT & C);
							~BLOC_CAT();
	BLOC_CAT&				operator=(const BLOC_CAT & C);
	void					Copy(const BLOC_CAT & C);
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	_INLINE_	const AABB&			GetBox()const;
	_INLINE_	int					GetMinX()const;
	_INLINE_	int					GetMinY()const;
	_INLINE_	int					GetMinZ()const;
	_INLINE_	int					GetMaxX()const;
	_INLINE_	int					GetMaxY()const;
	_INLINE_	int					GetMaxZ()const;
	_INLINE_	int					GetCenterX()const;
	_INLINE_	int					GetCenterY()const;
	_INLINE_	int					GetCenterZ()const;
	_INLINE_	float				GetBlocSizeX()const;
	_INLINE_	float				GetBlocSizeY()const;
	_INLINE_	float				GetBlocSizeZ()const;
	_INLINE_	int					GetNbrBlocsAtFront()const;
	_INLINE_	int					GetNbrBlocsBack()const;
	_INLINE_	int					GetNbrBlocsOnSide()const;
	_INLINE_	int					GetNbrBlocsUp()const;
	_INLINE_	int					GetNbrBlocsDown()const;
	_INLINE_ const COM_INSTANCE &	GetBaseNodeInst()const;
	_INLINE_	int					GetNbrBlocs()const;
	_INLINE_	int					GetMatrixNbrBlocs()const;
	_INLINE_	int					GetMatrixNbrBlocsX()const;
	_INLINE_	int					GetMatrixNbrBlocsY()const;
	_INLINE_	int					GetMatrixNbrBlocsZ()const;
	_INLINE_	int					GetNbrBlocsX()const;
	_INLINE_	int					GetNbrBlocsY()const;
	_INLINE_	int					GetNbrBlocsZ()const;
	_INLINE_	int					GetBlocOffset( int nNumBloc );
	_INLINE_ const COM_INSTANCE &	GetSGNodeInst()const;
	_INLINE_	READ_TASK*			GetCurrentReadTask();
	_INLINE_	COM_INSTANCE		GetTabOffset();
	_INLINE_	COM_INSTANCE		GetPartition();
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	_INLINE_	void			SetBox( const AABB& Box );
	_INLINE_	void			SetMinX( int nVal );
	_INLINE_	void			SetMinY( int nVal );
	_INLINE_	void			SetMinZ( int nVal );
	_INLINE_	void			SetMaxX( int nVal );
	_INLINE_	void			SetMaxY( int nVal );
	_INLINE_	void			SetMaxZ( int nVal );
	_INLINE_	void			SetCenterX( int nVal );
	_INLINE_	void			SetCenterY( int nVal );
	_INLINE_	void			SetCenterZ( int nVal );
	_INLINE_	void			SetBlocSizeX( float rVal );
	_INLINE_	void			SetBlocSizeY( float rVal );
	_INLINE_	void			SetBlocSizeZ( float rVal );
	_INLINE_	void			SetNbrBlocsAtFront( int nVal );
	_INLINE_	void			SetNbrBlocsBack( int nVal );
	_INLINE_	void			SetNbrBlocsOnSide( int nVal );
	_INLINE_	void			SetNbrBlocsUp( int nVal );
	_INLINE_	void			SetNbrBlocsDown( int nVal );
	_INLINE_	void			SetBaseNodeInst( const COM_INSTANCE & Inst );
	_INLINE_	void			SetNbrBlocs( int nVal );
	_INLINE_	void			SetNbrBlocsX( int nVal );
	_INLINE_	void			SetNbrBlocsY( int nVal );
	_INLINE_	void			SetNbrBlocsZ( int nVal );
	_INLINE_	void			SetBlocOffset( int nNumBloc, int nOffset );
	_INLINE_	void			SetSGNodeInst( const COM_INSTANCE & Inst );
	_INLINE_	void			SetCurrentReadTask( READ_TASK *pRT );
	_INLINE_	void			SetTabOffset( COM_INSTANCE CI );
	_INLINE_	void			SetPartition( COM_INSTANCE CI );
	/////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////
	_INLINE_	int				GetIndexFromCoord( int nX, int nY, int nZ )const;
	_INLINE_	void			GetCoordFromIndex( int nIndex, int&nX, int& nY, int& nZ )const;
	_INLINE_	bool			IsInMatrix( int nX, int nY, int nZ );
	_INLINE_	int				GetGlobalIndexFromMatrixIndex( int nIndex );
	_INLINE_	bool			UnloadListIsEmpty()const;
	_INLINE_	int				GetMatrixIndexFromMatrixCoord( int nX, int nY, int nZ );
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	void						GetWorldCoord( int nX, int nY, int nZ, VECTOR& P )const;
	void						GetWorldCoordFromMatrix( int nX, int nY, int nZ, VECTOR& P )const;
	void						GetCoordInBlocSpace( const VECTOR& P, int& nX, int& nY, int& nZ )const;
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	void						Init(const COM_INSTANCE & BaseNodeInst, bool bForceLoad = false);
	void						Update();
	void						ResetBlocCat( bool bForceLoad = false );
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	void						AddDynObjects();
	void						RemoveDynObjects();
	/////////////////////////////////////////////////////////

	// Communication
	static void							ComInit(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
	static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();
	
protected:

	/////////////////////////////////////////////////////////
#ifndef _MASTER
	bool						WriteVersion2(class IO_STREAM &	Stream) const;
#endif
	bool						ReadVersion2(class IO_STREAM &	Stream) ;
	/////////////////////////////////////////////////////////
	bool						ReadAttributes(class IO_STREAM &	Stream);
	bool						ReadDynamicObjects(class IO_STREAM &	Stream);
#ifndef _MASTER
	bool						WriteAttributes(class IO_STREAM &	Stream)const;
	bool						WriteDynamicObjects(class IO_STREAM &	Stream)const;
#endif
	/////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////
	void						TranslateMatrix( int nDeltaX, int nDeltaY, int nDeltaZ );
	void						InitMatrix( bool bForceLoad = false );
	void						ResetMatrix();
	/////////////////////////////////////////////////////////	

	/////////////////////////////////////////////////////////	
	void						UpdateMinMax( int nDeltaX, int nDeltaY, int nDeltaZ );
	void						UpdateMinMaxFromCenter();
	void						UpdateCenter( int& nDeltaX, int& nDeltaY, int& nDeltaZ );
	void						CheckBlocsToBeUnloaded();
	void						MoveMatrixBuffer( int nOffset );
	void						UpdateMatrixBuffer( int nX, int nY, int nZ );
	void						CheckBlocsToBeLoaded();
	/////////////////////////////////////////////////////////	
	
	/////////////////////////////////////////////////////////	
	void						PostReadTask();
	void						UnloadAllBlocs( bool bBlocking );
	BLOC*						LoadBloc( int nBlocIndex );
	BLOC*						ReadBloc( int nBlocIndex );
	void						UnloadBloc( BLOC *pBloc );
	void						AddBlocToUnload( BLOC *pB );
	void						ProcessUnloadList();
	void						ProcessBlockingload();
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	bool						BlocIsInMatrix( BLOC *pB );
	int							FindBlocInMatrix( BLOC *pB );
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	AABB					_Box;
	int						_nMinX;
	int						_nMinY;
	int						_nMinZ;
	int						_nMaxX;
	int						_nMaxY;
	int						_nMaxZ;
	int						_nCenterX;
	int						_nCenterY;
	int						_nCenterZ;
	int						_nNbrBlocs;
	float					_rBlocSizeX;
	float					_rBlocSizeY;
	float					_rBlocSizeZ;
	int						_nNbrBlocsX;
	int						_nNbrBlocsY;
	int						_nNbrBlocsZ;
	int						_nNbrBlocsFront;
	int						_nNbrBlocsBack;
	int						_nNbrBlocsOnSide;
	int						_nNbrBlocsUp;				//	Z
	int						_nNbrBlocsDown;				//	-Z
	QDT_VECTOR<BLOC*>		_vBlocMatrix;
	QDT_VECTOR<int>			_vBlocOffsets;
	QDT_DLIST<BLOC*>		_sBlocToUnload;
	COM_INSTANCE			_BaseNodeInst;
	COM_INSTANCE			_SGNodeInst;
	READ_TASK				*_pCurrentReadTask;
	/////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////
	QDT_VECTOR<COM_INSTANCE> _DynObjListInst;
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	COM_INSTANCE			_TabOffset;
	COM_INSTANCE			_Partition;
	/////////////////////////////////////////////////////////

private:
	
	// Communication
	static void		EM_Construct(void * const pO, void * const pR, void * const pP);
	static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

	// Commands
	static void		EM_SetGraphNode(void * const pO, void * const pR, void * const pP);
	static void		EM_SetBaseNode(void * const pO, void * const pR, void * const pP);
	static void		EM_SetMatrixWidth(void * const pO, void * const pR, void * const pP);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static void		CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
#ifndef _MASTER	
	static void		CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
