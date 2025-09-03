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
//	CLASS:	FILTER_SCRATCH
//	The FILTER_SCRATCH class implements ...
//
//	04-01-02:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __DISPLAY_VIEWPORT_FILTERSCRATCH_H__
#define __DISPLAY_VIEWPORT_FILTERSCRATCH_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(VIEWPORT)

#include INCL_KCOM(CommunicatorType/Communicator)
#include INCL_KMATH(Math/Color/Color)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)

namespace QDT
{
	namespace M3D
	{
		class	FILTER_SCRATCH : public COMMUNICATOR
		{
		public:

			// Constructor & destructor
												FILTER_SCRATCH();
											~FILTER_SCRATCH();

			// Accessors
			_INLINE_ const COM_INSTANCE &		GetMaterialInst() const;
		//	_INLINE_ MATERIAL_ABC *				GetMaterial() const;
		//	_INLINE_ GEOMETRY_ABC *				GetGeometry() const;
			_INLINE_ const COLOR &				GetColor() const;
			_INLINE_ bool						GetVisible() const;
			_INLINE_ unsigned int				GetDuration() const;
			_INLINE_ unsigned int				GetFrequency() const;
			_INLINE_ float						GetMaxSizeX() const;
			_INLINE_ float						GetMaxSizeY() const;
			_INLINE_ bool						GetRandomSize() const;

			// Modifiors
			_INLINE_ void						SetMaterial(const COM_INSTANCE & ciMaterial);
			_INLINE_ void						SetColor(const COLOR & C);
			_INLINE_ void						SetVisible(bool b);
			_INLINE_ void						SetDuration(unsigned int n);
			_INLINE_ void						SetFrequency(unsigned int n);
			_INLINE_ void						SetMaxSizeX(float r);
			_INLINE_ void						SetMaxSizeY(float r);
			_INLINE_ void						SetRandomSize(bool b);

			// Display
			void								Display(VIEWPORT *pViewport);
			
			// Init
			void								InitGeometry();

			static void							Init(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR*		GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR*		CreateGenericCommunicatorDescriptor();

		private:

			enum
			{
				CMD_SET_MATERIAL = CMD_COM_USER_START,
				CMD_GET_MATERIAL,
				CMD_SET_COLOR,
				CMD_GET_COLOR,
				CMD_SET_ALPHA,
				CMD_GET_ALPHA,
			};

			static void							EM_Construct(void * const pO, void * const pR, void * const pP);
			static void							EM_Destruct(void * const pO, void * const pR, void * const pP);
			static void							EM_SetMaterial(void * const pO, void * const pR, void * const pP);
			static void							EM_GetMaterial(void * const pO, void * const pR, void * const pP);
			static void							EM_SetColor(void * const pO, void * const pR, void * const pP);
			static void							EM_GetColor(void * const pO, void * const pR, void * const pP);
			static void							EM_SetAlpha(void * const pO, void * const pR, void * const pP);
			static void							EM_GetAlpha(void * const pO, void * const pR, void * const pP);
		#ifndef _MASTER_ONLY_DATABANK
			static void							CM_ReadChunk0(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void							CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER	
			static void							CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif

		//	GEOMETRY_ABC * *					_pGeometry;	
			unsigned char						_nCurrGeometry;

			COM_INSTANCE						_ciMaterial;
			COLOR								_Color;
			unsigned int						_nDuration;
			unsigned int						_nFrequency;
			float								_rDurationInSec;
			float								_rFrequencyInSec;
			float								_rMaxSizeX;
			float								_rMaxSizeY;

			float								_rCurrentTime;
			POINT2D								_Position;
			POINT2D								_Size;

			bool								_bVisible;
			bool								_bRandomSize;
		};
	}
}

#ifndef _DEBUG
	#include	"FilterScratch.inl"
#endif

#endif // __DISPLAY_VIEWPORT_FILTERSCRATCH_H__


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
