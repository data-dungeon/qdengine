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
//	CLASS:	ICE_ERROR_ITERATOR
//
//	06-02-14:	CAU - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		Equality operator
//
//	@param		it	the iterator to test with
//	@return			true if iterators are on the same error, false otherwise
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::operator==(const ICE_ERROR_ITERATOR &	it) const
{
	return (_pError == it._pError);
}

//-----------------------------------------------------------------------------
//	Name:		operator!=
//	Object:		Difference operator
//
//	@param		it	the iterator to test with
//	@return			false if iterators are on the same error, true otherwise
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::operator!=(const ICE_ERROR_ITERATOR &	it) const
{
	return !((*this)==it);
}

//-----------------------------------------------------------------------------
//	Name:		operator *()
//	Object:		Access operator
//
//	@return		the current pointed error
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC * ICE_ERROR_ITERATOR::operator * () const
{
	return _pError;
}

//-----------------------------------------------------------------------------
//	Name:		GetError
//	Object:		Get the current error.
//
// @return		The current error.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC *	ICE_ERROR_ITERATOR::GetError() const
{
	return _pError;
}

//-----------------------------------------------------------------------------
//	Name:		SetModuleContinuity
//	Object:		Set the module continuity to allow browsing through modules.
//
//	@param		bModuleContinuity	allows or not module continuity.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ICE_ERROR_ITERATOR::SetModuleContinuity(const bool	bModuleContinuity)
{
	_bModuleContinuity = bModuleContinuity;
}

//-----------------------------------------------------------------------------
//	Name:		GetModuleContinuity
//	Object:		Get the module continuity.
//
//	@return		the module continuity attribute.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GetModuleContinuity() const
{
	return _bModuleContinuity;
}

//-----------------------------------------------------------------------------
//	Name:		SetErrorLevelContinuity
//	Object:		Set the error level continuity to allow browsing through error levels.
//
//	@param bErrorLevelContinuity	allows or not error levels continuity.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ICE_ERROR_ITERATOR::SetErrorLevelContinuity(const bool	bErrorLevelContinuity)
{
	_bErrorLevelContinuity = bErrorLevelContinuity;
}

//-----------------------------------------------------------------------------
//	Name:		GetErrorLevelContinuity
//	Object:		Get the error level continuity.
//
//	@return		the error level continuity attribute.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GetErrorLevelContinuity() const
{
	return _bErrorLevelContinuity;
}

//-----------------------------------------------------------------------------
//	Name:		ModuleHasError
//	Object:		Allows to know if the current module has errors.
//
//	@return	true if the current module has errors, false otherwise
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::ModuleHasError() const
{
	return (IsValid() && _pModule->HasError());
}

//-----------------------------------------------------------------------------
//	Name:		GetModuleID
//	Object:		Get the current module ID.
//
//	@return		the current module ID if exists, MI_MAX otherwise.
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MANAGER::MODULE_ID	ICE_ERROR_ITERATOR::GetModuleID() const
{
	if(_pModule)
	{
		return _pModule->GetModuleID();
	}
	return ERROR_MANAGER::MI_MAX;
}	

//-----------------------------------------------------------------------------
//	Name:		ErrorLevelHasError
//	Object:		Allows to know if the current error level has errors.
//
//	@return		true if the current error level has errors, false otherwise.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::ErrorLevelHasError() const
{
	QDT_ASSERT(_eErrorLevel >= ICE_ERROR_ABC::EL_UNKNOW
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid
	return ( IsValid() && _pModule->HasError(_eErrorLevel) );
}

//-----------------------------------------------------------------------------
//	Name:		GetErrorLevel
//	Object:		Get the current error level.
//
//	@return		the current error level if it exists, EL_MAX otherwise.
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC::ERROR_LEVEL	ICE_ERROR_ITERATOR::GetErrorLevel() const
{
	if	(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
		&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX)
	{
		return _eErrorLevel;
	}
	return ICE_ERROR_ABC::EL_MAX;
}

//-----------------------------------------------------------------------------
//	Name:		operator++
//	Object:		Pre Increment operator.
//
//	@return		this
//	06-02-17:	CAU - Created
//-----------------------------------------------------------------------------
const ICE_ERROR_ITERATOR &	ICE_ERROR_ITERATOR::operator++()
{
	GoToNext();
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator++
//	Object:		Post Increment operator
//
//	@return		a incremented copy of this 
//	06-02-17:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ITERATOR	ICE_ERROR_ITERATOR::operator++(int)
{
	ICE_ERROR_ITERATOR newIt(*this);
	++(*this);
	return newIt;
}

//-----------------------------------------------------------------------------
//	Name:		operator+
//	Object:		Sum operator
//
//	@param		nInc	the step of the incrementation
//	@return				a incremented copy of this
//	06-02-17:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ITERATOR	ICE_ERROR_ITERATOR::operator+(int	nInc) const
{
	QDT_ASSERT(nInc>0);	// must be positive
	ICE_ERROR_ITERATOR newIt(*this);
	newIt += nInc;
	return newIt;
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		Sum affectation operator.
//
//	@param		nInc	the step of the incrementation
//	@return				this
//	06-02-17:	CAU - Created
//-----------------------------------------------------------------------------
const ICE_ERROR_ITERATOR &	ICE_ERROR_ITERATOR::operator+=(int	nInc)
{
	QDT_ASSERT(nInc>0);	// must be positive
	int i(0);
	for(;i<nInc && GoToNext();++i)
	{
		
	}
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator--
//	Object:		Pre Decrement operator.
//
//	@return		this
//	06-02-17:	CAU - Created
//-----------------------------------------------------------------------------
const ICE_ERROR_ITERATOR &	ICE_ERROR_ITERATOR::operator--()
{
	GoToPrevious();
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		operator--
//	Object:		Post Decrement operator.
//
//	@return		a decremented copy of this
//	06-02-17:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ITERATOR	ICE_ERROR_ITERATOR::operator--(int)
{
	ICE_ERROR_ITERATOR newIt(*this);
	--(*this);
	return newIt;
}

//-----------------------------------------------------------------------------
//	Name:		operator-
//	Object:		Sub operator.
//
//	@param		nDec	the step of the decrementation
//	@return				a decremented copy of this
//	06-02-17:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ITERATOR	ICE_ERROR_ITERATOR::operator-(int	nDec) const
{
	QDT_ASSERT(nDec>0);	// must be positive
	ICE_ERROR_ITERATOR newIt(*this);
	newIt += nDec;
	return newIt;
}

//-----------------------------------------------------------------------------
//	Name:		operator-=
//	Object:		Sub affectation operator.
//
//	@param		nDec	the step of the decrementation
//	@return				this
//	06-02-17:	CAU - Created
//-----------------------------------------------------------------------------
const ICE_ERROR_ITERATOR &	ICE_ERROR_ITERATOR::operator-=(int	nDec)
{
	QDT_ASSERT(nDec>0);	// must be positive
	int i(0);
	for(;i<nDec && GoToPrevious();++i)
	{
		
	}
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		IsValid
//	Object:		Checks if the iterator is valid
//
//	@return		true if the module, error and level are corrects, false otherwise
//	06-06-12:	VPI - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::IsValid() const
{
	return ( _pModule != NULL && 
			 _pError != NULL &&
			 _eErrorLevel >= ICE_ERROR_ABC::EL_UNKNOW 
			 && _eErrorLevel < ICE_ERROR_ABC::EL_MAX );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
