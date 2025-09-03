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
//	CLASS:	INPUT_DEVICE_INFO
//	The INPUT_DEVICE_INFO class implements ...
//
//	03-01-09:	JJA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KINPUT_INPUT_INPUTDEVICEINFO_H__
#define __KINPUT_INPUT_INPUTDEVICEINFO_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KINPUT
	{
		class	INPUT_DEVICE_INFO
		{
		public:
			INPUT_DEVICE_INFO();
			explicit INPUT_DEVICE_INFO(const INPUT_DEVICE_INFO & C);
			~INPUT_DEVICE_INFO();
			//	REMEMBER To make the destructor virtual if 
			//	you create a subclass, or if you have one

			INPUT_DEVICE_INFO &	operator=(const INPUT_DEVICE_INFO & C);
			void	Copy(const INPUT_DEVICE_INFO & C);

			void	SetNbrXAbsAxes( const int nNbrAxes );
			void	SetNbrYAbsAxes( const int nNbrAxes );
			void	SetNbrZAbsAxes( const int nNbrAxes );
			void	SetNbrXRelAxes( const int nNbrAxes );
			void	SetNbrYRelAxes( const int nNbrAxes );
			void	SetNbrZRelAxes( const int nNbrAxes );
			void	SetNbrRXAxes( const int nNbrAxes );
			void	SetNbrRYAxes( const int nNbrAxes );
			void	SetNbrRZAxes( const int nNbrAxes );
			void	SetNbrSliderAxes( const int nNbrAxes );
			void	SetNbrButtons( const int nNbrButtons );
			void	SetNbrKeys( const int nNbrKeys );
			void	SetNbrPOVs( const int nNbrPOVs );

			int		GetNbrXAbsAxes() const;
			int		GetNbrYAbsAxes() const;
			int		GetNbrZAbsAxes() const;
			int		GetNbrXRelAxes() const;
			int		GetNbrYRelAxes() const;
			int		GetNbrZRelAxes() const;
			int		GetNbrRXAxes() const;
			int		GetNbrRYAxes() const;
			int		GetNbrRZAxes() const;
			int		GetNbrSliderAxes() const;
			int		GetNbrButtons() const;
			int		GetNbrKeys() const;
			int		GetNbrPOVs() const;

			void	Reset();

		private:
			int	_nNbrXAbsAxes;
			int	_nNbrYAbsAxes;
			int	_nNbrZAbsAxes;
			int	_nNbrXRelAxes;
			int	_nNbrYRelAxes;
			int	_nNbrZRelAxes;
			int _nNbrRXAxes;
			int _nNbrRYAxes;
			int _nNbrRZAxes;
			int	_nNbrSliderAxes;
			int	_nNbrButtons;
			int	_nNbrKeys;
			int _nNbrPOVs;
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
