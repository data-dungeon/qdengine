#ifndef _MEM_LIBIDINTERFACE_H_
#define _MEM_LIBIDINTERFACE_H_

#define ADD_TAG(a) MEM_LIBID_##a,

enum MEM_LIB_ID
{			
	MEM_LIBID_DEFAULT = 0, // All non tagged code will default to this
	MEM_LIBID_UNLISTED = 1, // Non listed

	#include	INCL_KCORE(Mem/MemLibId)

	MEM_LIBID_MAX
};

#undef START_TAG
#undef ADD_TAG
#undef END_TAG

#define _iNumMaxLibs MEM_LIBID_MAX

#endif // _MEM_LIBIDINTERFACE_H_


