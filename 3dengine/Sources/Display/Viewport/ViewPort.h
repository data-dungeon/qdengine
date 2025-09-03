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
// CLASS: VIEWPORT
//
//
//	01-02-12:	RMA - Created
//*****************************************************************************


//=============================================================================
// Name: VIEWPORT
//	01-02-12:	RMA - Created
//=============================================================================
#ifndef __DISPLAY_VIEWPORT_H__
#define __DISPLAY_VIEWPORT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(VIEWPORT_STATE)
DECL_3DENGINE(NODE)
DECL_3DENGINE(DISPLAY_PARAMETERS)
DECL_3DENGINE(SCENE_GRAPH)

#include INCL_KRENDERER(PostProcessing/ViewportState)
#include INCL_3DENGINE(Display/TransparentObject/TransparentObject)
#include INCL_KCOM(CommunicatorType/Communicator)
#include INCL_KMATH(Math/Color/Color)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Ray/Ray)

namespace QDT
{
	namespace M3D
	{
		class VIEWPORT : public COMMUNICATOR
		{
		public:

			struct RAY_PLANE_TESTED
			{
				PLANE	_Plane;
				VECTOR	_ImpactPoint;
			};

			// Constructor & destructor
    		VIEWPORT();
			VIEWPORT(const VIEWPORT & ViewPort);
			~VIEWPORT();

			// Operators
									VIEWPORT & operator = (const VIEWPORT & ViewPort);

			// Accessors
			float					GetLeft() const								{ return (_rLeft); }					
			float					GetTop() const								{ return (_rTop); }				
			float					GetWidth() const							{ return (_rWidth); }
			float					GetHeight() const							{ return (_rHeight); }		 
			float					GetLeftBorder() const						{ return (0.0f); }
			float					GetRightBorder() const						{ return (0.0f); }
			float					GetTopBorder() const						{ return (0.0f); }
			float					GetDownBorder() const						{ return (0.0f); }	
			const COLOR &			GetBackgroundColor() const					{ return (_BackgroundColor); }
			const COLOR &			GetBorderColor() const						{ return (_BorderColor); }
			const COM_INSTANCE &	GetActiveCameraInst() const					{ return (_CameraInst); }
			const COM_INSTANCE &	GetSGInst()									{ return (_SGInst); }

			VIEWPORT_STATE* const 	GetViewportState()							{ return (&_ViewportState); }

			Bool					GetClearColor() const						{ return (_Flags.IsSet(VF_CLEAR_COLOR)); }
			Bool					GetClearDepth() const						{ return (_Flags.IsSet(VF_CLEAR_DEPTH)); }
			Bool					IsVisible() const							{ return (_Flags.IsSet(VF_VISIBLE)); }
			Bool					IsWireframeEnabled() const					{ return (_Flags.IsSet(VF_ENABLE_WIREFRAME)); }
			Bool					IsPostProcessingEnabled() const				{ return (_Flags.IsSet(VF_ENABLE_POST_PROCESSING)); }
			Bool					IsDefaultLightingEnabled() const			{ return (_Flags.IsSet(VF_ENABLE_DEFAULT_LIGHTING)); }			
			Bool					IsLightingEnabled() const					{ return (_Flags.IsSet(VF_ENABLE_LIGHTING)); }
			Bool					AreShadowsEnabled() const					{ return (_Flags.IsSet(VF_ENABLE_SHADOWS)); }
			Bool					IsDepthOfFieldDebugEnabled() const			{ return (_Flags.IsSet(VF_ENABLE_DEPTH_OF_FIELD_DEBUG)); }
			Bool					IsLightDebugEnabled() const			        { return (_Flags.IsSet(VF_ENABLE_LIGHT_DEBUG)); }
			Bool					IsSHAmbientEnabled() const					{ return (_Flags.IsSet(VF_ENABLE_SH_AMBIENT)); }

			// Modifiors
			void	SetLeft(float rValue)								{ _rLeft = rValue; }
			void	SetTop(float rValue)								{ _rTop = rValue; }
			void	SetWidth(float rValue)								{ _rWidth = rValue; }		
			void	SetHeight(float rValue)								{ _rHeight = rValue; }
			void	SetLeftBorder(float rValue)							{  }
			void	SetRightBorder(float rValue)						{  }
			void	SetTopBorder(float rValue)							{  }
			void	SetDownBorder(float rValue)							{  }	
			void	SetActiveCameraInst(const COM_INSTANCE & Inst)		{ _CameraInst = Inst; }
			void	SetBackgroundColor(const COLOR & Color)				{ _BackgroundColor = Color; }
			void	SetBorderColor(const COLOR & Color)					{ _BorderColor = Color; }
			void	SetOpacity(Float32 rOpacity)						{ _ViewportState.SetOpacity(rOpacity); }

			void	SetClearColor(Bool bState)							{ _Flags.Set(VF_CLEAR_DEPTH, bState); }
			void	SetClearDepth(Bool bState)							{ _Flags.Set(VF_CLEAR_COLOR, bState); } 
			void	SetVisible(Bool bState) 							{ _Flags.Set(VF_VISIBLE, bState); }
			void	EnableWireframe(Bool bState) 						{ _Flags.Set(VF_ENABLE_WIREFRAME, bState); }
			void	EnablePostProcessing(Bool bState)					{ _Flags.Set(VF_ENABLE_POST_PROCESSING, bState); }
			void	EnableDefaultLighting(Bool bState)					{ _Flags.Set(VF_ENABLE_DEFAULT_LIGHTING, bState); }
			void	EnableLighting(Bool bState)							{ _Flags.Set(VF_ENABLE_LIGHTING, bState); }
			void	EnableShadows(Bool bState)							{ _Flags.Set(VF_ENABLE_SHADOWS, bState); }
			void	EnableDepthOfFieldDebug(Bool bState)				{ _Flags.Set(VF_ENABLE_DEPTH_OF_FIELD_DEBUG, bState); }
			void	EnableLightDebug(Bool bState)						{ _Flags.Set(VF_ENABLE_LIGHT_DEBUG, bState); }
			void	EnableSHAmbient(Bool bState)						{ _Flags.Set(VF_ENABLE_SH_AMBIENT, bState); }

			// Display the view
			void					Display();
	        RAY_PLANE_TESTED *		TestRayCastOnPlanes(int nScreenX, int nScreenY);
	        void					AddPlane(PLANE & Plane);
	        void					RemovePlane(PLANE & Plane);
	        void					RemoveAllPlanes();
	        PLANE *					GetPlane(const unsigned int nIndex);
	        const unsigned int		GetNbrPlanes() const;

			// Test if a screen coordinate is inside a view port or not
			Bool					TestScreenCoordinate(int nX, int nY) ;
			// Transform the screen coordinates in viewport coordinates, return false if the coordiantes are outside the viewport
			Bool					GetRelativeCoordinates(int nScreenX, int nScreenY, int & nViewportX, int & nViewportY) ;
			// Get a ray which has his origin from the viewport coordinates x & y
			RAY						GetRay(int nViewportX, int nViewportY) const;
			//	Clamp coordinates 
			Bool					ClampRelativesCoordinates(int nPosX, int nPosY, int&nResX, int& nResY);

			void					DisplayCameraStats() const;
			
			void					AddNode(const COM_INSTANCE &ciNode);
			void					RemoveNode(const COM_INSTANCE &ciNode);
			void					ClearNodes();
			void					DisplayNodes(DISPLAY_PARAMETERS & DisplayParameters);

			// Camera Handling
			float					GetCurrentAspectRatio() const;

			static void							Init(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR*		GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR*		CreateGenericCommunicatorDescriptor();

			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

		private:

			SCENE_GRAPH *	GetSceneGraph() const;
			void			PrepareViewport();
			void			PrepareCamera(VIEWPORT_STATE & ViewportState);
			Bool			TestValidity();
	
			enum VIEWPORT_FLAG
			{		
				VF_VISIBLE						= 1 << 0,
				VF_ENABLE_WIREFRAME				= 1 << 1,
				VF_CLEAR_DEPTH					= 1 << 2,
				VF_CLEAR_COLOR					= 1 << 3,
				VF_ENABLE_POST_PROCESSING		= 1 << 4,
				VF_ENABLE_DEFAULT_LIGHTING		= 1 << 5,
				VF_ENABLE_LIGHTING				= 1 << 6,
				VF_ENABLE_SHADOWS				= 1 << 7,
				VF_ENABLE_DEPTH_OF_FIELD_DEBUG	= 1 << 8,
				VF_ENABLE_LIGHT_DEBUG			= 1 << 9,
				VF_ENABLE_SH_AMBIENT			= 1 << 10,
			};

			// Copy method to avoid code duplication in copy constructor & operator =
			void						Copy(const VIEWPORT & ViewPort);

			VIEWPORT_STATE				_ViewportState; // Post-processing parameters

			// Flags
			QDT_FLAGS					_Flags;

			// size in %
			float						_rLeft;
			float						_rTop;
			float						_rWidth;
			float						_rHeight;
			
			COLOR						_BackgroundColor;
			COLOR						_BorderColor;

	        QDT_VECTOR<PLANE>			_vPlanes;
	        RAY_PLANE_TESTED			_RayPlaneTested;

			COM_INSTANCE				_CameraInst;
			COM_INSTANCE				_SGInst;

			// Nodes to display
			QDT_VECTOR<COM_INSTANCE>	_vNodes;

			// Sorted materials
			//QDT_VECTOR<MATERIAL_ABC *>	_vSortedMaterials;

			// Transparent Objects
			//QDT_VECTOR<TRANSPARENT_OBJECT>	_vTransparentObjects;
			//QDT_DLIST<TRANSPARENT_OBJECT>	_dlSortedTransparentObjects;
		};
	}
}

#endif // __DISPLAY_VIEWPORT_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================

