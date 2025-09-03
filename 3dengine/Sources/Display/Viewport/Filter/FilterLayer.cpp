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
//	CLASS:	FILTER_LAYER
//
//	04-01-02:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Display/Viewport/Filter/FilterLayer)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComBasis/AttributeDescriptor)
#include	INCL_KCOM(ComplexType/TypeColor)
#include	INCL_KCOM(BaseType/TypeFloat)

#ifdef _DEBUG
	#include	"FilterLayer.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FILTER_LAYER constructor
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
FILTER_LAYER::FILTER_LAYER()
:
COMMUNICATOR(GetGenericCommunicatorDescriptor()),
//_pGeometry(NULL),
_nCurrGeometry(0),
_nFrequency(1),
_bVisible(true),
_bMatchPixel(false),
_bRandomUV(false),
_rCurrentTime(0.0f),
_rFrequencyInSec(33.333333f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		FILTER_LAYER destructor
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
FILTER_LAYER::~FILTER_LAYER()
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
//	04-01-12:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::InitGeometry()
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
//	Name:		Display
//	Object:		
//	04-01-02:	RMA - Created
//  04-07-27:	RMA - Modified : J'ai retiré la pondération du filtre qui n'est
//				plus nécéssaire maintenant que le blur est fait APRES le filtre.
//				Ceci était nécéssaire car s'il est fait avant (ce qui nécéssitait
//				une pondération) il y avait des interférences sévères avec le 
//				rendu négatif. Le débat est donc clos : filtre avant le blur et
//				pas de pondération!!!!
//-----------------------------------------------------------------------------
void	FILTER_LAYER::Display(VIEWPORT * pViewport)
{
	QDT_NOT_IMPLEMENTED();
/*
	if (_ciMaterial.HasHandle() && GetVisible())
	{
		RENDER_CONTEXT	*pRC = RENDER_MANAGER::Instance()->GetRenderContext();
		GEOMETRY_ABC * pCurrentGeometry;
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

		float rViewportWidth	= pViewport->GetWidth();
		float rViewportHeight	= pViewport->GetHeight();
		float rViewportLeft		= pViewport->GetLeft();
		float rViewportTop		= pViewport->GetTop();
				
		int nSizeX = RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeX();
		int nSizeY = RENDER_MANAGER::Instance()->GetRenderContext()->GetSizeY();

		// Compute viewport size in pixel
		rViewportWidth  = nSizeX * rViewportWidth * 0.01f;
		rViewportHeight = nSizeY * rViewportHeight * 0.01f;
		rViewportLeft = nSizeX * rViewportLeft * 0.01f;
		rViewportTop = nSizeY * rViewportTop * 0.01f;

		if (GetMatchPixel())
		{
			MATERIAL_ABC *pMaterial = GetMaterial();
			COM_INSTANCE ciTexture;
			if (pMaterial->GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PRELIT_MATERIAL)
			{
				ciTexture = ((PRELIT_MATERIAL*)(pMaterial))->GetBaseTexture();
			}
			else if (pMaterial->GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLOR_PER_VERTEX_MATERIAL)
			{
				ciTexture = ((COLOR_PER_VERTEX_MATERIAL*)(pMaterial))->GetBaseTexture();
			}
				
			if (ciTexture.HasHandle())
			{
//				TEXTURE_NEW * pTexture = (TEXTURE_NEW *)ciTexture.GetCommunicator();
//				unsigned int nTextureWidth = pTexture->GetSizeX();
//				unsigned int nTextureHeight = pTexture->GetSizeY();
//				
//
//				rMaxU = rViewportWidth / (float)nTextureWidth;
//				rMaxV = rViewportHeight / (float)nTextureHeight;
			}
		}

		float rDeltaFrameTime = TIME::Instance()->GetDefaultClock()->GetDeltaFrameTime();
		_rCurrentTime += rDeltaFrameTime;

		_rRandomU = MATH::Rand(1.0f);
		_rRandomV = MATH::Rand(1.0f);

		_rCurrentTime = 0.0;

		if (GetRandomUV())
		{
			rMinU = _rRandomU;
			rMinV = _rRandomV;

			rMaxU += rMinU;
			rMaxV += rMinV;
		}

		float rXMin = rViewportLeft;
		float rXMax = rViewportLeft + rViewportWidth;
		float rYMin = rViewportTop;
		float rYMax = rViewportTop + rViewportHeight;

		COLOR C = _Color;

		SHADER_ABC * pShader = static_cast<MATERIAL_ABC *>(_ciMaterial.GetCommunicator())->GetShader();

		VECTOR vUvs[4] = {VECTOR (rMinU, rMaxV, 1.0f),
						  VECTOR (rMaxU, rMaxV, 1.0f),
						  VECTOR (rMinU, rMinV, 1.0f),
						  VECTOR (rMaxU, rMinV, 1.0f)};

		if(pShader->UseTextureMatrix())
		{
			// We must handle uv anims here, because "Direct3D does not modify transformed and lit vertices"			
			MATRIX mD3DMAtrix = pShader->GetTextureMatrix();
			mD3DMAtrix(2, 1) = -mD3DMAtrix(2, 1);
	
			mD3DMAtrix.TransformDirection(vUvs[0]);
			mD3DMAtrix.TransformDirection(vUvs[1]);
			mD3DMAtrix.TransformDirection(vUvs[2]);
			mD3DMAtrix.TransformDirection(vUvs[3]);

			//Bypass the shader handle of the uv anim
			pShader->SetUseTextureMatrix(false);
		}
		

#ifndef PLATFORM_PS2
		if	(pCurrentGeometry->GetGeometryType() == GEOMETRY_ABC::GEOMETRY_TYPE_INDEXED_TRIANGLE_LIST)
		{
			pCurrentGeometry->SetVertex(0, VECTOR(rXMin,	rYMin, 0.0f));
			pCurrentGeometry->SetVertex(1, VECTOR(rXMax,	rYMin, 0.0f));
			pCurrentGeometry->SetVertex(2, VECTOR(rXMin,	rYMax, 0.0f));
			pCurrentGeometry->SetVertex(3, VECTOR(rXMax,	rYMax, 0.0f));
			pCurrentGeometry->SetTextureCoordinate(0, 0, POINT2D(vUvs[0].GetX(), vUvs[0].GetY()));
			pCurrentGeometry->SetTextureCoordinate(0, 1, POINT2D(vUvs[1].GetX(), vUvs[1].GetY()));
			pCurrentGeometry->SetTextureCoordinate(0, 2, POINT2D(vUvs[2].GetX(), vUvs[2].GetY()));
			pCurrentGeometry->SetTextureCoordinate(0, 3, POINT2D(vUvs[3].GetX(), vUvs[3].GetY()));
			pCurrentGeometry->SetDiffuseColor(0, C);
			pCurrentGeometry->SetDiffuseColor(1, C);
			pCurrentGeometry->SetDiffuseColor(2, C);
			pCurrentGeometry->SetDiffuseColor(3, C);
			pCurrentGeometry->Synchronize();
		}
		else
		{
			pCurrentGeometry->SetVertex(0, VECTOR(rXMin, rYMin, 0.0f));
			pCurrentGeometry->SetVertex(1, VECTOR(rXMax, rYMin, 0.0f));
			pCurrentGeometry->SetVertex(2, VECTOR(rXMin, rYMax, 0.0f));
			pCurrentGeometry->SetVertex(3, VECTOR(rXMax, rYMin, 0.0f));
			pCurrentGeometry->SetVertex(4, VECTOR(rXMax, rYMax, 0.0f));
			pCurrentGeometry->SetVertex(5, VECTOR(rXMin, rYMax, 0.0f));
			pCurrentGeometry->SetTextureCoordinate(0, 0, POINT2D(vUvs[0].GetX(), vUvs[0].GetY()));
			pCurrentGeometry->SetTextureCoordinate(0, 1, POINT2D(vUvs[1].GetX(), vUvs[1].GetY()));
			pCurrentGeometry->SetTextureCoordinate(0, 2, POINT2D(vUvs[2].GetX(), vUvs[2].GetY()));
			pCurrentGeometry->SetTextureCoordinate(0, 3, POINT2D(vUvs[1].GetX(), vUvs[1].GetY()));
			pCurrentGeometry->SetTextureCoordinate(0, 4, POINT2D(vUvs[3].GetX(), vUvs[3].GetY()));
			pCurrentGeometry->SetTextureCoordinate(0, 5, POINT2D(vUvs[2].GetX(), vUvs[2].GetY()));
			pCurrentGeometry->SetDiffuseColor(0, C);
			pCurrentGeometry->SetDiffuseColor(1, C);
			pCurrentGeometry->SetDiffuseColor(2, C);
			pCurrentGeometry->SetDiffuseColor(3, C);
			pCurrentGeometry->SetDiffuseColor(4, C);
			pCurrentGeometry->SetDiffuseColor(5, C);
			pCurrentGeometry->Synchronize();
		}
#else
		((GEOMETRY_PS2*)pCurrentGeometry)->BeginIterator();
		((GEOMETRY_PS2*)pCurrentGeometry)->NeedFlushCache(true);

		float * pV = (float *) pCurrentGeometry->GetVertex(0);
		POINT2D * pT = (POINT2D *) pCurrentGeometry->GetTextureCoordinate(0, 0);
		unsigned int * pD = (unsigned int *) pCurrentGeometry->GetDiffuseColor(0);

		(*pV++) = rXMin; (*pV++) = rYMin; (*pV++) = 0.0f;
		(*pV++) = rXMax; (*pV++) = rYMin; (*pV++) = 0.0f;
		(*pV++) = rXMin; (*pV++) = rYMax; (*pV++) = 0.0f;
		(*pV++) = rXMax; (*pV++) = rYMin; (*pV++) = 0.0f;
		(*pV++) = rXMax; (*pV++) = rYMax; (*pV++) = 0.0f;
		(*pV++) = rXMin; (*pV++) = rYMax; (*pV++) = 0.0f;

		(*pT++).SetXY(vUvs[0].GetX(), vUvs[0].GetY());
		(*pT++).SetXY(vUvs[1].GetX(), vUvs[1].GetY());
		(*pT++).SetXY(vUvs[2].GetX(), vUvs[2].GetY());
		(*pT++).SetXY(vUvs[1].GetX(), vUvs[1].GetY());
		(*pT++).SetXY(vUvs[3].GetX(), vUvs[3].GetY());
		(*pT++).SetXY(vUvs[2].GetX(), vUvs[2].GetY());

		C.SetAlpha(C.GetAlpha()>>1);
		unsigned int n = C.GetABGR();
		(*pD++) = n;
		(*pD++) = n;
		(*pD++) = n;
		(*pD++) = n;
		(*pD++) = n;
		(*pD++) = n;
#endif		
		
		pRC->GetRenderPipeline()->DisplayContainer(*pCurrentGeometry, MATRIX::IDENTITY);

//		pRC->WaitRendering();

		pRC->SetCullMode(CullMode);
		pRC->EnableDepthWrite(true);
		pRC->EnableDepthTest(true);
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *		pMD;
	ATTRIBUTE_DESCRIPTOR *	pAD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Attributes
	/*pAD = new	ATTRIBUTE_DESCRIPTOR("material", QDT_COMMENT(""), MATERIAL_ABC::GetGenericCommunicatorDescriptor());
	pAD->SetAccessors(EM_GetMaterial, EM_SetMaterial);
	pCD->AddAttribute(pAD);*/

	pAD = QDT_NEW	ATTRIBUTE_DESCRIPTOR("color", QDT_COMMENT(""), TYPE_COLOR::GetDescriptor());
	pAD->SetAccessors(EM_GetColor, EM_SetColor);
	pCD->AddAttribute(pAD);

	pAD = QDT_NEW	ATTRIBUTE_DESCRIPTOR("alpha", QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
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
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("FLAYER  ", 0, CM_ReadChunk0);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("FLAYER  ", 1, CM_ReadChunk1);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("FLAYER  ", 1, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	FILTER_LAYER::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_FILTER_LAYER) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	FILTER_LAYER::CreateGenericCommunicatorDescriptor()
{
	return ( QDT_NEW COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("FILTER_LAYER"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_FILTER_LAYER,
											QDT_COMMENT("52AF22FC-E731-6B3F-1D11-3DA569C5ABD3"),
											sizeof(FILTER_LAYER),
											Init,
											QDT_COMMENT("The FilterLayer class.")));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::EM_Construct(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	FILTER_LAYER *	pObject;
	pObject = static_cast<FILTER_LAYER *>(pO);
	QDT_NEW_PLACED(pObject, FILTER_LAYER)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::EM_Destruct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	FILTER_LAYER *	pObject;
	pObject = static_cast<FILTER_LAYER*> (pO);
	pObject->FILTER_LAYER::~FILTER_LAYER();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetMaterial
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::EM_SetMaterial(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	/*COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_LAYER	*pThis		= static_cast<FILTER_LAYER*>(pciThis->GetCommunicator());
	COM_INSTANCE	*pcMaterial	= static_cast<COM_INSTANCE*>(pP);

	pThis->SetMaterial(*pcMaterial);*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetMaterial
//	Object:		
//	04-01-20:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::EM_GetMaterial(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	/*COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_LAYER	*pThis		= static_cast<FILTER_LAYER*>(pciThis->GetCommunicator());
	COM_INSTANCE	*pcMaterial	= static_cast<COM_INSTANCE*>(pR);

	*pcMaterial = pThis->GetMaterialInst();*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetColor
//	Object:		
//	04-01-20:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::EM_SetColor(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_LAYER	*pThis		= static_cast<FILTER_LAYER*>(pciThis->GetCommunicator());
	COLOR			*pcColor	= static_cast<COLOR*>(pP);

	COLOR			 C = pThis->GetColor();
	unsigned char	 cAlpha = C.GetAlpha();
	C = *pcColor;
	C.SetAlpha(cAlpha);

	pThis->SetColor(C);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetColor
//	Object:		
//	04-01-20:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::EM_GetColor(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_LAYER	*pThis		= static_cast<FILTER_LAYER*>(pciThis->GetCommunicator());
	COLOR			*pcColor	= static_cast<COLOR*>(pR);

	*pcColor = pThis->GetColor();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetAlpha
//	Object:		
//	04-01-20:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER_LAYER::EM_SetAlpha(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_LAYER	*pThis		= static_cast<FILTER_LAYER*>(pciThis->GetCommunicator());
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
void	FILTER_LAYER::EM_GetAlpha(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER_LAYER	*pThis		= static_cast<FILTER_LAYER*>(pciThis->GetCommunicator());
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
void	FILTER_LAYER::CM_ReadChunk0(IO_STREAM &		Stream,
								    COMMUNICATOR *	pCommunicator)
{
	FILTER_LAYER *pLayer = static_cast<FILTER_LAYER *>(pCommunicator);

	COM_INSTANCE ciMaterial;
	COLOR C;
	bool bVisible, bMatchPixel, bRandomUV;

	Stream	>> ciMaterial 
			>> C 
			>> bVisible 
			>> bMatchPixel 
			>> bRandomUV;

	pLayer->SetMaterial(ciMaterial);
	pLayer->SetColor(C);
	pLayer->SetVisible(bVisible);
	pLayer->SetMatchPixel(bMatchPixel);
	pLayer->SetRandomUV(bRandomUV);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	04-01-21:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	FILTER_LAYER::CM_ReadChunk1(IO_STREAM &		Stream,
									COMMUNICATOR *	pCommunicator)
{
	FILTER_LAYER *pLayer = static_cast<FILTER_LAYER *>(pCommunicator);

	COM_INSTANCE ciMaterial;
	COLOR C;
	bool bVisible, bMatchPixel, bRandomUV;
	unsigned int nFrequency;

	Stream	>> ciMaterial 
			>> C 
			>> bVisible 
			>> bMatchPixel 
			>> bRandomUV
			>> nFrequency;

	pLayer->SetMaterial(ciMaterial);
	pLayer->SetColor(C);
	pLayer->SetVisible(bVisible);
	pLayer->SetMatchPixel(bMatchPixel);
	pLayer->SetRandomUV(bRandomUV);
	pLayer->SetFrequency(nFrequency);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	04-01-12:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	FILTER_LAYER::CM_WriteChunk(IO_STREAM &		Stream,
									COMMUNICATOR *	pCommunicator)
{
	FILTER_LAYER *pLayer = static_cast<FILTER_LAYER *>(pCommunicator);

	Stream.DebugOutput("Material = ");
	Stream << pLayer->GetMaterialInst();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Color = ");
	Stream << pLayer->GetColor();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Visible = ");
	Stream << pLayer->GetVisible();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("MatchPixel = ");
	Stream << pLayer->GetMatchPixel();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("RandomUV = ");
	Stream << pLayer->GetRandomUV();
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Frequency ) ");
	Stream << pLayer->GetFrequency();
	Stream.DebugOutput("\r\n");
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
