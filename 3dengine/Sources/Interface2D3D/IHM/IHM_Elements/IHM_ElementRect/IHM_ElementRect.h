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
//	CLASS:	IHM_ELEMENT_RECT
//	The IHM_ELEMENT_RECT class implements ...
//
//	01-11-30:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTRECT_IHMELEMENTRECT_H__
#define __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTRECT_IHMELEMENTRECT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementABC)

namespace QDT
{
	namespace M3D
	{
		class	IHM_ELEMENT_RECT : public IHM_ELEMENT_ABC
		{
		public:

			enum
			{
				CMD_SET_RELATIVE_POSITION = IHM_ELEMENT_ABC::CMD_LAST,
				CMD_GET_RELATIVE_POSITION,
				CMD_LAST
			};

							IHM_ELEMENT_RECT();				
							IHM_ELEMENT_RECT(CLASS_DESCRIPTOR * const pCD);
						~IHM_ELEMENT_RECT();

			/** Debug display of the element. */
		#ifndef _MASTER
			virtual void	DebugDisplay(VIEWPORT * pViewport);
		#endif

   			/** Returns 'true' if the point whose coordinates are 'nPointX' and 'nPointY' is inside the element displayed on screen. */
			virtual bool	IsPointInElement(VIEWPORT * pViewport, int nPointX, int nPointY);

			/** Get the position of en element in percentage of the display window. */
					void	GetRelativePosition(float &rLeft, float &rRight, float &rTop, float &rBottom) const;
					
					
					void	SetRelativePosLeft(  float rRelativePosLeft);
					void	SetRelativePosRight( float rRelativePosRight);
					void	SetRelativePosTop(   float rRelativePosTop);
					void	SetRelativePosBottom(float rRelativePosBottom);
					
					float	GetRelativePosLeft()   const;
					float	GetRelativePosRight()  const;
					float	GetRelativePosTop()    const;
					float	GetRelativePosBottom() const;
					
			/** Get the absolute left in pixel of the screen. */
					float	GetAbsolutePosLeft(VIEWPORT * pViewport)   const;
			/** Get the absolute right in pixel of the screen. */
					float	GetAbsolutePosRight(VIEWPORT * pViewport)  const;
			/** Get the absolute top in pixel of the screen. */
					float	GetAbsolutePosTop(VIEWPORT * pViewport)    const;
			/** Get the absolute bottom in pixel of the screen. */
					float	GetAbsolutePosBottom(VIEWPORT * pViewport) const;

			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private :

			// The position are private in order to force sub-classes to use SetPosition, GetPosition, etc.
			float	_rRelativePosLeft, _rRelativePosRight, _rRelativePosTop, _rRelativePosBottom;

			static void			Init(CLASS_DESCRIPTOR * const pCD);

			static void			EM_SetRelativePosition(void * const pO, void * const pR, void * const pP);
			static int			CM_SetRelativePosition(COM_PARAM P);
			static void			EM_GetRelativePosition(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTRECT_IHMELEMENTRECT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
