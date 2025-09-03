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
//	CLASS:	INPUT_OBJECT_ABC
//
//	02-06-24:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/InputObjectABC)

#include	INCL_KINPUT(Input/IObjectParam)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INPUT_OBJECT_ABC constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT_ABC::INPUT_OBJECT_ABC()
:
_eType(OBJ_UNKNOWN),
_strObjectName("unknown object name")
{

}

//-----------------------------------------------------------------------------
//	Name:		INPUT_OBJECT_ABC constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT_ABC::INPUT_OBJECT_ABC(const INPUT_OBJECT_ABC & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_OBJECT_ABC destructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT_ABC::~INPUT_OBJECT_ABC()
{
	ClearAllObserver();
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT_ABC &	INPUT_OBJECT_ABC::operator=(const INPUT_OBJECT_ABC & C)
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
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT_ABC::Copy(const INPUT_OBJECT_ABC & C)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		AddObserver
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
bool	INPUT_OBJECT_ABC::AddObserver(IOBJECT_PARAM *	pObjParam)
{
	QDT_ASSERT(pObjParam);
	
	//if not null and not already in
	bool bAlreadyIn = (_tVectObserver.Find( _tVectObserver.GetHead(), pObjParam) != _tVectObserver.GetTail());
	if( pObjParam && (bAlreadyIn == false) )
	{
		//add
		_tVectObserver.PushTail( pObjParam );
		return true;
	}
	QDT::KCORE::QDT_Warning(QDT_COMMENT("Object Observer déjà utilisé !"));
	return false;

}

//-----------------------------------------------------------------------------
//	Name:		RemoveObserver
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
bool	INPUT_OBJECT_ABC::RemoveObserver(IOBJECT_PARAM *	pObjParam)
{
	QDT_ASSERT( pObjParam );
	if( pObjParam )
	{
		QDT_VECTOR< IOBJECT_PARAM*>::ITERATOR ItFound = _tVectObserver.Find( _tVectObserver.GetHead(), pObjParam );
		if( ItFound != _tVectObserver.GetTail() )
		{	
			_tVectObserver.Remove( ItFound );
			return( true );
		}
		else
		{
			return( false );
		}
	}
	else
	{
		return( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrObserver
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
int	INPUT_OBJECT_ABC::GetNbrObserver() const
{
	return	(_tVectObserver.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetObserver
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
IOBJECT_PARAM *	INPUT_OBJECT_ABC::GetObserver(const int	nIndex)
{
	return	(_tVectObserver[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		ClearAllObserver
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT_ABC::ClearAllObserver()
{
	_tVectObserver.ClearAll();
}

//-----------------------------------------------------------------------------
//	Name:		SetType
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT_ABC::SetType(const OBJ_TYPE	eType)
{
	_eType = eType;
}


//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT_ABC::OBJ_TYPE	INPUT_OBJECT_ABC::GetType() const
{
	return	(_eType);
}

//-----------------------------------------------------------------------------
//	Name:		GetTypeString
//	Object:		
//	03-01-08:	FDE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
QDT_STRING	INPUT_OBJECT_ABC::GetTypeString() const
{
	QDT_ASSERT(GetType() > OBJ_UNKNOWN && GetType() < MAX_OBJ_TYPE);

	QDT_STRING sType;

	switch(GetType())
	{
		default:
		{
			QDT_FAIL();
			sType = "unknown";
			break;
		}
		case OBJ_X_AXIS_REL:
		{
			sType = "OBJ_X_AXIS_REL";
			break;
		}
		case OBJ_Y_AXIS_REL:
		{
			sType = "OBJ_Y_AXIS_REL";
			break;
		}
		case OBJ_Z_AXIS_REL:
		{
			sType = "OBJ_Z_AXIS_REL";
			break;
		}
		case OBJ_X_AXIS_ABS:
		{
			sType = "OBJ_X_AXIS_ABS";
			break;
		}
		case OBJ_Y_AXIS_ABS:
		{
			sType = "OBJ_Y_AXIS_ABS";
			break;
		}
		case OBJ_Z_AXIS_ABS:
		{
			sType = "OBJ_Z_AXIS_ABS";
			break;
		}
		case OBJ_RX_AXIS:
		{
			sType = "OBJ_RX_AXIS";
			break;
		}
		case OBJ_RY_AXIS:
		{
			sType = "OBJ_RY_AXIS";
			break;
		}
		case OBJ_RZ_AXIS:
		{
			sType = "OBJ_RZ_AXIS";
			break;
		}
		case OBJ_BUTTON:
		{
			sType = "OBJ_BUTTON";
			break;
		}
		case OBJ_KEY:
		{
			sType = "OBJ_KEY";
			break;
		}
		case OBJ_POV:
		{
			sType = "OBJ_POV";
			break;
		}
		case OBJ_SLIDER_AXIS:
		{
			sType = "OBJ_SLIDER_AXIS";
			break;
		}
	}

	return	(sType);		
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetHandle
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
int	INPUT_OBJECT_ABC::GetHandle() const 
{
	return _nHandle;
}

//-----------------------------------------------------------------------------
//	Name:		SetObjectName
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT_ABC::SetObjectName(const QDT_STRING &	strObjectName)
{
	_strObjectName = strObjectName;
}

//-----------------------------------------------------------------------------
//	Name:		GetObjectName
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	INPUT_OBJECT_ABC::GetObjectName() const
{
	return	(_strObjectName);
}


//-----------------------------------------------------------------------------
// Name: DispatchEvent
// Object: 
//01-07-25 FKE Created: 
//-----------------------------------------------------------------------------
void INPUT_OBJECT_ABC::DispatchEvent(const float rVal)
{	
	int i , nNbrObjObserver;

	nNbrObjObserver = GetNbrObserver();

	for (i = 0;i < nNbrObjObserver; ++i)
	{	
		GetObserver(i)->ProcessEvent(rVal);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetDefaultMinAbsAxisValue
//	Object:		returns the min value returned by an axis on a joystick
//	03-01-09:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_OBJECT_ABC::GetDefaultMinAbsAxisValue()
{
#if defined(PLATFORM_PC)
	return	(-1000);
#elif defined(PLATFORM_PS3)
	return	(-128); // TODO
#elif defined(PLATFORM_X360)
	return	(-32768); // TODO
#else
	#error CAN T IDENTIFY TARGET PLATFORM, PLEASE CHECK
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetDefaultMaxAbsAxisValue
//	Object:		returns the max value returned by an axis on a joystick
//	03-01-09:	JJA - Created
//-----------------------------------------------------------------------------
int	INPUT_OBJECT_ABC::GetDefaultMaxAbsAxisValue()
{
#if defined(PLATFORM_PC)
	return	(1000);
#elif defined(PLATFORM_PS3)
	return	(127); // TODO
#elif defined(PLATFORM_X360)
	return	(32767); // TODO
#else
	#error CAN T IDENTIFY TARGET PLATFORM, PLEASE CHECK
#endif
}

//-----------------------------------------------------------------------------
//	Name:		IsAnAxis
//	Object:		
//	06-03-24:	NHO - Created
//-----------------------------------------------------------------------------
bool	INPUT_OBJECT_ABC::IsAnAxis() const
{
	OBJ_TYPE eType = GetType();
	switch (eType)
	{
	case OBJ_POV:
//	case OBJ_X_AXIS_REL:
//	case OBJ_Y_AXIS_REL:
//	case OBJ_Z_AXIS_REL:
	case OBJ_X_AXIS_ABS:
	case OBJ_Y_AXIS_ABS:
	case OBJ_Z_AXIS_ABS:
	case OBJ_RX_AXIS:
	case OBJ_RY_AXIS:
	case OBJ_RZ_AXIS:
	case OBJ_SLIDER_AXIS:
		return true;
	default :
		return false;
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
