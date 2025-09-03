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
//	CLASS:	FILTER_SCRATCH
//
//	04-01-02:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Display/Viewport/Filter/FilterScratch)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComBasis/AttributeDescriptor)
#include	INCL_KCOM(ComplexType/TypeColor)
#include	INCL_KCOM(BaseType/TypeFloat)

#ifdef _DEBUG
	#include	"FilterScratch.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FILTER_SCRATCH constructor
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
FILTER_SCRATCH::FILTER_SCRATCH()
:
COMMUNICATOR(GetGenericCommunicatorDescriptor()),
//_pGeometry(NULL),
_bVisible(true),
_nDuration(5),
_nFrequency(5),
_rMaxSizeX(0.1f),
_rMaxSizeY(0.1f),
_rCurrentTime(0.0),
_Position(0.0,0.0),
_Size(1.0,1.0),
_nCurrGeometry(0)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		FILTER_SCRATCH destructor
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
FILTER_SCRATCH::~FILTER_SCRATCH()
{
/*	if ( _pGeometry )
	{
		for (int i=0; i<DISPLAY_FILTER_NBVIEWPORT_MAX; ++i)
		{
			if (_pGeometry[i])
			{
				delete _pGeometry[i];
			}
		}
		delete	[] _pGeometry;
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *		pMD;
	ATTRIBUTE_DESCRIPTOR *	pAD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Attributes
	/*pAD = new	ATTRIBUTE_DESCRIPTOR("material", QDT_COMMENT(""), MATERIAL_ABC::GetGenericCommunicatorDescriptor());
	pAD->SetAccessors(EM_GetMaterial, EM_SetMaterial);
	pCD->AddAttribute(pAD);*/

	pAD = new	ATTRIBUTE_DESCRIPTOR("color", QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pAD->SetAccessors(EM_GetColor, EM_SetColor);
	pCD->AddAttribute(pAD);

	pAD = new	ATTRIBUTE_DESCRIPTOR("alpha", QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pAD->SetAccessors(EM_GetAlpha, EM_SetAlpha);
	pCD->AddAttribute(pAD);

	// Methods
	pMD = METHOD_DESCRIPTOR__Allocate("SetMaterial", "");
	//METHOD_DESCRIPTOR__AddParameter(pMD, "Material", "", MATERIAL_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_MATERIAL, pMD, EM_SetMaterial);

	pMD = METHOD_DESCRIPTOR__Allocate("GetMaterial", "");
	//METHOD_DESCRIPTOR__SetReturn(pMD,"Material", "", MATERIAL_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_GET_MATERIAL, pMD, EM_GetMaterial);

	pMD = METHOD_DESCRIPTOR__Allocate("SetColor", "");
	METHOD_DESCRIPTOR__AddParameter(pMD, "Color", "", TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_SET_COLOR, pMD, EM_SetColor);

	pMD = METHOD_DESCRIPTOR__Allocate("GetColor", "");
	METHOD_DESCRIPTOR__AddParameter(pMD, "Color", "", TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_GET_COLOR, pMD, EM_GetColor);

	pMD = METHOD_DESCRIPTOR__Allocate("SetAlpha", "");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rAlpha", "", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_ALPHA, pMD, EM_SetAlpha);

	pMD = METHOD_DESCRIPTOR__Allocate("GetAlpha", "");
	METHOD_DESCRIPTOR__AddParameter(pMD, "rAlpha", "", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_ALPHA, pMD, EM_GetAlpha);

#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("FSCRATCH", 0, CM_ReadChunk0);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("FSCRATCH", 1, CM_ReadChunk1);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("FSCRATCH", 1, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	04-01-19:	FPR - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::Display(VIEWPORT *	pViewport)
{
	QDT_NOT_IMPLEMENTED();
/*
	if (_ciMaterial.HasHandle() && GetVisible())
	{	
		RENDER_CONTEXT	*pRC = RENDER_MANAGER::Instance()->GetRenderContext();
		GEOMETRY_ABC * pCurrentGeometry;
	
		// Computing user settings
		float rDeltaFrameTime = TIME::Instance()->GetDefaultClock()->GetDeltaFrameTime();
		_rCurrentTime += rDeltaFrameTime;

		if(_rCurrentTime > _rFrequencyInSec)
		{
			_Position[0] = MATH::Rand((float)pRC->GetSizeX());
			_Position[1] = MATH::Rand((float)pRC->GetSizeY());

			if (_bRandomSize)
			{
				_Size[0] = MATH::Rand(_rMaxSizeX * 0.666f) + _rMaxSizeX * 0.333f;
				_Size[1] = MATH::Rand(_rMaxSizeY * 0.666f) + _rMaxSizeY * 0.333f;
			}
			else
			{
				_Size[0] = _rMaxSizeX;
				_Size[1] = _rMaxSizeY;
			}

			_rCurrentTime = 0.0;
		}

		// End of life, we don't draw...
		if(_rCurrentTime>_rDurationInSec)return;

		//Display
		RENDER_CONTEXT::CULL_MODE	CullMode = pRC->GetCullMode();

		pRC->SetCullMode(RENDER_CONTEXT::CULLMODE_NONE);
		pRC->EnableDepthWrite(false);
		pRC->EnableDepthTest(false);

		if (!_pGeometry)
		{
			InitGeometry();
		}

		QDT_ASSERT(_pGeometry != NULL);

		pCurrentGeometry = _pGeometry[_nCurrGeometry];
		_nCurrGeometry++;
		if (_nCurrGeometry >= DISPLAY_FILTER_NBVIEWPORT_MAX) _nCurrGeometry = 0;

		float rMinU = 0.0f;
		float rMinV = 0.0f;
		float rMaxU = 1.0f;
		float rMaxV = 1.0f;

#ifndef PLATFORM_PS2
		if	(pCurrentGeometry->GetGeometryType() == GEOMETRY_ABC::GEOMETRY_TYPE_INDEXED_TRIANGLE_LIST)
		{
			pCurrentGeometry->SetVertex(0, VECTOR(_Position[0]-_Size[0],   _Position[1]-_Size[1],   0.0f));
			pCurrentGeometry->SetVertex(1, VECTOR(_Position[0]+_Size[0],   _Position[1]-_Size[1],   0.0f));
			pCurrentGeometry->SetVertex(2, VECTOR(_Position[0]-_Size[0],   _Position[1]+_Size[1], 0.0f));
			pCurrentGeometry->SetVertex(3, VECTOR(_Position[0]+_Size[0],   _Position[1]+_Size[1], 0.0f));
			pCurrentGeometry->SetTextureCoordinate(0, 0, POINT2D(rMinU, rMinV));
			pCurrentGeometry->SetTextureCoordinate(0, 1, POINT2D(rMaxU, rMinV));
			pCurrentGeometry->SetTextureCoordinate(0, 2, POINT2D(rMinU, rMaxV));
			pCurrentGeometry->SetTextureCoordinate(0, 3, POINT2D(rMaxU, rMaxV));
			pCurrentGeometry->SetDiffuseColor(0, _Color);
			pCurrentGeometry->SetDiffuseColor(1, _Color);
			pCurrentGeometry->SetDiffuseColor(2, _Color);
			pCurrentGeometry->SetDiffuseColor(3, _Color);
			pCurrentGeometry->Synchronize();
		}
		else
		{
			pCurrentGeometry->SetVertex(0, VECTOR(_Position[0]-_Size[0],   _Position[1]-_Size[1],   0.0f));
			pCurrentGeometry->SetVertex(1, VECTOR(_Position[0]+_Size[0],   _Position[1]-_Size[1],   0.0f));
			pCurrentGeometry->SetVertex(2, VECTOR(_Position[0]-_Size[0],   _Position[1]+_Size[1], 0.0f));
			pCurrentGeometry->SetVertex(3, VECTOR(_Position[0]+_Size[0],   _Position[1]-_Size[1],   0.0f));
			pCurrentGeometry->SetVertex(4, VECTOR(_Position[0]+_Size[0],   _Position[1]+_Size[1], 0.0f));
			pCurrentGeometry->SetVertex(5, VECTOR(_Position[0]-_Size[0],   _Position[1]+_Size[1], 0.0f));
			pCurrentGeometry->SetTextureCoordinate(0, 0, POINT2D(rMinU, rMinV));
			pCurrentGeometry->SetTextureCoordinate(0, 1, POINT2D(rMaxU, rMinV));
			pCurrentGeometry->SetTextureCoordinate(0, 2, POINT2D(rMinU, rMaxV));
			pCurrentGeometry->SetTextureCoordinate(0, 3, POINT2D(rMaxU, rMinV));
			pCurrentGeometry->SetTextureCoordinate(0, 4, POINT2D(rMaxU, rMaxV));
			pCurrentGeometry->SetTextureCoordinate(0, 5, POINT2D(rMinU, rMaxV));
			pCurrentGeometry->SetDiffuseColor(0, _Color);
			pCurrentGeometry->SetDiffuseColor(1, _Color);
			pCurrentGeometry->SetDiffuseColor(2, _Color);
			pCurrentGeometry->SetDiffuseColor(3, _Color);
			pCurrentGeometry->SetDiffuseColor(4, _Color);
			pCurrentGeometry->SetDiffuseColor(5, _Color);
		}
#else
		((GEOMETRY_PS2*)pCurrentGeometry)->BeginIterator();
		((GEOMETRY_PS2*)pCurrentGeometry)->NeedFlushCache(true);

		float * pV = (float *) pCurrentGeometry->GetVertex(0);
		POINT2D * pT = (POINT2D *) pCurrentGeometry->GetTextureCoordinate(0, 0);
		unsigned int * pD = (unsigned int *) pCurrentGeometry->GetDiffuseColor(0);

		(*pV++) = _Position[0]-_Size[0]; (*pV++) = _Position[1]-_Size[1]; (*pV++) = 0.0f;
		(*pV++) = _Position[0]+_Size[0]; (*pV++) = _Position[1]-_Size[1]; (*pV++) = 0.0f;
		(*pV++) = _Position[0]-_Size[0]; (*pV++) = _Position[1]+_Size[1]; (*pV++) = 0.0f;
		(*pV++) = _Position[0]+_Size[0]; (*pV++) = _Position[1]-_Size[1]; (*pV++) = 0.0f;
		(*pV++) = _Position[0]+_Size[0]; (*pV++) = _Position[1]+_Size[1]; (*pV++) = 0.0f;
		(*pV++) = _Position[0]-_Size[0]; (*pV++) = _Position[1]+_Size[1]; (*pV++) = 0.0f;

		(*pT++).SetXY(rMinU, rMinV);
		(*pT++).SetXY(rMaxU, rMinV);
		(*pT++).SetXY(rMinU, rMaxV);
		(*pT++).SetXY(rMaxU, rMinV);
		(*pT++).SetXY(rMaxU, rMaxV);
		(*pT++).SetXY(rMinU, rMaxV);

		COLOR C(_Color);
		C.SetAlpha(C.GetAlpha()>>1);
		unsigned int n = C.GetABGR();
		(*pD++) = n;
		(*pD++) = n;
		(*pD++) = n;
		(*pD++) = n;
		(*pD++) = n;
		(*pD++) = n;
#endif

		SHADER_ABC * pShader = static_cast<MATERIAL_ABC *>(_ciMaterial.GetCommunicator())->GetShader();

		pRC->GetRenderPipeline()->DisplayContainer(*pCurrentGeometry, MATRIX::IDENTITY);

		pRC->SetCullMode(CullMode);
		pRC->EnableDepthWrite(true);
		pRC->EnableDepthTest(true);
	}*/
}
	
//-----------------------------------------------------------------------------
//	Name:		InitGeometry
//	Object:		
//	04-01-19:	FPR - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::InitGeometry()
{
	QDT_NOT_IMPLEMENTED();
/*
	RENDER_CONTEXT	*pRC = RENDER_MANAGER::Instance()->GetRenderContext();

	_pGeometry = new GEOMETRY_ABC * [DISPLAY_FILTER_NBVIEWPORT_MAX];

	for (int i=0; i<DISPLAY_FILTER_NBVIEWPORT_MAX; ++i)
	{
		_pGeometry[i] = pRC->AllocateGeometry();
		_pGeometry[i]->Create(GEOMETRY_ABC::GEOMETRY_TYPE_INDEXED_TRIANGLE_LIST, GEOMETRY_ABC::VERTEX_TYPE_TRANSFORMED_LIT);
		_pGeometry[i]->InitQuad();
		_pGeometry[i]->SetNeedSynchronizeVertices(true);
		_pGeometry[i]->SetNeedSynchronizeIndices(true);
	}*/	
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	FILTER_SCRATCH::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_FILTER_SCRATCH) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	FILTER_SCRATCH::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("FILTER_SCRATCH"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_FILTER_SCRATCH,
											QDT_COMMENT("B1834721-38E2-BBCC-DA89-77DDE33E4707"),
											sizeof(FILTER_SCRATCH),
											Init,
											QDT_COMMENT("The FilterScratch class.")));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::EM_Construct(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	FILTER_SCRATCH *	pObject;
	pObject = static_cast<FILTER_SCRATCH *>(pO);
	QDT_NEW_PLACED(pObject, FILTER_SCRATCH)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::EM_Destruct(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	FILTER_SCRATCH *	pObject;
	pObject = static_cast<FILTER_SCRATCH*> (pO);
	pObject->FILTER_SCRATCH::~FILTER_SCRATCH();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetMaterial
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::EM_SetMaterial(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	/*COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_SCRATCH	*pThis		= static_cast<FILTER_SCRATCH*>(pciThis->GetCommunicator());
	COM_INSTANCE	*pcMaterial	= static_cast<COM_INSTANCE*>(pP);

	pThis->SetMaterial(*pcMaterial);*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetMaterial
//	Object:		
//	04-01-20:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::EM_GetMaterial(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	/*COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_SCRATCH	*pThis		= static_cast<FILTER_SCRATCH*>(pciThis->GetCommunicator());
	COM_INSTANCE	*pcMaterial	= static_cast<COM_INSTANCE*>(pR);

	*pcMaterial = pThis->GetMaterialInst();*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetColor
//	Object:		
//	04-01-20:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::EM_SetColor(void * const	pO,
								    void * const	pR,
								    void * const	pP)
{
	COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_SCRATCH	*pThis		= static_cast<FILTER_SCRATCH*>(pciThis->GetCommunicator());
	COLOR			*pcColor	= static_cast<COLOR*>(pP);

	pThis->SetColor(*pcColor);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetColor
//	Object:		
//	04-01-20:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::EM_GetColor(void * const	pO,
								    void * const	pR,
								    void * const	pP)
{
	COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_SCRATCH	*pThis		= static_cast<FILTER_SCRATCH*>(pciThis->GetCommunicator());
	COLOR			*pcColor	= static_cast<COLOR*>(pR);

	*pcColor = pThis->GetColor();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetAlpha
//	Object:		
//	04-01-20:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::EM_SetAlpha(void * const	pO,
								    void * const	pR,
								    void * const	pP)
{
	COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_SCRATCH	*pThis		= static_cast<FILTER_SCRATCH*>(pciThis->GetCommunicator());
	float			*prAlpha	= static_cast<float*>(pP);
	COLOR			 C;

	C = pThis->GetColor();
	C.SetAlpha((unsigned int)(*prAlpha * 255.0f));
	pThis->SetColor(C);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetAlpha
//	Object:		
//	04-01-20:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_SCRATCH::EM_GetAlpha(void * const	pO,
								    void * const	pR,
								    void * const	pP)
{
	COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_SCRATCH	*pThis		= static_cast<FILTER_SCRATCH*>(pciThis->GetCommunicator());
	float			*prAlpha	= static_cast<float*>(pR);
	COLOR			 C;

	C = pThis->GetColor();
	*prAlpha = (float)(C.GetAlpha()) / 255.0f;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	04-01-12:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	FILTER_SCRATCH::CM_ReadChunk0(IO_STREAM &	Stream,
									  COMMUNICATOR *	pCommunicator)
{
	FILTER_SCRATCH *pLayer = static_cast<FILTER_SCRATCH *>(pCommunicator);

	COM_INSTANCE ciMaterial;
	COLOR C;
	bool bVisible;
	unsigned int nDuration, nFrequency;
	float rMaxSizeX, rMaxSizeY;

	Stream >> ciMaterial 
		   >> C 
		   >> bVisible 
		   >> nDuration
		   >> nFrequency
		   >> rMaxSizeX
		   >> rMaxSizeY;

	pLayer->SetMaterial(ciMaterial);
	pLayer->SetColor(C);
	pLayer->SetVisible(bVisible);
	pLayer->SetDuration(nDuration);
	pLayer->SetFrequency(nFrequency);
	pLayer->SetMaxSizeX(rMaxSizeX*1000.0);
	pLayer->SetMaxSizeY(rMaxSizeY*1000.0);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	04-01-21:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	FILTER_SCRATCH::CM_ReadChunk1(IO_STREAM &		Stream,
									  COMMUNICATOR *	pCommunicator)
{
	FILTER_SCRATCH *pLayer = static_cast<FILTER_SCRATCH *>(pCommunicator);

	COM_INSTANCE ciMaterial;
	COLOR C;
	bool bVisible, bRandomSize;
	unsigned int nDuration, nFrequency;
	float rMaxSizeX, rMaxSizeY;

	Stream >> ciMaterial 
		   >> C 
		   >> bVisible 
		   >> nDuration
		   >> nFrequency
		   >> rMaxSizeX
		   >> rMaxSizeY
		   >> bRandomSize;

	pLayer->SetMaterial(ciMaterial);
	pLayer->SetColor(C);
	pLayer->SetVisible(bVisible);
	pLayer->SetDuration(nDuration);
	pLayer->SetFrequency(nFrequency);
	pLayer->SetMaxSizeX(rMaxSizeX * 1000.0);
	pLayer->SetMaxSizeY(rMaxSizeY * 1000.0);
	pLayer->SetRandomSize(bRandomSize);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	04-01-12:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	FILTER_SCRATCH::CM_WriteChunk(IO_STREAM &		Stream,
									  COMMUNICATOR *	pCommunicator)
{
	FILTER_SCRATCH *pLayer = static_cast<FILTER_SCRATCH *>(pCommunicator);

	Stream.DebugOutput("Material = ");
	Stream << pLayer->GetMaterialInst();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Color = ");
	Stream << pLayer->GetColor();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Visible = ");
	Stream << pLayer->GetVisible();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Duration = ");
	Stream << pLayer->GetDuration();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Frequency = ");
	Stream << pLayer->GetFrequency();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Max Size X = ");
	Stream << pLayer->GetMaxSizeX();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Max Size Y = ");
	Stream << pLayer->GetMaxSizeY();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Random size = ");
	Stream << pLayer->GetRandomSize();
	Stream.DebugOutput("\r\n");
}
#endif


//=============================================================================
//	CODE ENDS HERE
//=============================================================================

