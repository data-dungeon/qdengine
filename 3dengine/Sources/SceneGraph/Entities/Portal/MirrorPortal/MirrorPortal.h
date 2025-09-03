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
// CLASS: MIRROR_PORTAL
//
//
//	01-03-29:	ELE - Created
//*****************************************************************************


//=============================================================================
// Name: MIRROR_PORTAL
//	01-03-29:	ELE - Created
//=============================================================================
class MIRROR_PORTAL : public PORTAL_ABC
{
public:

	enum
	{
		CMD_MIRROR_PORTAL_LAST = CMD_PORTAL_LAST,
	};

							MIRROR_PORTAL();
							~MIRROR_PORTAL();

	/** Derivation from ENTITY_ABC : RayCast */
	bool					RayCast(RAY_CAST_PARAMETERS & RayCastParameters, bool & bContinue);

	/** Update the parameters needed to render the scene seen threw the portal */
	virtual bool			UpdatePortalZone(const MATRIX & ViewMatrix, const MATRIX & NodeGlobalMatrix, SORTED_PORTAL_ZONE *pPortalZone);
    
	/** Derivation from ENTITY_ABC : Display */
	virtual void			Display(DISPLAY_PARAMETERS & DisplayParameters);
	virtual void			DisplayTrans(DISPLAY_PARAMETERS & DisplayParameters, unsigned int nGeometryIndex, unsigned int nSkinClusterIndex);

	/** Display the portal in z-buffer */
	virtual void			DisplayZ(const MATRIX & WorldMatrix, const MATRIX &	ViewMatrix, int nMaskValue);

	/** Display the portal in stencil */
	void					DisplayMask(const MATRIX & WorldMatrix, const MATRIX & ViewMatrix, int nLevel, int nMaskValue);

	/** Display the sorted mirror */
	_INLINE_ void			DisplayMirror(DISPLAY_PARAMETERS & DisplayParameters);

	// Communication
	static void						ComInit(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
	static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

private:

	// Communication
	static void		EM_Construct(void * const pO, void * const pR, void * const pP);
	static void		EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
	static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static void		CM_ReadChunk7(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	static void		CM_ReadChunk8(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	static void		CM_ReadChunk9(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	static void		CM_ReadChunk10(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	static void		CM_ReadChunk11(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
	static void		CM_ReadChunk12(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
#ifndef _MASTER	
	static void		CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif

	static void*					_pVTable;
};

//=============================================================================
// CODE ENDS HERE
//=============================================================================
