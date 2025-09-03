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
//	CLASS:	IHM_ELEMENT_QUAD
//	The IHM_ELEMENT_QUAD class implements ...
//
//	01-09-27:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTPOLY_IHMELEMENTQUAD_H__
#define __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTPOLY_IHMELEMENTQUAD_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementPoly/IHM_ElementPolyABC)

DECL_KRENDERER(I_PIXEL_SHADER)
DECL_KRENDERER(I_VERTEX_SHADER)
DECL_KRENDERER(I_TEXTURE)
DECL_KRENDERER(I_RENDER_TARGET)
DECL_KRENDERER(I_VERTEX_BUFFER)
DECL_KRENDERER(I_VERTEX_DECLARATION)

namespace QDT
{
	namespace M3D
	{
		class	IHM_ELEMENT_QUAD : public IHM_ELEMENT_POLY_ABC
		{
		public:

			enum
			{
				CMD_SET_POINTS = IHM_ELEMENT_POLY_ABC::CMD_LAST,
				CMD_SET_COLORS,
				CMD_SET_UVS,
				CMD_GET_POINTS,
				CMD_GET_COLORS,
				CMD_GET_UVS,
				CMD_ENABLE_CLIPPING,
				CMD_LAST
			};	
												IHM_ELEMENT_QUAD();
			virtual							   ~IHM_ELEMENT_QUAD();

			//==== COMMON IHM FUNCTIONS =====

			/** Interface initialization. */
			virtual	void						Open(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList);
			/** Close the interface. */
			virtual	void						Close(IHM_INTERFACE *pInterface, IHM_PAGE *pPage, IHM_ELEMENTS_LIST *pElementsList);
			/** Returns 'true' if the point whose coordinates are 'nPointX' and 'nPointY' is inside the element displayed on screen. */
			virtual bool						IsPointInElement(VIEWPORT * pViewport, int nPointX, int nPointY);

			virtual	void						SetVerticesRelativeToScreen(VIEWPORT * pViewport);
			virtual	void						SetVerticesRelativeToScreenClipping(VIEWPORT * pViewport);
			virtual void						SetVerticesRelativeToWorld(POINT2D *pPoints, float rSizeX, float rSizeY);
			virtual	void						SetColors(COLOR *pColors);
			virtual	void						SetUVs(POINT2D *pUVs);
			virtual	void						SetZDepth(const float	rZ);
			virtual	int							GetPointsCount() const;

			static	COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

					void						SetVertices(POINT2D	* pPoints);

					void						InitGeometry();

		private:
			struct S_FILTER_VERTEX
			{
				Float32 x;
				Float32 y;
				Float32 z;
				Float32 w;
				Float32 u;
				Float32 v;
			};

			static void							Init(CLASS_DESCRIPTOR * const pCD);
												
			static void							EM_Construct(void * const pO, void * const pR, void * const pP);
			static void							EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static void							EM_Destruct(void * const pO, void * const pR, void * const pP);
												
			static void							EM_SetPoints(void * const pO, void * const pR, void * const pP);
			static int							CM_SetPoints(COM_PARAM P);
			static void							EM_SetColors(void * const pO, void * const pR, void * const pP);
			static int							CM_SetColors(COM_PARAM P);
			static void							EM_SetMappingCoordinates(void * const pO, void * const pR, void * const pP);
			static int							CM_SetMappingCoordinates(COM_PARAM P);
			static void							EM_GetPoints(void * const pO, void * const pR, void * const pP);
			static void							EM_GetColors(void * const pO, void * const pR, void * const pP);
			static void							EM_GetMappingCoordinates(void * const pO, void * const pR, void * const pP);
			static void							EM_EnableClipping(void * const pO, void * const pR, void * const pP);

		private:
		};
	}
}

#endif // __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTPOLY_IHMELEMENTQUAD_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
