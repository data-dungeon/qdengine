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
//	CLASS:	IHM_ELEMENT_BITMAP
//	The IHM_ELEMENT_BITMAP class implements ...
//
//	01-08-20:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTRECT_IHMELEMENTBITMAP_H__
#define __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTRECT_IHMELEMENTBITMAP_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(IHM_ELEMENT_QUAD)
DECL_3DENGINE(DISPLAY_PARAMETERS)

#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementRect)
#include	INCL_KMATH(Math/Color/Color)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)

namespace QDT
{
	namespace M3D
	{
		class	IHM_ELEMENT_BITMAP : public IHM_ELEMENT_RECT
		{
		public:
			
			enum
			{
				CMD_SET_CROPPING_COORDINATES = IHM_ELEMENT_RECT::CMD_LAST,
				CMD_COPY,
				CMD_SET_MATERIAL,
				CMD_LAST
			};
												IHM_ELEMENT_BITMAP();
			virtual							   ~IHM_ELEMENT_BITMAP();

			//==== COMMON IHM FUNCTIONS =====
			/** Interface initialization. */
			virtual	void						Open(IHM_INTERFACE * pInterface, IHM_PAGE * pPage, IHM_ELEMENTS_LIST * pElementsList);
			/** Close the interface. */			
			virtual	void						Close(IHM_INTERFACE * pInterface, IHM_PAGE * pPage, IHM_ELEMENTS_LIST * pElementsList);
			/** Display. */						
			virtual	void						DisplayElement(VIEWPORT * pViewport);
			/** Display. */						
			virtual void						Display(DISPLAY_PARAMETERS& DisplayParameters, float rSizeX, float rSizeY);
			/** Returns the type of the element. */
			virtual int							GetDatabaseType();									
			//====								
												
			/** Set the material of the element. */
					void						SetMaterialInst(const COM_INSTANCE & MaterialInst);
					const COM_INSTANCE &		GetMaterialInst() const;

			/** Set the color of the bitmap. Default is white. */
					void						SetColor(const COLOR & Color);
					COLOR 						GetColor() const;
												
			/** OBSOLETE */						
					void						SetCroppingCoordinates(int nLeft, int nRight, int nTop, int nBottom);
			/** Set the cropping coordinates of the bitmap in case you want to display only a part of the bitmap. If the function is not called, all the bitmap is displayed. */
					void						SetCroppingCoordinates(float rLeft, float rRight, float rTop, float rBottom);

			virtual void						SetZDepth(const float rZ);

			static	COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

			void			COM_PostLoad();

					void						SetGeometryInDatabank(const bool bAdd);
					void						SetMaterialInDatabank(const bool bAdd);

		private:

			static	void						Init(CLASS_DESCRIPTOR * const pCD);
											
			static	void						EM_Construct(void * const pO, void * const pR, void * const pP);
			static  void						EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static	void						EM_Destruct(void * const pO, void * const pR, void * const pP);
												
			static	void						EM_SetMaterial(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetCroppingCoordinates(void * const pO, void * const pR, void * const pP);
			static	void						EM_Copy(void * const pO, void * const pR, void * const pP);

					IHM_ELEMENT_QUAD *			GetIhmQuad() const;


		private:

			static void*					_pVTable;
					COM_INSTANCE			_IhmElementQuad;
					POINT2D					_pCroppingUVs[ 4 ];
		};
	}
}

#endif // __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTRECT_IHMELEMENTBITMAP_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
