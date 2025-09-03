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
// CLASS: VIEW_PORT
//
//
//	01-02-12:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************
//*****************************************************************************
//
// CLASS DESCRIPTION (VIEW_PORT)
//
//	The VIEW_PORT class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/Viewport/Viewport)
#include	INCL_3DENGINE(Display/DisplayParameters/DisplayParameters)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Entities/Camera/Camera)
#include	INCL_KMATH(Math/Geometry/Intersection/Intersection)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(Renderer/IRenderTarget)
#include	INCL_KRENDERER(PostProcessing/Photographic_Pipeline)
#include	INCL_KCORE(Profiler/Profiler)
#include	INCL_KCORE(Time/Time_ABC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Viewport constructor
//	Object:		Default constructor
//	01-02-12:	RMA - Created
//-----------------------------------------------------------------------------
VIEWPORT::VIEWPORT()
:
COMMUNICATOR		(GetGenericCommunicatorDescriptor()),
_Flags				(0),
_rLeft				(0.0f),
_rTop				(0.0f),
_rWidth				(100.0f),
_rHeight			(100.0f),
_CameraInst			(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_BackgroundColor	(255, 0, 0, 0),
_BorderColor		(255, 0, 0, 0)
{
	SetVisible(true);
	SetClearColor(true);
	SetClearDepth(true);

	EnablePostProcessing(false);
	EnableSHAmbient(true);
	EnableLighting(true);
	EnableShadows(true);
}

//-----------------------------------------------------------------------------
//	Name:		Viewport constructor
//	Object:		Copy constructor
//	01-02-12:	RMA - Created
//-----------------------------------------------------------------------------
VIEWPORT::VIEWPORT(const VIEWPORT & Viewport)
:
COMMUNICATOR(Viewport)
{
	Copy(Viewport);
}

//-----------------------------------------------------------------------------
//	Name:		Viewport destructor
//	Object:		Destructor
//	01-02-12:	RMA - Created
//-----------------------------------------------------------------------------
VIEWPORT::~VIEWPORT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Viewport::operator 
//	Object:		Operator =
//	01-02-12:	RMA - Created
//-----------------------------------------------------------------------------
VIEWPORT & VIEWPORT::operator = (const VIEWPORT & Viewport)
{
	if (this != &Viewport)
	{
		Copy(Viewport);
	}

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		Viewport::Copy
//	Object:		Copy method to avoid code duplication in operator = and copy
//				constructor
//	01-02-12:	RMA - Created
//-----------------------------------------------------------------------------
void VIEWPORT::Copy(const VIEWPORT & Viewport)
{
	_rLeft						= Viewport._rLeft ;
	_rTop						= Viewport._rTop ;
	_rWidth						= Viewport._rWidth ;
	_rHeight					= Viewport._rHeight ;
	_CameraInst					= Viewport._CameraInst ;
	_BorderColor				= Viewport._BorderColor;
	_BackgroundColor			= Viewport._BackgroundColor ;
	_Flags						= Viewport._Flags ;
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentAspectRatio
//	Object:		
//	04-09-24:	JJA - Created
//  06-03-06:	RMA - Hacked to manage render target with aspect ratio of one.
//				Very useful for generating cube map.
//-----------------------------------------------------------------------------
Float32	VIEWPORT::GetCurrentAspectRatio() const
{
	int		w, h;
	Float32	rAspectRatio = 0.0f;
		
	// Scandalous hack to manage render target with aspect ratio of one
	I_RENDER_TARGET * pRT = RENDER_CONTEXT::GetCurrentRenderTarget();
	if (pRT->GetWidth() == pRT->GetHeight())
	{
		return (1.0f);
	}

	w = RENDER_CONTEXT::GetScreenWidth();
	h = RENDER_CONTEXT::GetScreenHeight();
	rAspectRatio = (_rWidth / _rHeight) * (static_cast<Float32>(w) / static_cast<Float32>(h));

	return	( rAspectRatio );
}

//-----------------------------------------------------------------------------
//	Name:		GetSceneGraph
//	Object:		
//	06-02-20:	RMA - Created
//-----------------------------------------------------------------------------
SCENE_GRAPH *	VIEWPORT::GetSceneGraph() const
{
	if (_CameraInst.HasHandle() == false)
	{
		return (NULL);
	}

	CAMERA	* pCamera = static_cast<CAMERA *>(_CameraInst.GetCommunicator());
	QDT_ASSERT(pCamera);

	NODE * pCameraNode = pCamera->GetNode();

	SCENE_GRAPH	*pSG = pCamera->GetSceneGraph();
	
	if (pSG == NULL)
	{
		return (NULL);
	}

	return (pSG);
}

//-----------------------------------------------------------------------------
//	Name:		GetActiveCameraNode
//	Object:		return the current scene graph 
//				return NULL if the camera is not correctly set
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::PrepareCamera(VIEWPORT_STATE & ViewportState)
{
	QDT_ASSERT(_CameraInst.HasHandle());

	CAMERA	* pCamera = static_cast<CAMERA *>(_CameraInst.GetCommunicator());
	QDT_ASSERT(pCamera);

	NODE * pCameraNode = pCamera->GetNode();
	QDT_ASSERT(pCameraNode);

	SCENE_GRAPH * pSG = GetSceneGraph();

	Float32 fBorderScaleX = 1.0f, fBorderScaleY = 1.0f;
	if (IsPostProcessingEnabled())
	{
		ViewportState.GetScaleBorder(fBorderScaleX, fBorderScaleY);
	}

	// Update camera
	Float32 fAspectRatio = GetCurrentAspectRatio();
	DISPLAY_MANAGER::Instance()->SetActiveCamera(pCamera);
	pCamera->UpdateMatrices(fAspectRatio, fBorderScaleX, fBorderScaleY);

	DISPLAY_MANAGER::Instance()->SetViewMatrix(pCamera->GetViewMatrix());
	DISPLAY_MANAGER::Instance()->SetProjMatrix(pCamera->GetProjectionMatrix());
	
	FRUSTRUM Frustum;
	Frustum.CreateFromProjMatrix(
		pCamera->GetProjectionMatrix(), 
		pCamera->GetNearClippingDistance(),
		pCamera->GetFarClippingDistance());
	Frustum.Transform(pCameraNode->GetGlobalMatrix());

	DISPLAY_PARAMETERS	DisplayParameters;
	DisplayParameters.SetFrustrum(Frustum);
	DisplayParameters.SetCurrentSG(pSG);
	DISPLAY_MANAGER::Instance()->SetDisplayParameters(DisplayParameters);

	// Update viewport state with the camera parameters!
	pCamera->UpdateViewportState(ViewportState);

	PHOTOGRAPHIC_PIPELINE::EnableDebugDOF(IsDepthOfFieldDebugEnabled() && pCamera->GetEnableDepthOfField());
	PHOTOGRAPHIC_PIPELINE::EnableDebugLight(IsLightDebugEnabled());
}

//-----------------------------------------------------------------------------
//	Name:		PrepareViewport
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::PrepareViewport()
{
	/*
	UInt32 uiClearFlags = 0;
	
	if (GetClearColor())
	{
		uiClearFlags |= CF_CLEAR_COLOR;
	}
	
	if (GetClearDepth())
	{
		uiClearFlags |= CF_CLEAR_ZBUFFER;
	}*/

	// Init render flags
	if	(IsWireframeEnabled())
	{
		RENDER_CONTEXT::SetFillMode(FM_LINE);
	}
	else
	{
		RENDER_CONTEXT::SetFillMode(FM_SOLID);
	}

//	RENDER_CONTEXT::Clear(uiClearFlags, _BackgroundColor.GetRed(), _BackgroundColor.GetGreen(), _BackgroundColor.GetBlue(), 255, 1.0f);
}

//-----------------------------------------------------------------------------
//	Name:		TestValidity
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
Bool	VIEWPORT::TestValidity()
{
	// Too small!
	if	(_rWidth <= 1.0f || _rHeight <= 1.0f) return (false); 

	// To do: check if the viewport is outside the range of the window
	
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Viewport::Display 
//	Object:		
//	06-02-16:	RMA - Completly redesigned
//-----------------------------------------------------------------------------
void	VIEWPORT::Display()
{
	if (TestValidity() == false)
	{
		return;
	}

	SCENE_GRAPH * pSceneGraph = GetSceneGraph();

	if (pSceneGraph)
	{	
		DISPLAY_MANAGER::Instance()->SetCurrentViewportInst(COM_INSTANCE(this));

		_ViewportState.SetLocation(_rLeft * 0.01f, _rTop * 0.01f, _rWidth * 0.01f, _rHeight * 0.01f);
		
		if (IsPostProcessingEnabled())
		{
			PHOTOGRAPHIC_PIPELINE::InitViewPort(&_ViewportState, _BackgroundColor.GetRed(), _BackgroundColor.GetGreen(), _BackgroundColor.GetBlue());
		}
		else
		{
			I_RENDER_TARGET * pRT = RENDER_CONTEXT::GetCurrentRenderTarget();

			Float32 rDisplaySizeX = (Float32)(pRT->GetWidth());
			Float32 rDisplaySizeY = (Float32)(pRT->GetHeight());

			Float32 rLeft   = _rLeft * 0.01f  *  rDisplaySizeX;
			Float32 rTop    = _rTop * 0.01f *    rDisplaySizeY;
			Float32 rWidth  = _rWidth * 0.01f *  rDisplaySizeX;
			Float32 rHeight = _rHeight * 0.01f * rDisplaySizeY;

			UInt32 nLeft	= static_cast<UInt32>(rLeft);
			UInt32 nTop		= static_cast<UInt32>(rTop);
			UInt32 nWidth	= static_cast<UInt32>(rWidth);
			UInt32 nHeight	= static_cast<UInt32>(rHeight);

			RENDER_CONTEXT::SetViewport(nLeft, nTop, nWidth, nHeight);
			RENDER_CONTEXT::Clear(CF_CLEAR_COLOR | CF_CLEAR_ZBUFFER, _BackgroundColor.GetRed(), _BackgroundColor.GetGreen(), _BackgroundColor.GetBlue(), 255, 1.0f);
		}
		
		BEGIN_PROFILE(PROFILE_PREPARE_VIEWPORT);
		PrepareCamera(_ViewportState);
		PrepareViewport();
		END_PROFILE(PROFILE_PREPARE_VIEWPORT);

		pSceneGraph->Display();

		// Display physics debug
		if	(DISPLAY_MANAGER::Instance()->GetDisplayPhysics())
		{
			pSceneGraph->DisplayPhysics(DISPLAY_MANAGER::Instance()->GetViewMatrix(), DISPLAY_MANAGER::Instance()->GetProjMatrix());
		}

		if (IsPostProcessingEnabled())
		{
			BEGIN_PROFILE(PROFILE_PHOTO_PIPELINE);
			PHOTOGRAPHIC_PIPELINE::DoCompositing();
			END_PROFILE(PROFILE_PHOTO_PIPELINE);
		}

		DISPLAY_MANAGER::Instance()->SetCurrentViewportInst(COM_INSTANCE());
	}
}


//-----------------------------------------------------------------------------
//	Name:		AddNode
//	Object:		
//	03-09-10:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::AddNode(const COM_INSTANCE	& ciNode)
{
	_vNodes.PushTail(ciNode);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveNode
//	Object:		
//	03-09-24:	FDE - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::RemoveNode(const COM_INSTANCE &	ciNode)
{
	UInt32	nIndex;

	for (nIndex = 0; nIndex < _vNodes.GetSize(); ++nIndex)
	{
		if (_vNodes[nIndex] == ciNode)
		{
			QDT_VECTOR<COM_INSTANCE>::ITERATOR	it;

			it = _vNodes.GetHead();
			it += nIndex;
			_vNodes.Remove(it);
			return ;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		ClearNodes
//	Object:		
//	03-09-10:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::ClearNodes()
{
	_vNodes.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		DisplayNodes
//	Object:		
//	03-09-10:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::DisplayNodes(DISPLAY_PARAMETERS & DisplayParameters)
{
/*	UInt32 nNode;
	for (nNode = 0 ; nNode < _vNodes.GetSize() ; ++nNode)
	{
		NODE * pNode = static_cast<NODE *>(_vNodes[nNode].GetCommunicator());
		pNode->Display(DisplayParameters);
	}*/
}

//-----------------------------------------------------------------------------
//	Name: VIEWPORT::TestScreenCoordinate
//	Object: Test if screen coordinates (x, y) are inside the viewport or not.
//	01-02-15:	RMA - Created
//-----------------------------------------------------------------------------
bool VIEWPORT::TestScreenCoordinate(int nX, int nY)
{
	QDT_NOT_IMPLEMENTED();
	return	(false);
/*
	int nTop, nBottom, nLeft, nRight, nWindowPosX, nWindowPosY, nWindowHeight, nWindowWidth;

	RENDER_MANAGER *pRM = RENDER_MANAGER::Instance();

	if (DISPLAY_MANAGER::Instance()->GetFullScreen())
	{
		nWindowPosX = nWindowPosY = 0;

		// Get the screen resolution

		RENDER_MODE RenderMode;

		pRM->GetRenderContext()->GetAdapterMode(pRM->GetCurrentAdapter(), RENDER_MANAGER::Instance()->GetCurrentAdapterMode(), RenderMode);
		nWindowWidth	= RenderMode.Width;
		nWindowHeight	= RenderMode.Height;	
	}
	else 
	{
		// Windowed mode

		// Get the window position & size
		nWindowPosX		= pRM->GetRenderContext()->GetWindowPosX();
		nWindowPosY		= pRM->GetRenderContext()->GetWindowPosY();
		nWindowWidth	= pRM->GetRenderContext()->GetWindowSizeX();
		nWindowHeight	= pRM->GetRenderContext()->GetWindowSizeY();
	}

	// Get the top, bottom, left & right of the viewport exprimed in pixels
	nTop	= static_cast<int>((static_cast<Float32>(nWindowHeight)) * (		_rTop		 / 100.0f)) + nWindowPosY ;
	nBottom	= static_cast<int>((static_cast<Float32>(nWindowHeight)) * ((_rTop + _rHeight) / 100.0f)) + nWindowPosY ;
	nLeft	= static_cast<int>((static_cast<Float32>(nWindowWidth))  * (		_rLeft		 / 100.0f)) + nWindowPosX ;
	nRight	= static_cast<int>((static_cast<Float32>(nWindowWidth))  * ((_rLeft + _rWidth) / 100.0f)) + nWindowPosX ;

	// Check the screen coordinates
	return((nLeft <= nX) && (nX <= nRight) && (nTop <= nY) && (nY <= nBottom));*/
}

//-----------------------------------------------------------------------------
//	Name: WIEWPORT::GetRelativeCoordinates
//	Object: Transform the screen coordinates in viewport coordinates, such a
//			way the coordinates(0, 0) are in the middle of the viewport.
//			It returns false if the coordinates are outside the viewport.
//	01-02-15:	RMA - Created
//-----------------------------------------------------------------------------
bool VIEWPORT::GetRelativeCoordinates(int	nScreenX,	// Coordinate X in the screen repere
									  int	nScreenY,	// Coordinate Y in the screen repere
									  int & nViewportX, // Coordinate X in the viewport repere
									  int & nViewportY)	// Coordinate Y in the viewport repere
{
	int nViewportPosX ;		// Position X of the viewport
	int nViewportPosY ;		// Position Y of the viewport
	int nViewportWidth ;	// Width of the viewport
	int nViewportHeight ;	// Height of the viewport

	int Windowed = !RENDER_CONTEXT::IsFullScreen();

#ifdef PLATFORM_PC
	if (Windowed)	// Windowed mode
	{
		int	nWindowPosX		=	RENDER_CONTEXT::GetWindowPosX();
		int	nWindowPosY		=	RENDER_CONTEXT::GetWindowPosY();
		int	nWindowWidth	=	RENDER_CONTEXT::GetScreenWidth();
		int	nWindowHeight	=	RENDER_CONTEXT::GetScreenHeight();

		nViewportPosX	= nWindowPosX + static_cast<int>((static_cast<Float32>(nWindowWidth )) * (_rLeft / 100.0f)) ;
		nViewportPosY	= nWindowPosY + static_cast<int>((static_cast<Float32>(nWindowHeight)) * (_rTop  / 100.0f)) ;
		nViewportWidth	= static_cast<int>((static_cast<Float32>(nWindowWidth ))  * (_rWidth  / 100.0f)) ;
		nViewportHeight	= static_cast<int>((static_cast<Float32>(nWindowHeight))  * (_rHeight / 100.0f)) ;	
	}
	else	// Fullscreen Mode
#endif
	{
		int			 nScreenWidth ;		// Width of the screen
		int			 nScreenHeight ;	// Height of the screen

		nScreenWidth = RENDER_CONTEXT::GetScreenWidth();
		nScreenHeight = RENDER_CONTEXT::GetScreenHeight();

		nViewportPosX	= static_cast<int>((static_cast<Float32>(nScreenWidth ))  * (_rLeft / 100.0f)) ;
		nViewportPosY	= static_cast<int>((static_cast<Float32>(nScreenHeight))  * (_rTop  / 100.0f)) ;
		nViewportWidth	= static_cast<int>((static_cast<Float32>(nScreenWidth ))  * (_rWidth  / 100.0f)) ;
		nViewportHeight	= static_cast<int>((static_cast<Float32>(nScreenHeight))  * (_rHeight / 100.0f)) ;
	}

	nViewportX =   nScreenX - nViewportPosX - (nViewportWidth  / 2) ;
	nViewportY = -(nScreenY - nViewportPosY - (nViewportHeight / 2)) ;

	// We test if the cursor is inside the view
	if	((nScreenX > nViewportPosX)						&& 
		 (nScreenX < (nViewportPosX + nViewportWidth))	&& 
		 (nScreenY > nViewportPosY)						&&
		 (nScreenY < (nViewportPosY + nViewportHeight)))
	{
		return (true) ;
	}
	else
	{
		return (false) ;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ClampRelativesCoordinates
//	Object:		Clamp 2D coordinates ( in viewport relative ) with viewport dimension
//	01-08-14:	cvi - Created
//-----------------------------------------------------------------------------
bool	VIEWPORT::ClampRelativesCoordinates(int		nPosX,
											int		nPosY,
											int &	nResX,
											int &	nResY)
{
	QDT_NOT_IMPLEMENTED();
	return	(false);
/*
	int				nMaxWidth;
	int				nMaxHeight;
	bool			bRes;
	RENDER_MODE		RenderMode;
	int				nScreenWidth;
	int				nScreenHeight;

	RENDER_MANAGER::Instance()->GetRenderContext()->GetAdapterMode(RENDER_MANAGER::Instance()->GetCurrentAdapter(), RENDER_MANAGER::Instance()->GetCurrentAdapterMode(), RenderMode);
	
	//	First init width and height
	nScreenWidth = RenderMode.Width;
	nScreenHeight = RenderMode.Height;

	//	Get max values for width and height
	nMaxWidth	= (int) ( nScreenWidth * .5f );
	nMaxHeight	= (int) ( nScreenHeight * .5f );

	//	Default: coordinates not clipped
	bRes = false;

	//	Clip coordinates
	if ( nPosX >= nMaxWidth )
	{
		nPosX = nMaxWidth - 1;
		bRes = true;
	}
	else if ( nPosX <= (-nMaxWidth) )
	{
		nPosX = -nMaxWidth + 1;
		bRes = true;
	}

	if ( nPosY >= nMaxHeight )
	{
		nPosY = nMaxHeight - 1;
		bRes = true;
	}
	else if ( nPosY <= (-nMaxHeight) )
	{
		nPosY = -nMaxHeight + 1;
		bRes = true;
	}

	//	Get new coordinates for result
	nResX = nPosX;
	nResY = nPosY;
	
	return ( bRes );*/	
}


//-----------------------------------------------------------------------------
//	Name: VIEW_PORT::GetRay
//	Object: Get a ray which has his origin from the screen coordinates x & y.
//			This is used for picking 3D.
//			The coordinates must be exprimed in the viewport repere, so (0, 0)
//			is the center of the viewport.
//			The ray is in the global repere of the scenegraph in which is the
//			viewport camera.
//	01-02-15:	RMA - Created
//-----------------------------------------------------------------------------
RAY VIEWPORT::GetRay(int nViewportX, int nViewportY) const
{
	NODE   *pACNode	= NULL;
	CAMERA *pAC		= NULL;		// Camera of the viewport
	RAY		Ray;				// Ray that we are computing

	// The 3D screen is the screen exprimed in the 3D world
	// It depends of the camera parameters
	
	Float32		 r3DScreenWidth ;	// Width of the 3D screen
	Float32		 r3DScreenHeight ;	// Height of the 3D screen

	Float32		 rTan  ;	// Tangent

	VECTOR		 RayDir ;						// Direction of the ray
	VECTOR		 RayDirTrans ;					// Direction of the ray in the global repere
	VECTOR		 RayOrigin(VECTOR::ZERO) ;		// Origin of the ray

	int nViewportWidth;		// Viewport width in pixels
	int nViewportHeight;	// Viewport height in pixels

	QDT_ASSERT(_CameraInst.HasHandle());

	// Get the camera
	pAC = static_cast<CAMERA *>(_CameraInst.GetCommunicator());
	QDT_ASSERT(pAC);
	pACNode = pAC->GetNode();

	// Compute tangent
	rTan = MATH::Tan((pAC->GetHorizontalFieldOfView()) / 2.0f) ;

	int nScreenWidth = RENDER_CONTEXT::GetScreenWidth();
	int nScreenHeight = RENDER_CONTEXT::GetScreenHeight();

	nViewportWidth	= static_cast<int>((static_cast<Float32>(nScreenWidth )) * (_rWidth  / 100.0f)) ;
	nViewportHeight	= static_cast<int>((static_cast<Float32>(nScreenHeight)) * (_rHeight / 100.0f)) ;

	r3DScreenHeight	= rTan * 2.0f * pAC->GetNearClippingDistance();
//#ifdef PLATFORM_PC
	r3DScreenWidth = r3DScreenHeight * pAC->GetAspectRatio();
//#else
//	r3DScreenHeight = r3DScreenWidth / RENDER_MANAGER::Instance()->GetRenderContext()->GetMonitorAspect();
//#endif

	// Calcul of the ray direction
	RayDir.SetX((r3DScreenWidth  * static_cast<Float32>(nViewportX)) / static_cast<Float32>(nViewportWidth));
	RayDir.SetY((r3DScreenHeight * static_cast<Float32>(nViewportY)) / static_cast<Float32>(nViewportHeight));
	RayDir.SetZ(-pAC->GetNearClippingDistance());

	// Transform the direction of the ray in the global repere
	pACNode->GetGlobalMatrix().TransformDirection(RayDir, RayDirTrans);

	// Normalize the direction
	RayDirTrans.Normalize();

	// Transform the ray origin
	pACNode->GetGlobalMatrix().TransformPosition(RayOrigin);
	
	// Finally, we get the ray in the global repere
	Ray.SetOrigin(RayOrigin);
	Ray.SetDirection(RayDirTrans);

	return (Ray) ;
}

//-----------------------------------------------------------------------------
//	Name:		TestRayCastOnPlanes
//	Object:		
//	03-06-02:	FDE - Created
//-----------------------------------------------------------------------------
VIEWPORT::RAY_PLANE_TESTED *	VIEWPORT::TestRayCastOnPlanes(const int	nScreenX,
															  const int	nScreenY)
{
	RAY				Ray;
	int				nViewportX;
	int				nViewportY;
	bool			bFind = false;

	if (GetRelativeCoordinates(nScreenX, nScreenY, nViewportX, nViewportY))
	{
		int		i, nNbrElms;
		Float32	rDistance		= 0.0f;
		Float32	rCurDistance	= QDT_FLOAT32_MAX;

		// Point inside viewport
		Ray = GetRay(nViewportX, nViewportY);

		nNbrElms = _vPlanes.GetSize();
		for	(i = 0; i < nNbrElms; ++i)
		{
			if (INTERSECTION::IntersectPlaneWithRay(_vPlanes[i], Ray, rDistance))
			{
				if (rDistance < rCurDistance)
				{
					INTERSECTION::IntersectPlaneWithRay(_vPlanes[i], Ray, _RayPlaneTested._ImpactPoint);
					_RayPlaneTested._Plane	= _vPlanes[i];
					bFind					= true;
				}
			}
		}
	}

	if (bFind)
	{
		return &_RayPlaneTested;
	}
	else
	{
		return NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddPlane
//	Object:		
//	03-06-02:	FDE - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::AddPlane(PLANE &	Plane)
{
	_vPlanes.PushTail(Plane);
}

//-----------------------------------------------------------------------------
//	Name:		RemovePlane
//	Object:		
//	03-06-02:	FDE - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::RemovePlane(PLANE &	Plane)
{
	int i, nNbrElms;
	
	nNbrElms = _vPlanes.GetSize();
	for	(i = 0; i < nNbrElms; ++i)
	{
		if	(_vPlanes[i] == Plane)
		{
			QDT_VECTOR<PLANE>::ITERATOR it;
			it = _vPlanes.GetHead();
			it += i;
			_vPlanes.Remove(it);
		}
	}
}

//----------------------------------------------------------------------------
//	Name:		RemoveAllPlanes
//	Object:		
//	03-06-02:	FDE - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::RemoveAllPlanes()
{
	_vPlanes.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		GetPlane
//	Object:		
//	03-06-02:	FDE - Created
//-----------------------------------------------------------------------------
PLANE *	VIEWPORT::GetPlane(const UInt32	nIndex)
{
	QDT_ASSERT((nIndex >= 0) && (nIndex <= _vPlanes.GetSize()));
	return &_vPlanes[nIndex];
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPlanes
//	Object:		
//	03-06-02:	FDE - Created
//-----------------------------------------------------------------------------
const UInt32	VIEWPORT::GetNbrPlanes() const
{
	return _vPlanes.GetSize();
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-08:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::Init(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-08:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	VIEWPORT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_VIEWPORT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	VIEWPORT::CreateGenericCommunicatorDescriptor()
{
	return ( QDT_NEW COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("VIEWPORT"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_VIEWPORT, 
											QDT_COMMENT("15B2B552-1E0F-7D04-44A0-3059D58A1CE2"),
											sizeof(VIEWPORT), Init,
											QDT_COMMENT("The viewport class")));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-08-08:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::EM_Construct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	VIEWPORT *	pObject;
	pObject = static_cast<VIEWPORT *>(pO);
	QDT_NEW_PLACED(pObject, VIEWPORT)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-08-08:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT::EM_Destruct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	VIEWPORT *	pObject;
	pObject = static_cast<VIEWPORT*> (pO);
	pObject->VIEWPORT::~VIEWPORT();
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
