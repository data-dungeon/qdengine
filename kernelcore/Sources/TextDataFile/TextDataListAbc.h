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
//	CLASS:	TEXT_DATA_LIST_ABC
//	The TEXT_DATA_LIST_ABC class implements ...
//
//	05-05-31:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_TEXTDATAFILE_TEXTDATALISTABC_H__
#define __KCORE_TEXTDATAFILE_TEXTDATALISTABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Tools/QdtString)

namespace	QDT
{
	namespace KCORE
	{
		class	TEXT_DATA_LIST_ABC
		{
		public:
										TEXT_DATA_LIST_ABC();
			explicit					TEXT_DATA_LIST_ABC(const TEXT_DATA_LIST_ABC & C);
			virtual						~TEXT_DATA_LIST_ABC();
			TEXT_DATA_LIST_ABC &		operator=(const TEXT_DATA_LIST_ABC & C);

			virtual void				Copy(const TEXT_DATA_LIST_ABC & C);

			virtual int					GetCount() = 0;								//renvoi le nombre d'éléments contenus
			virtual QDT_STRING			GetName(int nIndex) = 0;					//renvoi le nom du nœud a l'index nIndex. Si cet objet est un nœud de type NODE_ELEMENT, on renverra la valeur de l'attribut nodeName du XMLNode. Si il est du type NODE_TEXT, NODE_COMMENT ou d'autre types n'ayant pas de fils, on renverra la valeur de l'attribut text du XMLNode.
			virtual QDT_STRING			GetText(int nIndex) = 0;					//renvoi le text du nœud a l'index nIndex. Si cet objet a des fils, ca renvoi la concaténation de tous les textes des fils. C'est reelement pratique que si le noeud n'a qu'un seul fils de tytpe Type ou comment
			virtual bool				HasChild(int nIndex) = 0;					//renvoi true/false si le nœud a des fils ou pas.
			virtual TEXT_DATA_LIST_ABC*	GetChildList(int nIndex) = 0;				//renvoi un object TEXT_DATA_LIST_ABC contenant la liste des nœuds fils du nœud nIndex.
			virtual bool				GetFirstName(QDT_STRING &sName) = 0;		//renvoi le premier nom. Retourne false si il n'y en a pas
			virtual bool				GetNextName(QDT_STRING &sName) = 0;			//renvoi le nom suivant. Retourne false si il n'y en a pas

		protected:

		private:

		};
	}
}

#endif //__KCORE_TEXTDATAFILE_TEXTDATALISTABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
