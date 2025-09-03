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
//	CLASS:	BLEND_SHAPE
//	The BLEND_SHAPE class implements ...
//
//	01-06-12:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	BLEND_SHAPE : public MESH
{
public:

	enum
	{
		CMD_BLEND_SHAPE_LAST = CMD_MESH_LAST,
	};

							BLEND_SHAPE();
							~BLEND_SHAPE();

	virtual void			ApplyBlendShape(const DISPLAY_PARAMETERS & DisplayParameters);
	virtual void			ApplyBlendShape(const DISPLAY_PARAMETERS & DisplayParameters, int nGeometryIndex);
//	virtual void			ApplyBlendShape(const DISPLAY_PARAMETERS & DisplayParameters, GEOMETRY_REF *pGeometryRef);

	/** Derivation from ENTITY_ABC */
	virtual void			Update();
	virtual bool			ToUpdate() const;

	/** Accessors */	
//	_INLINE_ void				SetBlendShapeDatas(BLEND_SHAPE_DATAS * pBlendShapeDatas);
//	_INLINE_ BLEND_SHAPE_DATAS*	GetBlendShapeDatas() const;
	_INLINE_ int				GetNbrWeights() const;
	_INLINE_ float *			GetWeights() const;
			 void				SetWeights(float *prWeights);
	_INLINE_ void				SetValid(bool bValue);
	_INLINE_ void				SetNumId( int nNum );
	_INLINE_ int				GetNumId()const;

	/** Interpolation */
	void					Interpolate();

	// Communication
	static void							ComInit(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
	static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

private:

	/** Specific vertex format interpolations */
	void					Interpolate(int nGeometryIndex);
	void					NewInterpolate();

	// To check if interpolation is valid
	bool					_bIValid;

	// Data for interpolation
//	BLEND_SHAPE_DATAS	   *_pBlendShapeDatas;
	float				   *_prWeights;
	int						_nNumId;

	// Communication
	static void		EM_Construct(void * const pO, void * const pR, void * const pP);
	static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

	// Chunks
#ifndef _MASTER_ONLY_DATABANK
	static void		CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	static void		CM_ReadChunk3(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	static void		CM_ReadChunk4(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	static void		CM_ReadChunk5(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
#ifndef _MASTER	
	static void		CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
};


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
