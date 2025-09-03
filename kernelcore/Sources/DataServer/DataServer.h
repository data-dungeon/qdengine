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
//	CLASS:	DATA_SERVER
//	The DATA_SERVER class implements ...
//
//	05-02-16:	AMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_DATASERVER_DATASERVER_H__
#define __KCORE_DATASERVER_DATASERVER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Tools/QdtString)
#include INCL_KCORE(Template/QDT_Vector)
#include INCL_KCORE(Template/Singleton)

namespace	QDT
{
	namespace KCORE
	{
		class	DATA_SERVER : public SINGLETON<DATA_SERVER>
		{
		public:
			static const char * DEFAULT_INI;

			DATA_SERVER(){};
			~DATA_SERVER(){};
			void					Init(const char* pIniFile = NULL);
			int						GetNbrDatabase();
			const QDT_STRING	&	GetDatabaseName(int nIndex);
			bool					IsExportDatabase(int nIndex);

		private:

			struct SERVER_INFO
			{
				bool			bExport;
				QDT_STRING		sName;
			};

			QDT_VECTOR<SERVER_INFO>	_vDatabase;
		};
	}
}

#endif //__KCORE_DATASERVER_DATASERVER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
