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
//	CLASS:	STATISTICS
//	The STATISTICS class contains all informations such as the number of
//  triangles send to the card, the frame rate...
//
//	01-10-25:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __DISPLAY_STATISTICS_H__
#define __DISPLAY_STATISTICS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D
	{
		class	STATISTICS
		{
		public:
					
									STATISTICS();
								~STATISTICS();	
					 
					void			UpdateStatisticsBeforeDisplay();
					void			UpdateStatisticsAfterDisplay();
					 
					void			DisplayShortStats(bool bValue);
					bool			IsShortStatsDisplay() const;
		             
					void			DisplayGraphicStats(bool bValue);
					bool			IsGraphicStatsDisplay() const;

					void			DisplayMemStats(bool bValue);
					bool			IsMemStatsDisplay() const;

					void			DisplayDataBankStats(bool bValue);
					bool			IsDataBankStatsDisplay() const;

					void			DisplayVMStats(bool bValue);
					bool			IsVMStatsDisplay() const;

					void			DisplayGraphicOptions(bool bValue);
					bool			IsGraphicOptionsDisplay() const;

					void			DisplaySoundStats(bool bValue);
					bool			IsSoundStatsDisplay() const;

			/** Called at each time you send polygones to the hardware */
			_INLINE_ void			AddFrustrumIntersections(unsigned int nNbrFrustrumIntersections)		{ _nNbrFrustrumIntersections += nNbrFrustrumIntersections; }
			_INLINE_ void			AddHexahedronIntersections(unsigned int nNbrHexahedronIntersections)	{ _nNbrHexahedronIntersections += nNbrHexahedronIntersections; }

			/** Accessors */
			_INLINE_ unsigned int	GetNbrFrustrumIntersections() const			{ return (_nNbrFrustrumIntersections); }
			_INLINE_ unsigned int	GetNbrHexahedronIntersections() const		{ return (_nNbrHexahedronIntersections); }

			/** Modifiors */
			_INLINE_ void			SetRefreshPeriod(unsigned int nPeriod)		{ _nPeriod = nPeriod; }
			
		private:

			void					AddText(const COM_INSTANCE &Text) const;
			void					RemoveText(const COM_INSTANCE &Text) const;
			void					InitText(const COM_INSTANCE &Text, float rLeft, float rRight, float rTop, float rBottom) const;

			void					InitFonts();
			bool					_bFontIsInit;

			/** Display flags */
			bool					_bDisplayShortStats;
			bool					_bDisplayGraphicStats;
			bool					_bDisplayMemStats;
			bool					_bDisplayDataBankStats;
			bool					_bDisplayVMStats;
			bool					_bDisplayGraphicOptions;
			bool					_bDisplaySoundStats;
			bool					_bChangeStats;


			/** For framerate display */
			COM_INSTANCE	   _ShortStatsText;		// Short stats : polygones count & FPS
			COM_INSTANCE	   _FullStatsText;
			COM_INSTANCE	   _MemStatsText;
			COM_INSTANCE	   _DataBankStatsText;
			COM_INSTANCE	   _VMStatsText;
			COM_INSTANCE	   _GraphicOptionsText;
			COM_INSTANCE	   _SoundStatsText;

			/** Performance statistics */

			unsigned int			_nNbrFrustrumIntersections;
			unsigned int			_nNbrHexahedronIntersections;

			/** Display Update Period */
			unsigned int			_nPeriod;			// in ms
			unsigned int			_nNextRefresh;
		};
	}
}

#ifndef _DEBUG
	#include	"Statistics.inl"
#endif

#endif // __DISPLAY_STATISTICS_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
