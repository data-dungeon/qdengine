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
// CLASS: IOUTPUT
//
//
//01-04-23 FKE Created: 
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Core/IOutput)

//=============================================================================
// CODE STARTS HERE
//=============================================================================
I_OUTPUT *I_OUTPUT::_pIOutput[I_OUTPUT::MAX_IOUTPUT];
int I_OUTPUT::_nNbIOutput = 0;

//-----------------------------------------------------------------------------
// Name: I_OUTPUT::AddToList
// Object: 
//01-04-23 FKE Created: 
//-----------------------------------------------------------------------------
void	I_OUTPUT::AddToList(I_OUTPUT *pIOutput)
{
	_pIOutput[_nNbIOutput] = pIOutput;
	_nNbIOutput++;
}

//-----------------------------------------------------------------------------
// Name: I_OUTPUT::RemoveFromList
// Object: 
//01-04-23 FKE Created: 
//-----------------------------------------------------------------------------
void	I_OUTPUT::RemoveFromList(I_OUTPUT *pOutput)
{
	int i;
	for (i = 0;i < _nNbIOutput;i++)
	{
		if (pOutput == _pIOutput[i])
		{
			break;
		}
	}
	i++;
	for (;i < _nNbIOutput;i++)
	{
		_pIOutput[i - 1] = _pIOutput[i];
	}
	_nNbIOutput--;
}

//-----------------------------------------------------------------------------
// Name: I_OUTPUT::Push
// Object: 
//01-04-23 FKE Created: 
//-----------------------------------------------------------------------------
void I_OUTPUT::Push(const char *pcString, const bool bIsWarning)
{
	int i;
	for (i = 0;i < _nNbIOutput;i++)
	{
		_pIOutput[i]->Output(pcString, bIsWarning);
	}
}

#if defined(PLATFORM_PC)
///#ifdef _DEBUG
class DPF : public I_OUTPUT
{
public: 
	DPF() {I_OUTPUT::AddToList(this);}
	~DPF() {I_OUTPUT::RemoveFromList(this);}
	void Output(const char *pcString, const bool bIsWarning) {QDT_OutputDebugString(pcString);QDT_OutputDebugString("\n");}
};
static DPF dpf;
#elif defined(PLATFORM_XBOX)
class DPF : public I_OUTPUT
{
public: 
			DPF()							{ I_OUTPUT::AddToList(this); }
			~DPF()							{ I_OUTPUT::RemoveFromList(this); }
	void	Output(const char *pcString, const bool bIsWarning)	{ OutputDebugString(pcString);OutputDebugString("\n"); }
};
static DPF dpf;

#endif
//#endif
//=============================================================================
// CODE ENDS HERE
//=============================================================================
