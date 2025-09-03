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
//	CLASS:	QDT_SERVICE_ABC
//
//	06-02-01:	GRI - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include "root.h"
#include "QDTServiceABC.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		QDT_SERVICE_ABC constructor
//	Object:		
//	06-02-01:	GRI - Created
//-----------------------------------------------------------------------------
QDT_SERVICE_ABC::QDT_SERVICE_ABC():
_pConnexion(NULL),
_bVisibleOnNetwork(false),
_strVersion("NOT DEFINED"),
_uReceptionMode(RM_MESSAGE_RECEIVE_DRIVEN),
_nLink(-1)
{
	// construction of the service.
}

//-----------------------------------------------------------------------------
//	Name:		QDT_SERVICE_ABC destructor
//	Object:		
//	06-02-01:	GRI - Created
//-----------------------------------------------------------------------------
QDT_SERVICE_ABC::~QDT_SERVICE_ABC()
{
		QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		SetConnexion
//	Object:		
//	06-02-01:	GRI - Created
//-----------------------------------------------------------------------------
void	QDT_SERVICE_ABC::SetConnexion(CONNEXION *	pConnexion)
{
	_pConnexion = pConnexion;
	//_pConnexion->SetOwnerType(_nType);
}

//-----------------------------------------------------------------------------
//	Name:		Run
//	Object:		
//	06-02-01:	GRI - Created
//-----------------------------------------------------------------------------
int	QDT_SERVICE_ABC::Run()
{
	QDT_NOT_IMPLEMENTED();
	return -1;
}

//-----------------------------------------------------------------------------
//	Name:		OnBrokenLink
//	Object:		
//	06-02-06:	GRI - Created
//-----------------------------------------------------------------------------
void	QDT_SERVICE_ABC::OnBrokenLink(Int32	nLink)
{
	QDT_Message("Link broken !");
}

//-----------------------------------------------------------------------------
//	Name:		OnRequestTimeOut
//	Object:		
//	06-02-06:	GRI - Created
//-----------------------------------------------------------------------------
//void	QDT_SERVICE_ABC::OnRequestTimeOut(OBJECT_REQUEST &	rRequest)
//{
//	QDT_Message("Time Out!");
//}

//-----------------------------------------------------------------------------
//	Name:		OnMessageReception
//	Object:		
//	06-02-06:	GRI - Created
//-----------------------------------------------------------------------------
void	QDT_SERVICE_ABC::OnMessageReception(NET_MESSAGE *	pMessage)
{
	QDT_Message("Reception of a message !");
}

//-----------------------------------------------------------------------------
//	Name:		OnDistantServiceAdded
//	Object:		
//	06-02-20:	GRI - Created
//-----------------------------------------------------------------------------
void	QDT_SERVICE_ABC::OnServiceAdded()
{
	//
}

////-----------------------------------------------------------------------------
////	Name:			OnTryToLink
////	Object:		if nLink is -1, check the error code. 
////	06-03-02:	GRI - Created
////-----------------------------------------------------------------------------
void	QDT_SERVICE_ABC::OnTryToLink(Int32 nTicket,
															 Int32 nLink) 
{
}

//-----------------------------------------------------------------------------
//	Name:		PushObjectRequest
//	Object:		
//	06-03-07:	GRI - Created
//-----------------------------------------------------------------------------
void	QDT_SERVICE_ABC::PushObjectRequest(SR_REQUEST *	pRequest)
{
	QDT_NOT_IMPLEMENTED();
}

	
//-----------------------------------------------------------------------------
//	Name:		PushObjectRequest
//	Object:		
//	06-03-07:	GRI - Created
//-----------------------------------------------------------------------------
void	QDT_SERVICE_ABC::PushRequest(SR_REQUEST* pRequest)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		DoMainThreadJob
//	Object:		
//	06-03-07:	GRI - Created
//-----------------------------------------------------------------------------
void 	QDT_SERVICE_ABC::DoMainThreadJob()
{
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
