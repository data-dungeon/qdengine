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
//	The QDT_SERVICE_ABC class implements the interface for the network service
//	As the core have a service, the abstract class is here, at the bottom of
//	the core's hierarchy.
//
//	06-02-01:	GRI - Created
//*****************************************************************************

#ifndef __KCORE_QDT_SERVICE_ABC_H__
#define __KCORE_QDT_SERVICE_ABC_H__
#pragma once

#include INCL_KCORE(Tools/QDTString)
#include INCL_KCORE(Tools/QdtFlags)

class CONNEXION;
class OBJECT_REQUEST;
class NET_MESSAGE;

//#include "NetServiceInfo.h"
//#include "ObjectRequest.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class SR_REQUEST
{
};

class	QDT_SERVICE_ABC
{
public:
	QDT_SERVICE_ABC();
	virtual ~QDT_SERVICE_ABC() = 0;

	virtual int		Run();					//-1 in case of error
	
	virtual void	OnBrokenLink(Int32 nLink);
	virtual void	OnMessageReception(NET_MESSAGE* pMessage);
	virtual void	OnServiceAdded();
	virtual void	OnTryToLink(Int32 nTicket,Int32 nLink);								// user try to link in non-blocking mode.
	virtual void	PushObjectRequest(SR_REQUEST* pRequest);
	virtual void	PushRequest(SR_REQUEST* pRequest);
	virtual	void 	DoMainThreadJob();

	virtual const char* 	GetServiceName() = 0;
	virtual Int32					GetServiceID() = 0;
	virtual QDT_STRING&		GetServiceVersion() = 0;

	void				SetConnexion(CONNEXION* pConnexion);
	CONNEXION*	GetConnexion() 	{ 		return _pConnexion; 	}
	bool				GetBoolVisibleOnNetwork() { return _bVisibleOnNetwork; }
	void 				SetBoolVisibleOnNetwork( bool bValue) { _bVisibleOnNetwork = bValue; }
	UInt8				GetReceptionMode() { return  _uReceptionMode;}

	bool				IsRunning() { return _FlagsServiceStatus.IsSet(QS_RUNNING); } 
	bool				IsLinked()	{ return _nLink == -1 ? false : true; }

public: // enums
	enum	RECEIVE_MODE
	{
		RM_DEFAULT = 0,
		RM_MESSAGE_RECEIVE_DRIVEN = 0,		// OnReceive est appellé pour chaque message
		RM_MESSAGE_POLLING								// OnQueueMessageNotEmpty est appellé,le service polle.
	};

	enum QDT_SERVICE_STATUS
	{
		QS_RUNNING	= 0x1,
		QS_ON_ERROR = 0x2
	};

protected:
	bool				_bVisibleOnNetwork;
	QDT_STRING	_strVersion;     
	CONNEXION*	_pConnexion;
	UInt8				_uReceptionMode;					// message driven, polling ..								
	Int32				_nLink;
	QDT_FLAGS		_FlagsServiceStatus;
};

#endif // 
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
