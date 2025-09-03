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
//	CLASS:	ENGINE_TEXTURE_FILE
//	The ENGINE_TEXTURE_FILE class implements ...
//
//	05-11-10:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREFLOW_H__
#define __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREFLOW_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KRENDERER(Renderer/RendererTypeDefs)
#include INCL_3DENGINE(SceneGraph/Texture/EngineTextureABC)

namespace QDT
{
	namespace M3D
	{
		class ENGINE_TEXTURE_FLOW : public ENGINE_TEXTURE_ABC
		{
		public:

			ENGINE_TEXTURE_FLOW();
			~ENGINE_TEXTURE_FLOW();

			virtual I_TEXTURE* GetTexture();

			static COMMUNICATOR_DESCRIPTOR * GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR * CreateGenericCommunicatorDescriptor();

			void SetUVSet(UInt32 val){_uvSet = val;}
			void SetMapSize(UInt32 val){_uiMapSize = val;}
			void SetUpdateFrequency(Float32 val){_fUpdateFrequency = val;}
			void SetDropSize(Float32 val){_fDropSize = val;}
			void SetEaseIn(Float32 val){_fEaseIn = val;}
			void SetEaseOut(Float32 val){_fEaseOut = val;}
			void SetInitialSpeed(Float32 val){_fInitialSpeed = val;}
			void SetAcceleration(Float32 val){_fAcceleration = val;}
			void SetFriction(Float32 val){_fFriction = val;}
			void SetMaxLife(Float32 val){_fMaxLife = val;}
			void SetHeightLoss(Float32 val){_fHeightLoss = val;}
			void SetRadiusLoss(Float32 val){_fRadiusLoss = val;}
			void SetTrailLoss(Float32 val){_fTrailLoss = val;}
			void SetSpawnFreqR(Float32 val){_fSpawnFreqR = val;}
			void SetSpawnFreqG(Float32 val){_fSpawnFreqG = val;}
			void SetSpawnFreqB(Float32 val){_fSpawnFreqB = val;}

			UInt32	GetUVSet(){return _uvSet;}
			UInt32  GetMapSize(){return _uiMapSize;}
			Float32 GetUpdateFrequency(){return _fUpdateFrequency;}
			Float32 GetDropSize(){return _fDropSize;}
			Float32 GetEaseIn(){return _fEaseIn;}
			Float32 GetEaseOut(){return _fEaseOut;}
			Float32 GetInitialSpeed(){return _fInitialSpeed;}
			Float32 GetAcceleration(){return _fAcceleration;}
			Float32 GetFriction(){return _fFriction;}
			Float32 GetMaxLife(){return _fMaxLife;}
			Float32 GetHeightLoss(){return _fHeightLoss;}
			Float32 GetRadiusLoss(){return _fRadiusLoss;}
			Float32 GetTrailLoss(){return _fTrailLoss;}
			Float32 GetSpawnFreqR(){return _fSpawnFreqR;}
			Float32 GetSpawnFreqG(){return _fSpawnFreqG;}
			Float32 GetSpawnFreqB(){return _fSpawnFreqB;}


		private:

			static void ComInit(CLASS_DESCRIPTOR * const pCD);
			static void EM_Construct(void * const pO, void * const pR, void * const pP);
			static void EM_Destruct(void * const pO, void * const pR, void * const pP);
			static void	CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);


			//Attributs à syncroniser
			UInt32	_uvSet;
			UInt32  _uiMapSize;
			Float32 _fUpdateFrequency;
			Float32 _fDropSize;
			Float32 _fEaseIn;
			Float32 _fEaseOut;
			Float32 _fInitialSpeed;
			Float32 _fAcceleration;
			Float32 _fFriction;
			Float32 _fMaxLife;
			Float32 _fHeightLoss;
			Float32 _fRadiusLoss;
			Float32 _fTrailLoss;
			Float32 _fSpawnFreqR;
			Float32 _fSpawnFreqG;
			Float32 _fSpawnFreqB;
		};
	}
}

#endif // __M3D_SCENEGRAPH_TEXTURE_ENGINETEXTUREFILE_H__

//=============================================================================
//	CODE END HERE
//=============================================================================

