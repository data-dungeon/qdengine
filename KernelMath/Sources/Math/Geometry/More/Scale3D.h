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
// CLASS: SCALE_3D
//
//
//00-11-10 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: SCALE_3D
//00-11-10 JVA Created: 
//=============================================================================

/*
class	SCALE3D
{
public:

	
	_INLINE_				SCALE3D();
	_INLINE_				SCALE3D(const bool	bFlagInit);
	_INLINE_				SCALE3D(const SCALE3D& Sca) ;
	_INLINE_				SCALE3D(const float rX, const float rY, const float rZ);

	_INLINE_ const SCALE3D& operator = (const SCALE3D& Sca) ;
	_INLINE_ const SCALE3D& SlowAssign(const SCALE3D& Sca) ;
	_INLINE_ bool			operator ==(const SCALE3D& Sca) const;
	_INLINE_ bool			operator !=(const SCALE3D& Sca) const;
	_INLINE_ float operator [] (const unsigned int i) const;
	_INLINE_ float& operator [] (const unsigned int i);

	_INLINE_ void			Set(const SCALE3D& Sca) ;
	_INLINE_ void			SlowSet(const SCALE3D& Sca) ;
	_INLINE_ void			SetXYZ(const float rX, const float rY, const float rZ);
	_INLINE_ void			GetXYZ(float &rXX, float &rYY, float &rZZ) const;
	_INLINE_ float			GetX() const;
	_INLINE_ float			GetY() const;
	_INLINE_ float			GetZ() const;

	_INLINE_ void			SetX( const float& rVal ); 
	_INLINE_ void			SetY( const float& rVal ); 
	_INLINE_ void			SetZ( const float& rVal ); 

			 void			Interpolate(float rT, SCALE3D&, SCALE3D&);

	void	Read(IO_STREAM &s);
	void	Write(IO_STREAM &s)const;

	friend IO_STREAM& operator <<(IO_STREAM &s, const SCALE3D &m)
	{
		m.Write(s);
		return s;
	}
	friend IO_STREAM& operator >>(IO_STREAM &s, SCALE3D &m)
	{
		m.Read(s);
		return s;
	}
 
	int GetDataSize() const { return 3*sizeof(float); }

	const float* ToPointer() const;
	float* ToPointer();

private:

	PLATFORM_VECTOR		_V;
};*/

//=============================================================================
// CODE ENDS HERE
//=============================================================================
