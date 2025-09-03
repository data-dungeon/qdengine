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
//	CLASS:	SINGLETON
//	The SINGLETON class implements a singleton template.
//
//	That singleton needs a call to CreateInstance to be created and a call to
//  ReleaseInstance to be deleted.
//	It is used to orderer (synchronize) singleton destruction
//	IMPORTANT : 
//	The class templatized must call in it's destructor the function
//								<< IsImplicitRelease()>> 
//	and ASSERT if return false
//
//	06-01-19:	GRI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_TEMPLATE_SINGLETON_H__
#define __KCORE_TEMPLATE_SINGLETON_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

template <typename T> class SINGLETON
{
public:

		// Destructor.
		~SINGLETON() 
		{
		}
		// Force SINGLETON instance. 
		static void SetInstance ( T* pSINGLETONInst )
		{
			_pSINGLETONInst = pSINGLETONInst;
		}

		// Create SINGLETON instance. 
		static void Create ()
		{
			QDT_ENSURE(_pSINGLETONInst == NULL);
			_pSINGLETONInst = new T();
		}

		// Return SINGLETON instance. 
		static T* Instance()
		{
			QDT_ASSERT(_pSINGLETONInst != NULL);
			return (_pSINGLETONInst);
		};

		// Return if have been instancied
		static bool IsInstancied()
		{
			return _pSINGLETONInst ? true : false;
		}

		// Release SINGLETON instance. 
		static void Release ()
		{
			if ( _pSINGLETONInst )
			{
				delete _pSINGLETONInst;
				_pSINGLETONInst = NULL;
			}
		}
protected:

		// Private constructor. 
		SINGLETON()
		{
		}
		// SINGLETON instance. 
		static T* _pSINGLETONInst;
};

template <typename T> T* SINGLETON <T>::_pSINGLETONInst = NULL;

#endif
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
