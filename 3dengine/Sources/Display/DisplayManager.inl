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
// FILE: DisplayManager.inl
//
//
//	01-02-15:	RMA - Created
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Name:		GetActiveCamera
//	Object:		
//	01-09-13:	ELE - Created
//-----------------------------------------------------------------------------
CAMERA *	DISPLAY_MANAGER::GetActiveCamera() const
{
	return	(_pActiveCamera);
}

//-----------------------------------------------------------------------------
//	Name:		SetActiveCamera
//	Object:		
//	01-09-13:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::SetActiveCamera(CAMERA *	pCamera)
{
	_pActiveCamera = pCamera;
}

//-----------------------------------------------------------------------------
//	Name:		GetStats
//	Object:		
//	01-10-25:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_FINAL_BUILD
STATISTICS &	DISPLAY_MANAGER::GetStats()
{
	return (_Statistics);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetCameraNode
//	Object:		
//	02-05-29:	ELE - Created
//-----------------------------------------------------------------------------
NODE *	DISPLAY_MANAGER::GetCameraNode()
{
	if	(_pActiveCamera)
	{
		return	(_pActiveCamera->GetNode());
	}

	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		GetDisplayParameters
//	Object:		
//	02-05-29:	ELE - Created
//-----------------------------------------------------------------------------
DISPLAY_PARAMETERS &	DISPLAY_MANAGER::GetDisplayParameters()
{
	return	(_DisplayParameters);
}

//-----------------------------------------------------------------------------
//	Name:		SetDisplayParameters
//	Object:		
//	02-05-29:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::SetDisplayParameters(const DISPLAY_PARAMETERS &	DisplayParameters)
{
	_DisplayParameters = DisplayParameters;
}

//-----------------------------------------------------------------------------
//	Name:		GetWindowSizeX
//	Object:		
//	03-10-09:	NCA - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::GetWindowSizeX() const
{
	QDT_NOT_IMPLEMENTED();
	return	(0);

//	return (RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeX());
}

//-----------------------------------------------------------------------------
//	Name:		GetWindowSizeY
//	Object:		
//	03-10-09:	NCA - Created
//-----------------------------------------------------------------------------
int	DISPLAY_MANAGER::GetWindowSizeY() const
{
	QDT_NOT_IMPLEMENTED();
	return	(0);

//	return (RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeY());
}

//-----------------------------------------------------------------------------
//	Name:		NeedTestNearFogPlane
//	Object:		
//	04-08-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::NeedTestNearFogPlane(bool	b)
{
	_bNeedTestNearFogPlane = b;
}

//-----------------------------------------------------------------------------
//	Name:		IsTestNearFogPlaneNeeded
//	Object:		
//	04-08-16:	ELE - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_MANAGER::IsTestNearFogPlaneNeeded() const
{
	return	(_bNeedTestNearFogPlane);
}

//-----------------------------------------------------------------------------
//	Name:		GetNearFogPlane
//	Object:		
//	04-08-13:	ELE - Created
//-----------------------------------------------------------------------------
const PLANE &	DISPLAY_MANAGER::GetNearFogPlane() const
{
	return	(_NearFogPlane);
}

//-----------------------------------------------------------------------------
//	Name:		EnableWideScreenMode
//	Object:		
//	04-11-30:	SBE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::EnableWideScreenMode(bool	bActive)
{
	QDT_NOT_IMPLEMENTED();

	float rMonitorAspect = 4.f / 3.f;

	if (bActive)
	{
		rMonitorAspect = 16.f / 9.f;		
	}

//	RENDER_MANAGER::Instance()->GetRenderContext()->SetMonitorAspect(rMonitorAspect);
}

//-----------------------------------------------------------------------------
//	Name:		IsWideScreenModeEnabled
//	Object:		
//	04-11-30:	SBE - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_MANAGER::IsWideScreenModeEnabled() const
{
	QDT_NOT_IMPLEMENTED();
	return	(false);

//	return (RENDER_MANAGER::Instance()->GetRenderContext()->GetMonitorAspect() == 16.f / 9.f);
}

//-----------------------------------------------------------------------------
//	Name:		ManageVideo
//	Object:		
//	05-03-08:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::ManageVideo(bool	b)
{
	_bManageVideo = b;
}

//-----------------------------------------------------------------------------
//	Name:		GetDisplayTriggerVolumes
//	Object:		
//	05-12-05:	RMA - Created
//-----------------------------------------------------------------------------
bool DISPLAY_MANAGER::GetDisplayTriggerVolumes() const
{
	return	(_DebugRenderFlags.IsSet(DRS_DISPLAY_TRIGGER_VOLUMES));
}

//-----------------------------------------------------------------------------
//	Name:		SetDisplayTriggerVolumes
//	Object:		
//	05-12-05:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::SetDisplayTriggerVolumes(bool b)
{
	_DebugRenderFlags.Set(DRS_DISPLAY_TRIGGER_VOLUMES, b);
}

//-----------------------------------------------------------------------------
//	Name:		GetDisplayPhysics
//	Object:		
//	05-12-15:	ELE - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_MANAGER::GetDisplayPhysics() const
{
	return	(_DebugRenderFlags.IsSet(DRS_DISPLAY_PHYSICS));
}

//-----------------------------------------------------------------------------
//	Name:		SetDisplayPhysics
//	Object:		
//	05-12-15:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::SetDisplayPhysics(bool	b)
{
	_DebugRenderFlags.Set(DRS_DISPLAY_PHYSICS, b);
	PHYSIC_CONTEXT::Instance()->EnableDebugDisplay(b);
}

//-----------------------------------------------------------------------------
//	Name:		GetHideSkinClusters
//	Object:		
//	05-12-19:	ELE - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_MANAGER::GetHideSkinClusters() const
{
	return	(_DebugRenderFlags.IsSet(DRS_HIDE_SKIN_CLUSTERS));
}

//-----------------------------------------------------------------------------
//	Name:		SetHideSkinClusters
//	Object:		
//	05-12-19:	ELE - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::SetHideSkinClusters(bool	b)
{
	_DebugRenderFlags.Set(DRS_HIDE_SKIN_CLUSTERS, b);
}

//-----------------------------------------------------------------------------
//	Name:		GetDisplaySkeleton
//	Object:		
//	06-02-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_MANAGER::GetDisplaySkeletons() const
{
	return	(_DebugRenderFlags.IsSet(DRS_DISPLAY_SKELETONS));
}

//-----------------------------------------------------------------------------
//	Name:		SetDisplaySkeleton
//	Object:		
//	06-02-08:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_MANAGER::SetDisplaySkeletons(bool	b)
{
	_DebugRenderFlags.Set(DRS_DISPLAY_SKELETONS, b);
}

//-----------------------------------------------------------------------------
//	Name:		GetDisplayPrimitives
//	Object:		
//	05-12-05:	RMA - Created
//-----------------------------------------------------------------------------
bool DISPLAY_MANAGER::GetDisplayPrimitives() const
{
	return	(_DebugRenderFlags.IsSet(DRS_DISPLAY_PRIMITIVES));
}

//-----------------------------------------------------------------------------
//	Name:		SetDisplayPrimitives
//	Object:		
//	05-12-05:	RMA - Created
//-----------------------------------------------------------------------------
void DISPLAY_MANAGER::SetDisplayPrimitives(bool b)
{
	_DebugRenderFlags.Set(DRS_DISPLAY_PRIMITIVES, b);
}

//-----------------------------------------------------------------------------
//	Name:		SupportHardwareShadowMap
//	Object:		
//	06-03-20:	SBE - Created
//-----------------------------------------------------------------------------
bool	DISPLAY_MANAGER::SupportHardwareShadowMap() const
{
	return (RENDER_CONTEXT::SupportHardwareShadowMap());
}






//=============================================================================
//	CODE ENDS HERE
//=============================================================================
