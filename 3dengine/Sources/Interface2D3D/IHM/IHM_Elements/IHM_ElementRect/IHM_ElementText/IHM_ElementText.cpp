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
//	CLASS:	IHM_ELEMENT_TEXT
//
//	01-08-20:	GGO - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"..\..\Include.h"

#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes2D/Rectangle)
#include	INC_KRNCORE(Stream)
#include	INC_KRNCOM(ComplexType)
#include	INC_3DENGINE(M3D)
#include	INC_3DENGINE(Display/Viewport)
#include	INC_3DENGINE(Interface2D3D/DisplayFont)
#include	INC_3DENGINE(SceneGraph/Material)

#ifndef _DEBUG
	#include	INL_KRNMATH(Math/Geometry/Shapes/Shapes2D/Rectangle)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_TEXT constructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_TEXT::IHM_ELEMENT_TEXT()
:
IHM_ELEMENT_RECT(GetGenericCommunicatorDescriptor()),
_Font	( COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION ),
_Text	( "" )
{
	_pTextParser = new TEXT_PARSER( this );
		
	Show();
}

//-----------------------------------------------------------------------------
//	Name:		~IHM_ELEMENT_TEXT destructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_TEXT::~IHM_ELEMENT_TEXT()
{
	delete _pTextParser;
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::Open(IHM_INTERFACE *		pInterfance,
							   IHM_PAGE *			pPage,
							   IHM_ELEMENTS_LIST *	pElementsList)
{
	// Do Nothing...	
}
	
//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::Close(IHM_INTERFACE *		pInterfance,
								IHM_PAGE *			pPage,
								IHM_ELEMENTS_LIST *	pElementsList)
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-10-01:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::DisplayElement(VIEWPORT * pViewport)
{
	QDT_NOT_IMPLEMENTED();
/*
	RENDER_CONTEXT *pRenderContext = RENDER_MANAGER::Instance()->GetRenderContext();
	IHM_INTERFACE * pInterface;
	bool		bWired = false;
	bool		bOldAlphaBlend = pRenderContext->IsAlphaBlendEnabled();
	DISPLAY_FONT	*pFont;
	MATERIAL		*pMaterial;

	if (!_Font.HasHandle())
	{
		return;
	}

	pFont = static_cast<DISPLAY_FONT*>(_Font.GetCommunicator());
	pMaterial = static_cast<MATERIAL*>(pFont->GetMaterialInst().GetCommunicator());

	pInterface = GetInterfaceContainer();

	if ( (pViewport == NULL) && ( pInterface != NULL ) )
	{
		pViewport = pInterface->GetViewport();
	}

	// If text parsing hasn't be done before, do it now.
	_pTextParser->Process( pViewport );

	if ( _pTextParser->GetNbLetters() == 0 )
	{
		return;
	}

	// Set all the stuff in the render context.
	pRenderContext->EnableAlphaBlend(true);
	pRenderContext->SetAlphaBlendSrcFactor(RENDER_CONTEXT::BLENDFACTOR_SRCALPHA);
	pRenderContext->SetAlphaBlendDstFactor(RENDER_CONTEXT::BLENDFACTOR_INVSRCALPHA);

	if	(_bWired == false)
	{
		bWired = pRenderContext->IsWireFrameEnabled();
		pRenderContext->EnableWireFrame(false);
	}

	// Display the text.
	const IHM_WORD *	pCurrentWord;
	STRING_ATTRIBUTES	StringAttributes;

	RECTANGLE			ClippingRectangle( GetAbsolutePosLeft(pViewport),
										   GetAbsolutePosRight(pViewport),
										   GetAbsolutePosTop(pViewport),
										   GetAbsolutePosBottom(pViewport) );

	pRenderContext->InitString(_pTextParser->GetNbLetters(),
											&pFont->GetDisplayFontData(),
											pMaterial->GetShader(),
										    &ClippingRectangle);

	_pTextParser->MoveFirstWord();
	do
	{
		pCurrentWord = _pTextParser->GetCurrentWord();

		if ( pCurrentWord != NULL )
		{
			StringAttributes = pCurrentWord->_StringAttributes;
			StringAttributes._bResizeOn169 = _Flags.IsSet(RESIZE_ON_169);
			
			if (!pRenderContext->PushString(pCurrentWord->_pBuffer,
										    pCurrentWord->_rPosX,
									        pCurrentWord->_rPosY,
										    pCurrentWord->_nLetterNbr,
											&StringAttributes))
			{
				pRenderContext->DisplayStrings();

				QDT_ENSURE(pRenderContext->PushString(pCurrentWord->_pBuffer,
										    pCurrentWord->_rPosX,
									        pCurrentWord->_rPosY,
										    pCurrentWord->_nLetterNbr,
											&StringAttributes));
			}
		}
	}
	while (_pTextParser->MoveNextWord());

	pRenderContext->DisplayStrings();

	if	(_bWired == false)
	{
		pRenderContext->EnableWireFrame(bWired);
	}

	pRenderContext->EnableAlphaBlend(bOldAlphaBlend);*/
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-12-05:	RMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::Display(DISPLAY_PARAMETERS &	DisplayParameters,
								  float					rSizeX,
								  float					rSizeY)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		GetText
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	IHM_ELEMENT_TEXT::GetText() const						
{
	return ( _Text );
}

//-----------------------------------------------------------------------------
//	Name:		SetText
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::SetText(const QDT_STRING & Text)
{
	_Text = Text;
}

//-----------------------------------------------------------------------------
//	Name:		GetFont
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
DISPLAY_FONT *	IHM_ELEMENT_TEXT::GetFont()							
{ 
	if	(_Font.HasHandle())
	{
		return	(static_cast<DISPLAY_FONT*>(_Font.GetCommunicator()));
	}

	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		GetFontIndex
//	Object:		
//	02-09-11:	IAM - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	IHM_ELEMENT_TEXT::GetFontInst()
{
	return	(_Font);
}


//-----------------------------------------------------------------------------
//	Name:		SetFont
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::SetFont(const COM_INSTANCE & Font)			
{
	_Font = Font;
}

//-----------------------------------------------------------------------------
//	Name:		SetVerticalAlignement
//	Object:		
//	01-10-19:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::SetVerticalAlignement(const IHM_ELEMENT_TEXT::VALIGN	VerticalAlign)
{
	_pTextParser->SetVarticalAlign( VerticalAlign );
}

//-----------------------------------------------------------------------------
//	Name:		ScrollUp
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::ScrollUp(VIEWPORT * pViewport)
{
	_pTextParser->ScrollUp( pViewport );
}
	
//-----------------------------------------------------------------------------
//	Name:		ScrollDown
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::ScrollDown(VIEWPORT * pViewport)
{
	_pTextParser->ScrollDown( pViewport );
}

//-----------------------------------------------------------------------------
//	Name:		ChangeTextColor
//	Object:		
//	01-12-07:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::ChangeTextColor(const COLOR &	Color)
{
	_pTextParser->ChangeTextColor( Color );
}

//-----------------------------------------------------------------------------
//	Name:		Scroll
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::Scroll(VIEWPORT *		pViewport,
								 const float	rScrollPercent)
{
	_pTextParser->Scroll( pViewport, rScrollPercent );
}

//-----------------------------------------------------------------------------
//	Name:		SetScrollStep
//	Object:		
//	02-03-27:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::SetScrollStep(const int	nStep)
{
	_pTextParser->SetScrollStep( nStep );
}

//-----------------------------------------------------------------------------
//	Name:		SetScrollSpeed
//	Object:		
//	02-04-11:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::SetScrollSpeed(const float	rSpeed)
{
	_pTextParser->SetScrollSpeed( rSpeed );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrLines
//	Object:		
//	04-10-13:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENT_TEXT::GetNbrLines() const
{
	return ( _pTextParser->GetNbrLines() );
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pM = METHOD_DESCRIPTOR__Allocate("GetText", "Returns the text of the element.");
	METHOD_DESCRIPTOR__SetReturn(pM, "Text", "Ask 3D Engine to document this parameter.", TYPE_STRING::GetDescriptor());
	pCD->AddMethod(CMD_GET_TEXT, pM, EM_GetText);

	pM = METHOD_DESCRIPTOR__Allocate("SetText", "Set the text of the element.");
	METHOD_DESCRIPTOR__AddParameter(pM, "Text", "Ask 3D Engine to document this parameter.", TYPE_STRING::GetDescriptor());
	pCD->AddMethod(CMD_SET_TEXT, pM, EM_SetText);

	pM = METHOD_DESCRIPTOR__Allocate("GetFont", "Returns the font of the element.");
	METHOD_DESCRIPTOR__SetReturn(pM, "Font", "Ask 3D Engine to document this parameter.", DISPLAY_FONT::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_GET_FONT, pM, EM_GetFont);

	pM = METHOD_DESCRIPTOR__Allocate("SetFont", "Set the font of the element.");
	METHOD_DESCRIPTOR__AddParameter(pM, "Font", "Ask 3D Engine to document this parameter.", DISPLAY_FONT::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_FONT, pM, EM_SetFont);

	pM = METHOD_DESCRIPTOR__Allocate("SetVerticalAlign", "Set the vertical alignement of the text.");
	METHOD_DESCRIPTOR__AddParameter(pM, "VAlign", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_SET_VERTICAL_ALIGN, pM, EM_SetVerticalAlign);

	pM = METHOD_DESCRIPTOR__Allocate("ChangeTextColor", "Change the color of the text in the ihm_element. Works only if there is no color tags in the text.");
	METHOD_DESCRIPTOR__AddParameter(pM, "Color", "Ask 3D Engine to document this parameter.", TYPE_COLOR::GetDescriptor());
	pCD->AddMethod(CMD_CHANGE_TEXT_COLOR, pM, EM_ChangeTextColor);

	pM = METHOD_DESCRIPTOR__Allocate("Scroll", "Scroll the text within the ihm_elementtext.");
	METHOD_DESCRIPTOR__AddParameter(pM, "rScrollPercent", "If set to 0, the text is at its top, and if 'nScrollPercent' is set to 100, the bottom of the text gets visible. Please alse note that negative percentage are allowed. So are ones larger than 100.", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SCROLL, pM, EM_Scroll);

	pM = METHOD_DESCRIPTOR__Allocate("SetScrollSpeed", "Sets the speed the ihm_elementtext scrolls.");
	METHOD_DESCRIPTOR__AddParameter(pM, "rScrollPercent", "Default is 1.0.", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_SCROLL_SPEED, pM, EM_SetScrollSpeed);

	pM = METHOD_DESCRIPTOR__Allocate("ScrollUp", "Scroll up the text in the ihm_elementtext. If the text fits entirely, does nothing.");
	pCD->AddMethod(CMD_SCROLL_UP, pM, EM_ScrollUp);

	pM = METHOD_DESCRIPTOR__Allocate("ScrollDown", "Scroll down the text in the ihm_elementtext. If the text fits entirely, does nothing.");
	pCD->AddMethod(CMD_SCROLL_DOWN, pM, EM_ScrollDown);

	pM = METHOD_DESCRIPTOR__Allocate("Copy", "Copy the IHM_ELEMENT.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "IhmElement", "The IhmElement to copy.", IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_COPY, pM, EM_Copy);

	pM = METHOD_DESCRIPTOR__Allocate("GetNbrLines", "Returns the numbers of lines of the text.");
	METHOD_DESCRIPTOR__SetReturn(pM, "nNbrLines", "The lines count.", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_GET_NBR_LINES, pM, EM_GetNbrLines);

	pM = METHOD_DESCRIPTOR__Allocate("GetTextBoundingRect", "Returns the smallest rectable in which the displayed text can fit.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rLeft",		"Left position of the rectangle.",		TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rRight",		"Right position of the rectangle.",		TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rTop",		"Top position of the rectangle.",		TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "rBottom",		"Bottom position of the rectangle.",	TYPE_FLOAT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "bRelative",	"If 'true', you'll get screen percentages, else pixel coordinates.", TYPE_BOOL::GetDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pM, "bOk", "'false' if failed (for exemple on an empty text).", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GET_TEXT_BOUNDING_RECT, pM, EM_GetTextBoundingRect);

#ifndef _MASTER
	pCD->AddEnum("VALIGN_CENTERED", VALIGN_CENTERED, "...");
	pCD->AddEnum("VALIGN_TOP",		VALIGN_TOP, "...");
	pCD->AddEnum("VALIGN_BOTTOM",	VALIGN_BOTTOM, "...");
#endif
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_Construct(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	IHM_ELEMENT_TEXT *	pObject;
	pObject = static_cast<IHM_ELEMENT_TEXT *>(pO);
	QDT_NEW_PLACED(pObject, IHM_ELEMENT_TEXT)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_Destruct(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	IHM_ELEMENT_TEXT *	pObject;
	pObject = static_cast<IHM_ELEMENT_TEXT*> (pO);
	pObject->IHM_ELEMENT_TEXT::~IHM_ELEMENT_TEXT();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetText
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_GetText(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE *			pObjet;	
	IHM_ELEMENT_TEXT *		pIhmElementText;
	QDT_STRING *			pText;

	QDT_ASSERT(pR);

	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementText);

	pText = (QDT_STRING *) pR;
	QDT_NEW_PLACED(pText, QDT_STRING)();

	*pText = pIhmElementText->GetText();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetText
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_SetText(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TEXT *		pIhmElementText;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementText);

	QDT_STRING * sText = (QDT_STRING *) pP;
	pIhmElementText->SetText(*sText);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetFont
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_GetFont(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TEXT *		pIhmElementText;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementText);

	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetFont
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_SetFont(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE *		pObjet;
	IHM_ELEMENT_TEXT *	pIhmElementText;
	COM_INSTANCE *		pParam;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementText);

	pParam = (COM_INSTANCE *) pP;
	pIhmElementText->SetFont( *pParam );
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetVerticalAlign
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_SetVerticalAlign(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TEXT *		pIhmElementText;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementText);

	int * nAlign = (int *) pP;

	pIhmElementText->SetVerticalAlignement( (VALIGN)*nAlign );
}

//-----------------------------------------------------------------------------
//	Name:		EM_ChangeTextColor
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_ChangeTextColor(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TEXT *		pIhmElementText;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementText);

	COLOR * pColor = (COLOR *) pP;
	pIhmElementText->ChangeTextColor(*pColor);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Scroll
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_Scroll(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TEXT *		pIhmElementText;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementText);

	float * pScrollValue = (float *) pP;
	pIhmElementText->Scroll(NULL, *pScrollValue);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetScrollSpeed
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_SetScrollSpeed(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TEXT *		pIhmElementText;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementText);

	float * pScrollSpeed = (float *) pP;
	pIhmElementText->SetScrollSpeed(*pScrollSpeed);
}

//-----------------------------------------------------------------------------
//	Name:		EM_ScrollUp
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_ScrollUp(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TEXT *		pIhmElementText;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementText);

	pIhmElementText->ScrollUp(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		EM_ScrollDown
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_ScrollDown(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENT_TEXT *		pIhmElementText;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementText);

	pIhmElementText->ScrollDown(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Copy
//	Object:		
//	02-08-22:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_Copy(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE *	pObject;
	COM_INSTANCE **	pParameter;

	pObject = (COM_INSTANCE*)pO;
	pParameter = (COM_INSTANCE**)pP;

	*pObject = **pParameter;
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetNbrLines
//	Object:		
//	04-10-13:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_GetNbrLines(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_TEXT *	pIhmElementText;
	int *				pNbrLines;
	
	pNbrLines		= (int *) pR;
	pObjet			= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementText);

	*pNbrLines = pIhmElementText->GetNbrLines();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetTextBoundingRect
//	Object:		
//	04-11-23:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::EM_GetTextBoundingRect(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_TEXT *	pIhmElementText;
	struct	PARAMS
	{
		float *	_rLeft;
		float *	_rRight;
		float *	_rTop;
		float *	_rBottom;
		bool	_bRelative;
	};
	PARAMS *			pParams;
	bool *				pReturn;
	
	pObjet			= (COM_INSTANCE *) pO;
	pIhmElementText	= (IHM_ELEMENT_TEXT *) pObjet->GetCommunicator();
	pReturn			= (bool *) pR;
	pParams			= (PARAMS *) pP;

	*pReturn = pIhmElementText->GetTextBoundingRect( NULL,
													 *pParams->_rLeft,
													 *pParams->_rRight,
													 *pParams->_rTop,
													 *pParams->_rBottom,
													 pParams->_bRelative );
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-21:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_TEXT::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("IHM_ELEMENT_TEXT"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT, 
										QDT_COMMENT("162017C8-CB6C-F6AC-5269-96BEF31BD1F1"),
										sizeof(IHM_ELEMENT_TEXT), 
										Init, 
										QDT_COMMENT("This ihm_element displays a text into the display window. If the element has no font set, the text can't be displayed. Please refer to PA010293.doc to get more informations on ihm_element_text. By default, text of the element is left aligned horizontally and centered vertically."),
										IHM_ELEMENT_RECT::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		GetTextBoundingRect
//	Object:		
//	04-11-23:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT::GetTextBoundingRect(VIEWPORT *	pRenderingViewport,
											  float &		rLeft,
											  float &		rRight,
											  float &		rTop,
											  float &		rBottom,
											  bool			bRelative)
{
	return ( _pTextParser->GetTextBoundingRect( pRenderingViewport, rLeft, rRight, rTop, rBottom, bRelative, true ) );
}

//-----------------------------------------------------------------------------
//	Name:		IsTextCropped
//	Object:		
//	04-11-25:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT::IsTextCropped(VIEWPORT *	pRenderingViewport)
{
	return ( _pTextParser->IsTextCropped( pRenderingViewport ) );
}

//-----------------------------------------------------------------------------
//	Name:		ProcessParser
//	Object:		
//	03-12-01:	NCA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::ProcessParser(VIEWPORT *	pRenderingViewport)
{
	_pTextParser->Process( pRenderingViewport );
}

//-----------------------------------------------------------------------------
//	Name:		ReserveParser
//	Object:		
//	05-01-14:	VMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT::ReserveParser()
{
	TEXT_PARSER*		pTextParser;
	IHM_WORD*			pIhmWorld;
	IHM_TEXT_LINE*		pIhmTextLine;

	pTextParser = new TEXT_PARSER( NULL );
	pIhmWorld = pTextParser->AllocateWord();
	pTextParser->UnallocateWord(pIhmWorld );
	pIhmTextLine = pTextParser->AllocateTextLine();
	pTextParser->UnallocateTextLine(pIhmTextLine );
	delete pTextParser;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

