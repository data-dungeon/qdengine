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
// CLASS: PARTICLE_SYSTEM_MANAGER
//
//
//00-11-13 JVA Created: 
//*****************************************************************************

class	NODE;
class	PARTICLE_SHAPE;
class	PARTICLE_SHAPE_SPRITE;

//=============================================================================
// Name: PARTICLE_SYSTEM_MANAGER
//00-11-13 JVA Created: 
//=============================================================================
class PARTICLE_SYSTEM_MANAGER : public COM_OBJECT
{
public:
    explicit PARTICLE_SYSTEM_MANAGER();
            ~PARTICLE_SYSTEM_MANAGER();
    //    REMEMBER To make the destructor virtual if 
    //    you create a subclass, or if you have one

	void					Init();
	void					Loop();
	void					Close();

	INDEX_INT				*GetParticleSystemNode(int nIndex)const;

	INDEX_INT				*CreateParticleShape	(char *pcName = NULL) ;
	INDEX_INT				*CreateParticleShapeSprite	(char *pcName = NULL) ;

	INDEX_INT				*GetParticleShape(int nIndex)const;
	void					AddParticleShape(INDEX_INT *pParticleShape);


	/** Derivation from COM_OBJECT : Get descriptor */
	DESC*			GetDesc() const;
	/** Derivation from COM_OBJECT : Get the number of chunks */
	int				GetNbChunk() const;
	/** Derivation from COM_OBJECT : Get a chunk */
	const CHUNK &	GetChunk(int nIndex) const;
	/** Derivation from COM_OBJECT : Read a chunk from a stream */
	bool			Read(const class CHUNK & Chunk, class IO_STREAM & Stream);
	/** Derivation from COM_OBJECT : Write a chunk in a stream */
	bool			Write(const class CHUNK & Chunk, class IO_STREAM & Stream) const;
	void			*GetObject();
	
	bool			ReceiveCommandProc(COMMAND_ID, PARAMETER_STACK &);

	static	PARTICLE_SYSTEM_MANAGER	*Instance();
	static	void Suicide();
private:
	enum
	{
		NB_CHUNK = 1,
	};
	static class CHUNK _ChunkList[NB_CHUNK];
	static int _nNbChunk;
	static PARTICLE_SYSTEM_MANAGER	*_pInstance;
	QDT_VECTOR<INDEX_INT*>				_vParticleSystemNodes; 

	QDT_VECTOR<INDEX_INT*>		_vParticleShapes; 

};
//=============================================================================
// CODE ENDS HERE
//=============================================================================
