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
// CLASS: DISPLAY_PARAMETERS
//
//
//	01-02-13:	RMA - Created
//*****************************************************************************


//=============================================================================
// Name: DISPLAY_PARAMETERS
//	01-02-13:	RMA - Created
//=============================================================================
#ifndef __DISPLAY_DISPLAY_PARAMETERS_H__
#define __DISPLAY_DISPLAY_PARAMETERS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(NODE);
DECL_3DENGINE(SCENE_GRAPH);
DECL_3DENGINE(DYNAMIC_CLUSTER);
DECL_KMATH(BOUNDING_VOLUME);

#include INCL_KCOM(CommunicatorType/ComInstance)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Frustrum/Frustrum)
#include INCL_KMATH(Math/Geometry/Matrix/Matrix)

namespace QDT
{
	namespace M3D
	{
		class DISPLAY_PARAMETERS
		{
		public:

			// Constructors & destructor
			DISPLAY_PARAMETERS();
			DISPLAY_PARAMETERS(const DISPLAY_PARAMETERS & DisplayParameters);
			~DISPLAY_PARAMETERS();

			// Operators
			DISPLAY_PARAMETERS &						operator = (const DISPLAY_PARAMETERS & DisplayParameters); 

			// Accessors
			_INLINE_ NODE *								GetNode() const;
			_INLINE_ const FRUSTRUM &					GetFrustrum() const;
			_INLINE_ BOUNDING_VOLUME *					GetBoundingVolume() const;
			_INLINE_ SCENE_GRAPH *						GetCurrentSG() const;
			_INLINE_ QDT_VECTOR <MATRIX> *				GetSkinningMatrices() const;
			_INLINE_ QDT_VECTOR <DYNAMIC_CLUSTER*> *	GetDynamicClusters() const;
			_INLINE_ QDT_VECTOR <COM_INSTANCE> *		GetLightNodesOnDynamicLit() const;
			_INLINE_ QDT_VECTOR <COM_INSTANCE> *		GetLightNodesOnPrelit() const;
		//	_INLINE_ QDT_VECTOR <LIGHT_SOURCE> *		GetLightSourcesOnDynamicLit() const;
		//	_INLINE_ QDT_VECTOR <LIGHT_SOURCE> *		GetLightSourcesOnPrelit() const;
			_INLINE_ bool								IsDoubleSidedMaterial() const;
			_INLINE_ bool								IsIgnoreGlobalMatrix() const;
			_INLINE_ bool								IsSkeletonInside() const;

			// Modifiors
			_INLINE_ void								SetNode(NODE *pNode);
			_INLINE_ void								SetFrustrum(const FRUSTRUM & Frustum);
			_INLINE_ void								SetBoundingVolume(BOUNDING_VOLUME * pBoundingVolume);
			_INLINE_ void								SetCurrentSG(SCENE_GRAPH * pSG);
			_INLINE_ void								SetDoubleSidedMaterial(bool b);
			_INLINE_ void								SetSkinningMatrices(QDT_VECTOR <MATRIX> * pSkinningMatrices);
			_INLINE_ void								SetDynamicClusters(QDT_VECTOR <DYNAMIC_CLUSTER*> * pDynamicClusters);
			_INLINE_ void								SetLightNodesOnDynamicLit(QDT_VECTOR <COM_INSTANCE> * pLightNodesOnDynamicLit);
			_INLINE_ void								SetLightNodesOnPrelit(QDT_VECTOR <COM_INSTANCE> * pLightNodesOnPrelit);
		//	_INLINE_ void								SetLightSourcesOnDynamicLit(QDT_VECTOR <LIGHT_SOURCE> * pLightSourcesOnDynamicLit);
		//	_INLINE_ void								SetLightSourcesOnPrelit(QDT_VECTOR <LIGHT_SOURCE> * pLightSourcesOnPrelit);
			_INLINE_ void								SetIgnoreGlobalMatrix(bool b);
			_INLINE_ void								SetSkeletonInside(bool b);

					float								UpdateFrustrumFar(float rNewFar);

		private:

			void								Copy(const DISPLAY_PARAMETERS & DisplayParameters) ; 

			NODE *								_pNode;
			BOUNDING_VOLUME *					_pBoundingVolume;

			QDT_VECTOR <MATRIX> *				_pSkinningMatrices;
			QDT_VECTOR <DYNAMIC_CLUSTER*> *		_pDynamicClusters;

			QDT_VECTOR <COM_INSTANCE> *			_pLightNodesOnDynamicLit;
			QDT_VECTOR <COM_INSTANCE> *			_pLightNodesOnPrelit;
		//	QDT_VECTOR <LIGHT_SOURCE> *			_pLightSourcesOnDynamicLit;
		//	QDT_VECTOR <LIGHT_SOURCE> *			_pLightSourcesOnPrelit;
			
			FRUSTRUM							_CameraFrustrum;

			SCENE_GRAPH *						_pCurrentSG;
			
			bool								_bDoubleSidedMaterial,
												_bIgnoreGlobalMatrix,
												_bIsSkeletonInside;
		};
	}
}

#ifndef _DEBUG
#include "DisplayParameters.inl"
#endif

#endif // __DISPLAY_DISPLAY_PARAMETERS_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
