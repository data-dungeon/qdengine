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
// CLASS: TWIN_PORTAL
//
//
//	01-03-29:	ELE - Created
//*****************************************************************************


//=============================================================================
// Name: TWIN_PORTAL
//	01-03-29:	ELE - Created
//=============================================================================
class TWIN_PORTAL : public PORTAL_ABC
{
public:

	enum
	{
		SET_TWIN_PORTAL	= CMD_PORTAL_LAST,
		UNSET_TWIN_PORTAL,
		NO_VISIBILITY,
		CMD_TWIN_PORTAL_LAST,
	};

							TWIN_PORTAL();
							~TWIN_PORTAL();

    /** Derivation from ENTITY_ABC : RayCast */
	bool					RayCast(RAY_CAST_PARAMETERS & RayCastParameters, bool & bContinue);
	
	bool					AutoChooseZoneRaycast(RAY_CAST_PARAMETERS & RayCastParameters, bool & bContinue);

	/** Get the twin portal node */
	_INLINE_ virtual COM_INSTANCE 	GetTwinPortalNodeInst() const;
	/** Set the twin portal node */
	_INLINE_ void					SetTwinPortalNodeInst(const COM_INSTANCE &PortalNodeInst);

	/** Update the parameters needed to render the scene seen threw the portal */
	virtual bool			UpdatePortalZone(const MATRIX & ViewMatrix, const MATRIX & NodeGlobalMatrix, SORTED_PORTAL_ZONE *pPortalZone);

	// Communication
	static void						ComInit(CLASS_DESCRIPTOR * const pCD);
	static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
	static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

private:

	/** Twin portal node */
	COM_INSTANCE	_TwinPortalNodeInst;

	bool			_bRayCasted;

	// Communication
	static void		EM_Construct(void * const pO, void * const pR, void * const pP);
	static void		EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
	static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

	// Commands
	static void		EM_SetTwinPortal(void * const pO, void * const pR, void * const pP);
	static void		EM_UnsetTwinPortal(void * const pO, void * const pR, void * const pP);
	static void		EM_NoVisibility(void * const pO, void * const pR, void * const pP);

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
