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
//	CLASS:	IHM_ELEMENT_POLY_ABC_DESC
//
//	01-09-27:	ELE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"..\Include.h"
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_3DENGINE(M3D)
#include	INC_3DENGINE(Display)
#include	INC_KRNCOM(OldCom/Communication)
#include	INC_KRNCORE(Display\RenderManager)

#ifndef _DEBUG
	#include	INL_3DENGINE(Display)
#endif
//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name: IHM_ELEMENT_POLY_ABC_DESC constructor
//	Object: 
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_POLY_ABC_DESC::IHM_ELEMENT_POLY_ABC_DESC(OBJECT_TYPE		Type,
													 const char *		pcName,
													 int				nNbrCommands,
													 const char * const	pcComment)
:
IHM_ELEMENT_ABC_DESC( Type, pcName, false, nNbrCommands, pcComment )
{
	const int nNbrCommandsElementABC = IHM_ELEMENT_ABC_DESC::NB_COMMANDS;

	_pCommandDescsTab[ nNbrCommandsElementABC + 0 ].Init( SET_ZDEPTH , 1, "SetZDepth", "Set the z depth of the element." );
	_pCommandDescsTab[ nNbrCommandsElementABC + 0 ].SetParameterDesc( 0 , OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "ZDepth" );

	_pCommandDescsTab[ nNbrCommandsElementABC + 1 ].Init( UNDER_2D_INTERFACE , 1, "Under2DInterface", "Set the position state of the element." );
	_pCommandDescsTab[ nNbrCommandsElementABC + 1 ].SetParameterDesc( 0 , OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bUnder" );

	_pCommandDescsTab[ nNbrCommandsElementABC + 2 ].Init( SET_TRANSPARENCY_MODE , 1, "SetTransparencyMode", "Set the transparency mode." );
	_pCommandDescsTab[ nNbrCommandsElementABC + 2 ].SetParameterDesc( 0 , OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "TransparencyMode.", "0: no transparency. 1: alpha transparency. 2: additive transparency." );

	_pCommandDescsTab[ nNbrCommandsElementABC + 3 ].Init( SET_TEXTURE , 1, "SetTexture", "Set the texture to be mapped on the element." );
	_pCommandDescsTab[ nNbrCommandsElementABC + 3 ].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Texture" );

	_pCommandDescsTab[ nNbrCommandsElementABC + 4 ].Init( SET_MATERIAL , 1, "SetMaterial", "Set the material to be used on the element." );
	_pCommandDescsTab[ nNbrCommandsElementABC + 4 ].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Material" );
}

//-----------------------------------------------------------------------------
//	Name: IHM_ELEMENT_POLY_ABC_DESC destructor
//	Object:
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_POLY_ABC_DESC::~IHM_ELEMENT_POLY_ABC_DESC()
{

}

//-----------------------------------------------------------------------------
//	Name: IHM_ELEMENT_POLY_ABC_DESC::ReceiveCommandProc
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_POLY_ABC_DESC::ReceiveCommandProc(void *			pObject,
													  COMMAND_ID		CommandID,
													  PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENT_POLY_ABC *pElement = static_cast<IHM_ELEMENT_POLY_ABC*> (pObject);	
	QDT_ASSERT( pElement );
	if ( !pElement )
	{
		return ( false );
	}

	switch(CommandID)
	{
		case SET_ZDEPTH:
			{
				pElement->SetZDepth(Stack.GetFloat(0));

				if	(DISPLAY_MANAGER::Instance()->RemoveIhmElement(pElement->GetIndex()))
				{
					DISPLAY_MANAGER::Instance()->AddIhmElement(pElement->GetIndex());
				}
			}
			break;

		case UNDER_2D_INTERFACE:
			{
				bool	bUnder = Stack.GetBool(0);

				if	(bUnder != pElement->IsUnder2DInterface())
				{
					pElement->SetUnder2DInterface(bUnder);

					if	(DISPLAY_MANAGER::Instance()->RemoveIhmElement(pElement->GetIndex()))
					{
						DISPLAY_MANAGER::Instance()->AddIhmElement(pElement->GetIndex());
					}
				}
			}
			break;

		case SET_TRANSPARENCY_MODE:
			{
				pElement->SetTransparencyMode(static_cast<IHM_ELEMENT_POLY_ABC::TRANSPARENCY_MODE>(Stack.GetInteger(0)));
			}
			break;

		case SET_TEXTURE:
		{
			const OBJECT_ID &Id = Stack[0];

			if (Id.GetType() != OTYPE_KERNEL_TEXTURE_NEW)
			{
				QDT_Warning("Command aborted : the parameter is not a texture");
				break;
			}

			INDEX_INT *pTextureIndex = DATABASE_MANAGER::Instance()->GetById(Id.GetType(), Id.GetID());
			
			if(!pTextureIndex)
			{
				QDT_Message("Command aborted : The texture has not been found");
				break;
			}

			pElement->SetTextureIndex(pTextureIndex);
		}
		break;

		// Default.
		default:
			return ( IHM_ELEMENT_ABC_DESC::ReceiveCommandProc( pObject, CommandID, Stack ) );
	}

	return	(true);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
