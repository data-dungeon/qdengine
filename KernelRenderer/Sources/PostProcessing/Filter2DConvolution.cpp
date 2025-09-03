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
//	CLASS:	FILTER_2DCONVOLUTION
//
//	06-01-16:	PDE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include "Root.h"
#include "Filter2DConvolution.h"
#include INCL_KRENDERER(Renderer\RenderContext)
#include INCL_KRENDERER(Renderer\IVertexShader)
#include INCL_KRENDERER(Renderer\IPixelShader)
#include INCL_KRENDERER(Renderer\ITexture)
#include INCL_KRENDERER(Renderer\IRenderTarget)
#include INCL_KRENDERER(PostProcessing\Filter2D)
#include INCL_KMATH(Math\Math)

#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

#include "Shaders/Filter2D_vs.h"
#include "Shaders/Filter2D_fixed_vs.h"
#include "Shaders/Convolution8_ps.h"
#include "Shaders/Convolution16_ps.h"
#include "Shaders/Convolution8_fixed_ps.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
Int32			 FILTER_2DCONVOLUTION::g_refcount = 0;

I_PIXEL_SHADER*  FILTER_2DCONVOLUTION::g_Convolution8;
I_PIXEL_SHADER*  FILTER_2DCONVOLUTION::g_Convolution16;
I_PIXEL_SHADER*  FILTER_2DCONVOLUTION::g_Convolution8_fixed;

I_PIXEL_SHADER*  FILTER_2DCONVOLUTION::g_Convolution8_square;
I_PIXEL_SHADER*  FILTER_2DCONVOLUTION::g_Convolution16_square;
I_PIXEL_SHADER*  FILTER_2DCONVOLUTION::g_Convolution8_fixed_square;

I_VERTEX_SHADER* FILTER_2DCONVOLUTION::g_VertexShader;
I_VERTEX_SHADER* FILTER_2DCONVOLUTION::g_VertexShader_fixed;

//-----------------------------------------------------------------------------
//	Name:		FILTER_2DCONVOLUTION constructor
//	Object:		
//	06-01-16:	PDE - Created
//-----------------------------------------------------------------------------
FILTER_2DCONVOLUTION::FILTER_2DCONVOLUTION() 
: 
FILTER_2D()
{
	_PixelShaderOverriden = false;
	_UseNoweights = false;
	_UseNoOffsets = false;

	if (!g_refcount)
	{
		g_Convolution8       = QDT_CREATE_PIXELSHADER(Convolution8_ps);
		g_Convolution16      = QDT_CREATE_PIXELSHADER(Convolution16_ps);
		g_Convolution8_fixed = QDT_CREATE_PIXELSHADER(Convolution8_fixed_ps);

		switch (RENDER_CONTEXT::GetRenderContextType())
		{
		default:
			QDT_FAIL();

		case RENDER_CONTEXT::RCT_PSGL:
		case RENDER_CONTEXT::RCT_DIRECT3D9:
			g_Convolution8_square       =  g_Convolution8;
			g_Convolution8_fixed_square =  g_Convolution8_fixed;
			g_Convolution16_square		=  g_Convolution16;
			break;

		case RENDER_CONTEXT::RCT_OPENGL:
		case RENDER_CONTEXT::RCT_OPENGL_FOR_MAYA:
#if defined (PLATFORM_PC)
			g_Convolution8_square       =  QDT_CREATE_PIXELSHADER(Convolution8_ps_NORECT);
			g_Convolution8_fixed_square =  QDT_CREATE_PIXELSHADER(Convolution8_fixed_ps_NORECT);
			g_Convolution16_square      =  QDT_CREATE_PIXELSHADER(Convolution16_ps_NORECT);
#endif			
			break;
		}

		g_VertexShader       = QDT_CREATE_VERTEXSHADER(Filter2D_vs);
		g_VertexShader_fixed = QDT_CREATE_VERTEXSHADER(Filter2D_fixed_vs);
	}

	_FixedKernel    = false;

	g_refcount++;
}

//-----------------------------------------------------------------------------
//	Name:		FILTER_2DCONVOLUTION destructor
//	Object:		
//	06-01-16:	PDE - Created
//-----------------------------------------------------------------------------
FILTER_2DCONVOLUTION::~FILTER_2DCONVOLUTION()
{
	g_refcount--;

	if (!g_refcount)
	{  
		if (g_Convolution8_square != g_Convolution8)
		{
			RENDER_CONTEXT::ReleasePixelShader(g_Convolution8_square);
			RENDER_CONTEXT::ReleasePixelShader(g_Convolution8_fixed_square);
			RENDER_CONTEXT::ReleasePixelShader(g_Convolution16_square);
		}

		RENDER_CONTEXT::ReleasePixelShader(g_Convolution8);
		RENDER_CONTEXT::ReleasePixelShader(g_Convolution8_fixed);
		RENDER_CONTEXT::ReleasePixelShader(g_Convolution16);

		RENDER_CONTEXT::ReleaseVertexShader(g_VertexShader);
		RENDER_CONTEXT::ReleaseVertexShader(g_VertexShader_fixed);
	}

	_FixedKernel    = false;
}


void FILTER_2DCONVOLUTION::SetOffsets(VECTOR_2* offsets)
{
	QDT_MEMCOPY(_Offsets, offsets, sizeof(_Offsets));
}

void FILTER_2DCONVOLUTION::SetWeights(VECTOR* coeffs)
{
	QDT_MEMCOPY(_Weights, coeffs, sizeof(_Weights));
}
		
void FILTER_2DCONVOLUTION::EnableFixedKernel(Bool value)
{
	_FixedKernel = value;
}

void FILTER_2DCONVOLUTION::ScaleKernel(Float32 scaleX, Float32 scaleY)
{
	Float32 *off = _Offsets;

	for (UInt32 i = 0; i < _numSample; i++)
	{
		*(off++) *= scaleX;
		*(off++) *= scaleY;
	}
}

void FILTER_2DCONVOLUTION::TranslateKernel(Float32 trX, Float32 trY)
{
	Float32 *off = _Offsets;

	for (UInt32 i = 0; i < _numSample; i++)
	{
		*(off++) += trX;
		*(off++) += trY;
	}
}

void FILTER_2DCONVOLUTION::InitCircleBlur(Float32 radius)
{
	EnableFixedKernel(true);
	SetNumSample(8);

	_Offsets[0] =    0.0f; _Offsets[1] = -radius;
	_Offsets[2] = -radius; _Offsets[3] =    0.0f;
	_Offsets[4] =    0.0f; _Offsets[5] =  radius;
	_Offsets[6] =  radius; _Offsets[7] =    0.0f;
	Float32 diag = radius * 0.8f;
	_Offsets[8]  =  diag; _Offsets[1] =  diag;
	_Offsets[10] =  diag; _Offsets[1] = -diag;
	_Offsets[12] = -diag; _Offsets[1] =  diag;
	_Offsets[14] = -diag; _Offsets[1] = -diag;

	for (UInt32 i = 0; i < 8 * 4; i++)
	{
		_Weights[i] = 1.0f / 8.0f;
	}
}

void FILTER_2DCONVOLUTION::InitDirectionalGaussianBlur(Float32 DirX, Float32 DirY)
{
	EnableFixedKernel(true);
	SetNumSample(8);

	Float32 stepX = DirX / 7.0f;
	Float32 stepY = DirY / 7.0f;
	Float32 startX = -DirX / 2.0f;
	Float32 startY = -DirY / 2.0f;


	Float32* offset = _Offsets;

	for (int i = 0; i < 8; i++)
	{
		*(offset++) = startX + i * stepX;
		*(offset++) = startY + i * stepY;
	}

	static Float32 GaussianWeights[8 * 4] =
	{
		0.04f, 0.04f, 0.04f, 0.04f,
		0.10f, 0.10f, 0.10f, 0.10f,
		0.15f, 0.15f, 0.15f, 0.15f,
		0.21f, 0.21f, 0.21f, 0.21f,
		0.21f, 0.21f, 0.21f, 0.21f,
		0.15f, 0.15f, 0.15f, 0.15f,
		0.10f, 0.10f, 0.10f, 0.10f,
		0.04f, 0.04f, 0.04f, 0.04f
	};

	QDT_MEMCOPY(_Weights, GaussianWeights, sizeof(GaussianWeights));
}


//-----------------------------------------------------------------------------
//	Name:		InitDirectionalGaussianBlur16
//	Object:		
//	06-02-27:	PDE - Created
//-----------------------------------------------------------------------------
void	FILTER_2DCONVOLUTION::InitDirectionalGaussianBlur16(Float32	DirX,
															Float32	DirY)
{
	EnableFixedKernel(false);
	SetNumSample(16);

	Float32 stepX = DirX / 15.0f;
	Float32 stepY = DirY / 15.0f;
	Float32 startX = -DirX / 2.0f;
	Float32 startY = -DirY / 2.0f;


	Float32* offset = _Offsets;

	for (int i = 0; i < 16; i++)
	{
		*(offset++) = startX + i * stepX;
		*(offset++) = startY + i * stepY;
	}

	static Float32 GaussianWeights[16 * 4] =
	{
		0.025083612f, 0.025083612f, 0.025083612f, 0.025083612f,
		0.036789298f, 0.036789298f, 0.036789298f, 0.036789298f, 
		0.050501672f, 0.050501672f, 0.050501672f, 0.050501672f,
		0.063210702f, 0.063210702f, 0.063210702f, 0.063210702f, 
		0.073913043f, 0.073913043f, 0.073913043f, 0.073913043f, 
		0.080936455f, 0.080936455f, 0.080936455f, 0.080936455f,
		0.084280936f, 0.084280936f, 0.084280936f, 0.084280936f,
		0.085284281f, 0.085284281f, 0.085284281f, 0.085284281f,
		0.085284281f, 0.085284281f, 0.085284281f, 0.085284281f,
		0.084280936f, 0.084280936f, 0.084280936f, 0.084280936f,
		0.080936455f, 0.080936455f, 0.080936455f, 0.080936455f,
		0.073913043f, 0.073913043f, 0.073913043f, 0.073913043f, 
		0.063210702f, 0.063210702f, 0.063210702f, 0.063210702f, 
		0.050501672f, 0.050501672f, 0.050501672f, 0.050501672f,
		0.036789298f, 0.036789298f, 0.036789298f, 0.036789298f,
		0.025083612f, 0.025083612f, 0.025083612f, 0.025083612f
	};

	QDT_MEMCOPY(_Weights, GaussianWeights, sizeof(GaussianWeights));
}



void FILTER_2DCONVOLUTION::InitStarBranch(Float32 DirX, Float32 DirY, Float32 Diffraction)
{
	EnableFixedKernel(true);
	SetNumSample(8);

	Float32 stepX = DirX / 7.0f;
	Float32 stepY = DirY / 7.0f;


	Float32* offset = _Offsets;

	for (int i = 0; i < 8; i++)
	{
		*(offset++) = i * stepX;
		*(offset++) = i * stepY;
	}

	static Float32 RainBow[8 * 3] =
	{
		0.0f, 1.0f, 0.8f, 
		0.0f, 0.2f, 1.0f, 
		0.58f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 
		1.0f, 1.0f, 0.0f, 
		1.0f, 0.5f, 0.0f, 
		1.0f, 0.0f, 0.0f, 
		1.0f, 1.0f, 1.0f, 
	};

	Float32* colIn  = RainBow;
	Float32* colOut = _Weights;

	Float32 invRatio = 1.0f - Diffraction;

	static Float32 intensity[8] = {0.3f, 0.25f, 0.20f, 0.18f, 0.16f, 0.12f, 0.10f, 0.08f};
	//Float32 totalIntensity = 0.0f;


	for (UInt32 i = 0; i < 8; i++)
	{
		//Float32 intensity = MATH::Pow(Sharpness, i);

		*(colOut++) = (Diffraction * (*(colIn++)) + invRatio * 1.0f) * intensity[i];
		*(colOut++) = (Diffraction * (*(colIn++)) + invRatio * 1.0f) * intensity[i];
		*(colOut++) = (Diffraction * (*(colIn++)) + invRatio * 1.0f) * intensity[i];
		*(colOut++) = 1.0f;
	}
}

void FILTER_2DCONVOLUTION::InitPoissonDisk()
{
	EnableFixedKernel(true);
	SetNumSample(8);

	static Float32 PoissonDistribution[8 * 2] =
	{ 0.0f			, 0.0f,
	  0.527837f		,-0.085868f,
	  -0.040088f	, 0.536087f,
	  -0.670445f	,-0.179949f,
	  -0.419418f	,-0.616039f,
	  0.440453f		,-0.639399f,
	  -0.757088f	, 0.349334f,
	  0.574619f		, 0.685879f};

	QDT_MEMCOPY(_Offsets, PoissonDistribution, sizeof(PoissonDistribution));

	for (UInt32 i = 0; i < 8 * 4; i++)
	{
		_Weights[i] = 1.0f / 8.0f;
	}
}


//#define DIAMX  0.447213595f
//#define DIAMY  0.894427191f
#define DIAMX  0.0f
#define DIAMY  1.0f

void FILTER_2DCONVOLUTION::InitDiamond()
{
	EnableFixedKernel(true);
	SetNumSample(8);



	static Float32 DiamondDistribution[8 * 2] =
	{ 
	  DIAMX		, DIAMY,  0.0f	   , 0.0f,
	  -DIAMY	, DIAMX,  0.0f	   , 0.0f,
	  -DIAMX	, -DIAMY, 0.0f	   , 0.0f,
	  DIAMY	    , -DIAMX, 0.0f	   , 0.0f};

	QDT_MEMCOPY(_Offsets, DiamondDistribution, sizeof(DiamondDistribution));

	for (UInt32 i = 0; i < 8 * 4; i++)
	{
		_Weights[i] = 1.0f / 5.0f;
	}
}

void FILTER_2DCONVOLUTION::InitBoxBlur16(Float32 radius)
{
	EnableFixedKernel(false);
	SetNumSample(16);


	Float32* offset = _Offsets;

	Float32 start = -radius * 0.5f;
	Float32 step  = radius / 3.0f;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			*(offset++) = start + x * step;
			*(offset++) = start + y * step;
		}
	}

	for (UInt32 i = 0; i < 16 * 4; i++)
	{
		_Weights[i] = 1.0f / 16.0f;
	}
}



void FILTER_2DCONVOLUTION::Init(I_RENDER_TARGET* InputTarget, I_RENDER_TARGET* OutputTarget)
{
	QDT_ASSERT(InputTarget->GetWidth() == OutputTarget->GetWidth() &&
			   InputTarget->GetHeight() == OutputTarget->GetHeight());

	_InputTarget = InputTarget;

	Init(InputTarget->GetTexture(), OutputTarget);
}


void FILTER_2DCONVOLUTION::OverridePixelShader(I_PIXEL_SHADER* pShader)
{
	_pShaderOverride = pShader;
	_PixelShaderOverriden = true;
}

void FILTER_2DCONVOLUTION::Init(I_TEXTURE* InputImage, I_RENDER_TARGET* OutputTarget,
								UInt32 SrcX, UInt32 SrcY, UInt32 SrcWidth, UInt32 SrcHeight,
								UInt32 DstX, UInt32 DstY, UInt32 DstWidth, UInt32 DstHeight)
{
	QDT_ASSERT(_numSample == 8 || _numSample == 16);

	Bool normalized = InputImage->GetUVNormalized();

	if (_FixedKernel && _numSample < 16)
	{
		OverrideVertexShader(g_VertexShader_fixed);

		if (_PixelShaderOverriden)
		{
			SetPixelShader(_pShaderOverride);
		}
		else
		{
			SetPixelShader(normalized ?  g_Convolution8_fixed_square : g_Convolution8_fixed);
		}
	}
	else
	{
		OverrideVertexShader(g_VertexShader);
		
		if (_PixelShaderOverriden)
		{
			SetPixelShader(_pShaderOverride);
		}
		else
		{
			switch (_numSample)
			{
				case 8:
					SetPixelShader(normalized ? g_Convolution8_square : g_Convolution8);
					break;
				case 16:
					SetPixelShader(normalized ? g_Convolution16_square : g_Convolution16);
					break;
			}
		}
	}

	FILTER_2D::Init(InputImage, OutputTarget, SrcX, SrcY, SrcWidth, SrcHeight,
											  DstX, DstY, DstWidth, DstHeight);
	

	if (_FixedKernel)
	{
		if (!_UseNoOffsets)
		{
			RENDER_CONTEXT::SetVertexShaderArrayConstant(1, _numSample * 2, (float *)_Offsets);
		}

		if (!_UseNoweights)
		{
			RENDER_CONTEXT::SetPixelShaderArrayConstant(0, _numSample * 4, (float *)(_Weights));
		}
	}
	else
	{
		if (!_UseNoOffsets)
		{
			RENDER_CONTEXT::SetPixelShaderArrayConstant(0, _numSample * 2, (float *)(_Offsets));
		}

		if (!_UseNoweights)
		{
			RENDER_CONTEXT::SetPixelShaderArrayConstant(_numSample / 2, _numSample * 4, (float *)(_Weights));
		}
	}
}


void  FILTER_2DCONVOLUTION::Init(I_TEXTURE* InputImage, I_RENDER_TARGET* OutputTarget)
{
	Init(InputImage, OutputTarget, 0, 0, InputImage->GetWidth(), InputImage->GetHeight(),
								   0, 0, OutputTarget->GetWidth(), OutputTarget->GetHeight());
}

/*
void FILTER_2DCONVOLUTION::Apply()
{
	FILTER2D::Apply(InputImage, OutputTarget, NoContextInit);
}
*/

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
