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
//	CLASS:	IHM_ELEMENT_POLY_ABC
//	The IHM_ELEMENT_POLY_ABC class implements ...
//
//	01-09-27:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTPOLY_IHMELEMENTPOLYABC_H__
#define __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTPOLY_IHMELEMENTPOLYABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(COLOR)
DECL_3DENGINE(VIEWPORT)

#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementABC)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Color/Color)

namespace QDT
{
	namespace M3D
	{
		class	IHM_ELEMENT_POLY_ABC : public IHM_ELEMENT_ABC
		{
		public:

			enum
			{
				CMD_SET_ZDEPTH = IHM_ELEMENT_ABC::CMD_LAST,
				CMD_UNDER_2D_INTERFACE,
				CMD_SET_MATERIAL,
				CMD_ROTATE,
				CMD_SET_ROTATION,
				CMD_STOP_ROTATION,
				CMD_FADE,
				CMD_CAN_FADE,
				CMD_SET_FADE_MINIMUM_ALPHA_VALUE,
				CMD_IS_FADING,
				CMD_LAST
			};

			enum	TRANSPARENCY_MODE
			{
				TRANSPARENCY_NONE   = 0,
				TRANSPARENCY_ALPHA,
				TRANSPARENCY_ADD,
			};

			enum	FADE_MODE
			{
				NO_FADE, 
				FADE_IN,
				FADE_OUT
			};


												IHM_ELEMENT_POLY_ABC(CLASS_DESCRIPTOR * const pCD);
			virtual							   ~IHM_ELEMENT_POLY_ABC();

			virtual	void						DisplayElement(VIEWPORT * pViewport);
		#ifndef _MASTER
			virtual void						DebugDisplay(VIEWPORT * pViewport);
		#endif
			
			virtual void						SetVerticesRelativeToScreen(VIEWPORT * pViewport) = 0;
			virtual void						SetVerticesRelativeToWorld(POINT2D * pPoints, float rSizeX, float rSizeY) = 0;
			virtual void						SetColors(COLOR * pColors) = 0;
			virtual void						SetUVs(POINT2D * pUVs) = 0;
			virtual void						SetZDepth(const float	rZ) = 0;
			virtual	int							GetPointsCount() const = 0;
			virtual void						InitGeometry();
												
		//	virtual void						Display(DISPLAY_PARAMETERS & DisplayParameters, float rSizeX, float rSizeY);
												
					const POINT2D &				GetPoint(int nIndex) const;
					COLOR 						GetColor(int nIndex);
					POINT2D						GetUV(int nIndex);

					void						SetUnder2DInterface(bool bUnder2DInterface);
					bool						IsUnder2DInterface() const;
					void						SetMaterialInst(const COM_INSTANCE & MaterialInst);
					void						Synchronize();
												
					const COM_INSTANCE &		GetMaterialInst() const;

			/** Set the rotation speed that allow to rotate the element. The element rotates at each frame. */
					void						SetRotationSpeedAndCenter(const float rRotationSpeed, const POINT2D & RotationCenter);
					float						GetRotationSpeed() const;
					const POINT2D &				GetRotationCenter() const;
			/** Here the rotation is only applied once. */
					void						Rotate(const float rRotationAngle, const POINT2D & RotationCenter);

					bool						CanFade() const;
					void						Fade(const float rTimeMS, const bool	bFadeIn);
					void						SetFadeMinimumAlphaValue(unsigned int nValue);
					bool						IsFading() const;

					void						SetGeometryInDatabank(const bool bAdd);
					void						SetMaterialInDatabank(const bool bAdd);

			static	COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static	COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		protected:

					VECTOR						RelativeToScreen(VIEWPORT *	pViewport, const POINT2D & Point, float rZ) const;
					VECTOR						RelativeToWorld(const POINT2D & Point, float rSizeX, float rSizeY, float rZ) const;

					bool						HasChanged() const;
					void						SetHasChanged(const bool bFlag);

					void						Loop();


		private:
												
			static	void						Init(CLASS_DESCRIPTOR * const pCD);
												
			static	void						EM_SetZDepth(void * const pO, void * const pR, void * const pP);
			static	void						EM_Under2DInterface(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetMaterial(void * const pO, void * const pR, void * const pP);
			static	int							CM_SetMaterial(COM_PARAM P);
			static	void						EM_Rotate(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetRotation(void * const pO, void * const pR, void * const pP);
			static	void						EM_StopRotation(void * const pO, void * const pR, void * const pP);
			static	void						EM_Fade(void * const pO, void * const pR, void * const pP);
			static	int							CM_Fade(COM_PARAM P);
			static	void						EM_CanFade(void * const pO, void * const pR, void * const pP);
			static	void					 	EM_Loop(void * const pO, void * const pR, void * const pP);
			static	void						EM_SetFadeMinimumAlphaValue(void * const pO, void * const pR, void * const pP);
			static	int							CM_SetFadeMinimumAlphaValue(COM_PARAM P);
			static	void						EM_IsFading(void * const pO, void * const pR, void * const pP);

		protected:

			enum
			{
				UNDER_2D_INTERFACE		= 1 << 0,
				TO_SYNC					= 1 << 1,
				GEOMETRY_IN_DATABANK	= 1 << 2,
				MATERIAL_IN_DATABANK	= 1 << 3,
				CLIPPING_IHM_QUAD		= 1 << 4
			};

		//			GEOMETRY_ABC *			_pGeometry;
		//			GEOMETRY_ABC *			_pUntransformedGeometry;
					QDT_FLAGS				_PolyFlags;
					POINT2D					_pPoints[4];
					POINT2D					_pUVs[4];
					COLOR					_Colors[4];



		private:

					COM_INSTANCE			_DatabaseMaterialInst;
					POINT2D					_RotationCenter;
					float					_rRotationSpeed;
					FADE_MODE				_eFadeMode;
					float					_rFadeTimeMS;
					float					_rElapsedTimeMS;
					
					bool					_bHasChanged;
					unsigned char			_nFadeMinAlphaValue;

					VIEWPORT				*_pLastViewport;
		};
	}
}

#endif // __M3D_INTERFACE2D3D_IHM_IHMELEMENTS_IHMELEMENTPOLY_IHMELEMENTPOLYABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
