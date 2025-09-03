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
//	CLASS:	FILTER
//
//	04-01-02:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PushLayer
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER::PushLayer(const COM_INSTANCE &	ciLayer)
{
	_vFilterLayers.PushTail(ciLayer);
}

//-----------------------------------------------------------------------------
//	Name:		PushScratch
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER::PushScratch(const COM_INSTANCE &	ciScratch)
{
	_vFilterScratches.PushTail(ciScratch);
}

//-----------------------------------------------------------------------------
//	Name:		ClearLayers
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER::ClearLayers()
{
	_vFilterLayers.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		ClearScratches
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER::ClearScratches()
{
	_vFilterScratches.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		GetLayerInst
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	FILTER::GetLayerInst(unsigned int i) const
{
	return (_vFilterLayers[i]);
}

//-----------------------------------------------------------------------------
//	Name:		GetScratchInst
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	FILTER::GetScratchInst(unsigned int i) const
{
	return (_vFilterScratches[i]);
}

//-----------------------------------------------------------------------------
//	Name:		GetLayer
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
FILTER_LAYER *	FILTER::GetLayer(unsigned int i) const
{
	return ((FILTER_LAYER *)(_vFilterLayers[i].GetCommunicator()));
}

//-----------------------------------------------------------------------------
//	Name:		GetScratch
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
FILTER_SCRATCH *	FILTER::GetScratch(unsigned int i) const
{
	return ((FILTER_SCRATCH *)(_vFilterScratches[i].GetCommunicator()));
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrLayers
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	FILTER::GetNbrLayers() const
{
	return (_vFilterLayers.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrScratches
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	FILTER::GetNbrScratches() const
{
	return (_vFilterScratches.GetSize());
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
