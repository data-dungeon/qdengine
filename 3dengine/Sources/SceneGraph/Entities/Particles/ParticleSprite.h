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
//	CLASS:	PARTICLE_SPRITE
//	The PARTICLE_SPRITE class implements ...
//
//	01-11-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	PARTICLE_SPRITE : public PARTICLE
{
public:
							PARTICLE_SPRITE();
							~PARTICLE_SPRITE();
							
	virtual void			Init();
	virtual void			Update(float rDeltaTime);
	virtual void			Display();

	_INLINE_ virtual bool	IsParticleSprite() const;

private:

	COLOR					_DiffuseColor;
	int						_nDiffuseColorTrack;
	POINT2D					_pUVs[4];
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
