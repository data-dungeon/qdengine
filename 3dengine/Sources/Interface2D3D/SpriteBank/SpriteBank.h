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
//	CLASS:	SPRITE_BANK
//	The SPRITE_BANK class implements ...
//
//	03-07-23:	FDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_INTERFACE2D3D_SPRITEBANK_H__
#define __M3D_INTERFACE2D3D_SPRITEBANK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(CommunicatorType/CommunicatorContainer)

namespace QDT
{
	namespace M3D
	{
		class	SPRITE_BANK : public COMMUNICATOR_CONTAINER
		{
		public:							

			struct SPRITE_BANK_ELM
			{
				int				_nID;
				COM_INSTANCE	_MaterialCI;
			};

														SPRITE_BANK(COM_HANDLE * pHandle = NULL);
														~SPRITE_BANK();

			// IsStored
				const unsigned int					HowManyStored(const QDT_STRING & Name);

			// Duplicate
				void									Duplicate(SPRITE_BANK *pElm);

			// Sprite Management
				void									AddSprite(const int nID, const COM_INSTANCE & MaterialCI);
				void									RemoveSprite(const int nID);
				void									RemoveSprite(const COM_INSTANCE & MaterialCI);
				const int							GetID(COM_INSTANCE & MaterialCI) const;
				const COM_INSTANCE					GetMaterial(const int nID) const;
				const bool							FindElm(const int nID) const;
				const bool							FindElm(const COM_INSTANCE & MaterialCI) const;
				const unsigned int					GetNbrElms() const;
				const SPRITE_BANK_ELM *				GetIthElm(const unsigned int nIndex) const;

			// Name and Export Management
				void									SetVarName(const QDT_STRING & sVarName);
				void									SetSourceFileName(const QDT_STRING & sSourceFileName);
				const QDT_STRING &					GetVarName() const;
				const QDT_STRING &					GetSourceFileName() const;

			// Communication
			static void									Init(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_CONTAINER_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_CONTAINER_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

			static void									EM_Construct(void * const pO, void * const pR, void * const pP);
			static void									EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static void									EM_Destruct(void * const pO, void * const pR, void * const pP);

			// Chunks
		#ifndef _MASTER_ONLY_DATABANK
			static void									CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER	
			static void									CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif

		private:
			
			static void*								_pVTable;

		private:

				QDT_STRING							_sVarName;
				QDT_STRING							_sSourceFileName;
				QDT_VECTOR<SPRITE_BANK_ELM *>		_vSprite;
		};
	}
}

#endif // __M3D_INTERFACE2D3D_SPRITEBANK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
