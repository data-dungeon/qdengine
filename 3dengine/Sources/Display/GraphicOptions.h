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
//	CLASS:	GRAPHIC_OPTIONS
//	The GRAPHIC_OPTIONS class implements ...
//
//	02-03-27:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	GRAPHIC_OPTIONS : public COM_OBJECT
{
public:

	enum GR_OP_FLAGS
	{
		GR_OP_DYNAMIC_SHADOWS_ENABLED		= 1 << 0,
		GR_OP_SELF_SHADOWING_ENABLED		= 1 << 1,
		GR_OP_DYNAMIC_LIGHTING_ENABLED		= 1 << 2,
		GR_OP_LOD_ENABLED					= 1 << 3,
		GR_OP_SHADOWS_LOD_ENABLED			= 1 << 4,
	};

	enum GR_OP_LOD
	{
		GR_OP_HIGH_DETAIL = 0,
		GR_OP_MEDIUM_DETAIL,
		GR_OP_LOW_DETAIL,
	};

	/** For singleton */
	static	GRAPHIC_OPTIONS *	Instance();
	static	void				Suicide();

	/** Modifiors */
	_INLINE_ void			EnableDynamicShadows(bool bValue)		{ _Flags.Set(GR_OP_DYNAMIC_SHADOWS_ENABLED, bValue); }
	_INLINE_ void			EnableSelfShadowing(bool bValue)		{ _Flags.Set(GR_OP_SELF_SHADOWING_ENABLED, bValue); }
	_INLINE_ void			EnableDynamicLighting(bool bValue)		{ _Flags.Set(GR_OP_DYNAMIC_LIGHTING_ENABLED, bValue); }
	_INLINE_ void			EnableLOD(bool bValue)					{ _Flags.Set(GR_OP_LOD_ENABLED, bValue); }
	_INLINE_ void			EnableShadowsLOD(bool bValue)			{ _Flags.Set(GR_OP_SHADOWS_LOD_ENABLED, bValue); }
	_INLINE_ void			SetTexturesDetail(GR_OP_LOD eLOD)		{ _TexturesDetail = eLOD; }
	_INLINE_ void			SetPolygonesDetail(GR_OP_LOD eLOD)		{ _PolygonesDetail = eLOD; }

	/** Accessors */
	_INLINE_ bool			IsDynamicShadowsEnabled() const			{ return (_Flags.IsSet(GR_OP_DYNAMIC_SHADOWS_ENABLED)); }
	_INLINE_ bool			IsSelfShadowingEnabled() const			{ return (_Flags.IsSet(GR_OP_SELF_SHADOWING_ENABLED)); }
	_INLINE_ bool			IsDynamicLightingEnabled() const		{ return (_Flags.IsSet(GR_OP_DYNAMIC_LIGHTING_ENABLED)); }
	_INLINE_ bool			IsLODEnabled() const					{ return (_Flags.IsSet(GR_OP_LOD_ENABLED)); }
	_INLINE_ bool			IsShadowsLODEnabled() const				{ return (_Flags.IsSet(GR_OP_SHADOWS_LOD_ENABLED)); }
	_INLINE_ GR_OP_LOD		GetTexturesDetail() const				{ return (_TexturesDetail); }
	_INLINE_ GR_OP_LOD		GetPolygonesDetail() const				{ return (_PolygonesDetail); }


	/** Derivation from COM_OBJECT */
	DESC*					GetDesc() const;
	int						GetNbChunk() const;
	const OLD_CHUNK &		GetChunk(int nIndex) const;
	bool					Read(const class OLD_CHUNK & Chunk, class IO_STREAM & Stream);
	bool					Write(const class OLD_CHUNK & Chunk, class IO_STREAM & Stream) const;
	void*					GetObject();
	bool					ReceiveCommandProc(COMMAND_ID CommandID, PARAMETER_STACK & Stack);


private:

	friend class			GRAPHIC_OPTIONS_DESC;

	/** Contructor & destructor */
							GRAPHIC_OPTIONS();
						   ~GRAPHIC_OPTIONS();

	/** Chunks */
	enum
	{
		NBR_CHUNKS = 1,
	};
	static int				_nNbrChunks;
	static OLD_CHUNK	_ChunksList[NBR_CHUNKS]; 

	/** For singleton */
	static GRAPHIC_OPTIONS *_pInstance;

	/** Flags */
	QDT_FLAGS				_Flags;

	/** Polygones detail */
	GR_OP_LOD				_PolygonesDetail;

	/** Textures detail */
	GR_OP_LOD				_TexturesDetail;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
