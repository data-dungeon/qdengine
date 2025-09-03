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
//	CLASS:	S_RENDER_MODE
//	The S_RENDER_MODE class implements ...
//
//	06-03-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_RENDER_STATS_H__
#define __KRENDERER_RENDERER_RENDER_STATS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace	QDT
{
	namespace	KRENDERER
	{
		class RENDER_STATS
		{
		public:

			RENDER_STATS();
			void Clear();
			
			// API calls accessors
			UInt32 GetNbrOfDrawPrimitive() const			{ return (_uiNbrOfDrawPrimitive); }
			UInt32 GetNbrOfPolygones() const				{ return (_uiNbrOfPolygones); }
			UInt32 GetNbrOfVertexBufferLock() const			{ return (_uiNbrOfVertexBufferLock); }
			UInt32 GetNbrOfIndexBufferLock() const			{ return (_uiNbrOfIndexBufferLock); }
			UInt32 GetNbrOfTextureLock() const				{ return (_uiNbrOfTextureLock); }
			UInt32 GetNbrOfSetRenderTarget() const			{ return (_uiNbrOfSetRenderTarget); }
			UInt32 GetNbrOfSetDepthBuffer() const			{ return (_uiNbrOfSetDepthBuffer); }
			UInt32 GetNbrOfSetTexture() const				{ return (_uiNbrOfSetTexture); }
			UInt32 GetNbrOfSetPixelShaderConstant() const	{ return (_uiNbrOfSetPixelShaderConstant); } 
			UInt32 GetNbrOfSetVertexShaderConstant() const	{ return (_uiNbrOfSetVertexShaderConstant); }
			UInt32 GetNbrOfSetPixelShader() const			{ return (_uiNbrOfSetPixelShader); }
			UInt32 GetNbrOfSetVertexShader() const			{ return (_uiNbrOfSetVertexShader); }
			UInt32 GetNbrOfSetVertexBuffer() const			{ return (_uiNbrOfSetVertexBuffer); }
			UInt32 GetNbrOfSetIndexBuffer() const			{ return (_uiNbrOfSetIndexBuffer); }

			// API calls modifors
			void AddDrawPrimitive(UInt32 ui)				{ _uiNbrOfDrawPrimitive += ui; }
			void AddPolygones(UInt32 ui)					{ _uiNbrOfPolygones += ui; }
			void AddVertexBufferLock(UInt32 ui)				{ _uiNbrOfVertexBufferLock += ui; }
			void AddIndexBufferLock(UInt32 ui)				{ _uiNbrOfIndexBufferLock += ui; }
			void AddTextureLock(UInt32 ui)					{ _uiNbrOfTextureLock += ui; }
			void AddSetRenderTarget(UInt32 ui)				{ _uiNbrOfSetRenderTarget += ui; }
			void AddSetDepthBuffer(UInt32 ui)				{ _uiNbrOfSetDepthBuffer += ui; }
			void AddSetTexture(UInt32 ui)					{ _uiNbrOfSetTexture += ui; }
			void AddSetPixelShaderConstant(UInt32 ui)		{ _uiNbrOfSetPixelShaderConstant += ui; }
			void AddSetVertexShaderConstant(UInt32 ui)		{ _uiNbrOfSetVertexShaderConstant += ui; }
			void AddSetPixelShader(UInt32 ui)				{ _uiNbrOfSetPixelShader += ui; }
			void AddSetVertexShader(UInt32 ui)				{ _uiNbrOfSetVertexShader += ui; }
			void AddSetVertexBuffer(UInt32 ui)				{ _uiNbrOfSetVertexBuffer += ui; }
			void AddSetIndexBuffer(UInt32 ui)				{ _uiNbrOfSetIndexBuffer += ui; }

			// API calls timer
			Float64 GetDrawPrimitiveTime() const			{ return (_fDrawPrimitiveTime); }
			Float64 GetSetVertexShaderConstantTime() const	{ return (_fSetVertexShaderConstantTime); }
			Float64 GetSetPixelShaderConstantTime() const	{ return (_fSetPixelShaderConstantTime); }
			Float64 GetSetRenderTargetTime() const			{ return (_fSetRenderTargetTime); }
			Float64 GetSetDepthBufferTime() const			{ return (_fSetDepthBufferTime); }
			Float64 GetSetTextureTime() const				{ return (_fSetTextureTime); }
			Float64 GetSetPixelShaderTime() const			{ return (_fSetPixelShaderTime); }
			Float64	GetSetVertexShaderTime() const			{ return (_fSetVertexShaderTime); }
			Float64 GetSetVertexBufferTime() const			{ return (_fSetVertexBufferTime); }
			Float64 GetSetIndexBufferTime() const			{ return (_fSetIndexBufferTime); }
			Float64 GetPresentTime() const					{ return (_fPresentTime); }

			// API calls modifiers
			void AddDrawPrimitiveTime(Float64 fTime) 			{ _fDrawPrimitiveTime += fTime;}
			void AddSetVertexShaderConstantTime(Float64 fTime) 	{ _fSetVertexShaderConstantTime += fTime;  }
			void AddSetPixelShaderConstantTime(Float64 fTime) 	{ _fSetPixelShaderConstantTime += fTime;  }
			void AddSetRenderTargetTime(Float64 fTime)			{ _fSetRenderTargetTime += fTime; }
			void AddSetDepthBufferTime(Float64 fTime)			{ _fSetDepthBufferTime += fTime; }
			void AddSetTextureTime(Float64 fTime)				{ _fSetTextureTime += fTime; }
			void AddSetPixelShaderTime(Float64 fTime)			{ _fSetPixelShaderTime += fTime; }
			void AddSetVertexShaderTime(Float64 fTime)			{ _fSetVertexShaderTime += fTime; }
			void AddSetVertexBufferTime(Float64 fTime)			{ _fSetVertexBufferTime += fTime; }
			void AddSetIndexBufferTime(Float64 fTime)			{ _fSetIndexBufferTime += fTime; }
			void AddPresentTime(Float64 fTime)					{ _fPresentTime += fTime; }

			// Ressources accessors
			UInt32 GetNbrOfVertexBuffers() const			{ return (_uiNbrOfVertexBuffers); }
			UInt32 GetSizeOfVertexBuffers() const			{ return (_uiSizeOfVertexBuffers); }
			UInt32 GetNbrOfIndexBuffers() const				{ return (_uiNbrOfIndexBuffers); }
			UInt32 GetSizeOfIndexBuffers() const			{ return (_uiSizeOfIndexBuffers); }
			UInt32 GetNbrOfRenderTargets() const			{ return (_uiNbrOfRenderTargets); }
			UInt32 GetSizeOfRenderTargets() const			{ return (_uiSizeOfRenderTargets); }
			UInt32 GetNbrOfCubeRenderTargets() const		{ return (_uiNbrOfCubeRenderTargets); }
			UInt32 GetSizeOfCubeRenderTargets() const		{ return (_uiSizeOfCubeRenderTargets); }
			UInt32 GetNbrOfTextures() const					{ return (_uiNbrOfTextures); }
			UInt32 GetSizeOfTextures() const				{ return (_uiSizeOfTextures); }
			UInt32 GetNbrOfCubeTextures() const				{ return (_uiNbrOfCubeTextures); }
			UInt32 GetSizeOfCubeTextures() const			{ return (_uiSizeOfCubeTextures); }
			UInt32 GetNbrOfVolumeTextures() const			{ return (_uiNbrOfVolumeTextures); }
			UInt32 GetSizeOfVolumeTextures() const			{ return (_uiSizeOfVolumeTextures); }
			UInt32 GetNbrOfVertexShaders() const			{ return (_uiNbrOfVertexShaders); }
			UInt32 GetSizeOfVertexShaders() const			{ return (_uiSizeOfVertexShaders); }
			UInt32 GetNbrOfPixelShaders() const				{ return (_uiNbrOfPixelShaders); }
			UInt32 GetSizeOfPixelShaders() const			{ return (_uiSizeOfPixelShaders); }
			
			// Ressources modifiors
			void AddVertexBuffer(UInt32 ui) 				{ _uiNbrOfVertexBuffers += ui; }
			void IncrSizeOfVertexBuffers(UInt32 ui) 		{ _uiSizeOfVertexBuffers += ui; }
			void AddIndexBuffer(UInt32 ui)					{ _uiNbrOfIndexBuffers += ui; }
			void IncrSizeOfIndexBuffers(UInt32 ui)			{ _uiSizeOfIndexBuffers += ui; }
			void AddRenderTarget(UInt32 ui)					{ _uiNbrOfRenderTargets += ui; }
			void IncrSizeOfRenderTargets(UInt32 ui)			{ _uiSizeOfRenderTargets += ui; }
			void AddTexture(UInt32 ui)						{ _uiNbrOfTextures += ui; }
			void IncrSizeOfTextures(UInt32 ui)				{ _uiSizeOfTextures += ui; }
			void AddCubeTexture(UInt32 ui)					{ _uiNbrOfCubeTextures += ui; }
			void IncrSizeOfCubeTextures(UInt32 ui)			{ _uiSizeOfCubeTextures += ui; }
			void AddCubeRenderTarget(UInt32 ui)				{ _uiNbrOfCubeRenderTargets += ui; }
			void IncrSizeOfCubeRenderTargets(UInt32 ui)		{ _uiSizeOfCubeRenderTargets += ui; }
			void AddVolumeTexture(UInt32 ui)				{ _uiNbrOfVolumeTextures += ui; }
			void IncrSizeOfVolumeTextures(UInt32 ui)		{ _uiSizeOfVolumeTextures += ui; }
			void AddVertexShader(UInt32 ui)					{ _uiNbrOfVertexShaders += ui; }
			void IncrSizeOfVertexShaders(UInt32 ui)			{ _uiSizeOfVertexShaders += ui; }
			void AddPixelShader(UInt32 ui)					{ _uiNbrOfPixelShaders += ui; }
			void IncrSizeOfPixelShaders(UInt32 ui)			{ _uiSizeOfPixelShaders += ui; }

		private:

			// API Calls
			UInt32 _uiNbrOfDrawPrimitive;
			UInt32 _uiNbrOfPolygones;
			UInt32 _uiNbrOfVertexBufferLock;
			UInt32 _uiNbrOfIndexBufferLock;
			UInt32 _uiNbrOfTextureLock;
			UInt32 _uiNbrOfSetRenderTarget;
			UInt32 _uiNbrOfSetDepthBuffer;
			UInt32 _uiNbrOfSetTexture;
			UInt32 _uiNbrOfSetPixelShader;
			UInt32 _uiNbrOfSetVertexShader;
			UInt32 _uiNbrOfSetPixelShaderConstant;
			UInt32 _uiNbrOfSetVertexShaderConstant;
			UInt32 _uiNbrOfSetVertexBuffer;
			UInt32 _uiNbrOfSetIndexBuffer;

			// API Calls timer
			Float64 _fDrawPrimitiveTime;
			Float64 _fSetPixelShaderConstantTime;
			Float64 _fSetVertexShaderConstantTime;
			Float64 _fSetRenderTargetTime;
			Float64 _fSetDepthBufferTime;
			Float64 _fSetTextureTime;
			Float64 _fSetPixelShaderTime;
			Float64 _fSetVertexShaderTime;
			Float64 _fSetVertexBufferTime;
			Float64 _fSetIndexBufferTime;
			Float64 _fPresentTime;

			// Ressources
			UInt32	_uiNbrOfVertexBuffers,
					_uiSizeOfVertexBuffers;
			UInt32	_uiNbrOfIndexBuffers,
					_uiSizeOfIndexBuffers;
			UInt32	_uiNbrOfRenderTargets,
					_uiSizeOfRenderTargets;
			UInt32	_uiNbrOfTextures,
					_uiSizeOfTextures;
			UInt32	_uiNbrOfCubeRenderTargets,
					_uiSizeOfCubeRenderTargets;
			UInt32	_uiNbrOfCubeTextures,
					_uiSizeOfCubeTextures;
			UInt32	_uiNbrOfVolumeTextures,
					_uiSizeOfVolumeTextures;
			UInt32	_uiNbrOfVertexShaders,
					_uiSizeOfVertexShaders;
			UInt32	_uiNbrOfPixelShaders,
					_uiSizeOfPixelShaders;
		};
	}
}

#endif // __KRENDERER_RENDERER_RENDER_STATS_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
