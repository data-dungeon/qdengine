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
//
//	05-11-10:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(Renderer/ITexture)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureFile)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_FILE Constructor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
ENGINE_TEXTURE_FILE::ENGINE_TEXTURE_FILE()
:
ENGINE_TEXTURE_ABC(GetGenericCommunicatorDescriptor()),
_pTexture(NULL),
_bCubeMap(false)
{
	for (UInt32 i = 0 ; i < 6 ; ++i)
	{
		_ppuiSize[i] = NULL;
		_ppRawData[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_FILE Destructor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
ENGINE_TEXTURE_FILE::~ENGINE_TEXTURE_FILE()
{
	RENDER_CONTEXT::ReleaseTexture(_pTexture);

	// This code is useful only when exporting textures
	for (UInt32 j = 0 ; j < 6 ; ++j)
	{
		if (_ppRawData[j])
		{
			for (UInt32 i = 0 ; i < _uiNbrMipmaps ; ++i)
			{
				if (_ppRawData[j][i])
				{
					delete [] _ppRawData[j][i];
				}
			}

			delete [] _ppRawData[j];
		}

		if(_ppuiSize[j])
		{
			delete [] _ppuiSize[j];
		}
	}
	//
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * ENGINE_TEXTURE_FILE::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ENGINE_TEXTURE_FILE));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * ENGINE_TEXTURE_FILE::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("ENGINE_TEXTURE_FILE"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ENGINE_TEXTURE_FILE,
										QDT_COMMENT("43044444-6DFA-F943-8BE3-26908EE88888"), 
										sizeof(ENGINE_TEXTURE_FILE), 
										ComInit, 
										"",
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::ComInit(CLASS_DESCRIPTOR * const pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("FILETEXT", 0, CM_ReadChunk0);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("FILETEXT", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("FILETEXT", 1, CM_WriteChunk1);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::EM_Construct(void * const pO, 
									  void * const pR, 
									  void * const pP)
{
	ENGINE_TEXTURE_FILE	*pShader;
	pShader = static_cast<ENGINE_TEXTURE_FILE *>(pO);

	QDT_NEW_PLACED(pShader, ENGINE_TEXTURE_FILE)();
	QDT_ASSERT(pShader);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::EM_Destruct(void * const pO, 
									 void * const pR, 
									 void * const pP)
{
	ENGINE_TEXTURE_FILE	*pShader;
	pShader = static_cast<ENGINE_TEXTURE_FILE *>(pO);

	QDT_DELETE_PLACED(pShader, ENGINE_TEXTURE_FILE);
}

//-----------------------------------------------------------------------------
//	Name:		SetFormat
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::SetFormat(PIXEL_FORMAT eFormat)
{
	_eFormat = eFormat;
}

//-----------------------------------------------------------------------------
//	Name:		SetWidth
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::SetWidth(UInt32 uiWidth)
{
	_uiWidth = uiWidth;
}

//-----------------------------------------------------------------------------
//	Name:		SetHeight
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::SetHeight(UInt32 uiHeight)
{
	_uiHeight = uiHeight;
}

//-----------------------------------------------------------------------------
//	Name:		SetDepth
//	Object:		
//	06-03-03:	SBE - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::SetDepth(UInt32 uiDepth)
{
	_uiDepth = uiDepth;
}

//-----------------------------------------------------------------------------
//	Name:		SetCubeMap
//	Object:		
//	06-03-20:	RMA - Created
//-----------------------------------------------------------------------------
void	ENGINE_TEXTURE_FILE::SetCubeMap(Bool	b)
{
	_bCubeMap = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrMipmaps
//	Object:		
//	05-11-15:	RMA - Created
//	06-02-10:	RMA - Modified the pointers are set to NULL to remove a crash
//				in Maya
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::SetNbrMipmaps(UInt32 uiNbr)
{
	_uiNbrMipmaps = uiNbr;
	if (_uiNbrMipmaps != 0)
	{
		UInt32 uiNbrOfFaces = 1;
		if (_bCubeMap)
		{
			uiNbrOfFaces = 6;
		}

		for (UInt32 j = 0 ; j < uiNbrOfFaces ; ++j)
		{
			_ppRawData[j] = new UInt8* [_uiNbrMipmaps];
			_ppuiSize[j] = new UInt32 [_uiNbrMipmaps];

			for (UInt32 i = 0 ; i < _uiNbrMipmaps ; ++i)
			{
				_ppRawData[j][i] = NULL;
			}
		}
	}
}
	
//-----------------------------------------------------------------------------
//	Name:		SetRawData
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::SetRawData(UInt32 uiCubeFace,
									 UInt32 uiLevel, 
									 UInt8 * pRawData, 
									 UInt32 uiSize)
{
	QDT_ASSERT(_ppRawData);
	_ppRawData[uiCubeFace][uiLevel] = pRawData;
	_ppuiSize[uiCubeFace][uiLevel] = uiSize;
}
	
//-----------------------------------------------------------------------------
//	Name:		SetFilter
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::SetFilter(FILTER_TYPE eFilter)
{
	_eFilter = eFilter;
}
	
//-----------------------------------------------------------------------------
//	Name:		SetAddressU
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::SetAddressU(ADDRESS_TYPE eAddress)
{
	_eAddressU = eAddress;
}
	
//-----------------------------------------------------------------------------
//	Name:		SetAddressV
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::SetAddressV(ADDRESS_TYPE eAddress)
{
	_eAddressV = eAddress;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::CM_ReadChunk0(IO_STREAM & Stream, 
									    COMMUNICATOR * pCommunicator)
{
	ENGINE_TEXTURE_FILE	*pTexture;
	pTexture = static_cast<ENGINE_TEXTURE_FILE *>(pCommunicator);

	UInt32 uiFormat;
	UInt32 uiFilter;
	UInt32 uiAddressU;
	UInt32 uiAddressV;

	Stream >> uiFormat;
	Stream >> pTexture->_uiWidth;
	Stream >> pTexture->_uiHeight;
	Stream >> pTexture->_uiNbrMipmaps;
	Stream >> uiFilter;
	Stream >> uiAddressU;
	Stream >> uiAddressV;

	pTexture->_eFormat = static_cast<PIXEL_FORMAT>(uiFormat);
	pTexture->_eFilter = static_cast<FILTER_TYPE>(uiFilter);
	pTexture->_eAddressU = static_cast<ADDRESS_TYPE>(uiAddressU);
	pTexture->_eAddressV = static_cast<ADDRESS_TYPE>(uiAddressV);

	QDT_ASSERT(pTexture->_uiNbrMipmaps > 0);

	pTexture->_ppRawData[0] = new UInt8* [pTexture->_uiNbrMipmaps];
	pTexture->_ppuiSize[0] = new UInt32 [pTexture->_uiNbrMipmaps];

	UInt32 i;
	for (i = 0 ; i < pTexture->_uiNbrMipmaps ; ++i)
	{
		Stream >> pTexture->_ppuiSize[0][i];

		pTexture->_ppRawData[0][i] = new UInt8 [pTexture->_ppuiSize[0][i]];

		Stream.GetBufferFast(pTexture->_ppRawData[0][i], pTexture->_ppuiSize[0][i]);
		/*if(pTexture->_uiNbrMipmaps == 1)
		{
			int k;
			int currentOffset = 0;
			FILE* fHandle = fopen("c:\\dumpTexture.txt", "w+");

			while(currentOffset<pTexture->_ppuiSize[0][i])
			{
				int l;
				for(l=0;l<8;l++)
				{
					fprintf(fHandle, "0x%08X, ", *(UInt32*)&pTexture->_ppRawData[0][i][currentOffset]);
					currentOffset+=4;
				}
				fprintf(fHandle, "\n");
			}

			fclose(fHandle);
		}*/
	}

	pTexture->CreateTexture();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		Add cube map support
//	06-03-17:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::CM_ReadChunk1(IO_STREAM & Stream, 
									    COMMUNICATOR * pCommunicator)
{
	ENGINE_TEXTURE_FILE	*pTexture;
	pTexture = static_cast<ENGINE_TEXTURE_FILE *>(pCommunicator);

	UInt32 uiFormat;
	UInt32 uiFilter;
	UInt32 uiAddressU;
	UInt32 uiAddressV;

	Stream >> uiFormat;
	Stream >> pTexture->_uiWidth;
	Stream >> pTexture->_uiHeight;
	Stream >> pTexture->_uiNbrMipmaps;
	Stream >> uiFilter;
	Stream >> uiAddressU;
	Stream >> uiAddressV;
	Stream >> pTexture->_bCubeMap;

	pTexture->_eFormat = static_cast<PIXEL_FORMAT>(uiFormat);
	pTexture->_eFilter = static_cast<FILTER_TYPE>(uiFilter);
	pTexture->_eAddressU = static_cast<ADDRESS_TYPE>(uiAddressU);
	pTexture->_eAddressV = static_cast<ADDRESS_TYPE>(uiAddressV);

	if (pTexture->_uiNbrMipmaps > 0)
	{
		UInt32 uiNbrOfFaces = 1;

		if (pTexture->_bCubeMap)
		{
			uiNbrOfFaces = 6;
		}

		UInt32 i, j;

		for (j = 0 ; j < uiNbrOfFaces ; ++j)
		{
			pTexture->_ppRawData[j] = new UInt8* [pTexture->_uiNbrMipmaps];
			pTexture->_ppuiSize[j] = new UInt32 [pTexture->_uiNbrMipmaps];

			for (i = 0 ; i < pTexture->_uiNbrMipmaps ; ++i)
			{
				Stream >> pTexture->_ppuiSize[j][i];

				pTexture->_ppRawData[j][i] = new UInt8 [pTexture->_ppuiSize[j][i]];

				Stream.GetBufferFast(pTexture->_ppRawData[j][i], pTexture->_ppuiSize[j][i]);
			}
		}

		pTexture->CreateTexture();
	}
	else
	{
		QDT_FAIL();
	}
}
	
//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::CM_WriteChunk1(IO_STREAM & Stream, 
										 COMMUNICATOR * pCommunicator)
{
	ENGINE_TEXTURE_FILE	*pTexture;
	pTexture = static_cast<ENGINE_TEXTURE_FILE *>(pCommunicator);

	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("ENGINE_TEXTURE_FILE :");
	Stream.DebugOutput(pTexture->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Format : ");
	Stream << pTexture->_eFormat;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Width : ");
	Stream << pTexture->_uiWidth;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Height : ");
	Stream << pTexture->_uiHeight;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Number of mipmaps : ");
	Stream << pTexture->_uiNbrMipmaps;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Filter : ");
	Stream << pTexture->_eFilter;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    AddressU : ");
	Stream << pTexture->_eAddressU;
	Stream.DebugOutput("\r\n");
	
	Stream.DebugOutput("    AddressV : ");
	Stream << pTexture->_eAddressV;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    CubeMap : ");
	Stream << pTexture->_bCubeMap;
	Stream.DebugOutput("\r\n");

	QDT_ASSERT(pTexture->_uiNbrMipmaps > 0);

	

	if (Stream.IsFileTextStreamDebug() == false) // Don't write full texture in debug file
	{
		UInt32 uiNbrOfFaces = 1;

		if (pTexture->_bCubeMap)
		{
			uiNbrOfFaces = 6;
		}

		UInt32 i, j;
		
		for (j = 0 ; j < uiNbrOfFaces ; ++j)
		{
			for (i = 0 ; i < pTexture->_uiNbrMipmaps ; ++i)
			{
				Stream << pTexture->_ppuiSize[j][i];
				Stream.PutBufferFast(pTexture->_ppRawData[j][i], pTexture->_ppuiSize[j][i]);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CreateTexture
//	Object:		
//	05-11-15:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_FILE::CreateTexture()
{
	if (_bCubeMap)
	{
		QDT_ASSERT(_uiWidth == _uiHeight);

		_pTexture = RENDER_CONTEXT::CreateCubeTexture(_uiWidth, _eFormat, _uiNbrMipmaps);

		UInt32 i, j;

		for (j = 0 ; j < 6 ; ++j)
		{
			for (i = 0 ; i < _uiNbrMipmaps ; ++i)
			{
				UInt8 * pBuffer = static_cast<UInt8 *>(_pTexture->Lock(i, j));
				memcpy(pBuffer, _ppRawData[j][i], _ppuiSize[j][i]);
				delete [] _ppRawData[j][i];
				_pTexture->Unlock(i, j);
			}

			delete [] _ppRawData[j];	_ppRawData[j] = NULL;
			delete [] _ppuiSize[j];		_ppuiSize[j] = NULL;
		}
	}
	else
	{
		_pTexture = RENDER_CONTEXT::CreateTexture(_uiWidth, _uiHeight, _eFormat, _uiNbrMipmaps);

		UInt32 i;
		for (i = 0 ; i < _uiNbrMipmaps ; ++i)
		{
			UInt8 * pBuffer = static_cast<UInt8 *>(_pTexture->Lock(i));
			memcpy(pBuffer, _ppRawData[0][i], _ppuiSize[0][i]);
			delete [] _ppRawData[0][i];
			_pTexture->Unlock(i);
		}

		delete [] _ppRawData[0];	_ppRawData[0] = NULL;
		delete [] _ppuiSize[0];		_ppuiSize[0] = NULL;
	}

	_pTexture->SetFilterType(_eFilter);
	_pTexture->SetAddressUType(_eAddressU);
	_pTexture->SetAddressVType(_eAddressV);
}

//-----------------------------------------------------------------------------
//	Name:		SetTexture
//	Object:		
//	06-02-09:	FBO - Created
//-----------------------------------------------------------------------------
void	ENGINE_TEXTURE_FILE::SetTexture(I_TEXTURE *	pTex)
{
	if (_pTexture)
	{
		delete _pTexture;
	}

	_pTexture = pTex;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
