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
// CLASS: BLEND_SHAPE_PC
//
//
//00-08-16 ELE Created: 
//*****************************************************************************

#ifndef __M3D_BLEND_SHAPE_PC_H__
#define __M3D_BLEND_SHAPE_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class BLEND_SHAPE_PC : public COMMUNICATOR
		{
		public :

			// Commands
			enum
			{
				CMD_BS_LAST = CMD_COM_USER_START,
			};

			enum PACKETS_VERSION
			{ 
				DIALOG_VERSION = 1, 
				CYBER_VERSION,
			};

			BLEND_SHAPE_PC();
			virtual ~BLEND_SHAPE_PC();

			unsigned int					GetNbFrames();

			void							SetContribArray(const float * Array, int size, int FormantCount);
			virtual bool					GetBSWeightsAt( float rCurrentFrame, float *prWeights, unsigned int WeightCount, bool bInterpolate);

			float							GetFPS() const			{ return _rFPS; }
			void							SetFPS(const float fps) { _rFPS = fps; }
			int								GetNbrWeights()const	{ return ( _nFormantCount); }

			// Communication
			static void						COMInit(CLASS_DESCRIPTOR * const pCD);
			static							COMMUNICATOR_DESCRIPTOR*	GetGenericCommunicatorDescriptor();
			static							COMMUNICATOR_DESCRIPTOR*	CreateGenericCommunicatorDescriptor();
			static	void					EM_Construct(void * const pO, void * const pR, void * const pP);
			static  void					EM_Construct_In_Memory(void * const pO, void * const pR, void * const pP);
			static	void					EM_Destruct(void * const pO, void * const pR, void * const pP);
#ifndef _MASTER_ONLY_DATABANK
			static	void					CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif
#ifndef _MASTER	
			static	void					CM_WriteChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
#endif

		private:

			// Private methods
			void 							Copy(const BLEND_SHAPE_PC &BlendShapePC);
			bool							ReadVersion1(IO_STREAM &s);
#ifndef _MASTER
			bool							WriteVersion1(IO_STREAM &s) const;
#endif

			// Paramters		
			float							_rFPS;

			float						   *_pContribArray;
			int								_nContribArraySize;
			int								_nFramesCount; // = _nContribArraySize / FORMANT_ANALYSER::FORMANT_COUNT

			int 							_nPacketsVersion;
			int 							_nFormantCount;
		};
	}
}

#endif //__M3D_BLEND_SHAPE_PC_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
