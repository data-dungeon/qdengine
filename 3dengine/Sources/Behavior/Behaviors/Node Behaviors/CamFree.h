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
// CLASS: CAM_FREE
//
//
//00-12-11 RMA Created: 
//*****************************************************************************

//=============================================================================
// Name: CAM_FREE
//00-12-11 RMA Created: 
//=============================================================================

#ifndef __M3D_CAM_FREE_H__
#define __M3D_CAM_FREE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_3DENGINE(Behavior/AnimNode/AnimNode)

DECL_3DENGINE(CAM_FREE_STRUCT)

namespace QDT
{
	namespace M3D
	{
		class	CAM_FREE : public ANIM_NODE
		{
		public:

			enum
			{
				CMD_SET_ACTIVE = CMD_ANIM_NODE_LAST,
				CMD_SET_KEY,
				CMD_GET_KEY,
				CMD_CAM_FREE_LAST,
			};

			explicit	CAM_FREE();
			CAM_FREE( CLASS_DESCRIPTOR* const pCD );



		protected:
			/* Copy constructor */
			explicit CAM_FREE(const CAM_FREE & BehCamFree) ;
			/** Destructor */
			~CAM_FREE() ;



		public :
			/** Operator = */
			CAM_FREE & operator=( const CAM_FREE & BehCamFree) ;

			/** Equally comparable operator*/
			bool			operator==( const CAM_FREE & BehCamFree) ;

			bool	CAM_FREE::IsActived();
			static CAM_FREE *	GetActive();
			static	CAM_FREE*		GetFreeCamAnimFromStruct( CAM_FREE_STRUCT* pStruct );
			void					SetActiveStatus( bool b );

			void	SetEntityInst(const COM_INSTANCE & EntityInst);

			void	Reset();
			float	GetRoll() const;
			void	SetRoll(const float rRoll);
			void	ReinitRoll(const float rRoll);

			///////////////////////////////////////////////////////////////
			//	COMMUNICATOR
			///////////////////////////////////////////////////////////////
			static void		COMInit(CLASS_DESCRIPTOR * const pCD);
			static			COMMUNICATOR_DESCRIPTOR*	GetGenericCommunicatorDescriptor();
			static			COMMUNICATOR_DESCRIPTOR*	CreateGenericCommunicatorDescriptor();
			const QDT_STRING&	GetClassGUID() const;
			void				SetClassGUID(const QDT_STRING& sClassGUID);
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////
			//	COMMANDS
			////////////////////////////////////////////////////////////////
			static void	EM_SetActive(void * const pO, void * const pR, void * const pP);
			static void	EM_SetKey(void * const pO, void * const pR, void * const pP);
			static void	EM_GetKey(void * const pO, void * const pR, void * const pP);
			////////////////////////////////////////////////////////////////


		private:
			/** Copy method to avoid some code duplication in copy constructor & operator = */ 
			void	Copy(const CAM_FREE & BehCamFree) ;
			static	CAM_FREE	   *_pActiveCamFree;
			bool					_bIsActived;

			///////////////////////////////////////////////////////////////
			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			QDT_STRING						_sClassGUID;
			///////////////////////////////////////////////////////////////

		};
	}
}

#endif // __M3D_CAM_FREE_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
