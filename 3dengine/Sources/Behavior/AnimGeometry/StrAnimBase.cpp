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
//	CLASS:	STR_ANIM_BASE
//
//	03-06-11:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"StrAnimBase.h"

#ifdef _DEBUG
	#include "StrAnimBase.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STR_ANIM_BASE constructor
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
STR_ANIM_BASE::STR_ANIM_BASE()
:
_bAbsolute(false),
_bTeleport(false)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		STR_ANIM_BASE constructor
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
STR_ANIM_BASE::STR_ANIM_BASE(const STR_ANIM_BASE & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		STR_ANIM_BASE destructor
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
STR_ANIM_BASE::~STR_ANIM_BASE()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
STR_ANIM_BASE &	STR_ANIM_BASE::operator=(const STR_ANIM_BASE & C)
{
	if	(this != &C)
	{
		Copy(C);
	}
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	STR_ANIM_BASE::Copy(const STR_ANIM_BASE & C)
{
	_AbsRel		= C._AbsRel;
	_Teleport	= C._Teleport;
	_bAbsolute	= C._bAbsolute;
	_bTeleport	= C._bTeleport;
}

//-----------------------------------------------------------------------------
//	Name:		Blend
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	STR_ANIM_BASE::Blend(const STR_ANIM_BASE &	SAB,
							 float					rCoeff)
{
	QUATERNION_VECTOR	QV;
	QUATERNION_VECTOR	QTemp1, QTemp2, tRel, t;
	QUATERNION_VECTOR	t1, t2;

	/////////////////////////////////////////
	if ( _bAbsolute || SAB._bAbsolute )
	{
		if ( _bAbsolute && SAB._bAbsolute )
		{
			QV.BasicInterSLERP( rCoeff, _AbsRel._Absolute, SAB._AbsRel._Absolute );
		}
		else if ( _bAbsolute )
		{
			QV = _AbsRel._Absolute;
			_bAbsolute = false;
		}
		else
		{
			//QV = SAB._AbsRel._Absolute;
			QV.BasicInterSLERP( rCoeff, _AbsRel._Absolute, SAB._AbsRel._Absolute );
		}
	}
	_AbsRel._Absolute = QV;
	/////////////////////////////////////////

	/////////////////////////////////////////
	if ( _bTeleport || SAB._bTeleport )
	{

		/////////////////////////////////////////
		t1 = _Teleport * _AbsRel._Relative;
		t2 = SAB._Teleport * SAB._AbsRel._Relative;
		/////////////////////////////////////////
		
		/////////////////////////////////////////
		tRel.BasicInterSLERP( rCoeff, t1, t2 );
		t.BasicInterSLERP( rCoeff, _Teleport, SAB._Teleport );
		/////////////////////////////////////////

		/////////////////////////////////////////
		_Teleport = t;
		/////////////////////////////////////////

		/////////////////////////////////////////
		t.Inverse();
		_AbsRel._Relative = t * tRel;
		/////////////////////////////////////////
	}
	else
	{
		tRel = _AbsRel._Relative;
		_AbsRel._Relative.BasicInterSLERP( rCoeff, tRel, SAB._AbsRel._Relative );
	}
	/////////////////////////////////////////
}


//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	03-07-25:	CVI - Created
//-----------------------------------------------------------------------------
void	STR_ANIM_BASE::Reset()
{
	_bAbsolute = false;
	_bTeleport = false;
	_AbsRel._Absolute.Id();
	_AbsRel._Relative.Id();
	_Teleport.Id();
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
