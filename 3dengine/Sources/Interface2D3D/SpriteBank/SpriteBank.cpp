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
//	CLASS:	SPRITE_BANK
//
//	03-07-23:	FDE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Interface2D3D/SpriteBank/SpriteBank)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KCOM(CommunicatorType/CommunicatorContainerDescriptor)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

void*								SPRITE_BANK::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		SPRITE_BANK constructor
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
SPRITE_BANK::SPRITE_BANK(COM_HANDLE *	pHandle)
:
COMMUNICATOR_CONTAINER	(GetGenericCommunicatorDescriptor(), pHandle)
{
}

//-----------------------------------------------------------------------------
//	Name:		SPRITE_BANK destructor
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
SPRITE_BANK::~SPRITE_BANK()
{
	_vSprite.ClearAll();
}

//-----------------------------------------------------------------------------
//	Name:		Duplicate
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
void	SPRITE_BANK::Duplicate(SPRITE_BANK *	pElm)
{
	unsigned int			i;
	const SPRITE_BANK_ELM	*pCurElm;

	ClearCommunicators();
	for (i = 0; i < pElm->GetNbrElms(); ++i)
	{
		pCurElm	= pElm->GetIthElm(i);
		AddSprite(pCurElm->_nID, pCurElm->_MaterialCI);
		PushComHandle(pCurElm->_MaterialCI.GetHandle());
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddSprite
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
void	SPRITE_BANK::AddSprite(const int			nID,
							   const COM_INSTANCE &	MaterialCI)
{
	SPRITE_BANK_ELM	*pElm = new SPRITE_BANK_ELM;

	pElm->_nID			= nID;
	pElm->_MaterialCI	= MaterialCI;

	PushComHandle(MaterialCI.GetHandle());

	_vSprite.PushTail(pElm);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveSprite
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
void	SPRITE_BANK::RemoveSprite(const int	nID)
{
	unsigned int	i, nNbrElms;

	nNbrElms	= _vSprite.GetSize();
	for	(i = 0; i < nNbrElms; ++i)
	{
		if	(_vSprite[i]->_nID == nID)
		{
			RemoveComInstance(_vSprite[i]->_MaterialCI);

			QDT_VECTOR<SPRITE_BANK_ELM *>::ITERATOR it;
			it = _vSprite.GetHead();
			it += i;
			_vSprite.Remove(it);
			return ;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveSprite
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
void	SPRITE_BANK::RemoveSprite(const COM_INSTANCE &	MaterialCI)
{
	unsigned int	i, nNbrElms;

	nNbrElms	= _vSprite.GetSize();
	for	(i = 0; i < nNbrElms; ++i)
	{
		if	(_vSprite[i]->_MaterialCI == MaterialCI)
		{
			RemoveComInstance(MaterialCI);

			QDT_VECTOR<SPRITE_BANK_ELM *>::ITERATOR it;
			it = _vSprite.GetHead();
			it += i;
			_vSprite.Remove(it);
			return ;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetID
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
const int	SPRITE_BANK::GetID(COM_INSTANCE &	MaterialCI) const
{
	unsigned int	i;

	for (i = 0; i < _vSprite.GetSize(); ++i)
	{
		if (_vSprite[i]->_MaterialCI == MaterialCI)
		{
			return _vSprite[i]->_nID;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterial
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE 	SPRITE_BANK::GetMaterial(const int	nID) const
{
	unsigned int	i;

	for (i = 0; i < _vSprite.GetSize(); ++i)
	{
		if (_vSprite[i]->_nID == nID)
		{
			return _vSprite[i]->_MaterialCI;
		}
	}
	return COM_INSTANCE();
}

//-----------------------------------------------------------------------------
//	Name:		FindElm
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
const bool	SPRITE_BANK::FindElm(const int	nID) const
{
	unsigned int	i;

	for (i = 0; i < _vSprite.GetSize(); ++i)
	{
		if (_vSprite[i]->_nID == nID)
		{
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		FindElm
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
const bool	SPRITE_BANK::FindElm(const COM_INSTANCE &	MaterialCI) const
{
	unsigned int	i;

	for (i = 0; i < _vSprite.GetSize(); ++i)
	{
		if (_vSprite[i]->_MaterialCI == MaterialCI)
		{
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrElms
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
const unsigned int	SPRITE_BANK::GetNbrElms() const
{
	return _vSprite.GetSize();
}

//-----------------------------------------------------------------------------
//	Name:		GetIthElm
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
const SPRITE_BANK::SPRITE_BANK_ELM *	SPRITE_BANK::GetIthElm(const unsigned int	nIndex) const
{
	QDT_ASSERT((nIndex >= 0) && (nIndex < _vSprite.GetSize()));
	return _vSprite[nIndex];
}

//-----------------------------------------------------------------------------
//	Name:		SetVarName
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
void	SPRITE_BANK::SetVarName(const QDT_STRING &	sVarName)
{
	_sVarName = sVarName;
}

//-----------------------------------------------------------------------------
//	Name:		SetSourceFileName
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
void	SPRITE_BANK::SetSourceFileName(const QDT_STRING &	sSourceFileName)
{
	_sSourceFileName = sSourceFileName;
}

//-----------------------------------------------------------------------------
//	Name:		HowManyStored
//	Object:		
//	03-07-30:	FDE - Created
//-----------------------------------------------------------------------------
const unsigned int	SPRITE_BANK::HowManyStored(const QDT_STRING &	Name)
{
	/*unsigned int	i;
	unsigned int	nNbr	= 0;
	COLOR_PER_VERTEX_MATERIAL *	pMat;

	for (i = 0; i < _vSprite.GetSize(); ++i)
	{
		pMat		= static_cast<COLOR_PER_VERTEX_MATERIAL *>(_vSprite[i]->_MaterialCI.GetCommunicator());
		if (pMat->GetBaseTexture().GetName() == Name)
		{
			++nNbr;
		}
	}
	return nNbr;	*/

	QDT_FAIL();
	return (0);
}

//-----------------------------------------------------------------------------
//	Name:		GetVarName
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	SPRITE_BANK::GetVarName() const
{
	return _sVarName;
}

//-----------------------------------------------------------------------------
//	Name:		GetSourceFileName
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	SPRITE_BANK::GetSourceFileName() const
{
	return _sSourceFileName;
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
void	SPRITE_BANK::Init(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors( EM_Construct, EM_Destruct );
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

	// Chunks
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk(  "SPB_CHK", 1, CM_ReadChunk );
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk( "SPB_CHK", 1, CM_WriteChunk );
#endif

	static_cast<COMMUNICATOR_CONTAINER_DESCRIPTOR *> (pCD)->SetRetrieveByVersion(false);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_CONTAINER_DESCRIPTOR *	SPRITE_BANK::GetGenericCommunicatorDescriptor()
{
	return ( (COMMUNICATOR_CONTAINER_DESCRIPTOR *)COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SKIN) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_CONTAINER_DESCRIPTOR *	SPRITE_BANK::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_CONTAINER_DESCRIPTOR(	QDT_COMMENT("SPRITE_BANK"),
													MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SPRITE_BANK,
													QDT_COMMENT("49631E3F-D748-7559-EFA1-735640933FC7"),
													sizeof(SPRITE_BANK),
													Init,
													QDT_COMMENT("Bank of sprite"),
													COMMUNICATOR_CONTAINER::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
void	SPRITE_BANK::EM_Construct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	SPRITE_BANK *	pObject;
	COM_HANDLE *	pHandle;

	pObject = static_cast<SPRITE_BANK *>(pO);
	pHandle = static_cast<COM_HANDLE *>(pP);

	QDT_NEW_PLACED(pObject, SPRITE_BANK)(pHandle);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	03-12-02:	VMA - Created
//-----------------------------------------------------------------------------
void	SPRITE_BANK::EM_Construct_In_Memory(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	QDT_FAIL();
}


//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
void	SPRITE_BANK::EM_Destruct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	SPRITE_BANK *	pObject;
	pObject = static_cast<SPRITE_BANK *>(pO);
	QDT_DELETE_PLACED(pObject, SPRITE_BANK);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	SPRITE_BANK::CM_ReadChunk(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	SPRITE_BANK *		pSpriteBank;
	SPRITE_BANK_ELM *	pElm;
	unsigned int		i;
	unsigned int		nNbrElms;


	pSpriteBank = static_cast<SPRITE_BANK *>( pCommunicator );
	Stream >> nNbrElms;

	for (i = 0; i < nNbrElms; ++i)
	{
		pElm				= new SPRITE_BANK_ELM;
		pElm->_MaterialCI	= pSpriteBank->GetCommunicatorInstance(i);

		Stream >> pElm->_nID;

		pSpriteBank->_vSprite.PushTail(pElm);
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-07-23:	FDE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	SPRITE_BANK::CM_WriteChunk(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	SPRITE_BANK *			pSpriteBank;
	const SPRITE_BANK_ELM *	pElm;
	unsigned int			i;


	pSpriteBank = static_cast<SPRITE_BANK *>( pCommunicator );
	Stream << pSpriteBank->GetNbrElms();

	for (i = 0; i < pSpriteBank->GetNbrElms(); ++i)
	{
		pElm = pSpriteBank->GetIthElm(i);
		Stream << pElm->_nID;
	}
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
