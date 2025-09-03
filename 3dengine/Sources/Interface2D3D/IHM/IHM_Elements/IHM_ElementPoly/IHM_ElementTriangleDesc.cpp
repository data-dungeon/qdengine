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
//	CLASS:	IHM_ELEMENT_TRIANGLE_DESC
//
//	01-09-28:	ELE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"..\Include.h"
#include	INC_3DENGINE(M3D)
#include	INC_KRNCOM(OldCom/Communication)

#ifndef _DEBUG
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point2D)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

IHM_ELEMENT_TRIANGLE_DESC*	IHM_ELEMENT_TRIANGLE_DESC::_pInstance      = NULL;
bool						IHM_ELEMENT_TRIANGLE_DESC::_bDestroyedOnce = false;
static const char * const	pcComment = "Ask 3D Engine to document this class.";

//-----------------------------------------------------------------------------
//	Name: IHM_ELEMENT_TRIANGLE_DESC constructor
//	Object: 
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_TRIANGLE_DESC::IHM_ELEMENT_TRIANGLE_DESC()
:
IHM_ELEMENT_POLY_ABC_DESC(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_ELEMENT_TRIANGLE, "ihm_elementtriangle", NB_COMMANDS, pcComment)
{
	const int nNbrCommandsElementPolyABC = IHM_ELEMENT_POLY_ABC_DESC::NB_COMMANDS;

	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 0 ].Init( SET_POINTS , 3, "SetPoints", "Set relative coordinates of the points." );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 0 ].SetParameterDesc( 0 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::IN_PARAMETER, "Point1" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 0 ].SetParameterDesc( 1 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::IN_PARAMETER, "Point2" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 0 ].SetParameterDesc( 2 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::IN_PARAMETER, "Point3" );

	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 1 ].Init( SET_COLORS , 3, "SetColors", "Set colors of the points." );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 1 ].SetParameterDesc( 0 , OTYPE_KERNEL_COLOR, 1, PARAMETER_DESC::IN_PARAMETER, "Color1" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 1 ].SetParameterDesc( 1 , OTYPE_KERNEL_COLOR, 1, PARAMETER_DESC::IN_PARAMETER, "Color2" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 1 ].SetParameterDesc( 2 , OTYPE_KERNEL_COLOR, 1, PARAMETER_DESC::IN_PARAMETER, "Color3" );

	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 2 ].Init( SET_UVS , 3, "SetMappingCoordinates", "Set mapping coordinates of each point." );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 2 ].SetParameterDesc( 0 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::IN_PARAMETER, "UV1" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 2 ].SetParameterDesc( 1 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::IN_PARAMETER, "UV2" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 2 ].SetParameterDesc( 2 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::IN_PARAMETER, "UV3" );

	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 3 ].Init( GET_POINTS , 3, "GetPoints", "Get relative coordinates of the points." );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 3 ].SetParameterDesc( 0 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::OUT_PARAMETER, "Point1" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 3 ].SetParameterDesc( 1 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::OUT_PARAMETER, "Point2" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 3 ].SetParameterDesc( 2 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::OUT_PARAMETER, "Point3" );

	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 4 ].Init( GET_COLORS , 3, "GetColors", "Get colors of the points." );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 4 ].SetParameterDesc( 0 , OTYPE_KERNEL_COLOR, 1, PARAMETER_DESC::OUT_PARAMETER, "Color1" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 4 ].SetParameterDesc( 1 , OTYPE_KERNEL_COLOR, 1, PARAMETER_DESC::OUT_PARAMETER, "Color2" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 4 ].SetParameterDesc( 2 , OTYPE_KERNEL_COLOR, 1, PARAMETER_DESC::OUT_PARAMETER, "Color3" );

	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 5 ].Init( GET_UVS , 3, "GetMappingCoordinates", "Get mapping coordinates of each point." );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 5 ].SetParameterDesc( 0 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::OUT_PARAMETER, "UV1" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 5 ].SetParameterDesc( 1 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::OUT_PARAMETER, "UV2" );
	_pCommandDescsTab[ nNbrCommandsElementPolyABC + 5 ].SetParameterDesc( 2 , OTYPE_KERNEL_POINT2D, 1, PARAMETER_DESC::OUT_PARAMETER, "UV3" );
}


//-----------------------------------------------------------------------------
//	Name: IHM_ELEMENT_TRIANGLE_DESC destructor
//	Object: 
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_TRIANGLE_DESC::~IHM_ELEMENT_TRIANGLE_DESC()
{

}


//-----------------------------------------------------------------------------
//	Name: IHM_ELEMENT_TRIANGLE_DESC::New
//	Object: 
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
COM_OBJECT *	IHM_ELEMENT_TRIANGLE_DESC::New()
{
	_nNbObjects++;

	return (new IHM_ELEMENT_TRIANGLE);
}


//-----------------------------------------------------------------------------
//	Name: IHM_ELEMENT_TRIANGLE_DESC::Delete
//	Object: 
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TRIANGLE_DESC::Delete(COM_OBJECT *	pC)
{
	_nNbObjects--;
	if ( _nNbObjects == 0 )
	{
		delete pC;
	}
}


//-----------------------------------------------------------------------------
//	Name: IHM_ELEMENT_TRIANGLE_DESC::Instance
//	Object: 
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
DESC* IHM_ELEMENT_TRIANGLE_DESC::Instance()
{
	QDT_ASSERT( _bDestroyedOnce == false );

	if ( _pInstance == NULL )
	{
		_pInstance = new IHM_ELEMENT_TRIANGLE_DESC();
	}
	
	return ( _pInstance );
}


//-----------------------------------------------------------------------------
//	Name: IHM_ELEMENT_TRIANGLE_DESC::Suicide
//	Object: 
//	01-09-27:	ELE - Created
//-----------------------------------------------------------------------------
void IHM_ELEMENT_TRIANGLE_DESC::Suicide()
{
	QDT_ASSERT( _bDestroyedOnce == false );
	QDT_ASSERT( _pInstance != NULL );
	
	if ( _pInstance != NULL )
	{
		delete _pInstance;
		_pInstance		 = NULL;
		_bDestroyedOnce	 = false;
	}
}

//-----------------------------------------------------------------------------
//	Name: IHM_ELEMENT_TRIANGLE_DESC::ReceiveCommandProc
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TRIANGLE_DESC::ReceiveCommandProc(void * pObject,
													  COMMAND_ID CommandID,
													  PARAMETER_STACK &	Stack) const
{
	int		i;
	POINT2D Points[4];
	COLOR	Colors[4];

	IHM_ELEMENT_TRIANGLE *pElement = static_cast<IHM_ELEMENT_TRIANGLE*> (pObject);	
	QDT_ASSERT( pElement );
	if ( !pElement )
	{
		return ( false );
	}

	switch(CommandID)
	{
		case SET_POINTS:
			{
				for	(i = 0 ; i < 3 ; ++i)
				{
					Points[i] = Stack.GetPoint2D(i);
				}
				pElement->SetVertices(Points);
			}
			break;

		case SET_COLORS:
			{
				for	(i = 0 ; i < 3 ; ++i)
				{
					Colors[i] = Stack.GetColor(i);
				}
				pElement->SetColors(Colors);
			}
			break;

		case SET_UVS:
			{
				for	(i = 0 ; i < 3 ; ++i)
				{
					Points[i] = Stack.GetPoint2D(i);
				}
				pElement->SetUVs(Points);
			}
			break;

		case GET_POINTS:
			{
				for	(i = 0 ; i < 3 ; ++i)
				{
					Stack.SetPoint2D(i, pElement->GetPoint(i));
				}
			}
			break;

		case GET_COLORS:
			{
				for	(i = 0 ; i < 3 ; ++i)
				{
					Stack.SetColor(i, pElement->GetColor(i));
				}
			}
			break;

		case GET_UVS:
			{
				for	(i = 0 ; i < 3 ; ++i)
				{
					Stack.SetPoint2D(i, pElement->GetUV(i));
				}
			}
			break;

		// Default.
		default:
			return ( IHM_ELEMENT_POLY_ABC_DESC::ReceiveCommandProc( pObject, CommandID, Stack ) );
	}

	return	(true);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
