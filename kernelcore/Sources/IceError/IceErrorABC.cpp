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
//	CLASS:	ICE_ERROR_ABC
//
//	06-02-14:	CAU - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(IceError/IceErrorABC)
#include	INCL_KCORE(Time/Time_ABC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

namespace QDT
{

namespace KCORE
{

#ifdef _DEBUG
	#include	"IceErrorABC.inl"
#endif //_DEBUG

//-----------------------------------------------------------------------------
//	Name:		GetErrorLevel
//	Object:		Access to the name of an Error Level as a string
//
//	@param		eErrorLevel	the error level
//	@return					the name of the error level
//	06-02-20:	CAU - Created
//-----------------------------------------------------------------------------
QDT_STRING	ICE_ERROR_ABC::GetErrorLevel(const ERROR_LEVEL &	eErrorLevel)
{
	QDT_ASSERT(eErrorLevel > EL_UNKNOW
			&& eErrorLevel < EL_MAX);	// must be valid
	switch(eErrorLevel)
	{
	default:
		{
			QDT_FAIL();
		}
	case	EL_WARNING1:
		{
			return "WARNING_LVL1";
		}
		break;
	case	EL_WARNING2:
		{
			return "WARNING_LVL2";
		}
		break;
	case	EL_WARNING3:
		{
			return "WARNING_LVL3";
		}
		break;
	case	EL_WARNING4:
		{
			return "WARNING_LVL4";
		}
		break;
	case	EL_ERROR:
		{
			return "ERROR";
		}
		break;
	case	EL_FATAL_ERROR:
		{
			return "FATAL_ERROR";
		}
		break;
	};
	return QDT_STRING();
}

//-----------------------------------------------------------------------------
//	Name:		ICE_ERROR_ABC constructor
//	Object:		Class Constructor.
//
//	@param	iErrorCode	the code of the error.
//	@param	eErrorLevel	the {@link ERROR_LEVEL} of the error.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC::ICE_ERROR_ABC(const int iErrorCode, const ERROR_LEVEL eErrorLevel)
:
_nCode(iErrorCode),
_eLevel(eErrorLevel)
{
	QDT_ASSERT(eErrorLevel > EL_UNKNOW
			&& eErrorLevel < EL_MAX);

	QDT_ASSERT(TIME::Instance());
	_nTime = TIME::Instance()->GetUniversalTime();
}

//-----------------------------------------------------------------------------
//	Name:		~ICE_ERROR_ABC destructor
//	Object:		Class Destructor.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC::~ICE_ERROR_ABC()
{

}

}; // KCORE

}; // QDT

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
